#include <stdio.h>
#include <math.h>
void main(){char d[1048576];int l,h[256],i,c;float H;while((c=getchar())!=-1)d[l++]=c;for(i=0;i<l;i++)h[(int)d[i]]++;for(i=0;i<256;i++)if(h[i])H-=(float)h[i]/l*log2((float)h[i]/l);printf("%f\n",H);}
