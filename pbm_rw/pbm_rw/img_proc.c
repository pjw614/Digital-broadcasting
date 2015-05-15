//
//  img_proc.c
//  pbm_rw
//
//  Created by Artem Lenskiy on 4/30/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#include "img_proc.h"

//void rgb2ycbcr(char *image, int cols, int rows){
//    
//    float T[3][3] = {{ .299,   .587,    .114},
//        {-.169,  -.334,    .5},
//        {.5,      -.419,    -.081}};
//    float offset[3] = {0, 128, 128};
//    float temp_pixel[3];
//    
//    int i,j;
//    char *pix;
//    for (j = 0; j < rows; ++j){
//        for (i = 0; i < cols; ++i){
//            pix = &image[j * cols * 3 + i * 3];
//            temp_pixel[0] =      T[0][0] * pix[0] +
//            T[0][1] * pix[1] +
//            T[0][2] * pix[2] +
//            offset[0];
//            temp_pixel[1] =  T[1][0] * pix[1] +
//            T[1][1] * pix[1] +
//            T[1][2] * pix[2] +
//            offset[1];
//            temp_pixel[2] =  T[2][0] * pix[2] +
//            T[2][1] * pix[1] +
//            T[2][2] * pix[2] +
//            offset[2];
//            
//            pix[0] = (char)temp_pixel[0];
//            pix[1] = (char)temp_pixel[1];
//            pix[2] = (char)temp_pixel[2];
//        }
//    }
//    
//}
//
//
//void ycbcr2rgb(char *input, int cols, int rows){
//    
//    float Ti[3][3] = {{ 1,   0.0,     1.402},
//        { 1,  -0.344,  -0.714},
//        { 1,   1.772,   0.0}};
//    float offset[3] = {0, 128, 128};
//    float temp_pixel[3];
//    
//    int i,j;
//    char *pix;
//    for (j = 0; j < rows; ++j){
//        for (i = 0; i < cols; ++i){
//            pix = &input[j * cols * 3 + i * 3];
//            temp_pixel[0] =      Ti[0][0] * (pix[0] - offset[0]) +
//            Ti[0][1] * (pix[1] - offset[1]) +
//            Ti[0][2] * (pix[2] - offset[2]);
//            temp_pixel[1] =  Ti[1][0] * (pix[1] - offset[1]) +
//            Ti[1][1] * (pix[1] - offset[1]) +
//            Ti[1][2] * (pix[2] - offset[2]);
//            temp_pixel[2] =  Ti[2][0] * (pix[2] - offset[2]) +
//            Ti[2][1] * (pix[1] - offset[1]) +
//            Ti[2][2] * (pix[2] - offset[2]);
//            
//            pix[0] = (char)temp_pixel[0];
//            pix[1] = (char)temp_pixel[1];
//            pix[2] = (char)temp_pixel[2];
//        }
//    }
//}



void rgb2ycbcr(const struct RGB *rgb, struct YCbCr *ycc ){
    //coefs summed to 65536(1<<16), so Y is always within [0, 255]
    ycc->Y = (unsigned char)((19595 * rgb->R + 38470 * rgb->G + 7471 * rgb->B ) >> 16);
    ycc->Cb = (unsigned char)((36962*(rgb->B - ycc->Y ) >> 16) + 128);
    ycc->Cr = (unsigned char)((46727*(rgb->R - ycc->Y ) >> 16) + 128);
}
//just convert an RGB pixel to Y component
void rgb2y(const struct RGB *rgb, unsigned char  *y){
    *y = (short)((19595 * rgb->R + 38470 * rgb->G + 7471 * rgb->B ) >> 16);
}
//limit value to lie within [0,255]
void chop ( int *r, int *g, int *b ){
    if ( *r < 0 ) *r = 0;
    else if ( *r > 255 ) *r = 255;
    if ( *g < 0 ) *g = 0;
    else if ( *g > 255 ) *g = 255;
    if ( *b < 0 ) *b = 0;
    else if ( *b > 255 ) *b = 255;
}
/*
 Convert from YCbCr to RGB domain. Using ITU-R standard:
 R = Y + 1.402Cr - 0.701
 G = Y - 0.714Cr - 0.344Cb + 0.529
 B = Y + 1.772Cb - 0.886
 Integer arithmetic is used to speed up calculations.
 */

void ycbcr2rgb(struct YCbCr *ycc, struct RGB *rgb ){
    int r, g, b;
    r = (ycc->Y + (91881 * ycc->Cr >> 16 ) - 179);
    g = (ycc->Y - ((22544 * ycc->Cb + 46793 * ycc->Cr) >> 16) + 135);
    b = (ycc->Y + (116129 * ycc->Cb >> 16) - 226);
    chop ( &r, &g, &b ); //enforce values to lie within [0,255]
    rgb->R = ( unsigned char ) r;
    rgb->G = ( unsigned char ) g;
    rgb->B = ( unsigned char ) b;
}


// Convert an RGB macro block ( 16x16 ) to 4:2:0 YCbCr sample blocks ( six 8x8 blocks ).
void macroblock2ycbcr (struct RGB *macro16x16,  struct YCbCr_MACRO *ycbcr_macro ){
    int i, j, k, r;
    struct YCbCr ycb;
    r = k = 0;
    for ( i = 0; i < 16; ++i ) {
        for ( j = 0; j < 16; ++j ) {
            if (!( i & 1 ) && !( j & 1 )){//one Cb, Cr for every 4 pixels
                rgb2ycbcr(&macro16x16[r], &ycb);//convert to Y, Cb, Cr values
                ycbcr_macro->Y[r] = ycb.Y;
                ycbcr_macro->Cb[k] = ycb.Cb;
                ycbcr_macro->Cr[k] = ycb.Cr;
                k++;
            } else {       //only need the Y component for other 3 pixels
                rgb2y ( &macro16x16[r], &ycbcr_macro->Y[r] );
            }
            r++;           //convert every pixel for Y
        }
    }
}

// Convert the six 8x8 YCbCr sample blocks to RGB macroblock(16x16).
void ycbcr2macroblock(struct YCbCr_MACRO *ycbcr_macro, struct RGB *macro16x16 ){
    int i, j, k, r;
    struct YCbCr ycb;
    r = k = 0;
    for ( i = 0; i < 16; ++i ) {
        for ( j = 0; j < 16; ++j ) {
            //one Cb, Cr has been saved for every 4 pixels
            if ( !( i & 1 ) && !( j & 1 ) ) {
                ycb.Y = ycbcr_macro->Y[r];
                ycb.Cb = ycbcr_macro->Cb[k];
                ycb.Cr = ycbcr_macro->Cr[k];
                ycbcr2rgb ( &ycb, &macro16x16[r]);
                k++;
            } else if(!( i & 1 )) {
                ycb.Y = ycbcr_macro->Y[r];
                ycbcr2rgb( &ycb, &macro16x16[r] );
            } else {
                ycb.Cb = ycbcr_macro->Cb[k - 8 + j/2];
                ycb.Cr = ycbcr_macro->Cr[k - 8 + j/2];
                ycb.Y = ycbcr_macro->Y[r];
                ycbcr2rgb( &ycb, &macro16x16[r] );
            }
            r++;
        }
    }
}


//save one YCbCr macroblock.
void save_yccblocks( struct YCbCr_MACRO *ycbcr_macro, FILE *fpo ){
    short block, i, j, k;
    unsigned char *py;
    //save four 8x8 Y sample blocks
    for ( block = 0; block < 4; block++ ) {
        if ( block < 2 )
            //points to beginning of block
            py = (unsigned char *) &ycbcr_macro->Y + 8*block;
        else
            py = (unsigned char *) &ycbcr_macro->Y+128+8*(block-2);
        for ( i = 0; i < 8; i++ ) { //one sample-block
            if ( i > 0 ) py += 16;    //advance py by 16( one row )
            for ( j = 0; j < 8; j++ ) {
                putc ( ( int )  *(py+j), fpo);   //save one byte of data
            }
        }
    }
    //save one 8x8 Cb block
    k = 0;
    for ( i = 0; i < 8; ++i ) {
        for ( j = 0; j < 8; ++j ) {
            putc( ( int ) ycbcr_macro->Cb[k++], fpo );
        } }
    //save one 8x8 Cr block
    k = 0;
    for ( i = 0; i < 8; ++i ) {
        for ( j = 0; j < 8; ++j ) {
            putc( ( int ) ycbcr_macro->Cr[k++], fpo );
        }
    }
}
/*
 Convert RGB to YCbCr and save the converted data.
 */
void encode (struct RGBImage *image, FILE *fpo ){
    short row, col, i, j, r;
    struct RGB macro16x16[256];    //16x16 pixel macroblock;24-bit RGB pixel
   // struct RGB macro16x16_test[256];
    struct YCbCr_MACRO ycbcr_macro;//macroblock for YCbCr samples
    struct RGB *p;                 //pointer to an RGB pixel
    static int nframe = 0;
    
    for ( row = 0; row < image->height; row += 16 ) {
        for ( col = 0; col < image->width; col += 16 ) {
            //points to beginning of macroblock
            p = (struct RGB *)image->ibuf+(row * image->width + col);
            r = 0;                    //note pointer arithmetic
            for ( i = 0; i < 16; ++i ) {
                for ( j = 0; j < 16; ++j )
                    macro16x16[r++] = (struct RGB) *p++;
                p += (image->width-16); //points to next row within macroblock
            }
            macroblock2ycbcr(macro16x16,  &ycbcr_macro );//RGB to YCbCr
            //ycbcr2macroblock(&ycbcr_macro, macro16x16_test);
            save_yccblocks( &ycbcr_macro, fpo );  //save one YCbCr macroblock
        } //for col
    } //for row
}

/*
 Get YCbCr data from file pointed by fpi. Put the four 8x8 Y sample
 blocks, one 8x8 Cb sample block and one 8x8 Cr sample block into a
 struct ( class object) of YCbCr_MACRO.
 Return: number of bytes read from file and put in
 YCbCr_MACRO struct.
 */
int get_yccblocks(struct YCbCr_MACRO *ycbcr_macro, FILE *fpi ){
    short r, row, col, i, j, k, n, block;
    short c;
    unsigned char *py;
    n = 0;
    //read data from file and put them in four 8x8 Y sample blocks
    for ( block = 0; block < 4; block++ ) {
        if ( block < 2 )
            //points to beginning of block
            py = ( unsigned char * ) &ycbcr_macro->Y + 8 * block;
        else
            py = (unsigned char *)&ycbcr_macro->Y + 128 + 8 * (block-2);
        for ( i = 0; i < 8; i++ ) {      //one sample-block
            if ( i > 0 ) py += 16;         //advance py by 16 (one row)
            for ( j = 0; j < 8; j++ ) {
                if((c = getc(fpi)) == EOF)//readonebyte break;
                    *(py + j) = (unsigned char) c; //save in YCbCr_MACRO struct
                n++;
            } //for j
        } //for i
    } //for block
    //now do that for 8x8 Cb block
    k = 0;
    for ( i = 0; i < 8; ++i ) {
        for ( j = 0; j < 8; ++j ) {
            if ( ( c = getc ( fpi )) == EOF )
                break;
            ycbcr_macro->Cb[k++] = (unsigned char )c;
            n++;
        }
    }
    //now do that for 8x8 Cr block
    k = 0;
    for ( i = 0; i < 8; ++i ) {
        for ( j = 0; j < 8; ++j ) {
            if ( ( c = getc ( fpi )) == EOF )
                break;
            ycbcr_macro->Cr[k++] = (unsigned char) c;
            n++;
        }
    }
    return n;
}

int decode_yccFrame (struct RGBImage *image, FILE *fpi ){
    short r, row, col, i, j;
    int n = 0;
    struct RGB macro16x16[256];    //16x16 pixel macroblock; 24-bit RGB pixel
    struct YCbCr_MACRO ycbcr_macro;//macroblock for YCbCr samples
    struct RGB *rgbp;               //pointer to an RGB pixel
    for ( row = 0; row < image->height; row += 16 ) {
        for ( col = 0; col < image->width; col += 16 ) {
            int m = get_yccblocks( &ycbcr_macro, fpi );
            if ( m <= 0 ) { printf("\nout of data\n"); return m;}
            n += m;
            ycbcr2macroblock(&ycbcr_macro, macro16x16);
            //points to beginning of macroblock
            rgbp = (struct RGB *)(image->ibuf) + (row * image->width + col);
            r = 0;
            for ( i = 0; i < 16; ++i ) {
                for ( j = 0; j < 16; ++j ) {
                    *rgbp++ = macro16x16[r++];
                }
                rgbp += (image->width - 16);//next row within macroblock
            }
        } //for col
    } //for row
    return n;
}