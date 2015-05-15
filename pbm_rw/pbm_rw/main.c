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
    struct image_header ih;
    unsigned long data_size;
    FILE *fp;
    
    if (argc != 3) {
        printf("Not enought arguments, the program call should as follows: %s <input.pbm> <output.pbm>", argv[0]);
        return 0;
    }
    
    data = readImage(argv[1], &data_size, &ih);
    
    struct RGBImage image = {ih.cols, ih.rows, data}; //initialize image
    
    fp = fopen("/Users/artemlenskiy/Desktop/temp.ycc", "wb+");
    encode (&image, fp );
    fclose(fp);
//
//    // now read temp.ycc and decode the image
//    // then save decode image
    fp = fopen("/Users/artemlenskiy/Desktop/temp.ycc", "r");
    decode_yccFrame (&image, fp);
    fclose(fp);

    if(data != NULL)
        writeImage(argv[2], image.ibuf, ih);
    
    return 0;
}
