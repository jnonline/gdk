/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

// Includes
#include "BasePCH.h"
#include "SampleGame.h"

using namespace Gdk;

GDK_GAME_CLASS(SampleGame);

// ***********************************************************************
SampleGame::SampleGame ()
{
	this->activeModule = NULL;
	this->activeModuleIndex = 0;
	this->changeToModuleIndex = 0;
}

// ***********************************************************************
bool SampleGame::OnLoadSettings(ApplicationSettings &settings)
{
	Game::OnLoadSettings(settings);

	settings.Title = L"Gdk Sample Game";
	settings.Width = 640;
	settings.Height = 480;
	settings.AllowResize = true;
	settings.ShowMaximizeBox = true;
	settings.ShowMinimizeBox = true;
    settings.ShowCloseBox = true;
	settings.FixedTimeStep = 0.02f;
	settings.UseFixedTimeStep = false;
	settings.ResourceLoaderBackgroundThreads = 0;

	Application::DebugStatsColor = Color(255,255,128,255);

	return true;
}


// ***********************************************************************
bool SampleGame::OnInit()
{
	// Do game initialization here
	// -------------------------------

	// Bind to Touch & Mouse event handlers
	TouchInput::TouchBegan.AddHandlerMethod(this, &SampleGame::OnTouchBegan);
	Mouse::MouseButtonDown.AddHandlerMethod(this, &SampleGame::OnMouseDown);

    // Register all the sample/test modules
    Module::Init();
    
	// Init the first module
	ASSERT(Module::GetRegistrations().size() > 0, L"No Registered Modules!");
	this->activeModuleIndex = this->changeToModuleIndex = 0;
	this->activeModule = Module::GetRegistrations()[this->activeModuleIndex]->CreateModule();

	return true;
}

// ***********************************************************************
void SampleGame::OnShutdown()
{
	// Do game shutdown here
	// -------------------------------

	// Shutdown the active module
	if(activeModule != NULL)
		GdkDelete(activeModule);

	// Unbind the input event handlers
    TouchInput::TouchBegan.RemoveHandlerMethod(this, &SampleGame::OnTouchBegan);
	Mouse::MouseButtonDown.RemoveHandlerMethod(this, &SampleGame::OnMouseDown);
}


// ***********************************************************************
void SampleGame::OnUpdate(float elapsedSeconds)
{
	// Do we need to change modules?
	if(this->activeModuleIndex != this->changeToModuleIndex)
	{
		// Delete the current module
		if(this->activeModule != NULL)
		{
			GdkDelete(this->activeModule);
		}

		// Wrap the module index around if it is outside the array bounds
		int numModules = Module::GetRegistrations().size();
		if(changeToModuleIndex >= numModules)
			changeToModuleIndex %= numModules;
		while(changeToModuleIndex < 0)
			changeToModuleIndex += numModules;

		// Create the new module
		this->activeModuleIndex = this->changeToModuleIndex;
		this->activeModule = Module::GetRegistrations()[this->activeModuleIndex]->CreateModule();
	}

	// Calculate the positions of the Next & Previous buttons
	int width = Application::GetWidth();
	int height = Application::GetHeight();
	prevButtonRect = Rectangle2((width * 0.5f) - 120.0f, height - 20.0f, 20.0f, 20.0f);
	nextButtonRect = Rectangle2((width * 0.5f) + 100.0f, height - 20.0f, 20.0f, 20.0f);

	// Calculate the position of the module name
	wstring& moduleName = Module::GetRegistrations()[this->activeModuleIndex]->Name;
	Vector2 nameTextSize = Gdk::SharedResources::Fonts.Arial20->GetTextSize(moduleName.c_str());
	nameTextPosition = Vector2(width * 0.5f - nameTextSize.X * 0.5f, height - 20.0f);

	// Update the current module
	if(this->activeModule != NULL)
		this->activeModule->OnUpdate(elapsedSeconds);
    
    // Quit if the user pressed escape
    if(Keyboard::IsKeyDown(Keys::Escape))
        Application::Exit();
}

// ***********************************************************************
void SampleGame::OnDraw(float elapsedSeconds)
{ 
	// Render the active module
	if(this->activeModule != NULL)
		this->activeModule->OnDraw();

	
	// Draw the Next & Previous button & the module name
	// -------------------------------------------------------------------

	// Setup a 2D projection matrix to draw in screen coordinates
    Matrix3D proj = Matrix3D::CreateOrthoOffCenter(
        0, (float) Application::GetWidth(),					// left / right
		(float) Application::GetHeight(), 0,				// bottom / top
		-1.0f, 1.0f											// far / near
		);
	Graphics::GlobalUniforms.Projection->SetMatrix4(proj);

	// Reset the identity world transform
	Renderer2D::SetWorldTransform(Matrix2D::IDENTITY);

	 // Draw the next/previous buttons
	Renderer2D::DrawRectangle(prevButtonRect, Color(64,64,64,128));
	Renderer2D::DrawRectangle(nextButtonRect, Color(64,64,64,128));
	Renderer2D::DrawRectangleOutline(prevButtonRect, Color(196,196,196,128));
	Renderer2D::DrawRectangleOutline(nextButtonRect, Color(196,196,196,128));
	Renderer2D::DrawText(SharedResources::Fonts.Arial20, L"<", prevButtonRect.Position + Vector2(3,0), Color(196, 255, 128));
	Renderer2D::DrawText(SharedResources::Fonts.Arial20, L">", nextButtonRect.Position + Vector2(4,0), Color(196, 255, 128));

	// Draw the module name
	wstring& moduleName = Module::GetRegistrations()[this->activeModuleIndex]->Name;
	Renderer2D::DrawText(SharedResources::Fonts.Arial20, moduleName.c_str(), nameTextPosition, Color(196, 255, 128));

	// Flush the 2D renderer
	Renderer2D::Flush();
}


// ***********************************************************************
void SampleGame::OnTouchBegan(Touch* touch)
{
	// Is this touch un-owned?
	if(touch->GetOwner() == NULL)
	{
		// Is this touch within the 'Prev' button?
		if(prevButtonRect.Contains(touch->GetPosition()))
		{
			// Claim this touch
			TouchInput::SetTouchOwner(*touch, this);

			// Move to the next module
			this->changeToModuleIndex = this->activeModuleIndex - 1;
		}
		// Is this touch within the 'Next' button?
		else if(nextButtonRect.Contains(touch->GetPosition()))
		{
			// Claim this touch
			TouchInput::SetTouchOwner(*touch, this);

			// Move to the next module
			this->changeToModuleIndex = this->activeModuleIndex + 1;
		}
	}
}

// ***********************************************************************
void SampleGame::OnMouseDown(MouseButton::Enum button)
{
	// Is this the left button?
	if(button == MouseButton::Left)
	{
		// Is the mouse within the 'Prev' button?
		if(prevButtonRect.Contains(Mouse::GetPosition()))
		{
			// Move to the next module
			this->changeToModuleIndex = this->activeModuleIndex - 1;
		}
		// Is the mouse within the 'Next' button?
		else if(nextButtonRect.Contains(Mouse::GetPosition()))
		{
			// Move to the next module
			this->changeToModuleIndex = this->activeModuleIndex + 1;
		}
	}
}
