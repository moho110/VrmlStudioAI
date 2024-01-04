#include <stdio.h>  
#include <stdlib.h>  
#include <math.h>  
  
#define MAX_ITERATIONS 100  
#define EPSILON 0.0001  
#define K 3  
  
typedef struct {  
    double x;  
    double y;  
} Point;  
  
typedef struct {  
    Point center;  
    int count;  
} Cluster;  
  
Cluster kmeans(Point data[], int n, int k) {  
    Cluster clusters[k];  
    int labels[n];  
    int i, j, index, min_distance, distance, count = 0;  
    Point temp;  
    for (i = 0; i < k; i++) {  
        clusters[i].center.x = clusters[i].center.y = 0.0;  
        clusters[i].count = 0;  
    }  
    for (i = 0; i < n; i++) {  
        distance = 0.0;  
        for (j = 0; j < k; j++) {  
            min_distance = distance;  
            index = j;  
            distance = sqrt(pow(data[i].x - clusters[j].center.x, 2) + pow(data[i].y - clusters[j].center.y, 2));  
            if (distance < min_distance) {  
                min_distance = distance;  
                index = j;  
            }  
            labels[i] = index;  
        }  
        clusters[labels[i]].count++;  
        clusters[labels[i]].center.x += data[i].x;  
        clusters[labels[i]].center.y += data[i].y;  
    }  
    for (i = 0; i < k; i++) {  
        clusters[i].center.x /= clusters[i].count;  
        clusters[i].center.y /= clusters[i].count;  
    }  
    for (i = 0; i < k; i++) {  
        do {  
            count = 0;  
            for (j = 0; j < n; j++) {  
                distance = sqrt(pow(data[j].x - clusters[i].center.x, 2) + pow(data[j].y - clusters[i].center.y, 2));  
                if (distance < EPSILON) {  
                    count++;  
                    if (abs(clusters[labels[j]].center.x - data[j].x) > EPSILON || abs(clusters[labels[j]].center.y - data[j].y) > EPSILON) {  
                        temp = clusters[labels[j]].center;  
                        clusters[labels[j]].center.x = data[j].x;  
                        clusters[labels[j]].center.y = data[j].y;  
                        data[j].x = temp.x;  
                        data[j].y = temp.y;  
                    }  
                } else {  
                    if (abs(clusters[i].center.x - data[j].x) > EPSILON || abs(clusters[i].center.y - data[j].y) > EPSILON) {  
                        data[j].x -= clusters[i].center.x - data[j].x;  
                        data[j].y -= clusters[i].center.y - data[j].y;  
                    }  
                }  
            }  
        } while (count > 0);  
    }  
    return clusters[0]; // 返回第一个聚类的中心点坐标和样本数，可以根据需要修改返回值结构体类型和数量。  
}

int main() {  
    Point data[] = {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}, {10, 11}};  
    int n = sizeof(data) / sizeof(data[0]);  
    Cluster result = kmeans(data, n, K);  
    printf("Cluster center: (%f, %f)\n", result.center.x, result.center.y);  
    printf("Cluster count: %d\n", result.count);  
    return 0;  
}

