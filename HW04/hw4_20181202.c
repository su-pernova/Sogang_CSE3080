#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_EXPR_SIZE 100
char expr[MAX_EXPR_SIZE];

// linked list stack을 구성하는 node
typedef struct node {
	char data;
	struct node *next;
} node;
node *top = NULL; // top of stack
node *pstart = NULL; // point token

// insert token to pstart
void insert(char a) {
	struct node *t, *child;
	child = (struct node *)malloc(sizeof(struct node));
	child->next = NULL;
	child->data = a;

	t = pstart;
	if (pstart == NULL) pstart = child;
	else {
		while (t->next != NULL) t = t->next;
		t->next = child;
	}
}

// stack push operation
void push(char token) {
	struct node *p;
	p = (struct node *)malloc(sizeof(struct node));
	p->data = token;

	if (top == NULL) {
		top = p;
		p->next = NULL; }
	else { p->next = top; top = p; }
}

// stack pop operation
char pop() {
	struct node *x;
	char k;
	if (top == NULL) {
		printf("stack underflow\n");
		return 0; }
	else {
		x = top;
		top = top->next; k = x->data;
		free(x);
		x = NULL;
		return k; }
}

// print postpix expression
void printpost() {
	struct node *to;
	if (pstart == NULL)
		printf("");
	else {
		to = pstart;
		while (to != NULL) {
			printf("%c", to->data);
			to = to->next;
		}}
}

// precedence selector
int precedence(char ch) {
	if (ch == '*' || ch == '/' || ch == '%')
		return (4);
	else if (ch == '+' || ch == '-')
		return (3);
	else
		return (2);
}

//infix to postfix conversion
void postfix(char infix[]) {
	int len, index = 0;
	char token, temp;
	len = strlen(infix);

	while (len > index) {
		token = infix[index];
		switch (token) {
		case '(':
			push(token);
			break;
		case ')':
			temp = pop();
			while (temp != '(') {
				insert(temp);
				temp = pop(); }
			break;
		case '^':
		case '+':
		case '-':
		case '*':
		case '%':
		case '/':
			if (top == NULL) push(token);
			else {
				while (top != NULL && (precedence(top->data) >= precedence(token))) {
					temp = pop();
					insert(temp);
				} push(token);
			} break;
		default:
			insert(token);
		} index++;
	}

	while (top != NULL) {
		temp = pop();
		insert(temp);
	}

	printpost();
}


int main() {
	// open expr.txt file
	FILE* fp;
	fp = fopen("expr.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }

	// save expr.txt in expr[]
	int index = 0;
	while (!feof(fp)) {
		fscanf(fp, "%s", &expr[index]);
		index++; }
	fclose(fp);

	// print Infix and Postfix
	printf("Infix : %s\n", expr);
	printf("Postfix : ");
	postfix(expr);
	printf("\n\n");

	system("pause");
	return 0;
}