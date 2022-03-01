#include <stdio.h>
#include <stdlib.h>
#define MAX_TERMS 100

typedef struct treeNode* treePtr;
typedef struct treeNode {
	treePtr leftChild;
	int data;
	treePtr rightChild;
} treeNode;

void inorder(treePtr);
void preorder(treePtr);
void insert_node(treePtr*, int);
treePtr modifiedSearch(treePtr, int);

void inorder(treePtr ptr) {
	if (ptr) {
		inorder(ptr->leftChild);
		printf("%d", ptr->data);
		inorder(ptr->rightChild);
	}
}

void preorder(treePtr ptr) {
	if (ptr) {
		printf("%d", ptr->data);
		preorder(ptr->leftChild);
		preorder(ptr->rightChild);
	}
}

void insert_node(treePtr *node, int num) {
	treePtr ptr, temp = modifiedSearch(*node, num);
	if (temp || !(*node)) {
		ptr = (treePtr)malloc(sizeof(*ptr));
		ptr->data = num;
		ptr->leftChild = ptr->rightChild = NULL;
		if (*node) {
			if (num < temp->data) temp->leftChild = ptr;
			else temp->rightChild = ptr;
		}
		else *node = ptr;
	}
}

treePtr modifiedSearch(treePtr node, int k) {
	treePtr temp = node;
	while (node) {
		temp = node;
		if (k < node->data) //삽입값이 data보다 작은경우
			node = node->leftChild; //왼쪽 tree 탐색
		else if (k > node->data) //삽입값이 data보다 큰 경우
			node = node->rightChild; // 오른쪽 tree 탐색
		else return NULL;
	}
	return temp;
}

void main(void) {
	int i, n, A[MAX_TERMS];
	treePtr tree = NULL;

	// 파일 오픈한다.
	FILE* fp;
	fp = fopen("input.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1);
	}

	// 파일을 읽어 정수의 개수는 n에, 삽입되어야 할 정수들은 A에 저장
	fscanf(fp, "%d", &n);
	for (i = 0; !feof(fp); i++)
		fscanf(fp, "%d", &A[i]);
	fclose(fp);

	// 순서대로 binary search tree에 삽입
	for (i = 0; i < n; i++)
		insert_node(&tree, A[i]);

	// 구성된 binary search tree를 inorder와 preorder로 출력
	printf("Inorder : ");
	inorder(tree); printf("\n");

	printf("Preorder : ");
	preorder(tree); printf("\n");

	system("pause");
}