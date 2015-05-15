//
//  main.c
//  pbm_rw
//
//  Created by Artem Lenskiy on 4/16/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#include "pbm.h"
#include "img_proc.h"

int main(int argc, const char * argv[]) {

    unsigned char *data;
    unsigned char data2[16*16*3];
    struct image_header ih;
    unsigned long data_size;
    FILE *fp;
    
    if (argc != 3) {
        printf("Not enought arguments, the program call should as follows: %s <input.pbm> <output.pbm>", argv[0]);
        return 0;
    }
    
    data = readImage(argv[1], &data_size, &ih);
    
    struct RGBImage image = {ih.cols, ih.rows, data}; //initialize image
    
    
    
//    struct RGB macro16x16[256];
//    struct RGB macro16x16_test[256];
//    struct RGB *p = (struct RGB *) data;
//    struct RGB *rgbp = (struct RGB *) data2;
//    struct YCbCr_MACRO ycbcr_macro;;
//    
//    int i,j, r = 0;
//    for ( i = 0; i < 16; ++i ) {
//        for ( j = 0; j < 16; ++j )
//            macro16x16[r++] = (struct RGB) *p++;
//        p += (image.width - 16); //points to next row within macroblock
//    }
//    
//    macroblock2ycbcr(macro16x16, &ycbcr_macro);
//    ycbcr2macroblock(&ycbcr_macro, macro16x16_test);
//    
//    r = 0;
//    for ( i = 0; i < 16; ++i ) {
//        for ( j = 0; j < 16; ++j ) {
//            *rgbp++ = macro16x16_test[r++];
//        }
//        rgbp += (image.width - 16);//next row within macroblock
//    }
//    image.ibuf = data2;
    
    
    fp = fopen("/Users/artemlenskiy/Desktop/temp.ycc", "wb+");
    encode (&image, fp );
    fclose(fp);
//
//    // now read temp.ycc and decode the image
//    // then save decode image
    fp = fopen("/Users/artemlenskiy/Desktop/temp.ycc", "r");
    decode_yccFrame (&image, fp);
    fclose(fp);
//
    if(data != NULL)
        writeImage(argv[2], image.ibuf, ih);
    
    return 0;
}
