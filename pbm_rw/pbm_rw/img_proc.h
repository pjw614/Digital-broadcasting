//
//  img_proc.h
//  pbm_rw
//
//  Created by Artem Lenskiy on 4/30/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#ifndef __pbm_rw__img_proc__
#define __pbm_rw__img_proc__

#include <stdio.h>

//defines an RGB pixel
struct RGB {
    unsigned char R;
    unsigned char G;
    unsigned char B;
};
struct YCbCr {
    unsigned char Y;
    unsigned char Cb;
    unsigned char Cr;
};
//4:2:0 YCbCr Macroblock
struct YCbCr_MACRO {
    unsigned char Y[256];
    unsigned char Cb[64];
    unsigned char Cr[64];
};
struct RGBImage {
    short width;
    short height;
    unsigned char *ibuf;
};

void rgb2ycbcr(const struct RGB *rgb, struct YCbCr *ycc );
void rgb2y(const struct RGB *rgb, unsigned char  *y);
void chop ( int *r, int *g, int *b );
void ycbcr2rgb(struct YCbCr *ycc, struct RGB *rgb );

//void rgb2ycbcr(char *image, int cols, int rows);
//void ycbcr2rgb(char *input, int cols, int rows);


// Convert an RGB macro block ( 16x16 ) to 4:2:0 YCbCr sample blocks ( six 8x8 blocks ).
void macroblock2ycbcr (struct RGB *macro16x16,  struct YCbCr_MACRO *ycbcr_macro );
// Convert the six 8x8 YCbCr sample blocks to RGB macroblock(16x16).
void ycbcr2macroblock(struct YCbCr_MACRO *ycbcr_macro, struct RGB *macro16x16 );

void save_yccblocks( struct YCbCr_MACRO *ycbcr_macro, FILE *fpo );
void encode (struct RGBImage *image, FILE *fpo );
int get_yccblocks(struct YCbCr_MACRO *ycbcr_macro, FILE *fpi );
int decode_yccFrame (struct RGBImage *image, FILE *fpi );


#endif /* defined(__pbm_rw__img_proc__) */

