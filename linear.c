#include <stdio.h>  
#include <math.h>  
  
// 线性回归结构体  
typedef struct {  
    double x;  
    double y;  
} Point;  
  
// 计算斜率b1和截距b0  
void linearRegression(Point* data, int n, double* b0, double* b1) {  
    double sum_x = 0, sum_y = 0, sum_x2 = 0, sum_xy = 0;  
    double mean_x = 0, mean_y = 0;  
    int i;  
  
    for (i = 0; i < n; i++) {  
        sum_x += data[i].x;  
        sum_y += data[i].y;  
        sum_x2 += data[i].x * data[i].x;  
        sum_xy += data[i].x * data[i].y;  
    }  
  
    mean_x = sum_x / n;  
    mean_y = sum_y / n;  
  
    *b1 = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);  
    *b0 = mean_y - *b1 * mean_x;  
}  
  
int main() {  
    // 示例数据点  
    Point data[] = {  
        {1, 3},  
        {2, 5},  
        {3, 7},  
        {4, 9},  
        {5, 11}  
    };  
    int n = sizeof(data) / sizeof(data[0]);  // 数据点数量  
    double b0, b1;  // 截距和斜率  
    double x, y;  // 用于预测的变量  
    printf("Enter x for prediction: ");  
    scanf("%lf", &x);  // 读取预测的x值  
    linearRegression(data, n, &b0, &b1);  // 计算斜率和截距  
    y = b0 + b1 * x;  // 使用线性回归模型进行预测  
    printf("Predicted y: %.2lf\n", y);  // 输出预测的y值，保留两位小数  
    return 0;  
}
