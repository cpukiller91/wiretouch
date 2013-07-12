//
//  interpolator.cpp
//  wiretouch_monitor
//
//  Created by Georg Kaindl on 12/7/13.
//
//

#include "interpolator.h"

wtmInterpolator::wtmInterpolator(int sw, int sh, int wf, int hf)
{
    this->sourceWidth           = sw;
    this->sourceHeight          = sh;
    this->widthUpsampling       = wf;
    this->heightUpsampling      = hf;
    this->interpolatedWidth     = (sw-1) * wf;
    this->interpolatedHeight    = (sh-1) * hf;
    this->_fx                   = 1.0 / (2.0 * (double)wf);
    this->_fy                   = 1.0 / (2.0 * (double)hf);
    
    this->interpolatedBuffer    = (double*)malloc(this->interpolatedHeight * this->interpolatedWidth * sizeof(double));
}

double wtmInterpolator::sensorValueAt(int x, int y)
{
    double val = 0.0;
    
    if (NULL != this->values) {
        x = MAX(0, MIN(x, this->sourceWidth-1));
        y = MAX(0, MIN(y, this->sourceHeight-1));
    
        val = (double)this->values[y * this->sourceHeight + x] / 1024.0;
    }
    
    return val;
}

void wtmInterpolator::runInterpolation(uint16_t* sensorValues)
{
    this->values = sensorValues;
    this->beginInterpolation();
    
    for (int i=0; i<this->sourceWidth-1; i++) {
        for (int j=0; j<sourceHeight-1; j++) {
            this->beginInterpolate4(i, j);
            
            for (int k=0; k<this->widthUpsampling; k++) {
                for (int l=0; l<this->heightUpsampling; l++) {
                    double val =  this->interpolate4(i, j, k, l, this->_fx * (1.0 + 2*k), this->_fy * (1.0 + 2*l));
                    val = MIN(MAX(0, val), 1.0);
                    
                    this->interpolatedBuffer[((j * this->heightUpsampling + l) * this->interpolatedWidth) + i * this->widthUpsampling + k] = val;
                }
            }
            
            this->finishInterpolate4(i, j);
        }
    }
    
    this->finishInterpolation();
    this->values = NULL;
}

void wtmInterpolator::beginInterpolation()
{
}

void wtmInterpolator::finishInterpolation()
{
}

void wtmInterpolator::beginInterpolate4(int x, int y)
{
}

void wtmInterpolator::finishInterpolate4(int x, int y)
{
}

double wtmInterpolator::interpolate4(int x, int y, int ix, int iy, float fx, float fy)
{
    return 0.0;
}