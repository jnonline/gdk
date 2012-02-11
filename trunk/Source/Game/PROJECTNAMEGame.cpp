
// Include the Pre-Compiled header
// This MUST be the first non-comment line ALL of your .cpp files
#include "BasePCH.h"		

// Include our header
#include "PROJECTNAMEGame.h"

using namespace Gdk;

GDK_GAME_CLASS(PROJECTNAMEGame);

// ***********************************************************************
PROJECTNAMEGame::PROJECTNAMEGame ()
{
}

// ***********************************************************************
bool PROJECTNAMEGame::OnLoadSettings(ApplicationSettings &settings)
{
	Game::OnLoadSettings(settings);

	settings.Title = "Gdk Project: PROJECTNAME";
	settings.Width = 640;
	settings.Height = 480;
    settings.AllowResize = true;
    settings.ShowMinimizeBox = true;
    settings.ShowMaximizeBox = true;
	settings.FixedTimeStep = 0.02f;
	settings.UseFixedTimeStep = false;
	settings.ResourceLoaderBackgroundThreads = 0;

	Application::DebugStatsColor = Color(255,255,128,255);

	return true;
}

// ***********************************************************************
bool PROJECTNAMEGame::OnInit()
{
	// Do game initialization here
	// -------------------------------

	return true;
}

// ***********************************************************************
void PROJECTNAMEGame::OnShutdown()
{
	// Do game shutdown here
	// -------------------------------

}

// ***********************************************************************
void PROJECTNAMEGame::OnUpdate(float elapsedSeconds)
{
    // Quit if the user pressed escape
    if(Keyboard::IsKeyDown(Keys::Escape))
        Application::Exit();
}

// ***********************************************************************
void PROJECTNAMEGame::OnDraw(float elapsedSeconds)
{ 
	// Clear the background
	Graphics::Clear(
		true, true, false, 
		Color::FromNormalized(0.2f, 0.2f, 0.2f, 1.0f),
		1.0f
		);

	// Setup a 2D projection matrix to draw in screen coordinates
	Matrix3D proj = Matrix3D::CreateOrthoOffCenter(
		0, (float) Application::GetWidth(),					// left / right
		(float) Application::GetHeight(), 0,				// bottom / top
		-1.0f, 1.0f											// far / near
		);
	Graphics::GlobalUniforms.Projection->SetMatrix4(proj);

	// Draw some text
	char* sampleText = "Sample Text";
	Drawing2D::DrawText(SharedResources::Fonts.Arial20, sampleText, Vector2(10,10), Color(196, 255, 128));

	// Flush the 2D renderer
	Drawing2D::Flush();
}
