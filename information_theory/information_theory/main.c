#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXLEN 1048576 //maximum data size is 1Mb
#define ALPHABET_SIZE 256
//estimate P(s_i)
void makehist(unsigned char *data,int *hist,int length){
    int i,histlen;
    histlen = 0;
    //Build the histogram
    for(i = 0; i < length; i++)
        hist[ (unsigned)data[i] ]++;
    //Calcualte a number of different symobls in the data
    for (i = 0; i < ALPHABET_SIZE; i++)
        if (hist[i] != 0) histlen++;
}

double entropy(int *hist,int len){
    int i;
    double H;
    H=0;
    for(i = 0; i < ALPHABET_SIZE; i++)
        if (hist[i] != 0) // Only consider the symbols at appear at lest once
            //The probbaility is estimated as P(s_i) = (double)hist[i] / len
            H -= (double)hist[i] / len * log2((double)hist[i] / len);
    
    return H;
}

int main(void){
    unsigned char data[MAXLEN];
    int len, hist[ALPHABET_SIZE], c;
    double H;

    while ((c = getchar()) != EOF)
        data[len++] = c;
    
    makehist(data,hist,len);
    
    H = entropy(hist,len);
    printf("%lf\n",H);
    return 0;
}



//#include <stdio.h>
//unsigned int PRNG(){
//    // our initial starting seed is 5323
//    static unsigned int nSeed = 5323;
//    // Take the current seed and generate a new value from it
//    // Due to our use of large constants and overflow, it would be
//    // very hard for someone to predict what the next number is
//    // going to be from the previous one.
//    nSeed = (8253729 * nSeed + 2396403);
//    // Take the seed and return a value between 0 and 32767
//    return nSeed  % 32767;
//}
//
//int main(){
//    // Print 100 random numbers
//    for (int nCount=0; nCount < 100; ++nCount){
//        printf("%5u\t", PRNG());
//        // If we've printed 5 numbers, start a new column
//        if ((nCount+1) % 5 == 0)
//            printf("\n");
//    }
//}



//randtest.cpp: Generates 10000 bytes of data
//Compile: gcc -o randtest randtest.cpp
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//int main() {
//    int x;
//    char *pc = (char *)&x;
//    for ( int i = 0; i < 10000; ++i ){ //output 10000 bytes
//        x = rand();                    //output only lowest byte
//        putchar ( (int) *pc );         //output one byte
//    }
//    return 0;
//}

