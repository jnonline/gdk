
#!/bin/sh
# ECHO OFF

# Display Help Text - Function
# ---------------------------------------

function DisplayUsage {

    ECHO "----------------------------------------------------------------"
    ECHO "USAGE:  CreateNewProject-MAC [1] [ProjectName]"
    ECHO "WHERE:"
    ECHO "   1:  Path where the project will be created."
    ECHO "                Example: \"/Projects/MonkeyFight\""
    ECHO "   ProjectName: Name for the new project.  Must be an Alphanumeric, C++ friendly identifier"
    ECHO "                Example: \"MonkeyFight\""
    ECHO "----------------------------------------------------------------"

    exit
}

# Parameter Checking
# ---------------------------------------

# Check that all required parameters were specified

if [ -z "$1" ]; then 
    DisplayUsage 
fi
if [ -z "$2" ]; then 
    DisplayUsage 
fi

# Verify the destination folder does not already exist

if [ -d "$DIRECTORY" ]; then
    ECHO ERROR: The destination path already exists.  Cannot continue.
    exit
fi

if [ "$1" == "." ]; then 
    ECHO ERROR: Please specify a different directory than: $1
    exit
fi

# Create the new project
# ---------------------------------

# Create the project folders

mkdir "$1"
mkdir "$1/Assets"
mkdir "$1/Build"
mkdir "$1/Source"
mkdir "$1/Tools"

# Copy all the template files over
cp -p -r ./Assets/* "$1/Assets"
cp -p -r ./BuildTemplate/* "$1/Build"
cp -p -r ./Source/* "$1/Source"
cp -p -r ./Tools/* "$1/Tools"

# Remove any .svn folders
find $1 -name ".svn" -type d -exec rm -rf {} +;

# Rename the various project files & folders
mv "$1/Source/Game/PROJECTNAMEGame.cpp"    "$1/Source/Game/$2Game.cpp"
mv "$1/Source/Game/PROJECTNAMEGame.h"      "$1/Source/Game/$2Game.h"
mv "$1/Build/iOS/PROJECTNAME.xcodeproj"    "$1/Build/iOS/$2.xcodeproj"
mv "$1/Build/MacOS/PROJECTNAME.xcodeproj"  "$1/Build/MacOS/$2.xcodeproj"
mv "$1/Build/Windows/PROJECTNAME.vcproj"   "$1/Build/Windows/$2.vcproj"

# Replace all the ProjectName strings in the project & source files
sed "s/PROJECTNAME/$2/g" "$1/Source/Game/$2Game.cpp" > tmp.tmp   && mv tmp.tmp "$1/Source/Game/$2Game.cpp"
sed "s/PROJECTNAME/$2/g" "$1/Source/Game/$2Game.h" > tmp.tmp     && mv tmp.tmp "$1/Source/Game/$2Game.h"
sed "s/PROJECTNAME/$2/g" "$1/Build/iOS/$2.xcodeproj/project.pbxproj" > tmp.tmp   && mv tmp.tmp "$1/Build/iOS/$2.xcodeproj/project.pbxproj"
sed "s/PROJECTNAME/$2/g" "$1/Build/MacOS/$2.xcodeproj/project.pbxproj" > tmp.tmp && mv tmp.tmp "$1/Build/MacOS/$2.xcodeproj/project.pbxproj"
sed "s/PROJECTNAME/$2/g" "$1/Build/Windows/$2.vcproj" > tmp.tmp  && mv tmp.tmp "$1/Build/Windows/$2.vcproj"

# All done
ECHO "---------------------------------------"
ECHO "Created Project $2"
ECHO "In the folder: $1"


# The End
# ---------------------------------
