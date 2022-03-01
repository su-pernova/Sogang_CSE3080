#include<stdio.h>
#include<stdlib.h>
#define MAX_TERMS 100
#define COMPARE(x, y) (((x)<(y)) ? -1 : ((x)==(y)) ? 0 : 1)

//구조체 polynomial을 선언한다
typedef struct {
	int coef;
	int expon;
} polynomial;
polynomial terms[MAX_TERMS];
int avail = 0; //값을 추가하기 시작할 수 있는 위치

//새로운 항(term)을 추가하는 함수 정의
void attach(int coefficient, int exponent) {
	if (avail >= MAX_TERMS) {
		fprintf(stderr, "다항식에 항이 너무 많습니다.");
		exit(1);
	}
	terms[avail].coef = coefficient;
	terms[avail++].expon = exponent;
}

int readPoly(int i, FILE* fp) {
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	while (!feof(fp)) {
		fscanf(fp, "%d", &terms[i].coef);
		fscanf(fp, "%d", &terms[i].expon);
		i++; }
	return i;
}

void printPoly() {
	for (int i = startD; i <= finD; i++) {
		if (i == finD) { //마지막 항 출력시에는 콤마 출력하지 않는다
			printf("(%d,%d)\n", terms[i].coef, terms[i].expon);
			continue;
		}
		printf("(%d,%d),", terms[i].coef, terms[i].expon);
	}
}

//두 다항식의 덧셈을 수행하는 함수 정의 : A(x) + B(x) = D(x)
void padd(int startA, int finA, int startB, int finB, int *startD, int *finD) {
	int coefficient;
	*startD = avail;

	while (startA <= finA && startB <= finB)
		switch (COMPARE(terms[startA].expon, terms[startB].expon)) {
		case-1: //A_expon < B_expon
			attach(terms[startB].coef, terms[startB].expon);
			startB++;
			break;

		case 0: //A_expon = B_expon
			coefficient = terms[startA].coef + terms[startB].coef;
			if (coefficient) attach(coefficient, terms[startA].expon);
			startA++; startB++;
			break;

		case 1: //A_expon > B_expon
			attach(terms[startA].coef, terms[startA].expon);
			startA++;
			break;
		}

	//A(x)의 나머지 항들을 추가한다
	for (; startA <= finA; startA++)
		attach(terms[startA].coef, terms[startA].expon);

	//B(x)의 나머지 항들을 추가한다.
	for (; startB <= finB; startB++)
		attach(terms[startB].coef, terms[startB].expon);

	*finD = avail - 1;
}

int main() {
	FILE* fp;
	int i = 0;
	int A_len, B_len;

	//A.txt를 입력받아 배열 terms에 저장
	fp = fopen("A.txt", "r");
	i = readPoly(i, fp);
	fclose(fp);
	A_len = i;

	//B.txt를 입력받아 배열 terms에 이어서 저장
	fp = fopen("B.txt", "r");
	i = readPoly(i, fp);
	fclose(fp);
	B_len = i - A_len;

	//다항식 덧셈 함수 call
	int startD, finD;
	avail = A_len + B_len;
	padd(0, A_len-1, A_len, avail-1, &startD, &finD);

	//덧셈 결과 출력
	printPoly()

	system("pause");
	return 0;
}