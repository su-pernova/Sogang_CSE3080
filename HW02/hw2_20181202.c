#include<stdio.h>
#include<stdlib.h>
#define MAX_TERMS 100
#define COMPARE(x, y) (((x)<(y)) ? -1 : ((x)==(y)) ? 0 : 1)

//����ü polynomial�� �����Ѵ�
typedef struct {
	int coef;
	int expon;
} polynomial;
polynomial terms[MAX_TERMS];
int avail = 0; //���� �߰��ϱ� ������ �� �ִ� ��ġ

//���ο� ��(term)�� �߰��ϴ� �Լ� ����
void attach(int coefficient, int exponent) {
	if (avail >= MAX_TERMS) {
		fprintf(stderr, "���׽Ŀ� ���� �ʹ� �����ϴ�.");
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
		if (i == finD) { //������ �� ��½ÿ��� �޸� ������� �ʴ´�
			printf("(%d,%d)\n", terms[i].coef, terms[i].expon);
			continue;
		}
		printf("(%d,%d),", terms[i].coef, terms[i].expon);
	}
}

//�� ���׽��� ������ �����ϴ� �Լ� ���� : A(x) + B(x) = D(x)
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

	//A(x)�� ������ �׵��� �߰��Ѵ�
	for (; startA <= finA; startA++)
		attach(terms[startA].coef, terms[startA].expon);

	//B(x)�� ������ �׵��� �߰��Ѵ�.
	for (; startB <= finB; startB++)
		attach(terms[startB].coef, terms[startB].expon);

	*finD = avail - 1;
}

int main() {
	FILE* fp;
	int i = 0;
	int A_len, B_len;

	//A.txt�� �Է¹޾� �迭 terms�� ����
	fp = fopen("A.txt", "r");
	i = readPoly(i, fp);
	fclose(fp);
	A_len = i;

	//B.txt�� �Է¹޾� �迭 terms�� �̾ ����
	fp = fopen("B.txt", "r");
	i = readPoly(i, fp);
	fclose(fp);
	B_len = i - A_len;

	//���׽� ���� �Լ� call
	int startD, finD;
	avail = A_len + B_len;
	padd(0, A_len-1, A_len, avail-1, &startD, &finD);

	//���� ��� ���
	printPoly()

	system("pause");
	return 0;
}