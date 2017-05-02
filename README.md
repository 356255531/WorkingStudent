# Visual Navigation Project on Raspberry Pi 3

This repository includes the research materials and gpu based computer vision task implementations e.g. the basic program framework from @Chris Cummings (http://robotblogging.blogspot.de/2013/10/gpu-accelerated-camera-processing-on.html) and visual navigation project of chair navigation and communication (Technical University of Munich) as well as the official demo from userland. This is a part of my working student work an the chair.

## dependency
All you need is to build and install the source files from userland (https://github.com/raspberrypi/userland).
But you of course need to have git.

## Usage
We are currently working an the project directory and a robust harris detector were implemented there.
You have the full functionality by input the following code:
```
    git clone git@github.com:356255531/WorkingStudent.git
    cd WorkingStudent/Project/
    build .
    make
    ./picam
```
For another demo program just enter the folder and play around.

The stable version is under the master brach while the new and probably still in developing features will be placed under the dev branch.