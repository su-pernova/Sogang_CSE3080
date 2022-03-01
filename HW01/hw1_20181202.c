#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void selection_sort(int list[], int n);

int main() {
	int i, n, data, j = 0;
	int *list;
	double duration;
	long repetitions;

	//lab.data ������ �����Ѵ�
	FILE* fp;
	fp = fopen("lab1.data", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error\n");
		exit(1); }

	//�ð� ���� ����(���� ���� �ð� ����)
	clock_t start;
	repetitions = 0;
	start = clock();

	fscanf(fp, "%d", &n); //n = �迭�� ũ��
	list = (int*)malloc(sizeof(int)*n);	//���� �Ҵ翡 ���� �迭 ����

	//�迭�� content�� lab.data ���Ͽ��� �Է¹ޱ�
	while (fscanf(fp, "%d", &data) != -1) {
		list[j] = data;
		j++; }

	//�迭�� content ���
	printf("Input : ");
	for (i = 0; i < n; i++) {
		if (i == 0) printf("%d", list[i]);
		else printf(", %d", list[i]); }
	printf("\n");

	//Selection Sort ����
	do {
		repetitions++;
		selection_sort(list, n);
	} while (clock() - start < 1000);

	duration = ((double)(clock() - start)) / CLOCKS_PER_SEC;
	duration /= repetitions;

	//Selection Sort ��� ���
	printf("Sorted : ");
	for (i = 0; i < n; i++) {
		if (i == 0) printf("%d", list[i]);
		else printf(", %d", list[i]);
	}
	printf("\n\n");

	//���α׷� ���� �ð� ���
	printf("< Performance of Selection Sort >\n");
	printf(" - n : %d \n - repetitions : %d \n - time : %f\n\n", n, repetitions, duration);

	free(list);
	fclose(fp);
	system("pause");
	return 0;
}

void selection_sort(int list[], int n) {
	int i, j;
	int min, temp;
	for (i = 0; i < n; i++) {
		min = i;
		for (j = i + 1; j < n; j++)
			if (list[j] < list[min])
				min = j;
		temp = list[i];
		list[i] = list[min];
		list[min] = temp;
	}
}
