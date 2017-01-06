# Pac-Man (CS324)
Computer Graphics coursework, recreating Pac-Man in C++ from OpenGL first principles.

## Dependencies
There are a couple of dependencies required to compile and run the project.
* **C++ v11+** previous versions do not support some parts of the code
* **libpng** enables use of PNG sprites within the game

## Compiling the Project
#### Windows:
The project was developed on Windows, requiring MingW64 (Git BASH) to both compile and run.
> make -f Makefile coursework

#### Linux:
A slightly modified Makefile is required to compile on Linux.
> make -f Makefile.linux coursework

## Running the Project:
Once the code is compiled, the game is started using the same command on all systems.
> ./coursework

## Playing the Game:
The game is controlled by keyboard input only.
* Arrow keys to move
* ESC key to pause game and view help screen
* ESC key from pause screen to quit
* Any key from pause screen to resume
* Any key from game over screen to restart

## Note:
**I don't own, nor claim to own, Pac-Man.**
This project surmised a piece of university coursework which I'm pretty proud of.
