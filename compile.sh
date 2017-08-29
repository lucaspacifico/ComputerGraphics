#!/bin/bash

sFileName="main"
sProjectPath="./"

g++ -O3 -mcmodel=medium -m64 -g -std=c++11 -I $sProjectPath -Wno-deprecated-declarations graphicPrimitives.cpp object3D.cpp $sFileName.cpp  -lcrypt -lglut -lGLU -lGL -lm -lpthread -o  $sFileName `pkg-config --cflags --libs opencv`


# g++ -o $sFileName $sFileName.cpp -lglut -lGLU -lGL -lm
