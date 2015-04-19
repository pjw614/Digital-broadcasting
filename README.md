# Digital-broadcasting


###Real-time Transfer Protocol

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

###Informaation theory

######Problem 1: Implement a function that calculates the information entropy (Shannon entropy) of a given data.

1. To test the implemented code for entropy estimation, use the source code program as an input:

__./entropy < main.c__

2. Generate a 10000 bytes file with random characters. Use the following code

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
3. Use two PBM files attached as inputs for your program, compare the entropies of the content of each file.

######Problem 2: Compare Shannon's entropy and Kolmogorov's complexity

Modify your program in such way that its source size is minimized, then calculate its entropy and Kolmogorov complexity then compare them with the original code’s entropies and Kolmogorov complexity.

