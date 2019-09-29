# 3D OpenGL Spider
This project is a GLUT application that implements a spider that moves on the screen

## Dependencies
To compile the project the following libraries is needed: OpenGL Utility Toolkit (GLUT), Miscellaneous Mesa GL, X11 miscellaneous utility library, X11 Input extension library and OpenCV2. This can be done using the following commands
```shell
sudo apt-get install freeglut3-dev
sudo apt-get install mesa-utils
sudo apt-get install libxmu-dev libxi-dev
```
CMake is also needed to create the Makefile
```shell
sudo apt-get install cmake
```

## Install
To install, you need to clone this repository
```shell
git clone https://github.com/Exilio016/3D-Spider.git
```
In the project directory, you need to compile using cmake
```shell
cd 3D-Spider
cmake .
make
```
Then you can run the file Aranha2D
```shell
./Spider
```
## LICENSE
This project is licensed under the terms of the MIT license.

[LICENSE](https://github.com/Exilio016/CG-Spider-2D/blob/master/LICENSE)

