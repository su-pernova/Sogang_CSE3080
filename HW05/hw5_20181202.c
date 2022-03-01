#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE  50
typedef enum { head, entry } tagfield;
typedef struct matrixNode *matrixPointer;

typedef struct entryNode {
	int row;
	int col;
	int value;
};

typedef struct matrixNode {
	matrixPointer down;
	matrixPointer right;
	tagfield tag;
	union {
		matrixPointer next;
		struct entryNode entry;
	} u;
};
matrixPointer hdNode[MAX_SIZE];

matrixPointer mread(FILE* fp);
void mwrite(matrixPointer);
void merase(matrixPointer* node);

void main(void)
{
	matrixPointer a, b;
	FILE *fp1, *fp2;
	fp1 = fopen("A.txt", "r");
	fp2 = fopen("B.txt", "r");

	a = mread(fp1);
	mwrite(a);
	merase(&a);

	b = mread(fp2);
	mwrite(b);
	merase(&b);

	mwrite(a);
	mwrite(b);

	fclose(fp1);
	fclose(fp2);
	system("pause");
	return 0;
}

matrixPointer mread(FILE* fp) {
	int numRows, numCols, numEntries, numHeads, i;
	int row, col, value, CurrentRow;
	matrixPointer temp, last, node;

	fscanf(fp, "%d%d", &numRows, &numCols);
	numEntries = 19;

	numHeads = (numCols > numRows) ? numCols : numRows;
	node = (matrixPointer)malloc(sizeof(struct matrixNode));

	node->tag = entry;
	node->u.entry.row = numRows;
	node->u.entry.col = numCols;

	if (!numHeads) node->right = node;
	else {
		for (i = 0; i < numHeads; i++) {
			temp = (matrixPointer)malloc(sizeof(struct matrixNode));
			hdNode[i] = temp;
			hdNode[i]->tag = head;
			hdNode[i]->right = temp;
			hdNode[i]->u.next = temp;
		}
		CurrentRow = 0;
		last = hdNode[0];

		for (int x = 0; x < numRows; x++) {
			row = x; //row
			for (int y = 0; y < numCols; y++) {
				col = y; //col
				fscanf(fp, "%d", &value);
				if (value == 0) continue;

				if (row > CurrentRow) {
					last->right = hdNode[CurrentRow];
					CurrentRow = row;
					last = hdNode[row];
				}
				temp = (matrixPointer)malloc(sizeof(struct matrixNode));
				temp->tag = entry;
				temp->u.entry.row = row;
				temp->u.entry.col = col;
				temp->u.entry.value = value;
				last->right = temp;          /* link into row list */
				last = temp;
				hdNode[col]->u.next->down = temp;  /* link into column list */
				hdNode[col]->u.next = temp;
			}
		}

		/*close last row */
		last->right = hdNode[CurrentRow];
		/* close all column lists */
		for (i = 0; i < numCols; i++)
			hdNode[i]->u.next->down = hdNode[i];
		/* link all head nodes together */
		for (i = 0; i < numHeads - 1; i++)
			hdNode[i]->u.next = hdNode[i + 1];
		hdNode[numHeads - 1]->u.next = node;
		node->right = hdNode[0];
	}
	return node;
	fclose(fp);
}

void merase(matrixPointer* node) {
	/* erase the matrix, return the pointers to the heap */
	matrixPointer x, y;
	int i, numHeads;
	/* free the entry pointers by row */
	for (i = 0; i < (*node)->u.entry.row; i++) {
		y = hdNode[i]->right;
		while (y != hdNode[i]) {
			x = y;
			y = y->right;
			free(x);
		}
	}
	/* determine the number of head nodes and free these pointers */
	numHeads = ((*node)->u.entry.row > (*node)->u.entry.col) ?
		(*node)->u.entry.row : (*node)->u.entry.col;
	for (i = 0; i < numHeads; i++)
		free(hdNode[i]);
	*node = NULL;
}


void mwrite(matrixPointer node) {
	/* print out the matrix in row major form */
	int i, j;
	int numRows, numCols;
	matrixPointer temp, head;

	if (!node) printf("The matrix is empty.\n\n");
	else {
		head = node->right;
		/* matrix dimensions */
		numRows = node->u.entry.row;
		numCols = node->u.entry.col;

		printf("numRows = %d, numCols = %d \n\n", numRows, numCols);

		temp = head->right;
		for (i = 0; i < numRows; i++) {
			if (temp == head) {
				head = head->u.next;
				temp = head->right;
			}
			for (j = 0; j < numCols; j++) {
				if (i == temp->u.entry.row && j == temp->u.entry.col) {
					printf("%5d", temp->u.entry.value);
					temp = temp->right;
				}
				else
					printf("%5d", 0);
			}
			printf("\n\n");
		}
	}
}