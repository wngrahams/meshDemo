//
//  Geometry.h
//  meshReduction
//
//  Created by Graham Stubbs on 8/24/17.
//  Copyright (c) 2017 Graham Stubbs. All rights reserved.
//

#ifndef __meshReduction__Geometry__
#define __meshReduction__Geometry__

#include <stdio.h>
#include <iostream>

class Geometry {
public:
    Geometry(){};
    
    Geometry (float *normals, float *vertices, int *indices, int numTris){
        
        for (int i=0; i<numTris*3; i++) {
            std::cout << vertices[3*i] << std::endl;
            std::cout << vertices[3*i + 1] << std::endl;
            std::cout << vertices[3*i + 2] << std::endl;
            
//            std::cout << normals[i] << std::endl;
        }
        
        delete normals;
        delete vertices;
        delete indices;
    };
};

#endif /* defined(__meshReduction__Geometry__) */
