# CascadeNightmare

Application for generating difficult images for cascade classifier.

# Requirements

You need to install cmake and OpenCV:
 
 sudo apt-get install cmake libopencv-dev


# Building and launching

Note: if you are using OpenCV v.2, you need to remove opencv_imgcodecs from CMakeLists.txt!

 cmake .
 
 make
 
 ./CascadeNightmare

# Settings

Currently you can set some parameters on top of the main.cpp (requires rebuilding):

1)cascadePath - path, which leads to correct cascade classifier (in OpenCV format).

2)minSize, maxSize - boundary sizes for sliding window. Try to set same proportions as proportions in your cascade.

3)imw, imh - generated pictures width and height.

# Output

When parameters are set and the CascadeNighmare is launched, genetic algorithm will work for you.

Generations of pictures will be processed with set cascade, and the most fitting pictures and theirs crossovers will go to the next generations.

Fitting function is just number of detections found by cascade.

After each generation CascadeNightmare prints into stdout two numbers: maximum number of detections on single picture and minimum number (on this generation). If picture with new maximum found, CascadeNightmare will save It.