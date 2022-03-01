#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define log2(x) log(x)/log(2)
int data_num, feature_num, test_num;
int data[500][500]; int test[500][500];
int data_class[500]; int used_feature[500];
int nodes[200000] = { NULL };

// ------- 1. Ʈ���� �����ϴ� ���. �������� �ڽĳ�尡 NULL ------- 
typedef struct treenode *treePtr;
typedef struct treenode {
	int data;
	treePtr leftchild;
	treePtr rightchild;
} treenode;

//  ------------- 2. Ʈ���� ���ο� ��带 �߰��ϴ� �Լ� -------------- 
treePtr insert_node(treePtr node, int num) {
	treePtr ptr = (treePtr)malloc(sizeof(*ptr));
	ptr->data = num;
	ptr->leftchild = ptr->rightchild = NULL;
	if (node->leftchild == NULL) node->leftchild = ptr;
	else node->rightchild = ptr;
	return ptr;
}

//  ---------- 3. ��� ���� class�� �������� check�ϴ� �Լ� ----------- 
int check_class(int data_idx[], int num_data) {
	int yes = 0, no = 0;
	for (int i = 0; i < num_data; i++) {
		if (data_class[data_idx[i]] == 1) yes++;
		else if (data_class[data_idx[i]] == 0) no++; }
	if (yes == 0) { return 0; }
	else if (no == 0) { return 1; }
	else { return 2; }
}

//  -------------- 4. ������ feature�� entropy ��� �Լ� --------------- 
float feature_entropy(int feature_class[], int data_idx[], int data_number) {
	float yes, no, yes_entropy, no_entropy, result;
	float yes_yes = 0, yes_no = 0;
	float no_yes = 0, no_no = 0;

	// ���� feature���� ��/�����̸鼭 class�� yes/no
	for (int i = 0; i < data_number; i++) {
		if (feature_class[i] == 1 && data_class[data_idx[i]] == 1) yes_yes++;
		else if (feature_class[i] == 1 && data_class[data_idx[i]] == 0) yes_no++;
		else if (feature_class[i] == 0 && data_class[data_idx[i]] == 1) no_yes++;
		else if (feature_class[i] == 0 && data_class[data_idx[i]] == 0) no_no++; }
	yes = yes_yes + yes_no;
	no = no_yes + no_no;

	// yes_entropy�� ����Ѵ�
	if (yes_yes != 0 && yes_no != 0)
		yes_entropy = (yes / (float)data_number)*((yes_yes / yes)*(log2(yes / yes_yes)) + (yes_no / yes)*(log2(yes / yes_no)));
	else if (yes_yes == 0 && yes_no != 0)
		yes_entropy = (yes / (float)data_number)*((yes_no / yes)*(log2(yes / yes_no)));
	else if (yes_yes != 0 && yes_no == 0)
		yes_entropy = (yes / (float)data_number)*((yes_yes / yes)*(log2(yes / yes_yes)));
	else if (yes == 0)
		yes_entropy = 0;
	// no_entropy�� ����Ѵ�
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

//  ------- 5. ���� ȹ�淮�� ���� ū feature�� index�� ã�� �Լ� ------- 
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

//  ------------- 6. leftchild�� rightchild�� ����� �Լ� -------------- 
void make_child(treePtr node, int feature_idx, int for_node, int data_idx[], int data_len) {
	treePtr temp1, temp2;
	int for_node1 = for_node, for_node2 = for_node;
	int *yes_data = malloc(sizeof(int) * 500);
	int *no_data = malloc(sizeof(int) * 500);
	int check, y_idx = 0, n_idx = 0;
	int left_feature = 0, right_feature = 0;
	int y_class_num = 0, n_class_num = 0;

	// leftchild ������ ���հ� rightchild ������ �з�
	for (int i = 0; i < data_len; i++) {
		if (data[data_idx[i]][feature_idx] == 1) { yes_data[y_idx] = data_idx[i]; y_idx++; }
		else if (data[data_idx[i]][feature_idx] == 0) { no_data[n_idx] = data_idx[i]; n_idx++; }
	}

	// ��� �������� class count
	for (int i = 0; i < data_len; i++) {
		if (data_class[data_idx[i]] == 1) y_class_num++;
		else if (data_class[data_idx[i]] == 0) n_class_num++; }

	// ���� ���� �������� class�� ��� ���� ���
	if (y_class_num == 0 || n_class_num == 0) return;

	// ���̻� ������ �з��� ����� feature�� ���� ���
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

	else { // leftchild ��� ����
		check = check_class(yes_data, y_idx);
		if (check == 0 || check == 1) { // �������� class�� ��� ���� ���
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

		// rightchild ��� ����
		check = check_class(no_data, n_idx);
		if (check == 0 || check == 1) { // �������� class�� ��� ���� ���
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

// ------------------ 7. decisiontree�� �����ϴ� �Լ� ------------------ 
treePtr create_tree(int data_index[]) {
	int check, feature_idx, for_node = 0;
	treePtr rootNode = (treePtr)malloc(sizeof(*rootNode));

	// ó�� �Է¹��� �������� Ŭ������ ��� ���� ���
	check = check_class(data_index, data_num);
	if (check == 0 || check == 1) {
		printf("�ƹ��� �з��� �̷������ �ʾҽ��ϴ�.");
		rootNode->data = check;
		rootNode->leftchild = NULL;
		rootNode->rightchild = NULL;
		return rootNode; }

	// decision tree�� ��Ʈ��� ����
	feature_idx = max_info(data_index, data_num, for_node);
	rootNode->data = feature_idx;
	rootNode->leftchild = NULL;
	rootNode->rightchild = NULL;
	for_node++;
	nodes[for_node] = feature_idx;

	// ��Ʈ ��� ������ ���ο� Ʈ�� ����
	make_child(rootNode, feature_idx, for_node, data_index, data_num);
	printf("Decision Tree�� �����Ǿ����ϴ� . . .\n");
	return rootNode;
}

//  --------- 8. Decision Tree ���� ��� �޸� �Ҵ� ���� �Լ� --------- 
void postorderTraverse(treePtr np) {
	if (np == NULL) return;
	postorderTraverse(np->leftchild);
	postorderTraverse(np->rightchild); }
void delTree(treePtr np) {
	if (np != NULL) {
		postorderTraverse(np->leftchild);
		postorderTraverse(np->rightchild);
		free(np); }
	printf("Ʈ���� ��� ��尡 �����Ǿ����ϴ� . . .\n");
}

//  --------------------------- 9. main �Լ� ---------------------------
int main() {
	int data_feature, feature_idx;
	// data.txt ���� ���� �� ���� ����
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

	// �ʱ� ������ idx �迭 ����
	int *data_index = malloc(sizeof(int) * data_num);
	for (int i = 0; i < data_num; i++)
		data_index[i] = i;

	// test.txt ���� ���� �� ���� ����
	fp = fopen("test.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	fscanf(fp, "%d", &test_num);
	for (int i = 0; i < test_num; i++)
		for (int j = 0; j < feature_num; j++)
			fscanf(fp, "%d", &test[i][j]);
	fclose(fp);

	// decisiontree ����
	treePtr rootNode, temp;
	rootNode = create_tree(data_index);
	temp = rootNode;
	feature_idx = temp->data;

	// test ������ �з�
	fp = fopen("output.txt", "wt");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	for (int i = 0; i < test_num; i++) {
		feature_idx = rootNode->data;
		temp = rootNode;
		// �ڽĳ�尡 �� �� NULL�� �ƴϸ� ��� �ݺ�
		while (temp->leftchild != NULL && temp->rightchild != NULL) {
			data_feature = test[i][feature_idx];
			if (data_feature == 1) temp = temp->leftchild;
			else if (data_feature == 0) temp = temp->rightchild;
			feature_idx = temp->data; }
		// �ڽĳ���� �� �� �ϳ��� NULL�� ��� (������ left)
		if (temp->leftchild != NULL && temp->rightchild == NULL) {
			temp = temp->leftchild;
			feature_idx = temp->data; }// ��������� ������ �� ��ȯ
		fprintf(fp, "%d\n", feature_idx);
	} printf("output.txt ������ �����Ǿ����ϴ� . . .\n");
	fclose(fp);

	// ������ Ʈ���� ��� ��� �޸� �Ҵ� ����
	delTree(rootNode);

	free(data_index);
	system("pause");
	return 0;
}