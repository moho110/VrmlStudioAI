#include <stdio.h>  
#include <stdlib.h>  
  
// 结构体表示一个节点  
typedef struct Node {  
    int featureIndex; // 特征索引  
    double threshold; // 分割阈值  
    struct Node* left; // 左子节点  
    struct Node* right; // 右子节点  
} Node;  
  
// 创建新节点  
Node* createNode(int featureIndex, double threshold, Node* left, Node* right) {  
    Node* newNode = (Node*)malloc(sizeof(Node));  
    newNode->featureIndex = featureIndex;  
    newNode->threshold = threshold;  
    newNode->left = left;  
    newNode->right = right;  
    return newNode;  
}  
  
// 函数用于根据给定的特征值和阈值判断应该进入哪个子节点  
Node* findLeaf(Node* root, double* sample) {  
    if (root == NULL) {  
        return NULL; // 空节点，返回NULL  
    }  
    if (root->left == NULL && root->right == NULL) { // 叶子节点，返回该节点  
        return root;  
    }  
    if (sample[root->featureIndex] < root->threshold) { // 进入左子树  
        return findLeaf(root->left, sample);  
    } else { // 进入右子树  
        return findLeaf(root->right, sample);  
    }  
}  
  
// 主函数入口  
int main() {  
    // 创建和初始化一棵简单的CART树（这里仅为示例，实际应用中树的结构和初始化方式会有所不同）  
    Node* root = createNode(0, 0.5, NULL, NULL); // 根节点，特征0，阈值0.5，无左右子节点  
    Node* leaf = findLeaf(root, (double*)malloc(sizeof(double))); // 寻找叶子节点，这里假设只有一个样本点，值为0.35  
    if (leaf != NULL) {  
        printf("Leaf node classification: The sample belongs to class 1.\n"); // 这里假设属于class 1  
    } else {  
        printf("No leaf node found.\n"); // 没找到叶子节点，输出此信息  
    }  
    free(leaf); // 释放内存  
    free(root); // 释放内存  
    return 0;  
}
