//
//  main.c
//  im_comp
//
//  Created by Artem Lenskiy on 5/19/15.
//  Copyright (c) 2015 Artem Lenskiy. All rights reserved.
//

#include <stdio.h>
#include "pbm.h"
#include "im_comp.h"

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

	short row, col, i, j, r;
	unsigned char *p;
	unsigned char blocks8x8[64][64];

	for ( row = 0; row < ih.rows; row += 16 ) {
		for ( col = 0; col < ih.cols; col += 16 ) {
			//points to beginning of macroblock
			p = data+(row * ih.cols + col);
			r = 0;                    //note pointer arithmetic
			for ( i = 0; i < 16; ++i ) {
				for ( j = 0; j < 16; ++j )
					blocks8x8[r++] = (struct RGB) *p++;
				p += (ih.cols-16); //points to next row within macroblock
			}
		} //for col
	} //for row


	//    * Read a grayscale P5 type PBM image
	//    * Split into 8 x 8 blocks and apply DCT to every block
	//    * Quantize DCT coefficients
	//    * Apply zigzag reordering
	//    * Apply run-level encoding and store the codes in `Run3D  runs[64];`
	//    * Print them on the screen, while running the program redirect standard stream to a file i.e.
	//        __./encode image.pbm > run3d.code__
	return 0;
}
