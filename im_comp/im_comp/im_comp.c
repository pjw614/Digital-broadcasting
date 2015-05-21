//
//  im_comp.c
//  im_comp
//
//  Created by Artem Lenskiy on 5/19/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#include "im_comp.h"

int Qstep = 12;

void quantize_block ( short coef[8][8] ){
	int i ,j;
    for ( i = 0; i < 8; i++ )
        for ( j = 0; j < 8; j++ )
            coef[i][j] = ( short ) round ( (double)coef[i][j] / Qstep );
}
//inverse quantize one block
void inverse_quantize_block ( short coef[8][8] )
{
	int i ,j;
    for ( i = 0; i < 8; i++ )
        for ( j = 0; j < 8; j++ )
            coef[i][j] = (short) (  coef[i][j] * Qstep );
}

//input : Y,  output : Yr
void reorder ( short Y[8][8], short Yr[8][8] ){
    int k, i1, j1;
    k = 0;
    int i ,j;
    for ( i = 0; i < 8; i++ ){
        for ( j = 0; j < 8; j++ ){
            i1 =  zigzag[k] / 8;
            j1 = zigzag[k] % 8;
            Yr[i][j] = Y[i1][j1];
            k++; }
    } }
//input : Yr, output : Y
void reverse_reorder ( short Yr[8][8], short Y[8][8] ){
    int k, i1, j1;
    k = 0;
    int i ,j;
    for ( i = 0; i < 8; i++ ){
        for ( j = 0; j < 8; j++ ){
            i1 = zigzag[k] / 8;
            j1 = zigzag[k] % 8;
            Y[i1][j1] = Yr[i][j];
            k++; }
    } }




void run_block ( short Y[8][8], struct Run3D runs[] ){
    unsigned char run_length = 0, k = 0;
    int i ,j;
    for ( i = 0; i < 8; i++ ) {
        for ( j = 0; j < 8; j++ ) {
            if(Y[i][j]==0 ){
                run_length++;
                continue; }
            runs[k].run = run_length;
            runs[k].level = Y[i][j];
            runs[k].last = 0;
            run_length = 0;
            k++;
        }
    } if ( k > 0 )
        runs[k-1].last = 1;
    else {
        runs[0].run = 64;
        runs[0].level = 0;
        runs[0].last = 1;
    }
}

void run_decode (struct Run3D runs[], short Y[8][8] ){
    int i, j, r, k = 0, n = 0;
    while ( n < 64 ) {
        for(r = 0; r < runs[k].run; r++){
            i = n / 8;
            j = n % 8;
            Y[i][j] = 0;
            n++;
        }
        if ( n < 64 ){
            i = n / 8;
            j = n % 8;
            Y[i][j] = runs[k].level;
            n++;
        }
        
        if ( runs[k].last != 0 ) break;
        k++;
    }
    //run of 0s to end
    while ( n < 64 ) {
        i = n / 8;
        j = n % 8;
        Y[i][j] = 0;
        n++;
    }
}
