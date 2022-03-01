#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_ITER 50
#define MAX_SIZE 500

float dataset[MAX_SIZE][MAX_SIZE];
float centroid[MAX_SIZE][MAX_SIZE];
int data_num, feature_num, K;
int random = 1000;

/* ***** Singly Linked List�� �����ϴ� ��� ***** */
typedef struct node {
	float data[MAX_SIZE];
	struct node *next;
} node;

/* ***** ��� �ڿ� ��� �ϳ��� �߰��ϴ� �Լ� ***** */
void addFirst(struct node *target, float data[MAX_SIZE]) {
	struct node *newNode = malloc(sizeof(struct node));
	newNode->next = target->next;
	for (int i = 0; i < feature_num; i++)
		newNode->data[i] = data[i];
	target->next = newNode;
}

/* ***** ��� �ڿ� ��� �ϳ��� �����ϴ� �Լ� ***** */
void removeFirst(struct node *target) {
	struct node *removeNode = target->next;
	target->next = removeNode->next;
	free(removeNode);
}

/* ***** ������ ������ �迭�� �����ͼ��� �����ϴ� �Լ� ***** */
void fileopen(char *filename) {
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }

	fscanf(fp, "%d", &data_num); // ������ ����
	fscanf(fp, "%d", &feature_num); // ������ feature ���� 
	fscanf(fp, "%d", &K); // K��

	// �����͸� 2���� �迭�� ����
	for (int i = 0; i < data_num; i++) {
		for (int k = 0; k < feature_num; k++)
			fscanf(fp, "%f", &dataset[i][k]);
	} 
	fclose(fp);
}

/* ***** random centroid ���� �����ϴ� �Լ�(���� : 0~3 ����) ***** */
float rand_cent() {
	srand(random);
	float result = rand() % 3 + rand() % 100000 / 100000.0;
	random *= 2;
	return result;
}

/* ***** main �Լ� ���� ***** */
int main() {
	// ���� ���� �� ������ ����
	fileopen("input.txt");

	// K���� centroid �ʱⰪ�� �������� �������ش�.
	for (int i = 0; i < K; i++) {
		for (int k = 0; k < feature_num; k++)
			centroid[i][k] = rand_cent();
	}

	// K���� �����带 �����ϴ� �迭 �����Ҵ� �� �ʱⰪ ����
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

	/* ***** 1. �ʱ� ��ũ�帮��Ʈ�� �����Ѵ� ***** */
	int cent_idx;
	float sum, subsum, min_distance = 0;
	for (int i = 0; i < data_num; i++) {
		for (int j = 0; j < K; j++) {
			sum = 0, subsum = 0;
			// ���� ����� centroid�� ã�´�
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
		// ���� ����� centroid���� ������ �ڿ� ����
		addFirst(head_node[cent_idx], dataset[i]);
	}
	
	float data_sum, number, new_cent;
	float new_subsum, new_sum, new_minsum = 0;
	int new_centidx, change;
	struct node *curr;
	struct node *before;

	/* ***** 2. ITERATION ���� ***** */
	for (int iter = 1; iter < MAX_ITER; iter++) {
		change = 0;

		// centroid�� ��հ����� ������Ʈ
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

		// �� ���� ������Ʈ
		for (int j = 0; j < K; j++) { // 0~K��° ��������

			before = head_node[j]; // �Ѵܰ� �� ��� ����Ŵ
			curr = head_node[j]->next; // ���� ��� ����Ŵ

			while (curr != NULL) { // ��尡 ���̻� ���������� �� ���� Ž��
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

				if (new_centidx != j) { // �Ҽӵ� ������ ���� �ϴ� ���
					change++; // ������ �߻������� ǥ������
					addFirst(head_node[new_centidx], curr->data); // �� ��� ����
					curr = curr->next; // ���� �����ͷ� �̵�
					removeFirst(before); // ��� ����
				}

				else { // ���� �̵� ���� ���
					before = before->next; // ���� �̵� ������ next
					curr = curr->next; // ���� �����ͷ� �̵�
				}

			}

		}

		if (change == 0) break; // ���̻� ������ ��ȭ�� ������ �۾� �ߴ�
	}

	/* ***** 3. Clustering ��� �� ���� ��� ***** */
	FILE *fp = fopen("output.txt", "wt");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1);}

	// k-means clusteing ����� Ȯ���Ѵ�
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