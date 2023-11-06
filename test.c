#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include "sm3.h"

int main()
{
	FILE* file;
	errno_t err;

	// ʹ��fopen_s���ļ�
	err = fopen_s(&file, "ccc.txt", "r");

	clock_t start, finish;


	if (file == NULL) {
		printf("�޷����ļ�\n");
		return 1;
	}
	unsigned char s[N] = {0};
	size_t bytesRead = fread(s, 1, sizeof(s) - 1, file);
	s[bytesRead] = '\0';
	fclose(file);
	UChar data[65] = { 0 };
	start = clock();

	operation_sm3(s, data);//�㷨�ӿ�

	finish = clock();
	double duration;
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("Use Time:%f\n", duration);
	printf("\n���յ�������:%s\n", data);
	int length = 0;
	for (int i = 0; s[i] != 0; i++, length++);
	printf("\n������ٶ��ǣ�%.2fMbps\n", 8 * length / (duration * 1000 * 1000));
}