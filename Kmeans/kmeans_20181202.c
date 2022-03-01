#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_ITER 50
#define MAX_SIZE 500

float dataset[MAX_SIZE][MAX_SIZE];
float centroid[MAX_SIZE][MAX_SIZE];
int data_num, feature_num, K;
int random = 1000;

/* ***** Singly Linked List를 구성하는 노드 ***** */
typedef struct node {
	float data[MAX_SIZE];
	struct node *next;
} node;

/* ***** 노드 뒤에 노드 하나를 추가하는 함수 ***** */
void addFirst(struct node *target, float data[MAX_SIZE]) {
	struct node *newNode = malloc(sizeof(struct node));
	newNode->next = target->next;
	for (int i = 0; i < feature_num; i++)
		newNode->data[i] = data[i];
	target->next = newNode;
}

/* ***** 노드 뒤에 노드 하나를 삭제하는 함수 ***** */
void removeFirst(struct node *target) {
	struct node *removeNode = target->next;
	target->next = removeNode->next;
	free(removeNode);
}

/* ***** 파일을 오픈해 배열에 데이터셋을 저장하는 함수 ***** */
void fileopen(char *filename) {
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }

	fscanf(fp, "%d", &data_num); // 데이터 개수
	fscanf(fp, "%d", &feature_num); // 데이터 feature 개수 
	fscanf(fp, "%d", &K); // K값

	// 데이터를 2차원 배열에 저장
	for (int i = 0; i < data_num; i++) {
		for (int k = 0; k < feature_num; k++)
			fscanf(fp, "%f", &dataset[i][k]);
	} 
	fclose(fp);
}

/* ***** random centroid 값을 생성하는 함수(범위 : 0~3 사이) ***** */
float rand_cent() {
	srand(random);
	float result = rand() % 3 + rand() % 100000 / 100000.0;
	random *= 2;
	return result;
}

/* ***** main 함수 시작 ***** */
int main() {
	// 파일 오픈 및 데이터 저장
	fileopen("input.txt");

	// K개의 centroid 초기값을 랜덤으로 설정해준다.
	for (int i = 0; i < K; i++) {
		for (int k = 0; k < feature_num; k++)
			centroid[i][k] = rand_cent();
	}

	// K개의 헤더노드를 저장하는 배열 동적할당 후 초기값 삽입
	node **head_node;
	head_node = (node**)malloc(sizeof(node*) * K);
	for (int i = 0; i < K; i++) {
		struct node *head = malloc(sizeof(struct node));
		for (int k = 0; k < feature_num; k++) {
			head->data[k] = centroid[i][k];
		}
		head->next = NULL;
		head_node[i] = head;
	}

	/* ***** 1. 초기 링크드리스트를 생성한다 ***** */
	int cent_idx;
	float sum, subsum, min_distance = 0;
	for (int i = 0; i < data_num; i++) {
		for (int j = 0; j < K; j++) {
			sum = 0, subsum = 0;
			// 가장 가까운 centroid를 찾는다
			for (int k = 0; k < feature_num; k++) {
				subsum = (head_node[j]->data[k] - dataset[i][k]);
				subsum = pow(subsum, 2);
				sum += subsum;
			} sum = sqrt(sum);
			if (j == 0) {
				min_distance = sum;
				cent_idx = j; }
			else if (sum < min_distance) {
				min_distance = sum;
				cent_idx = j; }
		}
		// 가장 가까운 centroid값의 헤더노드 뒤에 삽입
		addFirst(head_node[cent_idx], dataset[i]);
	}
	
	float data_sum, number, new_cent;
	float new_subsum, new_sum, new_minsum = 0;
	int new_centidx, change;
	struct node *curr;
	struct node *before;

	/* ***** 2. ITERATION 시작 ***** */
	for (int iter = 1; iter < MAX_ITER; iter++) {
		change = 0;

		// centroid를 평균값으로 업데이트
		for (int j = 0; j < K; j++) {
			for (int i = 0; i < feature_num; i++) {
				data_sum = 0; number = 0;
				curr = head_node[j]->next;
				while (curr != NULL) {
					data_sum += curr->data[i];
					number++;
					curr = curr->next;
				}
				new_cent = data_sum / number;
				head_node[j]->data[i] = new_cent;
			}
		}

		// 각 군집 업데이트
		for (int j = 0; j < K; j++) { // 0~K번째 군집까지

			before = head_node[j]; // 한단계 전 노드 가리킴
			curr = head_node[j]->next; // 현재 노드 가리킴

			while (curr != NULL) { // 노드가 더이상 없을때까지 각 군집 탐색
				for (int k = 0; k < K; k++) {
					new_sum = 0;
					for (int i = 0; i < feature_num; i++) {
						new_subsum = head_node[k]->data[i] - curr->data[i];
						new_subsum = pow(new_subsum, 2);
						new_sum += new_subsum;
					} new_sum = sqrt(new_sum);
					if (k == 0) {
						new_minsum = new_sum;
						new_centidx = k;
					}
					else if (new_sum < new_minsum) {
						new_minsum = new_sum;
						new_centidx = k;
					}
				}

				if (new_centidx != j) { // 소속된 군집을 변경 하는 경우
					change++; // 변경이 발생했음을 표시해줌
					addFirst(head_node[new_centidx], curr->data); // 새 노드 생성
					curr = curr->next; // 다음 데이터로 이동
					removeFirst(before); // 노드 삭제
				}

				else { // 군집 이동 없는 경우
					before = before->next; // 군집 이동 없으면 next
					curr = curr->next; // 다음 데이터로 이동
				}

			}

		}

		if (change == 0) break; // 더이상 군집에 변화가 없으면 작업 중단
	}

	/* ***** 3. Clustering 결과 및 파일 출력 ***** */
	FILE *fp = fopen("output.txt", "wt");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1);}

	// k-means clusteing 결과를 확인한다
	for (int j = 0; j < K; j++) {
		struct node *curr2 = head_node[j]->next;
		fprintf(fp, "%d\n", j); printf("%d\n", j);
		while (curr2 != NULL) {
			for (int i = 0; i < feature_num; i++) {
				fprintf(fp, "%f ", curr2->data[i]);
				printf("%f ", curr2->data[i]);
			} fprintf(fp, "\n"); printf("\n");
			curr2 = curr2->next;
		}
	} fclose(fp);

	system("pause");
	return 0;
}