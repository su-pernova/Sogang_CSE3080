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
		if (k < node->data) //���԰��� data���� �������
			node = node->leftChild; //���� tree Ž��
		else if (k > node->data) //���԰��� data���� ū ���
			node = node->rightChild; // ������ tree Ž��
		else return NULL;
	}
	return temp;
}

void main(void) {
	int i, n, A[MAX_TERMS];
	treePtr tree = NULL;

	// ���� �����Ѵ�.
	FILE* fp;
	fp = fopen("input.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1);
	}

	// ������ �о� ������ ������ n��, ���ԵǾ�� �� �������� A�� ����
	fscanf(fp, "%d", &n);
	for (i = 0; !feof(fp); i++)
		fscanf(fp, "%d", &A[i]);
	fclose(fp);

	// ������� binary search tree�� ����
	for (i = 0; i < n; i++)
		insert_node(&tree, A[i]);

	// ������ binary search tree�� inorder�� preorder�� ���
	printf("Inorder : ");
	inorder(tree); printf("\n");

	printf("Preorder : ");
	preorder(tree); printf("\n");

	system("pause");
}