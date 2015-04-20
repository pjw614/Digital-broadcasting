# Digital-broadcasting

###Middle term project

The goal of the project is to develop image transmission software that uses RTP as a transport. Before the transmission an image is compressed. 

The project employs RTP transmission software we studied before, and color compression that employs imperfections in human visual system. Thus, the software should include the following functionality

* Reading and writing PBM files
* Color space conversion RGB to YCbCr and YCbCr to RGB
* Down and upsampling
* Sending and receiving data using RTP

For simplicity assume that receiver knows parameters of the image being transmitted. Make sure that the image is transmitted in a compressed form.

The main function of the sender can be represented in the flow chart shown below:

`Read a PBM image --> Convert to YCbCr --> Downsample Cb and Cr --> Send using RTP`

The main function of the receiver can be represented in the flow chart shown below:

`Receive using RTP --> Upsample --> Convert to RGB --> Write to a PBM file`


You have to decide how compressed image is transmitted. Either every channel is transmitted separately i.e. with three different `rtp_send_packets calls`, or first you combine three color channels Y, Cb and Cr into a contiguous memory block and then send it at once.

The submitted source codes should contain substantial amount of comments.



###Lab: Imaging basics

######Problem 1: Implement two function for reading and writing of the P5 and P6 type of PGM files i.e. binary and either grayscale or color images

The header for PGM files is defined as a C structure

```
struct image_header{
    char format[3]; //Image format, example: P5
    int rows;       //Image height
    int cols;       //Image width
    int levels;     //Number of gray/each color levels
};
```

######Problem 2: Implement RGB to YCbCr and YCbCr to RGB color space conversion

The formulas for converion RGB to YCbCr color spaces using integer arithmetics is given below
```
Y = ( 19595 * R + 38470 * G + 7471 * B ) >> 16;
Cb = ( 36962 * ( B - Y ) >> 16) + 128;
Cr = (46727 * ( R - Y ) >> 16) + 128;
```
and for coverting back to RGB from YCbCr is as follows
```
R = Y + (91881 * Cr >> 16) - 179;
G = Y -( ( 22544 * Cb + 46793 * Cr ) >> 16) + 135;
B = Y + (116129 * Cb >> 16) - 226;
```
######Problem 3: Image down- and up-sampling
Implement two functions. The first function accepts a YCbCr image and returns downsampled Cb and Cr channels according to 4:2:0 scheme.

The second function acceptes downsampled version of YCbCr image and upsamples it by simply copying each value to the four nearest neighbors in up-sampled image.
(http://i.stack.imgur.com/768xM.png)

######Problem 4: Calculate PSNR
Implement a function that accepts two argumetns, which is an original image and areconstructed image and returns Peak Signal-to-Noise Ratio (PSNR). The PSNR is calcualted as follows

`MSE = (1/(m*n))*sum(sum((f-g).^2))`

`PSNR = 20*log(max(max(f)))/((MSE)^0.5)`

######Problem 5: Test of the above function.

* Read a color PPM image.
* Convert RGB image to YCbCr.
* Down-sample YCbCr to 4:2:0, i.e. uses the 2:1 horizontal downsampling and the 2:1 vertical downsampling. You will irreversibly lose information here.
* Up-sample Cb and Cr channels to the original resolution
* Convert obtained YCbCr image to RGB image.
* Calculate PSNR between original RGB image and the reconstructed one.


###Lab: Real-time Transfer Protocol

######Problem 1:  Download the source codes attached. Build sender and receiver. To build use the following commands in the terminal:
__gcc receiver.c rtp.c -o receiver__

__gcc sender.c rtp.c -o sender__

Select an image file of your choice, and execute the receiver as follows

 __./receiver 12345 > image_rcv.jpg__

 then in a new terminal window execute

 __./sender 127.0.0.1 12345 < image.jpg__

 where 12345 is a port of a receiver and image.jpg is your image file.
 The symbols < and > are used to redirect standard output and input.  By typing sender < image.jpg we redirect input from a file, instead of a keyboard and by typing receiver > image_rcv.jpg  we redirect output to  a file instead of a screen.

######Problem 2:  Study source codes very carefully and add detailed comments for as many statements as you think is necessary, keeping in mind that the more the better. The goal of this problem is to understand the codes in depth.  

###Lab: Information theory

######Problem 1: Implement a function that calculates the information entropy (Shannon entropy) of a given data.

* To test the implemented code for entropy estimation, use the source code program as an input:

__./entropy < main.c__

* Generate a 10000 bytes file with random characters. Use the following code

```
//randtest.cpp: Generates 10000 bytes of data
//Compile: gcc -o randtest randtest.cpp
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main() {
    int x;
    char *pc = (char *)&x;
    for ( int i = 0; i < 10000; ++i ){ //output 10000 bytes
        x = rand();                    //output only lowest byte
        putchar ( (int) *pc );         //output one byte
    }
    return 0;
}
```
* Use two PBM files attached as inputs for your program, compare the entropies of the content of each file.

######Problem 2: Compare the Shannon's entropy and the Kolmogorov's complexity

Modify your program in such way that its source size is minimized, then calculate its entropy and Kolmogorov complexity then compare them with the original code’s entropies and Kolmogorov complexity.

