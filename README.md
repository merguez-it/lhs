# lhs

Lightweight HTTP Server

*This is a Work In Progress.* So test it but don't use it.

## Dependencies

### Compiling

* [cmake](http://www.cmake.org/)

### Documentation

* [doxygen](http://doxygen.org/) (optional)
* [graphviz](http://graphviz.org/) (optional)

### Testing 

* [gtest](http://code.google.com/p/googletest/) 

## Compiling

Install [cmake](http://www.cmake.org/cmake/help/install.html), [gtest](http://code.google.com/p/googletest/), [doxygen](http://www.stack.nl/~dimitri/doxygen/install.html) and [graphviz](http://www.graphviz.org/Download.php), then :

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

Not yet...

