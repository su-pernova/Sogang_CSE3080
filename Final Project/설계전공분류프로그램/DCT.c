#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define log2(x) log(x)/log(2)
int data_num, feature_num;
int data[500][500];
int data_class[500];
int nodes[150000] = { NULL };

// ��Ʈ���� ����Լ� 1
float s_ent(int num, int num_class) {
	float output;
	if (num == 0 || num_class == 0) return 0;
	output = ((float)num_class / (float)num) * log2(((float)num / (float)num_class));
	return output;
}
// ��Ʈ���� ����Լ� 2
float m_ent(int num, int by_class[]) {
	float fin = (float)num / (float)data_num;
	float sum = 0;
	for (int i = 0; i < 6; i++)
		sum += s_ent(num, by_class[i]);
	sum *= fin;
	return sum;
}
// ��Ʈ���� ����Լ� 3
float l_lent(int zero_num, int one_num, int zero_class[], int one_class[]) {
	float output = 0;
	output += m_ent(zero_num, zero_class);
	output += m_ent(one_num, one_class);
	return output;
}

// ----------- 0. 6���� ��� �� MAX����  return �ϴ� �Լ� ------------
int checkMax(int a, int b, int c, int d, int e, int f) {
	if (a >= b && a >= c && a >= d && a >= e && a >= f) return 1;
	else if (b >= a && b >= c && b >= d && b >= e && b >= f) return 2;
	else if (c >= a && c >= b && c >= d && c >= e && c >= f) return 3;
	else if (d >= a && d >= b && d >= c && d >= e && d >= f) return 4;
	else if (e >= a && e >= b && e >= c && e >= d && e >= f) return 5;
	else if (f >= a && f >= b && f >= c && f >= d && f >= e) return 6;
}

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
	int major_class[6] = { 0 };
	for (int i = 0; i < num_data; i++) {
		for (int j = 0; j < 6; j++)
			if (data_class[data_idx[i]] == j + 1) major_class[j]++;
	}
	if (major_class[1] == 0 && major_class[2] == 0 && major_class[3] == 0 && major_class[4] == 0 && major_class[5] == 0) return 1;
	else if (major_class[0] == 0 && major_class[2] == 0 && major_class[3] == 0 && major_class[4] == 0 && major_class[5] == 0) return 2;
	else if (major_class[0] == 0 && major_class[1] == 0 && major_class[3] == 0 && major_class[4] == 0 && major_class[5] == 0) return 3;
	else if (major_class[0] == 0 && major_class[1] == 0 && major_class[2] == 0 && major_class[4] == 0 && major_class[5] == 0) return 4;
	else if (major_class[0] == 0 && major_class[1] == 0 && major_class[2] == 0 && major_class[3] == 0 && major_class[5] == 0) return 5;
	else if (major_class[0] == 0 && major_class[1] == 0 && major_class[2] == 0 && major_class[3] == 0 && major_class[4] == 0) return 6;
	else return 0;
}

//  -------------- 4. ������ feature�� entropy ��� �Լ� --------------- 
float feature_entropy(int feature_class[], int data_idx[], int data_number) {
	float yes = 0, no = 0, result;
	float yes_yes = 0, yes_no = 0;
	float no_yes = 0, no_no = 0;
	int zero_class[6] = { 0 }, one_class[6] = { 0 };

	// ���� feature���� ��/�����̸鼭 class�� 1~6
	for (int i = 0; i < data_number; i++) {
		for (int j = 0; j < 6; j++)
			if (feature_class[i] == 0 && data_class[data_idx[i]] == j + 1) zero_class[j]++;
		for (int j = 0; j < 6; j++)
			if (feature_class[i] == 1 && data_class[data_idx[i]] == j + 1) one_class[j]++; }

	// 0, 1 �� �� ���� count
	for (int i = 0; i < 6; i++)
		no += zero_class[i];
	for (int i = 0; i < 6; i++)
		yes += one_class[i];

	// entropy�� ����Ѵ�
	result = l_lent(no, yes, zero_class, one_class);
	return result;
}

//  ------- 5. ���� ȹ�淮�� ���� ū feature�� index�� ã�� �Լ� ------- 
int max_info(int data_idx[], int data_len, int for_node) {
	float min_entropy = 100, flag = 0;
	int min_feature_idx = 0, for_for = for_node;
	int *feature = malloc(sizeof(int) * data_len);
	for (int feature_idx = 0; feature_idx < feature_num; feature_idx++) {
		for_for = for_node;
		for (int i = 0; i < data_len; i++) {
			feature[i] = data[data_idx[i]][feature_idx];
		}
		if (feature_entropy(feature, data_idx, data_len) < min_entropy) {
			while (for_for != 0) {
				if (feature_idx == nodes[for_for]) {
					flag = 1;
					break; }
				for_for /= 2; }
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
	int major_class[6] = { 0 };
	int max;

	// leftchild ������ ���հ� rightchild ������ �з�
	for (int i = 0; i < data_len; i++) {
		if (data[data_idx[i]][feature_idx] == 1) { yes_data[y_idx] = data_idx[i]; y_idx++; }
		else if (data[data_idx[i]][feature_idx] == 0) { no_data[n_idx] = data_idx[i]; n_idx++; }
	}

	// ��� �������� class count
	for (int i = 0; i < data_len; i++) {
		for (int j = 0; j < 6; j++)
			if (data_class[data_idx[i]] == j + 1) major_class[j]++;
	}

	// ���� ���� �������� class�� ��� ���� ���
	if (major_class[1] == 0 && major_class[2] == 0 && major_class[3] == 0 && major_class[4] == 0 && major_class[5] == 0) return;
	else if (major_class[0] == 0 && major_class[2] == 0 && major_class[3] == 0 && major_class[4] == 0 && major_class[5] == 0) return;
	else if (major_class[0] == 0 && major_class[1] == 0 && major_class[3] == 0 && major_class[4] == 0 && major_class[5] == 0) return;
	else if (major_class[0] == 0 && major_class[1] == 0 && major_class[2] == 0 && major_class[4] == 0 && major_class[5] == 0) return;
	else if (major_class[0] == 0 && major_class[1] == 0 && major_class[2] == 0 && major_class[3] == 0 && major_class[5] == 0) return;
	else if (major_class[0] == 0 && major_class[1] == 0 && major_class[2] == 0 && major_class[3] == 0 && major_class[4] == 0) return;

	// �迭 �ִ�ġ�� �ʰ��ϴ� ��� Ʈ�� ���� �ߴ�
	if (for_node * 2 > 65535) {
		max = checkMax(major_class[0], major_class[1], major_class[2], major_class[3], major_class[4], major_class[5]);
		temp1 = insert_node(node, max);
		for_node1 *= 2;
		nodes[for_node1] = max;
		return;
	}

	else { // leftchild ��� ����
		check = check_class(yes_data, y_idx);
		if (check != 0) { // �������� class�� ��� ���� ���
			temp1 = insert_node(node, check);
			for_node1 *= 2;
			nodes[for_node1] = check;
			make_child(temp1, left_feature, for_node1, yes_data, y_idx);
		}
		else {
			left_feature = max_info(yes_data, y_idx, for_node1);
			for_node1 *= 2;
			nodes[for_node1] = left_feature;
			temp1 = insert_node(node, left_feature);
			make_child(temp1, left_feature, for_node1, yes_data, y_idx);
		}

		// rightchild ��� ����
		check = check_class(no_data, n_idx);
		if (check != 0) { // �������� class�� ��� ���� ���
			temp2 = insert_node(node, check);
			for_node2 = (for_node2 * 2) + 1;
			nodes[for_node2] = check;
			make_child(temp2, right_feature, for_node2, no_data, n_idx);
		}
		else {
			right_feature = max_info(no_data, n_idx, for_node2);
			for_node2 = (for_node2 * 2) + 1;
			nodes[for_node2] = right_feature;
			temp2 = insert_node(node, right_feature);
			make_child(temp2, right_feature, for_node2, no_data, n_idx);
		}
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
	if (check != 0) {
		printf("�ƹ��� �з��� �̷������ �ʾҽ��ϴ�.");
		rootNode->data = check;
		rootNode->leftchild = NULL;
		rootNode->rightchild = NULL;
		return rootNode;
	}

	// decision tree�� ��Ʈ��� ����
	feature_idx = max_info(data_index, data_num, for_node);
	rootNode->data = feature_idx;
	rootNode->leftchild = NULL;
	rootNode->rightchild = NULL;
	for_node++;
	nodes[for_node] = feature_idx;

	// ��Ʈ ��� ������ ���ο� Ʈ�� ����
	make_child(rootNode, feature_idx, for_node, data_index, data_num);
	return rootNode;
}

//  --------- 8. Decision Tree ���� ��� �޸� �Ҵ� ���� �Լ� --------- 
void postorderTraverse(treePtr np) {
	if (np == NULL) return;
	postorderTraverse(np->leftchild);
	postorderTraverse(np->rightchild);
}
void delTree(treePtr np) {
	if (np != NULL) {
		postorderTraverse(np->leftchild);
		postorderTraverse(np->rightchild);
		free(np);
	}
}

//  -------------------------- 9. main �Լ� ----------------------------
int main() {
	int data_feature, feature_idx;
	char major_name[20];
	char basic_major[26][50];
	char class_list[6][50];
	char student_major[59][50];
	int major_num = 0, total_major_num = 0;
	float major_percent = 0;
	int total_major[26];
	int test[26];

	// data.txt ���� ���� �� ���� ����
	FILE *fp;
	fp = fopen("input.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	fscanf(fp, "%d%d", &data_num, &feature_num);
	for (int i = 0; i < data_num; i++) {
		for (int j = 0; j < feature_num; j++)
			fscanf(fp, "%d", &data[i][j]);
		fscanf(fp, "%d", &data_class[i]);
	} fclose(fp);

	// ���� �к� ���� ����Ʈ ����
	fp = fopen("major_list.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	for (int i = 0; i < 26; i++)
		fscanf(fp, "%s", &basic_major[i]);
	fclose(fp);

	// ���� �о� ����Ʈ ����
	fp = fopen("class_list.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	for (int i = 0; i < 6; i++)
		fscanf(fp, "%s", &class_list[i]);
	fclose(fp);

	// ���� ���� ����Ʈ ����
	fp = fopen("student_major.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	for (int i = 0; i < 59; i++)
		fscanf(fp, "%s", &student_major[i]);
	fclose(fp);

	// �ʱ� ������ idx �迭 ����
	int *data_index = malloc(sizeof(int) * data_num);
	for (int i = 0; i < data_num; i++)
		data_index[i] = i;

	// ���ο� �������� ������ �ޱ�
	printf("���ο� ���������� �̸��� �Է��� �ּ��� (��������/20�� �̳�)\n\n>> ");
	scanf("%s", major_name);
	printf("\n������ �ش� ������ �����ϴ� �а� ���� ���� �����Դϴ�.\n\n");
	for (int i = 0; i < 26; i++) {
		printf("- %s���� ������ ������ �Է����ּ��� : ", basic_major[i]);
		scanf("%d", &major_num);
		total_major[i] = major_num;
		total_major_num += major_num; }
	for (int i = 0; i < 26; i++) {
		major_percent = (float)total_major[i] / (float)total_major_num;
		if (major_percent > 0.2) test[i] = 1;
		else test[i] = 0; }

	// decisiontree ����
	treePtr rootNode, temp;
	rootNode = create_tree(data_index);
	temp = rootNode;
	feature_idx = temp->data;

	// test ������ �з� : �ڽĳ�尡 �� �� NULL�� �ƴϸ� ��� �ݺ�
	while (temp->leftchild != NULL && temp->rightchild != NULL) {
		data_feature = test[feature_idx];
		if (data_feature == 1) temp = temp->leftchild;
		else if (data_feature == 0) temp = temp->rightchild;
		feature_idx = temp->data;
		//printf("%d %d \n", temp->leftchild->data, temp->rightchild->data);
	}
	// �ڽĳ���� �� �� �ϳ��� NULL�� ��� (������ left)
	if (temp->leftchild != NULL && temp->rightchild == NULL) {
		temp = temp->leftchild;
		feature_idx = temp->data;
	}// ��������� ������ �� ��ȯ

	printf("\n< '%s'�� '%s' �о� �Դϴ�. >\n", major_name, class_list[feature_idx-1]);
	printf("< �Ʒ��� �ش� �о߿� �����Ǿ��ִ� �������� ����Ʈ �Դϴ�. Ȯ���غ�����. >\n\n");
	for (int i = 0; i < 59; i++) {
		if (data_class[i] == feature_idx) printf(">> %s\n", student_major[i]);
	} printf("\n");

	// ������ Ʈ���� ��� ��� �޸� �Ҵ� ����
	delTree(rootNode);

	free(data_index);
	system("pause");
	return 0;
}