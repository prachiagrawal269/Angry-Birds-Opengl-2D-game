### IceCream World
IceCream World is a 2D game developed using Opengl (c++ lamguage). 

####Game Layout
The game environment consists of an icecream trolley which contains of a cone with a icecream scoop over it. There is a target cone on which a player can fire the scoop.
The player can change the angle of projection of the scoop and also vary its speed which will be visible in the speed bar.
There is a "warm region" present in the game environment (above part of the game scene, orage-yellow in colour). As soon as the icecream scoop enters into this region, it melts. As a consequence of which the radius of the scoop decreases.
All laws of physics including gravity, ground friction, air friction, collision (both elastic and inelastic) are taken into account. 
The player has 3 chances in all to project the ball onto the target cone. The scoring of the game depends upon the radius of the scoop that finally reaches into the target cone.


####Directory Structure:
The "IcecreamWorld" module consists of following files:

a.) game.cpp : Contains code controlling major functionality of the game.
b.) board.cpp : Contains functions for "creation of various shapes e.g. rectangle, triangle, line, etc". Also contains classes for various objects along with their attributes and methods.
c.) global.h : Its a header file that contains all the global variables used across the game files.
d.) glad : folder that contains the file "glad.c" essential for running the codebase.
e.) Makefile : Its a makefile that contains the command to compile the code and create the "executable".
f.) game : Its the executale file that will actually run the code.
g.) help.pdf : This file contains the basic instructions to play the game.
h.) game.frag  : Fragment Shader
i.) game.vert : vertex Shader
j.) glad.c

####Setting up the Game Enviroment
---------------------------------------------------------------
#####DEPENDENCIES

Linux/Windows/ Mac OSX - Dependencies: (Recommended)
 GLFW
 GLAD 
 GLM

Linux - Dependencies: (alternative)
 FreeGLUT
 GLEW
 GLM

----------------------------------------------------------------
#####INSTALLATION

######GLFW:
 - Install CMake
 - Obtain & Extract the GLFW source code from
   https://github.com/glfw/glfw/archive/master.zip
 - Compile with below commands
   $ cd glfw-master
   $ mkdir build
   $ cd build
   $ cmake -DBUILD_SHARED_LIBS=ON ..
   $ make && sudo make install

######GLAD:
 - Go to http://glad.dav1d.de
 - Language: C/C++
   Specification: OpenGL
   gl: Version 4.5
   gles1: Version 1.0
   gles2: Version 3.2
   Profile: Core
   Select 'Add All' under extensions and click Generate.
 - Download the zip file generated.
 - Copy contents of include/ folder in the downloaded directory 
   to /usr/local/include/
 - src/glad.c should be always compiled along with your OpenGL 
   code

######GLM:
 - Download the zip file from 
   https://github.com/g-truc/glm/releases/tag/0.9.7.2
 - Unzip it and copy the folder glm/glm/ to /usr/local/include

Ubuntu users can also install these libraries using apt-get.
It is recommended to use GLFW+GLAD+GLM on all OSes.


FreeGLUT+GLEW+GLM can be used only on linux but not recommended.
Use apt-get to install FreeGLUT, libglew, glm on Linux.

####How to run the code: (only on linux platform)
1. Run "make" into the "GAME" folder.
2. Run "./game" 
