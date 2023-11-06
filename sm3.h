#pragma once
#ifndef SM3_H_INCLUDED
#define SM3_H_INCLUDED
#define N 50000000


#define FF1(x, y, z) ((x)^(y)^(z))
#define FF2(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define GG1(x, y, z) ((x)^(y)^(z))
#define GG2(x, y, z) (( (x) & (y) ) | ((~x) & (z)))
#define loopshift(x, n) ( ((x) << (n)) | ((x) >> (32 - (n))))
#define P0(x) ((x)^(loopshift((x), 9))^(loopshift((x), 17)) )
#define P1(x) ((x)^(loopshift((x), 15))^(loopshift((x), 23)) )
#define T1 0x79cc4519
#define T2 0x7a879d8a

typedef unsigned char   UChar;
UChar HexToAscii(unsigned int c);
int chartoword(unsigned char* Originaltext, int start);
void divide(unsigned char* Originaltext, int* group, int length);
void Getw(unsigned int w[], unsigned int group[], int llong);
unsigned int Step(unsigned int w[], int t);




#endif 