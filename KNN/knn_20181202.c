#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_SIZE 500
float dataset[MAX_SIZE][MAX_SIZE];
float testset[MAX_SIZE][MAX_SIZE];
float *L2_distance;
int answerset[MAX_SIZE]; int answer = 0;


// 두 데이터 사이의 거리를 계산하는 함수
void *distance(float dataset[][MAX_SIZE], float one_testset[], int data_num, int feature_num) {
	float sum, subsum;
	float result[MAX_SIZE];
	for (int i = 0; i < data_num; i++) {
		sum = 0;
		for (int j = 0; j < feature_num; j++) {
			subsum = (one_testset[j] - dataset[i][j]);
			subsum = pow(subsum, 2);
			sum += subsum;
		}
		sum = sqrt(sum);
		L2_distance[i] = sum;
	}
}

int main() {
	int data_num, test_num;
	int feature_num;

	// 데이터가 저장되어 있는 data.txt파일 오픈 후 dataset에 저장
	FILE* fp1;
	fp1 = fopen("data.txt", "r");
	if (fp1 == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	fscanf(fp1, "%d", &data_num);
	fscanf(fp1, "%d", &feature_num);
	while (!feof(fp1)) {
		for (int i = 0; i < data_num; i++)
			for (int j = 0; j < feature_num + 1; j++)
				fscanf(fp1, "%f", &dataset[i][j]);
	} fclose(fp1);

	//class 타입만 따로 저장해 둘 배열 생성
	int *class_type;
	class_type = (int*)malloc(sizeof(int)*data_num);

	// 테스트 데이터가 저장되어 있는 test.txt파일 오픈 후 testset에 저장
	FILE* fp2;
	fp2 = fopen("test.txt", "r");
	if (fp2 == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }

	fscanf(fp2, "%d", &test_num);

	while (!feof(fp2)) {
		for (int i = 0; i < test_num; i++)
			for (int j = 0; j < feature_num; j++)
				fscanf(fp2, "%f", &testset[i][j]);
	} fclose(fp2);


	/************ 모든 testdata의 class값 추측 시작 ***********/
	for (int check = 0; check < test_num; check++) {

		// 각 행의 마지막값을 class_type에 저장
		for (int i = 0; i < data_num; i++)
			class_type[i] = dataset[i][feature_num];

		// distance 값 계산
		L2_distance = (float*)malloc(sizeof(float)*data_num);
		distance(dataset, testset[check], data_num, feature_num);

		// Selection Sort (거리순 정렬과 동시에 classtype도 거리 따라 이동)
		float temp; int class_temp;
		for (int i = 0; i < data_num - 1; i++) {
			for (int j = i + 1; j < data_num; j++) {
				if (L2_distance[i] > L2_distance[j]) {
					temp = L2_distance[i];
					class_temp = class_type[i];

					L2_distance[i] = L2_distance[j];
					class_type[i] = class_type[j];

					L2_distance[j] = temp;
					class_type[j] = class_temp;
				}
			}
		}

		// K-Nearest Neighbor의 K값을 7보다 크게 해야 하는 경우
		int K = 6;
		while (1) {
			for (int i = 7; i < data_num; i++) {
				if (L2_distance[K] == L2_distance[i]) K = i;
				else break; }
			break; }

		// 정렬된 class_type 에서 K번째 까지의 최빈값 구하기
		int most = class_type[0], freq, count = 1, hello = 0;
		for (int i = 0; i <= K; i++) {
			freq = 0; hello = 0;
			for (int j = 0; j <= K; j++) {
				if (class_type[i] == class_type[j]) freq += 1; }
			if (freq > count) {
				most = class_type[i];
				count = freq; }
			else if (freq == count && most > class_type[i]) {
				most = class_type[i];
				count = freq; }
		}

		// 해당 testdata의 class 추측값을 answerset 배열에 저장
		answerset[answer++] = most;

		free(L2_distance);
	}

	// answerset 배열에 저장해둔 class추측값을 output.txt에 출력
	FILE *fp3 = fopen("output.txt", "wt");
	if (fp3 == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	for (int i = 0; i < test_num - 1; i++)
		fprintf(fp3, "%d\n", answerset[i]);
	fprintf(fp3, "%d", answerset[test_num-1]);
	printf("output.txt 파일이 생성되었습니다. . .\n\n");

	fclose(fp3);
	free(class_type);
	system("pause");
	return 0;

}