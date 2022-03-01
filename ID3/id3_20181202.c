#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define log2(x) log(x)/log(2)
int data_num, feature_num, test_num;
int data[500][500]; int test[500][500];
int data_class[500]; int used_feature[500];
int nodes[200000] = { NULL };

// ------- 1. 트리를 구성하는 노드. 리프노드는 자식노드가 NULL ------- 
typedef struct treenode *treePtr;
typedef struct treenode {
	int data;
	treePtr leftchild;
	treePtr rightchild;
} treenode;

//  ------------- 2. 트리에 새로운 노드를 추가하는 함수 -------------- 
treePtr insert_node(treePtr node, int num) {
	treePtr ptr = (treePtr)malloc(sizeof(*ptr));
	ptr->data = num;
	ptr->leftchild = ptr->rightchild = NULL;
	if (node->leftchild == NULL) node->leftchild = ptr;
	else node->rightchild = ptr;
	return ptr;
}

//  ---------- 3. 모두 같은 class를 가지는지 check하는 함수 ----------- 
int check_class(int data_idx[], int num_data) {
	int yes = 0, no = 0;
	for (int i = 0; i < num_data; i++) {
		if (data_class[data_idx[i]] == 1) yes++;
		else if (data_class[data_idx[i]] == 0) no++; }
	if (yes == 0) { return 0; }
	else if (no == 0) { return 1; }
	else { return 2; }
}

//  -------------- 4. 데이터 feature별 entropy 계산 함수 --------------- 
float feature_entropy(int feature_class[], int data_idx[], int data_number) {
	float yes, no, yes_entropy, no_entropy, result;
	float yes_yes = 0, yes_no = 0;
	float no_yes = 0, no_no = 0;

	// 현재 feature에서 참/불참이면서 class가 yes/no
	for (int i = 0; i < data_number; i++) {
		if (feature_class[i] == 1 && data_class[data_idx[i]] == 1) yes_yes++;
		else if (feature_class[i] == 1 && data_class[data_idx[i]] == 0) yes_no++;
		else if (feature_class[i] == 0 && data_class[data_idx[i]] == 1) no_yes++;
		else if (feature_class[i] == 0 && data_class[data_idx[i]] == 0) no_no++; }
	yes = yes_yes + yes_no;
	no = no_yes + no_no;

	// yes_entropy를 계산한다
	if (yes_yes != 0 && yes_no != 0)
		yes_entropy = (yes / (float)data_number)*((yes_yes / yes)*(log2(yes / yes_yes)) + (yes_no / yes)*(log2(yes / yes_no)));
	else if (yes_yes == 0 && yes_no != 0)
		yes_entropy = (yes / (float)data_number)*((yes_no / yes)*(log2(yes / yes_no)));
	else if (yes_yes != 0 && yes_no == 0)
		yes_entropy = (yes / (float)data_number)*((yes_yes / yes)*(log2(yes / yes_yes)));
	else if (yes == 0)
		yes_entropy = 0;
	// no_entropy를 계산한다
	if (no_yes != 0 && no_no != 0)
		no_entropy = (no / (float)data_number)*((no_yes / no)*(log2(no / no_yes)) + (no_no / no)*(log2(no / no_no)));
	else if (no_yes == 0 && no_no != 0)
		no_entropy = (no / (float)data_number)*((no_no / no)*(log2(no / no_no)));
	else if (no_yes != 0 && no_no == 0)
		no_entropy = (no / (float)data_number)*((no_yes / no)*(log2(no / no_yes)));
	else if (no == 0)
		no_entropy = 0;

	result = yes_entropy + no_entropy;
	return result;
}

//  ------- 5. 정보 획득량이 가장 큰 feature의 index를 찾는 함수 ------- 
int max_info(int data_idx[], int data_len, int for_node) {
	float min_entropy = 100, flag = 0;
	int min_feature_idx = 0, for_for = for_node;
	int *feature = malloc(sizeof(int) * data_len);
	for (int feature_idx = 0; feature_idx < feature_num; feature_idx++) {
		for_for = for_node;
		for (int i = 0; i < data_len; i++)
			feature[i] = data[data_idx[i]][feature_idx];
		if (feature_entropy(feature, data_idx, data_len) < min_entropy) {
			while (for_for != 0) {
				if (feature_idx == nodes[for_for]) {
					flag = 1;
					break;
				}
				for_for /= 2;
			}
			if (flag == 1) { flag = 0; continue; }
			min_entropy = feature_entropy(feature, data_idx, data_len);
			min_feature_idx = feature_idx;
		}
	}
	free(feature);
	return min_feature_idx;
}

//  ------------- 6. leftchild와 rightchild를 만드는 함수 -------------- 
void make_child(treePtr node, int feature_idx, int for_node, int data_idx[], int data_len) {
	treePtr temp1, temp2;
	int for_node1 = for_node, for_node2 = for_node;
	int *yes_data = malloc(sizeof(int) * 500);
	int *no_data = malloc(sizeof(int) * 500);
	int check, y_idx = 0, n_idx = 0;
	int left_feature = 0, right_feature = 0;
	int y_class_num = 0, n_class_num = 0;

	// leftchild 데이터 집합과 rightchild 집합을 분류
	for (int i = 0; i < data_len; i++) {
		if (data[data_idx[i]][feature_idx] == 1) { yes_data[y_idx] = data_idx[i]; y_idx++; }
		else if (data[data_idx[i]][feature_idx] == 0) { no_data[n_idx] = data_idx[i]; n_idx++; }
	}

	// 모든 데이터의 class count
	for (int i = 0; i < data_len; i++) {
		if (data_class[data_idx[i]] == 1) y_class_num++;
		else if (data_class[data_idx[i]] == 0) n_class_num++; }

	// 전달 받은 데이터의 class가 모두 같은 경우
	if (y_class_num == 0 || n_class_num == 0) return;

	// 더이상 데이터 분류에 사용할 feature가 없는 경우
	if (for_node * 2 > pow(2, feature_num) - 1) {
		if (y_class_num > n_class_num) {
			temp1 = insert_node(node, 1);
			for_node1 *= 2;
			nodes[for_node1] = 1;
			return; }
		else if (n_class_num >= y_class_num) {
			temp2 = insert_node(node, 0);
			for_node2 = (for_node2 * 2) + 1;
			nodes[for_node2] = 0;
			return; }}

	else { // leftchild 노드 생성
		check = check_class(yes_data, y_idx);
		if (check == 0 || check == 1) { // 데이터의 class가 모두 같은 경우
			temp1 = insert_node(node, check);
			for_node1 *= 2;
			nodes[for_node1] = check;
			make_child(temp1, left_feature, for_node1, yes_data, y_idx); }
		else {
			left_feature = max_info(yes_data, y_idx, for_node1);
			for_node1 *= 2;
			nodes[for_node1] = left_feature;
			temp1 = insert_node(node, left_feature);
			make_child(temp1, left_feature, for_node1, yes_data, y_idx); }

		// rightchild 노드 생성
		check = check_class(no_data, n_idx);
		if (check == 0 || check == 1) { // 데이터의 class가 모두 같은 경우
			temp2 = insert_node(node, check);
			for_node2 = (for_node2 * 2) + 1;
			nodes[for_node2] = check;
			make_child(temp2, right_feature, for_node2, no_data, n_idx); }
		else {
			right_feature = max_info(no_data, n_idx, for_node2);
			for_node2 = (for_node2 * 2) + 1;
			nodes[for_node2] = right_feature;
			temp2 = insert_node(node, right_feature);
			make_child(temp2, right_feature, for_node2, no_data, n_idx); }
	}
	free(yes_data);
	free(no_data);
}

// ------------------ 7. decisiontree를 생성하는 함수 ------------------ 
treePtr create_tree(int data_index[]) {
	int check, feature_idx, for_node = 0;
	treePtr rootNode = (treePtr)malloc(sizeof(*rootNode));

	// 처음 입력받은 데이터의 클래스가 모두 같은 경우
	check = check_class(data_index, data_num);
	if (check == 0 || check == 1) {
		printf("아무런 분류도 이루어지지 않았습니다.");
		rootNode->data = check;
		rootNode->leftchild = NULL;
		rootNode->rightchild = NULL;
		return rootNode; }

	// decision tree의 루트노드 생성
	feature_idx = max_info(data_index, data_num, for_node);
	rootNode->data = feature_idx;
	rootNode->leftchild = NULL;
	rootNode->rightchild = NULL;
	for_node++;
	nodes[for_node] = feature_idx;

	// 루트 노드 밑으로 새로운 트리 생성
	make_child(rootNode, feature_idx, for_node, data_index, data_num);
	printf("Decision Tree가 생성되었습니다 . . .\n");
	return rootNode;
}

//  --------- 8. Decision Tree 구성 노드 메모리 할당 해제 함수 --------- 
void postorderTraverse(treePtr np) {
	if (np == NULL) return;
	postorderTraverse(np->leftchild);
	postorderTraverse(np->rightchild); }
void delTree(treePtr np) {
	if (np != NULL) {
		postorderTraverse(np->leftchild);
		postorderTraverse(np->rightchild);
		free(np); }
	printf("트리의 모든 노드가 삭제되었습니다 . . .\n");
}

//  --------------------------- 9. main 함수 ---------------------------
int main() {
	int data_feature, feature_idx;
	// data.txt 파일 오픈 및 정보 저장
	FILE *fp;
	fp = fopen("data.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	fscanf(fp, "%d%d", &data_num, &feature_num);
	for (int i = 0; i < data_num; i++) {
		for (int j = 0; j < feature_num; j++)
			fscanf(fp, "%d", &data[i][j]);
		fscanf(fp, "%d", &data_class[i]); }
	fclose(fp);

	// 초기 데이터 idx 배열 생성
	int *data_index = malloc(sizeof(int) * data_num);
	for (int i = 0; i < data_num; i++)
		data_index[i] = i;

	// test.txt 파일 오픈 및 정보 저장
	fp = fopen("test.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	fscanf(fp, "%d", &test_num);
	for (int i = 0; i < test_num; i++)
		for (int j = 0; j < feature_num; j++)
			fscanf(fp, "%d", &test[i][j]);
	fclose(fp);

	// decisiontree 생성
	treePtr rootNode, temp;
	rootNode = create_tree(data_index);
	temp = rootNode;
	feature_idx = temp->data;

	// test 데이터 분류
	fp = fopen("output.txt", "wt");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	for (int i = 0; i < test_num; i++) {
		feature_idx = rootNode->data;
		temp = rootNode;
		// 자식노드가 둘 다 NULL이 아니면 계속 반복
		while (temp->leftchild != NULL && temp->rightchild != NULL) {
			data_feature = test[i][feature_idx];
			if (data_feature == 1) temp = temp->leftchild;
			else if (data_feature == 0) temp = temp->rightchild;
			feature_idx = temp->data; }
		// 자식노드의 둘 중 하나가 NULL인 경우 (무조건 left)
		if (temp->leftchild != NULL && temp->rightchild == NULL) {
			temp = temp->leftchild;
			feature_idx = temp->data; }// 리프노드의 데이터 값 반환
		fprintf(fp, "%d\n", feature_idx);
	} printf("output.txt 파일이 생성되었습니다 . . .\n");
	fclose(fp);

	// 생성된 트리의 모드 노드 메모리 할당 해제
	delTree(rootNode);

	free(data_index);
	system("pause");
	return 0;
}