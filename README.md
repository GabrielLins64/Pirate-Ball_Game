<h1>Game: The Pirate Ball</h1>

A little score-based 3D game written purely in C++ and OpenGL, made for the Computer Graphics college course.

<hr>

<h2>Index</h2>

- [Requirements](#requirements)
- [Installation (Linux)](#installation-linux)
- [Executing the game (Linux)](#executing-the-game-linux)
- [Objective](#objective)
    - [Tips](#tips)
- [Controls](#controls)

<hr>

## Requirements

- C++ ([gcc compiler](https://gcc.gnu.org/));

- [OpenGL GLUT](https://www.opengl.org/resources/libraries/glut/);

<hr>

## Installation (Linux)

Install the dependencies:

`$ sudo apt install freeglut3-dev freeglut3`

Build the project. In the root directory:

`$ make`

<small>\*Or, alternatively:</small>

```
$ mkdir -p bin
$ g++ src/*.cpp -Iinclude -o bin/game -lglut -lGL -lGLU
```

<hr>

## Executing the game (Linux)

After the project build, do:

`$ ./bin/game`

<hr>

## Objective

Grab the gold bar and take it to the treasure chest. Do not let the blue wave touch you.

#### Tips

- When are you carrying the Gold Bar, you'll become slower and perform a lower jump. So, if the wave is nearby, just drop it and take back later.

<hr>

## Controls

**W** - Roll forward;<br>
**S** - Roll back;<br>
**A** - Roll left;<br>
**D** - Roll right;<br>
**Spacebar** - Jump;<br>
**i** - Increase ball size;<br>
**k** - Decrease ball size;<br>
**p** - Pause / Unpause;<br>
**esc** - Exit<br>
**Left mouse button** - Grab the gold bar;<br>
**Right mouse button** - Drop the gold bar;<br>
**Mouse scroll** - Change camera distance;<br>
**Mouse movement** - Rotate view.

<hr>
