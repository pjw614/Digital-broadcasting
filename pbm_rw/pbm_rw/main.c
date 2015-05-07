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

    char *data;
    struct image_header ih;
    unsigned long data_size;
    
    if (argc != 3) {
        printf("Not enought arguments, the program call should as follows: %s <input.pbm> <output.pbm>", argv[0]);
        return 0;
    }
    
    data = readImage(argv[1], &data_size, &ih);
    rgb2ycbcr(data, ih.cols, ih.rows);
    ycbcr2rgb(data, ih.cols, ih.rows);
    
    if(data != NULL)
        writeImage(argv[2], data, ih);
    
    return 0;
}
