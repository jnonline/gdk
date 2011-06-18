
ECHO OFF

:: Parameter Checking
:: ---------------------------------------

:: Check that all required parameters were specified

IF "%1" == "" GOTO DisplayUsage
IF "%2" == "" GOTO DisplayUsage

:: Verify the destination folder doesnt already exist

IF EXIST "%~f1" (
	ECHO ERROR: The destination folder already exists.  Cannot continue.
	ECHO Destination: %~f1
	GOTO End	
)

:: Copy/Format the parameters into env variables

SETLOCAL
SET DestFolder=%~f1
SET ProjectName=%2

GOTO CreateProject

:: Display Help Text
:: ---------------------------------------

:DisplayUsage

ECHO ----------------------------------------------------------------
ECHO USAGE:  CreateNewProject [DestFolder] [ProjectName]
ECHO WHERE:
ECHO    DestFolder:  Path where the project will be created.          
ECHO                 Example: "C:\Projects\MyGame"
ECHO    ProjectName: Name for the new project.  Must be an Alphanumeric, C++ friendly identifier 
ECHO                 Example: "MonkeyFight"
ECHO ----------------------------------------------------------------

GOTO End

:: Create the new project
:: ---------------------------------

:CreateProject

:: Create the project folders

MD "%DestFolder%"
MD "%DestFolder%\Assets"
MD "%DestFolder%\Build"
MD "%DestFolder%\Source"
MD "%DestFolder%\Tools"

:: Copy all the template files over
XCOPY /E /Y Assets\* "%DestFolder%\Assets"
XCOPY /E /Y BuildTemplate\* "%DestFolder%\Build"
XCOPY /E /Y Source\* "%DestFolder%\Source"
XCOPY /E /Y Tools\* "%DestFolder%\Tools"

:: Remove any .svn folders
for /r %DestFolder% %%R in (.svn) do if exist "%%R" (rd /s /q "%%R")

:: Rename the various project files & folders
RENAME %DestFolder%\Source\Game\PROJECTNAMEGame.cpp  %ProjectName%Game.cpp
RENAME %DestFolder%\Source\Game\PROJECTNAMEGame.h  %ProjectName%Game.h
RENAME %DestFolder%\Build\iOS\PROJECTNAME.xcodeproj  %ProjectName%.xcodeproj
RENAME %DestFolder%\Build\iOS\PROJECTNAME-Info.plist  %ProjectName%-Info.plist
RENAME %DestFolder%\Build\MacOS\PROJECTNAME.xcodeproj  %ProjectName%.xcodeproj
RENAME %DestFolder%\Build\MacOS\PROJECTNAME-Info.plist  %ProjectName%-Info.plist
RENAME %DestFolder%\Build\Windows\PROJECTNAME.vcproj  %ProjectName%.vcproj

:: Replace all the ProjectName strings in the project & source files
Tools\fart %DestFolder%\Source\Game\%ProjectName%Game.cpp PROJECTNAME %ProjectName%
Tools\fart %DestFolder%\Source\Game\%ProjectName%Game.h PROJECTNAME %ProjectName%
Tools\fart %DestFolder%\Build\iOS\%ProjectName%.xcodeproj\project.pbxproj PROJECTNAME %ProjectName%
Tools\fart %DestFolder%\Build\MacOS\%ProjectName%.xcodeproj\project.pbxproj PROJECTNAME %ProjectName%
Tools\fart %DestFolder%\Build\Windows\%ProjectName%.vcproj PROJECTNAME %ProjectName%

ECHO ---------------------------------------
ECHO Created Project %ProjectName%
ECHO In the folder: %DestFolder%


GOTO End

:: The End
:: ---------------------------------

:End