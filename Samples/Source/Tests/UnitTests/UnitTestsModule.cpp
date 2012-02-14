/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

// Includes
#include "BasePCH.h"
#include "UnitTestsModule.h"

// TODO(P2):
//   The unit tests should be re-worked into a TreeView with a Log for each node..
//   But this can wait until the UI layer is written :)

// ***********************************************************************
TestLog::TestLog()
{
    Indent = 0;
}

// ***********************************************************************
TestLog::~TestLog()
{
    // Clear the log
    Clear();
}

// ***********************************************************************
void TestLog::Clear()
{
    // Delete all the log lines
    for(vector<TestLogLine*>::iterator iter = this->Lines.begin(); iter != this->Lines.end(); iter++)
    {
        TestLogLine* line = (*iter);
        GdkDelete(line);
    }
    
    this->Lines.clear();
}

// ***********************************************************************
void TestLog::WriteLine(Gdk::LogLevel::Enum logLevel, const char *format, ...)
{
	char temp[2048];
    char *buf = temp;
    
    // Add the indent
    for(int i = 0; i < this->Indent; i++)
    {
        temp[i*2  ] = ' ';
        temp[i*2+1] = ' ';
    }
    buf += (this->Indent * 2);
    
	// Build the full string from the format and args
	va_list args;
	va_start (args, format);
	GDK_VSPRINTF(buf, 2048 - this->Indent*2, format, args);
	va_end (args);
    
    // Push the new item onto the queue
    TestLogLine* line = GdkNew TestLogLine(temp, logLevel);
    this->Lines.push_back(line);

}

// ###########################################################################################################

// ***********************************************************************
TestTreeNode::TestTreeNode(const char* name, TestDelegate* testMethod)
    : Name(name)
{
    this->Parent = NULL;
    this->Method = testMethod;
    this->Status = TestStatus::NotRun;
}

// ***********************************************************************
TestTreeNode::~TestTreeNode()
{
    // Delete the delegate
    GdkDelete(this->Method);
}

// ***********************************************************************
TestTreeNode* TestTreeNode::AddChild(TestTreeNode* child)
{
    this->Children.push_back(child);
    child->Parent = this;
    return child;
}


// ***********************************************************************
TestTreeNode* TestTreeNode::AddChild(const char* name, TestDelegate* testMethod)
{
    // Create the child node
    TestTreeNode* child = GdkNew TestTreeNode(name, testMethod);
    
    // Add the child
    return AddChild(child);
}

// ###########################################################################################################

// ***********************************************************************
UnitTestsModule::UnitTestsModule ()
{
	// Bind to Touch & Mouse event handlers
	TouchInput::TouchBegan.AddHandlerMethod(this, &UnitTestsModule::OnTouchBegan);
    TouchInput::TouchMoved.AddHandlerMethod(this, &UnitTestsModule::OnTouchMoved);
    TouchInput::TouchEnded.AddHandlerMethod(this, &UnitTestsModule::OnTouchEnded);
    
	Mouse::MouseButtonDown.AddHandlerMethod(this, &UnitTestsModule::OnMouseDown);
    Mouse::MouseWheelScroll.AddHandlerMethod(this, &UnitTestsModule::OnMouseWheelScroll);
    
    // Initialize properties
    this->rootNode = NULL;
    this->currentTestIndex = -1;
    this->currentLogPosition = 0.0f;
    this->logScrollVelocity = 0.0f;
    
    BuildTestTree();
}

// ***********************************************************************
UnitTestsModule::~UnitTestsModule()
{
	// Unbind the input event handlers
    TouchInput::TouchBegan.RemoveHandlerMethod(this, &UnitTestsModule::OnTouchBegan);
    TouchInput::TouchMoved.RemoveHandlerMethod(this, &UnitTestsModule::OnTouchMoved);
    TouchInput::TouchEnded.RemoveHandlerMethod(this, &UnitTestsModule::OnTouchEnded);
    
	Mouse::MouseButtonDown.RemoveHandlerMethod(this, &UnitTestsModule::OnMouseDown);
    Mouse::MouseWheelScroll.RemoveHandlerMethod(this, &UnitTestsModule::OnMouseWheelScroll);
    
    // Destroy the test tree
    GdkDelete(this->rootNode);
}

// ***********************************************************************
void UnitTestsModule::BuildTestTree()
{
    // Macros for RootNodes, ContainerNodes, and TestNodes (Leaf)
    #define CNODE(parent, inst, name)   TestTreeNode* inst = parent->AddChild( GdkNew TestTreeNode(name) );
    #define TNODE(parent, name, method) parent->AddChild( GdkNew TestTreeNode(name, TestDelegate::FromMethod( this, &UnitTestsModule::method) ) );
    
    // Create the root test tree node
    this->rootNode = GdkNew TestTreeNode("Unit Tests", NULL);
    
    // System Tests
    // -----------------------
 
    CNODE(this->rootNode, systemTests, "System Tests");
        TNODE(systemTests, "Logging", Test_System_Logging);
        TNODE(systemTests, "Memory", Test_System_Memory);
        CNODE(systemTests, systemContainerTests, "Containers");
            TNODE(systemContainerTests, "StringHashMap", Test_System_Containers_StringHashMap);
            TNODE(systemContainerTests, "SortedVector", Test_System_Containers_SortedVector);
        CNODE(systemTests, systemThreadingTests, "Threading");
            TNODE(systemThreadingTests, "ThreadedWorkQueue", Test_System_Threading_ThreadedWorkQueue);
    
    // Math Tests
    // -----------------------
    
    CNODE(this->rootNode, mathTests, "Math Tests");
        TNODE(mathTests, "Randoms", Test_Math_Randoms);
        TNODE(mathTests, "Vectors", Test_Math_Vectors);
        TNODE(mathTests, "Distance 2D", Test_Math_Distance2D);
        TNODE(mathTests, "Intersection 2D", Test_Math_Intersection2D);
        TNODE(mathTests, "Distance 3D", Test_Math_Distance3D);
        TNODE(mathTests, "Intersection 3D", Test_Math_Intersection3D);
    
    // -----------------------
    
    #undef CNODE
    #undef TNODE
    
    // Build a flat list from the tree
    RecurseFlattenTree(this->rootNode);
}

// ***********************************************************************
void UnitTestsModule::RecurseFlattenTree(TestTreeNode* node)
{
    // Add this node
    this->flatTestTree.push_back(node);
    
    // Recurse to the child nodes
    for(list<TestTreeNode*>::iterator iter = node->Children.begin(); iter != node->Children.end(); iter++)
    {
        RecurseFlattenTree(*iter);
    }
}

// ***********************************************************************
void UnitTestsModule::OnUpdate(float elapsedSeconds)
{
    // Are we running tests?
    if(this->currentTestIndex >= 0)
    {
        TestTreeNode* activeTestNode = this->flatTestTree[this->currentTestIndex];
        
        // Determine the depth of this node
        int depth = 0;
        TestTreeNode* temp = activeTestNode;
        while(temp->Parent != NULL)
        {
            temp = temp->Parent;
            depth++;
        }
        
        // Write the node header log & set the log depth
        this->log.Indent = depth;
        size_t headerLogIndex = this->log.Lines.size();
        this->log.WriteLine(LogLevel::System, "NODE: %s", activeTestNode->Name.c_str());
        this->log.Indent = depth+1;
        
        // Does the active node have a test method?
        if(activeTestNode->Method != NULL)
        {
            // Create the execution context
            TestExecutionContext context(activeTestNode, &this->log);
            
            // Execute the test method
            activeTestNode->Status = activeTestNode->Method->Invoke(&context);
            
            if(activeTestNode->Status == TestStatus::Fail)
                this->log.Lines[headerLogIndex]->Level = LogLevel::Error;
        }
    
        // Advance to the next test  (stop if we get to the end)
        this->currentTestIndex++;
        if(this->currentTestIndex >= (int)this->flatTestTree.size())
            this->currentTestIndex = -1;
    }
    
    // Update log scrolling velocity
    this->currentLogPosition += this->logScrollVelocity;
    this->logScrollVelocity *= (1.0f - elapsedSeconds * 4.0f);
}

// ***********************************************************************
void UnitTestsModule::OnDraw()
{
	// Clear the background
	Graphics::Clear(
		true, false, false,
		Color::FromNormalized(0.1f, 0.5f, 0.4f, 1.0f),
		1.0f
		);
	
	// Setup the projection matrix to draw in screen coordinates
    Graphics::SetProjectionToScreenOrtho();
    
	Vector2 textScale(0.7f, 0.7f);
	float textHeight = 14.0f;

	// Draw the display log 
	// -----------------------------------

    float currentTextY = 25.0f;
    
    // Restrict log scroll position to within the visible log lines
    int numLogLines = this->log.Lines.size();
    if(this->currentLogPosition > numLogLines - 1.0f)
        this->currentLogPosition = numLogLines - 1.0f;
    if(this->currentLogPosition < 0.0f)
        this->currentLogPosition = 0.0f;
    
    float scrollOffset = Math::Mod(this->currentLogPosition, 1.0f);
    currentTextY -= scrollOffset * textHeight;
    UInt8 alpha = (UInt8)(scrollOffset * 255);
    
    // Draw the log lines from our current index to the end of the available space
    for(int logIndex = (int)this->currentLogPosition; 
            logIndex < numLogLines && currentTextY < Application::GetHeight(); 
            logIndex++, currentTextY += textHeight)
    {
        // Get the log line at this index
        TestLogLine* line = this->log.Lines[logIndex];
        
        // Determine the color for this log line
        Gdk::Color color;
        switch(line->Level)
        {
            case LogLevel::System:		color = Gdk::Color(255,255,255);	break;
            case LogLevel::Warning:		color = Gdk::Color(255,224,24);		break;
            case LogLevel::Assert:		
            case LogLevel::Error:		color = Gdk::Color(255,115,15);		break;
            case LogLevel::Info:		color = Gdk::Color(224,224,200);	break;
            case LogLevel::Verbose:
            default:					color = Gdk::Color(200,200,200);	break;
        }
        
        if(logIndex == (int)this->currentLogPosition)
            color.A = 255 - alpha;
        else if((currentTextY + textHeight) >= Application::GetHeight())
            color.A = alpha;
        
        // Draw this log text
        Drawing2D::DrawText(
            SharedResources::Fonts.Arial20, 
            line->Text.c_str(), 
            Vector2(4.0f, currentTextY), 
            color, 
            textScale
            );
    }

    // Draw the "Run" button & Status text
	// -----------------------------------
    
    // Draw the header box
    Rectangle2 headerBox(2.0f, 2.0f, Application::GetWidth() - 4.0f, 22.0f);
    Drawing2D::DrawRectangle(headerBox, Color(64,64,64));
    Drawing2D::DrawRectangleOutline(headerBox, Color(196,196,196));
    
    // Are the unit tests currently running?
    if(this->currentTestIndex >= 0)
    {
        // Draw the "Running: Test Name" text
        TestTreeNode* activeTestNode = this->flatTestTree[this->currentTestIndex];
        Drawing2D::DrawText(SharedResources::Fonts.Arial20, "Running: ", headerBox.Position + Vector2(2,2), Color(128,255,128), textScale);
		Drawing2D::DrawText(SharedResources::Fonts.Arial20, activeTestNode->Name.c_str(), headerBox.Position + Vector2(62,2), Color(128,255,128), textScale);
    }
    else
    {
        // Draw the "Click here to Start" text
        Drawing2D::DrawText(SharedResources::Fonts.Arial20, "Click here to start testing!", headerBox.Position + Vector2(2,2), Color(255,255,255), textScale);
	}

	// Flush the 2D renderer
	Drawing2D::Flush();
}

// ***********************************************************************
void UnitTestsModule::OnTouchBegan(Touch* touch)
{
	// Is this touch un-owned?
	if(touch->GetOwner() == NULL)
	{
        // Are we not running tests?
        if(this->currentTestIndex < 0)
        {
            // Is this touch within the "Start" box
            Rectangle2 headerBox(2.0f, 2.0f, Application::GetWidth() - 4.0f, 22.0f);
            if(headerBox.Contains(touch->GetPosition()) == true)
            {
                // Start running from the beginning
                this->currentTestIndex = 0;
                this->log.Clear();
            }
        }
	}
}

// ***********************************************************************
void UnitTestsModule::OnTouchMoved(Touch* touch)
{
    // Is this touch un-owned?
	if(touch->GetOwner() == NULL)
	{
        // Scroll the log
        float deltaY = touch->GetPreviousPosition().Y - touch->GetPosition().Y;
        currentLogPosition += deltaY * 0.1f;
        logScrollVelocity = 0.0f;
    }
}

// ***********************************************************************
void UnitTestsModule::OnTouchEnded(Touch* touch)
{
    // Is this touch un-owned?
	if(touch->GetOwner() == NULL)
	{
        // Set the log scrolling velocity
        float deltaY = touch->GetPreviousPosition().Y - touch->GetPosition().Y;
        logScrollVelocity = deltaY * 0.1f;
    }
}

// ***********************************************************************
void UnitTestsModule::OnMouseDown(MouseButton::Enum button)
{
	// Is this the left button?
	if(button == MouseButton::Left)
	{
		// Are we not running tests?
        if(this->currentTestIndex < 0)
        {
            // Is this touch within the "Start" box
            Rectangle2 headerBox(2.0f, 2.0f, Application::GetWidth() - 4.0f, 22.0f);
            if(headerBox.Contains(Mouse::GetPosition()) == true)
            {
                // Start running from the beginning
                this->currentTestIndex = 0;
                this->log.Clear();
            }
        }
	}
}

// ***********************************************************************
void UnitTestsModule::OnMouseWheelScroll(float deltaX, float deltaY)
{
    // Move the log position
    currentLogPosition += deltaY * 0.2f;
}
