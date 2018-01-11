//
// RunLoop.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		Light
//
// Created by 	Ruslan Maslouski, 1/11/18 23:56
// 				Ruslan Maslouski
//
// Copyright 	(c) Ruslan Maslouski, 2018
// Licence		<#licence#>
//
// See 			RunLoop.h and ReadMe.txt for references
//


// Library header
#include "RunLoop.h"

// Code

void RunLoop::addRepeatBlock(RunLoopHandlerFunction handler) {
    _items[_index] = handler;
    _index += 1;
}
void RunLoop::process() {
    for (int i = 0; i < _index; i += 1) {
        RunLoopHandlerFunction handler = _items[i];
        handler();
    }
}

RunLoop MainRunLoop;
