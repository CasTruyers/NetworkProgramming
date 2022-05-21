# NetworkProgramming : Connect-Four

*This program is writtin on a UNIX based machine and might not work on Windows*

## Description:

Connect Four is a two-player connection game, in which the players take turns dropping tokens into a seven-column, six-row vertically suspended grid. The pieces fall straight down, occupying the lowest available space within the column. The objective of the game is to be the first to form a horizontal, vertical, or diagonal line of four of one's own tokens.

## Features:

- Start Multiple Connect-Four games simultaneously
- Play against each other from across the globe.
- Play through a GUI or terminal

## How to compile:
- clone repository
- In the cloned project, create folder **"libraries"**
- In the folder "libraries", create folder **"include"**
- download **zmq_addon** library using [brew](https://formulae.brew.sh/formula/cppzmq) or by cloning the [github repository](https://github.com/zeromq/cppzmq/releases/tag/v4.8.1)
- Place the file **"zmq_addon.hpp"** in the include folder
- run the command: **"Cmake -B build/"**
- run **"make"** in the build folder
- Client and server application is created in the build folder
