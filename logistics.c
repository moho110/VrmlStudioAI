#include <stdio.h>  
#include <math.h>  
#include <stdlib.h>  
  
#define MAX_ITERATIONS 1000 // 最大迭代次数  
#define EPSILON 0.00001 // 收敛精度  
  
// 定义特征和标签数据结构  
typedef struct {  
    double* features; // 特征数组  
    int num_features; // 特征数量  
    double label; // 标签值  
} DataPoint;  
  
// 定义训练数据集  
typedef struct {  
    DataPoint* data; // 数据点数组  
    int num_data; // 数据点数量  
} Dataset;  
  
// 定义模型数据结构  
typedef struct {  
    double* weights; // 权重数组  
    int num_weights; // 权重数量  
} Model;  
  
// 初始化模型权重  
void initialize_weights(Model* model, int num_features) {  
    model->weights = (double*)malloc(num_features * sizeof(double));  
    model->num_weights = num_features;  
    for (int i = 0; i < num_features; i++) {  
        model->weights[i] = (rand() % 1000) / 1000.0 - 0.5; // 随机初始化权重值，范围为[-0.5, 0.5]  
    }  
}  
  
// 计算sigmoid函数值  
double sigmoid(double z) {  
    return 1.0 / (1.0 + exp(-z));  
}  
  
// 训练模型  
void train(Dataset* dataset, Model* model) {  
    int num_data = dataset->num_data;  
    int num_features = dataset->data[0].num_features;  
    double learning_rate = 0.01; // 学习率  
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {  
        double total_error = 0.0; // 总误差  
        for (int i = 0; i < num_data; i++) {  
            DataPoint* point = dataset->data + i;  
            double z = 0.0; // 线性函数的输出值  
            for (int j = 0; j < num_features; j++) {  
                z += point->features[j] * model->weights[j];  
            }  
            double prediction = sigmoid(z); // 使用sigmoid函数进行概率转换  
            double error = prediction - point->label; // 计算误差值  
            total_error += error * error; // 累加误差平方值  
            for (int j = 0; j < num_features; j++) {  
                model->weights[j] += learning_rate * error * point->features[j]; // 更新权重值  
            }  
        }  
        total_error /= num_data; // 计算平均误差平方值  
        if (total_error < EPSILON || iter % 50 == 49) { // 检查收敛精度或迭代次数是否达到最大值，满足任一条件则退出循环  
            printf("Epoch %d: error = %.6f\n", iter + 1, total_error); // 输出误差信息，保留小数点后六位  
            break;  
        }  
    }  
}  
  
// 进行预测，返回概率值（介于[0,1]之间）  
double predict(Model* model, double* features) {  
    double z = 0.0; // 线性函数的输出值  
    for (int i = 0; i < model->num_weights; i++) {  
        z += features[i] * model->weights[i]; // 对每个特征乘以对应的权重并累加得到线性函数输出值z  
    }  
    return sigmoid(z); // 使用sigmoid函数进行概率转换，返回概率值（介于[0,1]之间）  
}  
  
int main() {  
    // 创建训练数据集（示例数据）和测试数据集（示例数据）以及模型数据结构并初始化权重值（示例初始化）  
    Dataset train_dataset = { NULL, 0 }; // 训练数据集，包含数据点数组和数据点数量，这里不包含实际数据，仅为示例结构体定义，实际使用时需要添加数据点数据和数量信息等。同理，测试数据集和模型数据结构也需要根据实际需求进行初始化。  
    Model model = { NULL, 0};
    initialize_weights(&model, 2); // 初始化模型权重（示例初始化）
    // 训练模型  
    train(&train_dataset, &model);  
 
    // 进行预测，输出概率值（介于[0,1]之间）  
    double features_test[] = {0.5, 0.5}; // 测试数据集的特征数组，这里不包含实际数据，仅为示例数组定义，实际使用时需要添加特征数据。  
    double probability = predict(&model, features_test); // 进行预测，返回概率值（介于[0,1]之间）  
    printf("Prediction probability: %.6f\n", probability); // 输出概率值，保留小数点后六位  
 
return 0;
}
