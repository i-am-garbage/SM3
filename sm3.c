#include "sm3.h"
#include<stdlib.h>
#include<stdio.h>

unsigned int A = 0x7380166f, B = 0x4914b2b9, C = 0x172442d7, D = 0xda8a0600, E = 0xa96F30bc, F = 0x163138aa, G = 0xE38dee4d, H = 0xb0fb0e4e;
unsigned int A0 = 0x7380166f, B0 = 0x4914b2b9, C0 = 0x172442d7, D0 = 0xda8a0600, E0 = 0xa96F30bc, F0 = 0x163138aa, G0 = 0xE38dee4d, H0 = 0xb0fb0e4e;

UChar HexToAscii(unsigned int c) {
	if (c > 9) {
		return (c + 55);
	}
	else {
		return (c + 48);
	}
}

//字节转换
int chartoword(unsigned char* Originaltext, int start)
{
	return((int)((Originaltext[start] & 0x000000ff) << 24) | (int)((Originaltext[start + 1] & 0x000000ff) << 16) | (int)((Originaltext[start + 2] & 0x000000ff) << 8) | (int)((Originaltext[start + 3] & 0x000000ff)));
}

void divide(unsigned char* Originaltext, int* group, int length)
{
	int temp = length / 4, l = length, llong = length / 64 + (length % 64) / 56;
	while (l >= 0)
	{
		if (l / 4)
		{
			for (int j = 0; j < temp; j++, l -= 4)
			{
				group[j] = chartoword(Originaltext, j * 4);
			}
		}
		else
		{
			Originaltext[temp * 4 + (l + 4) % 4] = 0x80;
			for (int j = length + 1; j <= 3 + temp * 4; Originaltext[j] = 0, j++);
			group[temp] = chartoword(Originaltext, temp * 4);
			l -= 4;
		}
	}
	for (int i = temp + 1; i < 15 + 16 * llong; i++)
	{
		group[i] = 0;
	}
}

void Getw(unsigned int w[], unsigned int group[], int llong)
{
	int i ,j;
	for (i = 0; i < llong + 1; i++)
	{
		for (j = 0; j < 16; j++)
		{
			w[i * 132 + j] = group[i * 16 + j];
			//printf("第%2d组明文是0x%08X\n", i * 132 + j + 1, w[i * 132 + j]);   111
		}
		for (j = 16; j < 68; w[i * 132 + j++] = P1(w[i * 132 + j - 16] ^ w[i * 132 + j - 9] ^ (loopshift(w[i * 132 + j - 3], 15))) ^ (loopshift(w[i * 132 + j - 13], 7)) ^ w[i * 132 + j - 6]);
			//printf("第%2d组明文是0x%08X\n", i * 132 + j, i * 132 + w[j - 1]);  222
		for (j = 68; j < 132; w[i * 132 + j++] = w[i * 132 + j - 68] ^ w[i * 132 + j - 64]);
			//printf("第%2d组明文是0x%08X\n", i * 132 + j, w[i * 132 + j - 1]);  333
		//printf("第%2d组明文是0x%08X\n", i * 132 + 132, w[i * 132 + 131]);     444
	}
}

unsigned int Step(unsigned int w[], int llong)
{
	
	unsigned int SS1, SS2, TT1, TT2, T,tt,loop;
	for (int t = 0; t <= 63 + llong * 64; t++)
	{
		tt = t % 64;
		loop = t / 64;
		if (tt == 0 && t != 0)
		{
			A = A ^ A0;
			A0 = A;
			B = B ^ B0;
			B0 = B;
			C = C ^ C0;
			C0 = C;
			D = D ^ D0;
			D0 = D;
			E = E ^ E0;
			E0 = E;
			F = F ^ F0;
			F0 = F;
			G = G ^ G0;
			G0 = G;
			H = H ^ H0;
			H0 = H;
		}
		if (tt >= 0 && tt <= 15)
		{
			T = T1;
			SS1 = loopshift(((loopshift(A, 12)) + E + loopshift(T, tt)), 7);
			SS2 = SS1 ^ loopshift(A, 12);
			TT1 = FF1(A, B, C) + D + SS2 + w[68 + tt + 132 * loop];
			TT2 = GG1(E, F, G) + H + SS1 + w[tt + 132 * loop];
		}
		else if (tt >= 16 && tt <= 63)
		{
			T = T2;
			SS1 = loopshift(((loopshift(A, 12)) + E + loopshift(T, tt)), 7);
			SS2 = SS1 ^ loopshift(A, 12);
			TT1 = FF2(A, B, C) + D + SS2 + w[68 + tt + 132 * loop];
			TT2 = GG2(E, F, G) + H + SS1 + w[tt + 132 * loop];
		}
		D = C;
		C = loopshift(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = loopshift(F, 19);
		F = E;
		E = P0(TT2);
	}
	A = A ^ A0;
	B = B ^ B0;
	C = C ^ C0;
	D = D ^ D0;
	E = E ^ E0;
	F = F ^ F0;
	G = G ^ G0;
	H = H ^ H0;
	//printf("第%2d轮加密后的密文是%08X %08X %08X %08X %08X %08X %08X %08X\n", t, A, B, C, D, E, F, G, H);   555
}

UChar operation_sm3(unsigned char s[], UChar data[])
{
	unsigned int length = 0;
	int xx = 0;
	unsigned int w[N] = {0};
	for (int i = 0; s[i] != 0; i++, length++)
	{
		xx += 8;
		w[i] = 0;
	}
		
	unsigned int llong = length / 64 + (length % 64) / 56;
	unsigned int group[N] = { 0 };
	//unsigned int* group = (int*)malloc(sizeof(int) * N);
	
	//unsigned int* w = (int*)malloc(sizeof(int) * N);
	group[(llong + 1) * 16 - 1] = xx;

	divide(s, group, length);
	Getw(w, group, llong);
	//printf("                    A        B        C        D        E        F        G        H\n");   666
	//printf("在初始时候的的密文是%08X %08X %08X %08X %08X %08X %08X %08X\n", A, B, C, D, E, F, G, H);    777
	Step(w, llong);
	//printf("\n最终的密文是：%08X%08X%08X%08X%08X%08X%08X%08X", A, B, C, D, E, F, G, H);
	unsigned int S[8] = { A, B, C, D, E, F, G, H };
	unsigned int temp;
	//printf("\n%d", sizeof(S));
	int bb = sizeof(S);
	for (int i = 0; i < sizeof(S) / 4; i++) {
		unsigned int aa = S[i];
		//printf("\n%08x", aa);
		temp = (S[i] >> 28) & 0x0000000f;   // 取16进制数高位放到 HexToAscii 函数中转成字符
		*(data + 8 * i) = HexToAscii(temp);
		temp = S[i] & 0x0f000000;   // 取16进制数低位放到 HexToAscii 函数中转成字符
		*(data + i * 8 + 1) = HexToAscii(temp >> 24);
		temp = S[i] & 0x00f00000;
		*(data + i * 8 + 2) = HexToAscii(temp >> 20);
		temp = S[i] & 0x000f0000;
		*(data + i * 8 + 3) = HexToAscii(temp >> 16);
		temp = S[i] & 0x0000f000;
		*(data + i * 8 + 4) = HexToAscii(temp >> 12);
		temp = S[i] & 0x00000f00;
		*(data + i * 8 + 5) = HexToAscii(temp >> 8);
		temp = S[i] & 0x000000f0;
		*(data + i * 8 + 6) = HexToAscii(temp >> 4);
		temp = S[i] & 0x0000000f;
		*(data + i * 8 + 7) = HexToAscii(temp);
	}
	return data;
}