//
//  BinaryStlLoader.h
//  meshReduction
//
//  Created by Graham Stubbs on 8/23/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#ifndef __meshReduction__BinaryStlLoader__
#define __meshReduction__BinaryStlLoader__

#include <iostream>
#include <string>
#include <fstream>

#include "Geometry.h"

const int HEADER_SIZE = 80;
const int TRIANGLE_COUNT_SIZE = 4;
const int COORD_SIZE = 4;
const int UNUSED_BUFFER_SIZE = 2;

Geometry loadBinarySTL (std::string filename);


#endif /* defined(__meshReduction__BinaryStlLoader__) */
