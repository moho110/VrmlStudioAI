#include <stdio.h>  
#include <stdlib.h>  
#include <math.h>  
  
#define NUM_CLASSES 2 // 假设有两个类别  
#define NUM_FEATURES 2 // 假设有两个特征  
  
// 结构体表示一个样本点  
typedef struct Sample {  
    double features[NUM_FEATURES]; // 特征值  
    int classLabel; // 类别标签  
} Sample;  
  
// 结构体表示一个类别  
typedef struct Class {  
    int label; // 类别标签  
    double priorsProb; // 先验概率  
    double conditionalProb[NUM_FEATURES]; // 条件概率  
} Class;  
  
// 计算先验概率  
double computePriorProbability(Class* classes, int numClasses, int classLabel) {  
    int count = 0;  
    for (int i = 0; i < numClasses; i++) {  
        if (classes[i].label == classLabel) {  
            count++;  
        }  
    }  
    return (double)count / numClasses;  
}  
  
// 计算条件概率  
void computeConditionalProbability(Class* classes, int numClasses, int classLabel, double* featureValues) {  
    double sum = 0.0;  
    for (int i = 0; i < numClasses; i++) {  
        if (classes[i].label == classLabel) {  
            for (int j = 0; j < NUM_FEATURES; j++) {  
                if (classes[i].conditionalProb[j] != 0) {  
                    sum += pow((featureValues[j] - classes[i].conditionalProb[j]), 2) / (2 * classes[i].conditionalProb[j]);  
                } else {  
                    sum += 1e-100; // 防止除数为0的情况，使用一个非常小的数代替0  
                }  
            }  
        }  
    }  
    double numerator = 1.0 / (sqrt(2 * M_PI) * sum);  
    double denominator = 0.0;  
    for (int j = 0; j < NUM_FEATURES; j++) {  
        denominator += pow((featureValues[j] - classes[classLabel].conditionalProb[j]), 2) / (2 * classes[classLabel].conditionalProb[j]);  
    }  
    classes[classLabel].conditionalProb[featureValues[0]] = numerator / denominator;  
}  
  
// 使用Naive Bayes分类器对样本进行分类  
int classifySample(Class* classes, int numClasses, Sample* sample) {  
    double logProb = 0.0; // 对数概率值，用于比较不同类别的概率大小  
    for (int i = 0; i < numClasses; i++) {  
        double prob = computePriorProbability(classes, numClasses, i); // 先验概率  
        for (int j = 0; j < NUM_FEATURES; j++) {  
            prob *= classes[i].conditionalProb[j]; // 条件概率的乘积，计算样本属于该类别的概率  
        }  
        logProb = log(prob); // 对数概率值，方便比较不同类别的概率大小  
        if (logProb > 0) { // 如果概率大于0，则认为该样本属于该类别，返回类别标签和概率值  
            return i;  
        } else { // 如果概率小于等于0，则认为该样本不属于该类别，继续计算下一个类别的概率值和类别标签，返回最大的概率值和对应的类别标签，这里直接返回-1表示分类失败，因为概率值不可能为负数，这里仅作为示例代码，实际情况需要根据具体需求进行修改和优化。  
            break;  
        }  
    }  
    return -1; // 分类失败，返回-1表示无法确定样本的类别标签和概率值。这里仅作为示例代码，实际情况需要根据具体需求进行修改和优化。
}

// 初始化类别和条件概率  
void initializeClasses(Class* classes, int numClasses, Sample* samples) {  
    for (int i = 0; i < numClasses; i++) {  
        classes[i].label = i;  
        classes[i].priorsProb = 1.0 / numClasses;  
        for (int j = 0; j < NUM_FEATURES; j++) {  
            classes[i].conditionalProb[j] = 0.0;  
        }  
    }  
    for (int i = 0; i < numSamples; i++) {  
        int classLabel = samples[i].classLabel;  
        for (int j = 0; j < NUM_FEATURES; j++) {  
            classes[classLabel].conditionalProb[j] += samples[i].features[j];  
        }  
    }  
}  
  
int main() {  
    // 假设有3个样本点，每个样本点有两个特征和对应的类别标签  
    Sample samples[] = {  
        { {1.0, 2.0}, 1 },  
        { {3.0, 4.0}, 1 },  
        { {5.0, 6.0}, 0 }  
    };  
    int numSamples = sizeof(samples) / sizeof(samples[0]);  
  
    // 初始化类别和条件概率  
    Class classes[NUM_CLASSES];  
    initializeClasses(classes, NUM_CLASSES, samples);  
  
    // 假设要对一个新的样本点进行分类，特征值为{7.0, 8.0}  
    Sample newSample = { {7.0, 8.0}, -1 }; // -1表示该样本点的类别标签未确定，需要根据分类器的输出进行更新。  
    int classLabel = classifySample(classes, NUM_CLASSES, &newSample); // 对新样本点进行分类，返回类别标签和概率值。这里假设返回的类别标签为1，概率值为0.9。  
    newSample.classLabel = classLabel; // 更新样本点的类别标签为分类器输出的类别标签。  
    printf("The sample belongs to class %d with probability %.2f.\n", classLabel, newSample.probability); // 输出分类结果和概率值。这里假设输出为"The sample belongs to class 1 with probability 0.9."。  
  
    return 0;  
}

