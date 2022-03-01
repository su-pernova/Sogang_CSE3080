#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max_string_size 100
#define max_pat_size 100

int failure[max_pat_size];
char string[max_string_size];
char pat[max_pat_size];

//KMP(Knuth, Morris, Pratt) 패턴 매칭 알고리즘
int pmatch(char *string, char *pat) { 
	int i = 0, j = 0;
	int lens = strlen(string);
	int lenp = strlen(pat);
	while (i < lens && j < lenp) {
		if (string[i] == pat[j]) {
			i++;
			j++;
		}
		else if (j == 0) i++;
		else j = failure[j - 1] + 1;
	}
	return ((j == lenp) ? (i - lenp) : -1);
}

//패턴의 실패 함수 계산
void fail(char*pat) { 
	int i, n = strlen(pat);
	failure[0] = -1;

	for (int j = 1; j < n; j++) {
		i = failure[j - 1];
		while ((pat[j] != pat[i + 1] && (i >= 0))) i = failure[i];
		if (pat[j] == pat[i + 1]) failure[j] = i + 1;
		else failure[j] = -1;
	}
}

int main() {
	//kmp.txt 파일의 내용을 string, pat 배열에 입력받는다
	FILE* fp;
	fp = fopen("kmp.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	while (!feof(fp)) {
		fscanf(fp, "%s", &string);
		fscanf(fp, "%s", &pat); }
	fclose(fp);

	//KMP 알고리즘 실행 및 결과 출력
	fail(pat);
	printf("%d\n", pmatch(string, pat));

	system("pause");
	return 0;
}