#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTICES 30
#define MIN2(x,y) ((x) < (y) ? (x) : (y))
#define FALSE 0
#define TRUE 1

// variables, arrays and functions
short int dfn[MAX_VERTICES];
short int low[MAX_VERTICES];
short int visited[MAX_VERTICES];
int num;
void init(void);
void bicon(int, int);

// node for adjacency list
typedef struct node *node_pointer;
typedef struct node {
	int vertex;
	node_pointer link;
};
node_pointer graph[MAX_VERTICES];
node_pointer createnode(int data);

// node for singly linked list stack
typedef struct edge {
	int data_a;
	int data_b;
	struct edge *next;
} edge;
edge *top = NULL;

// stack push operation
void push(int x, int y) {
	struct edge *p;
	p = (struct edge *)malloc(sizeof(struct edge));
	p->data_a = x;
	p->data_b = y;
	if (top == NULL) { top = p; p->next = NULL; }
	else { p->next = top; top = p; }
}

// stack pop operation
void pop(int *x, int *y) {
	struct edge *p;
	if (top == NULL) printf("stack underflow\n");
	else {
		p = top;
		top = top->next;
		*x = p->data_a;
		*y = p->data_b;
		free(p);
		p = NULL; }
}

// create new node for adjacency list
node_pointer createnode(int data) {
	node_pointer ptr;
	ptr = (node_pointer)malloc(sizeof(struct node));
	ptr->vertex = data;
	ptr->link = NULL;
	return ptr;
}

// create new biconnected component
void bicon(int u, int v) {
	node_pointer ptr;
	int w, x, y;
	dfn[u] = low[u] = num++;

	for (ptr = graph[u]; ptr; ptr = ptr->link) {
		w = ptr->vertex;
		if (v != w && dfn[w] < dfn[u]) {
			push(u, w);
			if (dfn[w] < 0) {
				bicon(w, u);
				low[u] = MIN2(low[u], low[w]);
				if (low[w] >= dfn[u]) {
					printf("New biconnected component: ");
					do {
						pop(&x, &y);
						printf("<%d,%d> ", x, y);
					} while (!((x == u) && (y == w)));
					printf("\n");
				}
			}
			else if (w != v) low[u] = MIN2(low[u], dfn[w]);
		}
	}
}

// initialize dfn and low
void init(void) {
	int i;
	for (i = 0; i < MAX_VERTICES; i++) {
		visited[i] = FALSE;
		dfn[i] = low[i] = -1;
	}
	num = 0;
}

// main function
void main(void) {
	int total_vertex, vertex_num, n;
	int flag = 0; char trash;
	node_pointer *vertex;

	// read input.txt file
	FILE* fp;
	fp = fopen("input.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	fscanf(fp, "%d", &total_vertex); 
	fscanf(fp, "%c", &trash);


	// make adjacency list
	for (int i = 0; !feof(fp); i++) {
		fscanf(fp, "%d", &n);
		if (flag == 0) {
			vertex_num = n;
			vertex = &graph[vertex_num];
			flag++; } 
		else { 
			*vertex = createnode(n);
			vertex = &((*vertex)->link); }

		fscanf(fp, "%c", &trash);
		if (trash == '\n') flag = 0;
	}

	// create biconnected components
	init();
	bicon(3, -1);

	system("pause");
}