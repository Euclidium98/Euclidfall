# Euclidfall
daggerfall ish 2 attempt game

to be compiled using Raylib

engine features:
chunk/zone based locations making map very large without floating point errors
2D animations
2D Billboarding that actually renders
limited lighting
bitmap generation (used only for making the map)


Bugs:
everything that doesn't work, the most buggy being the inventory 


fpstest.c: 
this is the main file in which it should be compiled from, include raylib libraries

Updatestack.bat: this calls filecounter.exe inside of the "tools" folder, a handful of things suchs as models and textures are kept on the stack, to update whenever you add a new texture or model you will be required to run this, otherwise just have it run before complication.
