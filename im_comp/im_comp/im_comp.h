//
//  im_comp.h
//  im_comp
//
//  Created by Artem Lenskiy on 5/19/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#ifndef __im_comp__im_comp__
#define __im_comp__im_comp__

#include <stdio.h>

struct Run3D {
    unsigned char run;
    short level;
    char last;
};

int zigzag[] = {
    0, 1, 8,16, 9, 2, 3,10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};

void quantize_block ( short coef[8][8] );
//inverse quantize one block
void inverse_quantize_block ( short coef[8][8] );


//input : Y,  output : Yr
void reorder ( short Y[8][8], short Yr[8][8] );
//input : Yr, output : Y
void reverse_reorder ( short Yr[8][8], short Y[8][8] );


void run_block ( short Y[8][8], struct Run3D runs[] );
void run_decode (struct Run3D runs[], short Y[8][8] );


#endif /* defined(__im_comp__im_comp__) */
