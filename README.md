# lhs

Lightweight HTTP Server

*This is a Work In Progress.* So test it but don't use it.

## Dependencies

### Compiling

* [cmake](http://www.cmake.org/)
* [MinGW](http://www.mingw.org) - on Windows

### Documentation

* [doxygen](http://doxygen.org/) (optional)
* [graphviz](http://graphviz.org/) (optional)

### Testing 

* [gtest](http://code.google.com/p/googletest/)

## Compiling

Install [cmake](http://www.cmake.org/cmake/help/install.html), [doxygen](http://www.stack.nl/~dimitri/doxygen/install.html) and [graphviz](http://www.graphviz.org/Download.php), then :

### On MacOSX, or Linux

    cd <path to lhs root>
    mkdir build
    cd build
    cmake ..
    make
    make test
    make doc
    make install

### On Windows

    cd <path to lhs root>
    mkdir build
    cd build
    cmake -G "MinGW Makefiles" ..
    mingw32-make
    mingw32-make test
    mingw32-make doc
    mingw32-make install

