### Ships & Giggles

## Overview

Ships & Giggles is a simple game written on the OpenGL 4.3 library. You're steering a ship and your goal is to destroy the others placed around you.

## How to run

Either you clone the main branch, which is currently pretty huge due to the use of the Boost library. I'll try and shrink the dependencies folder in the future. Otherwhise you can clone the "noDependencies" branch. If so, you'll have to put the dependencies folder together yourself. For this you'll need the following libraries:
  - Boost
  - Glad
  - GLFW

The project file is already linked to {Working Directory}/dependencies.

## Settings

In the main.cpp you can change some settings. Most of it is either self-explanatory or shown through comments. 
To mention is the camera mode, which can be either `Camera_Mode::ISOMETRIC` or `Camera_Mode::FREE_FLOAT`.

## Controls

W - Move ship forwards
A - Turn ship left
S - Move ship backwards
D - Turn ship right

SPACE - Shoot the ships' cannons
