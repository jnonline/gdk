
// Includes
#include "Simple3DTests.h"

// ***********************************************************************
Simple3DTests::Simple3DTests()
{
	this->ViewCenter = Vector2::ZERO;
	this->ViewExtents = Vector2(10.0f, 10.0f);

    this->totalElapsedSeconds = 0.0f;
	this->rectTimer = 0.0f;
	this->circleTimer = 0.0f;
	this->diamondTimer = 0.0f;
	this->textScaleTimer = 0.0f;
	this->textSpinTimer = 0.0f;

	this->sprite1 = GdkNew Sprite(SharedAssets::AtlasAnimations.Chopper);
	this->sprite1->Scale = Vector2(0.03f, -0.03f);
	this->sprite1->Position = Vector2(-2.0f, -6.0f);

	this->sprite2 = GdkNew Sprite(SharedAssets::AtlasAnimations.Chopper);
	this->sprite2->Scale = Vector2(0.04f, -0.04f);
	this->sprite2->Position = Vector2(2.0f, -6.0f);
	this->sprite2->BlendMode = BlendMode::Add;

}

// ***********************************************************************
Simple3DTests::~Simple3DTests()
{
	GdkDelete(this->sprite1);
	GdkDelete(this->sprite2);
}

// ***********************************************************************
void Simple3DTests::OnUpdate(float elapsedSeconds)
{
	// Calculate the 2D View bounds / area
	// ---------------------------------------------

	// Determine the size of the view accounting for aspect ratio of the screen.  (assumes pixels are square)
	float aspect = Application::GetWidth() / (float)Application::GetHeight();
	this->ViewExtents.X = 10.0f;
	this->ViewExtents.Y = 10.0f;
	if(aspect < 1.0f)	// The view is smaller on the X-Axis
	{
		this->ViewExtents.Y /= aspect;
	}
	else	// The view is smaller on the Y-Axis
	{
		this->ViewExtents.X *= aspect;
	}

	// Update the animation timers
	// ---------------------------------------------

    this->totalElapsedSeconds += elapsedSeconds;
	this->rectTimer = Math::Mod(this->rectTimer + elapsedSeconds * 0.18f, 1.0f);
	this->circleTimer = Math::Mod(this->circleTimer + elapsedSeconds * 0.33f, 1.0f);
	this->diamondTimer = Math::Mod(this->diamondTimer + elapsedSeconds * 0.111f, 1.0f);

	this->textScaleTimer = Math::Mod(this->textScaleTimer + elapsedSeconds * 0.33f, 1.0f);
	this->textSpinTimer = Math::Mod(this->textSpinTimer + elapsedSeconds * 0.12f, 1.0f);

	// Update the sprites
	// --------------------------------

	sprite1->Update(elapsedSeconds);
	sprite2->Update(elapsedSeconds);
}

// ***********************************************************************
void Simple3DTests::OnDraw()
{
	// Clear the background
	Graphics::Clear(
		(ClearBuffers::Enum) (ClearBuffers::Color | ClearBuffers::Depth), 
		Color::FromNormalized(0.2f, 0.2f, 0.2f, 1.0f),
		1.0f
		);
	
	// Setup 2D projection
	// ----------------------------------------------------

	// Setup a 2D projection matrix where a 20x20 square is centered on the screen center
    Matrix3D proj;
	proj.OrthoOffCenter(
		this->ViewCenter.X - this->ViewExtents.X, this->ViewCenter.X + this->ViewExtents.X,		// left / right
		this->ViewCenter.Y - this->ViewExtents.Y, this->ViewCenter.Y + this->ViewExtents.Y,		// bottom / top
		-1.0f, 1.0f				// far / near
		);
	Graphics::GlobalUniforms.Projection->SetMatrix4(proj);

	// 2D Primitive Drawing
	// ----------------------------------------------------

	Matrix2D world;

	// Reset the identity world transform
	Renderer2D::SetWorldTransform(Matrix2D::IDENTITY);
    
    // Lines
    {
        float xScale = 0.5f;
        float xStep = 0.1f;
        float xStart = -12.0f;
        float xEnd = 12.0f;
        
        float t1 = (xStart + totalElapsedSeconds) * xScale;
        float y1 = PerlinNoise::CompoundNoise1(t1) * 2.0f;
        for(float x=xStart; x<=xEnd; x+=xStep)
        {
            float t2 = (x+xStep + totalElapsedSeconds) * xScale;
            float y2 = PerlinNoise::CompoundNoise1(t2) * 2.0f;
            if(Math::Floor(t1) != Math::Floor(t2))
                Renderer2D::DrawLine(Vector2(x, y1), Vector2(x+xStep, y2), Color::BLUE);
            else
                Renderer2D::DrawLine(Vector2(x, y1), Vector2(x+xStep, y2), Color::PURPLE);
            t1 = t2;
            y1 = y2;
        }
    }

	// Rects
	float sinRectTimer = Math::Sin(rectTimer * Math::TWO_PI);
	for(int i = 0; i < 10; i++)
	{
		if(i % 2 == 0)
			Renderer2D::DrawRectangle(Rectangle2(-10 + i * 2, sinRectTimer * 10.0f, 1.0f, 1.0f), Color(128,0,128,255));
		else
			Renderer2D::DrawRectangleOutline(Rectangle2(sinRectTimer * 10.0f, -10 + i * 2, 1.0f, 1.0f), Color(255,128,255,255));
	}

	// Diamonds
	for(int i = 0; i < 20; i++)
	{
		// Setup a world transform for the diamond
		Matrix2D diamondTransform = Matrix2D::CreateRotateScaleTranslate(
			1.0f, 1.0f, 
			Math::Cos((diamondTimer + 0.05f * i) * Math::TWO_PI) * 10.0f, Math::Sin((diamondTimer + 0.05f * i) * Math::TWO_PI * 2.0f) * 5.0f,
			(circleTimer + 0.1f * i) * Math::TWO_PI
			) * Matrix2D::CreateRotation(this->diamondTimer * Math::TWO_PI);
		Renderer2D::SetWorldTransform(diamondTransform);

		// Draw the diamonds
		if(i % 2 == 0)
			Renderer2D::DrawDiamond(Rectangle2(-0.5f, -0.5f, 1.0f, 1.0f), Color(128,128,128,128), BlendMode::Add);
		else
			Renderer2D::DrawDiamondOutline(Rectangle2(-0.5f, -0.5f, 1.0f, 1.0f), Color(255,255,255,255), BlendMode::Add);
	}
    Renderer2D::SetWorldTransform(Matrix2D::IDENTITY);
    
    // Circles (Outlined)
	for(int i = 0; i < 8; i++)
	{
		float angle1 = (circleTimer + i * 0.1f) * Math::TWO_PI;
		Renderer2D::DrawCircleOutline(Circle2(i * Math::Cos(angle1) + 5.0f, i * Math::Sin(angle1) + 5.0f, 0.7f), 16, Color(0,255,255,255));
	}
    
    // Circles
    {
        float scale = 0.25f;
        float step = 1.0f;
        float xStart = -12.0f;
        float xEnd = 12.0f;
        float yStart = -12.0f;
        float yEnd = 12.0f;
        
        for(float y=yStart; y<=yEnd; y+=step)
        {
            for(float x=xStart; x<=xEnd; x+=step)
            {
                float z = PerlinNoise::Noise2((x + totalElapsedSeconds) * scale, y * scale);
                
                if(z > 0)
                    Renderer2D::DrawCircle(Circle2(x,y,step * z), 16, Color(0,64,128,64));
                else
                    Renderer2D::DrawCircle(Circle2(x,y,step * -z), 16, Color(0,128,128,64));
            }
        }
    }
    
    // Polyline
    {
        Vector2 vertices[201];
        int steps = 200;
        
        float radiusScale = 6.0f + Math::Sin(this->rectTimer * Math::TWO_PI) * 2.0f;
        float frequency = 4.0f + Math::Sin(this->diamondTimer * Math::TWO_PI) * 3.0f;
        
        for(int step = 0; step <= steps; step++)
        {
            float t = step / (float)steps;
            float theta = t * Math::TWO_PI;
            float r = radiusScale * Math::Sin(frequency * theta);
            Vector2 p = Vector2::FromPolarCoordinate(r, theta);
            vertices[step] = p + Vector2(-5.0f, -5.0f);
        }
        
        Renderer2D::DrawPolyline(vertices, steps+1, Color(64,128,0,255));
    }


	// Transform Stack test
	// ----------------------------------------------------

	// Stack 1
	Matrix2D stack1 = Matrix2D::CreateTranslation(4.0f, 4.0f) * Matrix2D::CreateRotation(rectTimer * Math::TWO_PI);
	Renderer2D::SetWorldTransform(stack1);
	Renderer2D::DrawCircle(Circle2(0.0f, 0.0f, 1.0f), 16, Color(0,128,0,128));

	// Stack 2
	Matrix2D stack2 = Matrix2D::CreateTranslation(-1.6f, -1.8f) * Matrix2D::CreateRotation(rectTimer * Math::TWO_PI) * stack1;
	Renderer2D::SetWorldTransform(stack2);
	Renderer2D::DrawCircle(Circle2(0.0f, 0.0f, 0.8f), 16, Color(24,164,24,128));

	// Stack 3
	Matrix2D stack3 = Matrix2D::CreateTranslation(1.2f, 0.7f) * Matrix2D::CreateRotation(circleTimer * Math::TWO_PI) * stack2;
	Renderer2D::SetWorldTransform(stack3);
	Renderer2D::DrawCircle(Circle2(0.0f, 0.0f, 0.6f), 16, Color(48,196,48,128));

	// Stack 4
	Matrix2D stack4 = Matrix2D::CreateTranslation(-0.7f, 0.5f) * Matrix2D::CreateRotation(diamondTimer * Math::TWO_PI) * stack3;
	Renderer2D::SetWorldTransform(stack4);
	Renderer2D::DrawCircle(Circle2(0.0f, 0.0f, 0.4f), 16, Color(64,224,64,128));

	// Text
	// ----------------------------------------------------

	// Create a spinning / scaling transform
	float textScale = 0.03f + Math::Sin(textScaleTimer * Math::TWO_PI) * 0.01f;
	world = Matrix2D::CreateScaleRotateTranslate(textScale, -textScale, 5.0f, -5.0f, textSpinTimer * Math::TWO_PI);
	Renderer2D::SetWorldTransform(world);

	// Draw some Text
    wchar_t* sampleText = L"Sample Text...\nNew Line...";
	Renderer2D::DrawText(SharedAssets::Fonts.Arial20, sampleText, Vector2::ZERO, Color(255,224,160));

	// Images & Anims & Sprites
	// ----------------------------------------------------

	// Back to the identity transform
	Renderer2D::SetWorldTransform(Matrix2D::IDENTITY);

	// Test the DrawImage() methods
	Renderer2D::DrawImage(SharedAssets::AtlasImages.Test1, Vector2(-5.0f, 0.0f), Color::WHITE, rectTimer * Math::TWO_PI, 0.04f, 0.04f);
	Renderer2D::DrawImage(SharedAssets::AtlasImages.Test1, Vector2(-9.0f, 5.0f), Vector2(-7.0f, 2.0f), Color(196,255,196));

	// Test sprite drawing
	Renderer2D::DrawSprite(sprite1);
	Renderer2D::DrawSprite(sprite2);


	// Flush the 2D renderer
	Renderer2D::Flush();
}
