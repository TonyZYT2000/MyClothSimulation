# My Cloth Simulation

## Description

This is a simple cloth simulation program written in C++ and OpenGL. It is based on spring-damper model and uses a 50 \* 50 mesh with thousands springs and dampers interconnected. There are also gravity and wind force acting on the cloth. With all these forces and diverse controls, the program presents a intuitive demonstration of cloth simulation.

## Build

The project is managed using Visual Studio on Win10. It depends on OpenGL, GLEW and GLM. With these dependencies configured correctly, this project should also be able to run on OS X and Linux. Instructions can be found [here](http://ivl.calit2.net/wiki/index.php/BasecodeCSE167F20).

## Usage

- Press `W`, `A`, `S` and `D` to move the cloth around.
- Press `Q` and `E` to rotate the cloth.
- Press left `Shift` and left `Ctrl` to move the cloth up and down.
- Press arrow keys, right `Shift` and right `Ctrl` to control winds.
- Press `P` to turn on and off polygon view.
- Press `Space` to pause the animation.
- Press `Backspace` to release and recapture the cloth.

## Artworks!

Here are some GIFs of the running program. Try download it and interact with it!

![](https://cdn.jsdelivr.net/gh/TonyZYT2000/ImageHost@master/Cloth1.gif)

![](https://cdn.jsdelivr.net/gh/TonyZYT2000/ImageHost@master/Cloth2.gif)

![](https://cdn.jsdelivr.net/gh/TonyZYT2000/ImageHost@master/Cloth3.gif)