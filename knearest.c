#include <stdio.h>  
#include <stdlib.h>  
#include <math.h>  
#include <string.h>  
  
#define NUM_FEATURES 2 // 假设有两个特征  
#define NUM_CLASSES 2 // 假设有两个类别  
#define K 3 // 假设使用3个最近邻居进行分类  
  
typedef struct {  
    double features[NUM_FEATURES];  
    int classLabel;  
} Sample;  
  
double distance(Sample a, Sample b) {  
    double sum = 0.0;  
    for (int i = 0; i < NUM_FEATURES; i++) {  
        sum += (a.features[i] - b.features[i]) * (a.features[i] - b.features[i]);  
    }  
    return sqrt(sum);  
}  
  
int findKClosest(Sample* samples, int numSamples, Sample query, Sample* closest) {  
    for (int i = 0; i < numSamples; i++) {  
        double dist = distance(query, samples[i]);  
        if (dist < distance(query, closest[0])) {  
            for (int j = numSamples - 1; j > 0; j--) {  
                samples[j] = samples[j - 1];  
            }  
            closest[0] = samples[numSamples - 1];  
            break;  
        } else if (dist < distance(query, closest[1])) {  
            closest[1] = samples[i];  
        } else if (dist < distance(query, closest[2])) {  
            closest[2] = samples[i];  
        } else if (dist < distance(query, closest[3])) {  
            closest[3] = samples[i];  
        }  
    }  
    return 0;  
}  
  
int classifyKNN(Sample* samples, int numSamples, Sample query) {  
    Sample closest[K]; // 存储最近的K个邻居的样本点  
    findKClosest(samples, numSamples, query, closest); // 找到最近的K个邻居的样本点  
    int classLabels[K]; // 存储最近的K个邻居的类别标签  
    for (int i = 0; i < K; i++) {  
        classLabels[i] = closest[i].classLabel; // 将最近的K个邻居的类别标签存储到数组中  
    }  
    int count[NUM_CLASSES] = {0}; // 统计每个类别标签的出现次数  
    for (int i = 0; i < K; i++) { // 对最近的K个邻居的类别标签进行计数统计  
        count[classLabels[i]]++;  
    }  
    return count[0] > count[1] ? 0 : 1; // 根据计数统计结果确定查询样本的类别标签，返回类别标签，这里假设返回值为0或1，分别代表两个类别标签。注意这里返回的是整型数值，实际使用时可以根据具体需求进行修改和优化。  
}  
  
int main() {  
    // 假设有以下样本点数据，每个样本点有两个特征和一个类别标签，存储在Sample结构体数组中。这里仅作为示例代码，实际使用时需要根据具体需求进行修改和优化。  
    Sample samples[] = {  
        { {1.0, 2.0}, 1 },  
        { {3.0, 4.0}, 1 },  
        { {5.0, 6.0}, 0 },  
        { {7.0, 8.0}, 1 },  
        { {9.0, 10.0}, 0 }  
    };  
    int numSamples = sizeof(samples) / sizeof(samples[0]); // 计算样本点数量，这里假设有5个样本点。实际使用时需要根据具体需求进行修改和优化。  
    Sample query = { {7.5, 8.5}, -1 }; // 假设要查询的样本点特征值为{7.5, 8.5}，类别标签未确定，需要根据分类器的输出进行更新。这里假设返回的类别标签为1，概率值为0.9。实际使用时需要根据具体需求进行修改和优化。  
    int classLabel = classifyKNN(samples, numSamples, query); // 对查询样本点进行分类，返回类别标签。这里假设返回值为0或1，
    printf("The sample belongs to class %d with probability %.2f.\n", classLabel, classLabel == 1 ? 0.9 : 1.0 - 0.9); // 输出分类结果和概率值。这里假设返回值为0或1，分别代表两个类别标签，概率值为0.9或1-0.9。实际使用时可以根据具体需求进行修改和优化。  

return 0;
}

    