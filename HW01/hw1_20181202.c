#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void selection_sort(int list[], int n);

int main() {
	int i, n, data, j = 0;
	int *list;
	double duration;
	long repetitions;

	//lab.data 파일을 오픈한다
	FILE* fp;
	fp = fopen("lab1.data", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error\n");
		exit(1); }

	//시간 측정 시작(파일 오픈 시간 제외)
	clock_t start;
	repetitions = 0;
	start = clock();

	fscanf(fp, "%d", &n); //n = 배열의 크기
	list = (int*)malloc(sizeof(int)*n);	//동적 할당에 의한 배열 선언

	//배열의 content를 lab.data 파일에서 입력받기
	while (fscanf(fp, "%d", &data) != -1) {
		list[j] = data;
		j++; }

	//배열의 content 출력
	printf("Input : ");
	for (i = 0; i < n; i++) {
		if (i == 0) printf("%d", list[i]);
		else printf(", %d", list[i]); }
	printf("\n");

	//Selection Sort 실행
	do {
		repetitions++;
		selection_sort(list, n);
	} while (clock() - start < 1000);

	duration = ((double)(clock() - start)) / CLOCKS_PER_SEC;
	duration /= repetitions;

	//Selection Sort 결과 출력
	printf("Sorted : ");
	for (i = 0; i < n; i++) {
		if (i == 0) printf("%d", list[i]);
		else printf(", %d", list[i]);
	}
	printf("\n\n");

	//프로그램 실행 시간 출력
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
