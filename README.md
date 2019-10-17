# RER_Tree - Rapidly Exploring Random Tree
This is an old project showing how a simple rapidly exploring random tree (RRT)
can work. This was mainly used to see how a RRT works and to learn a bit of C++.

![Rapidly Exploring Random Tree Demo](examples/example_simple_rer_tree.gif)
![Rapidly Exploring Random Tree Large Demo](examples/example_rer_tree.gif)


A rapidly exploring random tree is a search algorithm which we can employ 
in an n-dimensional space (not just 2d or 3d). We explore the space by picking
random points / configurations and try to connect them to the tree. The tree grows
this way until the end point is found. We can then traverse the tree from the root to
the end point. I came first upon this topic when reading this 
![blog post](http://the-witness.net/news/2012/12/mapping-the-islands-walkable-surfaces/).
by Casey Muratori for ![the witness](http://the-witness.net/).

## Getting Started

### Prerequisites
You need the ![sdl2](https://www.libsdl.org/download-2.0.php) graphics library and 
![cmake](https://cmake.org/) to build it.

### Installing
* clone or download the repo
`git clone git@github.com:Laeri/rer_tree.git`
* enter the folder and build it with cmake and make
`cmake .`
`make`
* run the binary in the bin folder
`bin/rer_tree`

## Usage
### Keybindings
<kbd>s</kbd> - starts or stops the tree expansion

<kbd>d</kbd> - set unset rect drawing (main purpose was to create images from the tree
    without having the rectangles there

<kbd>f</kbd> - toggle fill rectangles or draw outlines

<kdb>r</kbd> - reset everything

<kbd>left mouse</kbd> - click twice for setting start and end point
             while the tree is expanding you can manually "help" it a bit if you want to.

<kbd>ctrl<kbd> - while pressing the control key you can start drawing
       new rectangles. These rectangles represent obstacles

<kbd>Escape</kbd> or <kbd>q</kbd> - quit

