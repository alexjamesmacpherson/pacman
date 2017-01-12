# Pac-Man (CS324)
Computer Graphics coursework, recreating Pac-Man in C++ from OpenGL first principles. For more info, please refer to the [project report](https://github.com/alexjamesmacpherson/pacman/blob/master/Report.pdf/ "View Report").

## Dependencies
There are a couple of dependencies required to compile and run the project.
* **C++ v11+** previous versions do not support some parts of the code
* **libpng** enables use of PNG sprites within the game

## Compiling the Project
#### Windows:
The project was developed on Windows, requiring MinGW *(or another UNIX-like terminal environment)* to both compile and run. To compile the project:
> make -f Makefile.windows pacman

#### Linux:
A slightly modified Makefile is required to compile on Linux:
> make -f Makefile.linux pacman

#### Additional Info:
Alternatively, a symbolic link can be created to the Makefile, removing the need to type *-f Makefile.foo* every time the project is compiled:
> ln -fs Makefile.foo Makefile

Replace **foo** with *windows* or *linux* depending on the compiling OS. The project can then be easily compiled using:
> make pacman

The flag *-B* forces a recompile, if required:
> make pacman -B

## Running the Project:
Once the code is compiled, the game is started using the same command on all systems.
> ./pacman

## Playing the Game:
1. The game is controlled by keyboard input only:
  * Arrow keys to move
  * ESC key to pause game and view help screen
  * Any key from pause screen to resume
  * ESC key from pause screen to quit
  * Any key from game over screen to restart
2. Don't let the ghosts catch you or you'll lose a life
3. Gobble pills and fruits to increase your score
4. Eat big pills to scare the ghosts, then you can consume THEM - every ghost you eat before the timer runs out increases the score multiplier
5. Eat all the pills to advance to the next level
6. When you have no lives left, the game is over and your highest score is saved to a file

## Note:
**I don't own, nor claim to own, Pac-Man.**

This project surmised a piece of university coursework which I'm pretty proud of.
