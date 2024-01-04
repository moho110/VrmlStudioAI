#include <math.h>
#include <stdio.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-6

typedef struct {
    double data[3][3];  // 3x3矩阵
} Matrix;

typedef struct {
    double values[3];  // 三个主成分的值
} Components;

// 计算矩阵的转置
Matrix transpose(Matrix A) {
    Matrix AT;
    AT.data[0][0] = A.data[0][0];
    AT.data[0][1] = A.data[1][0];
    AT.data[0][2] = A.data[2][0];
    AT.data[1][0] = A.data[0][1];
    AT.data[1][1] = A.data[1][1];
    AT.data[1][2] = A.data[2][1];
    AT.data[2][0] = A.data[0][2];
    AT.data[2][1] = A.data[1][2];
    AT.data[2][2] = A.data[2][2];
    return AT;
}

// 计算矩阵的逆
Matrix inverse(Matrix A) {
    Matrix AT = transpose(A);
    Matrix adjoint = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    double det =
        A.data[0][0] *
            (A.data[1][1] * A.data[2][2] - A.data[1][2] * A.data[2][1]) -
        A.data[0][1] *
            (A.data[1][0] * A.data[2][2] - A.data[1][2] * A.data[2][0]) +
        A.data[0][2] *
            (A.data[1][0] * A.data[2][1] - A.data[1][1] * A.data[2][0]);
    adjoint.data[0][0] =
        (A.data[1][1] * A.data[2][2] - A.data[1][2] * A.data[2][1]) / det;
    adjoint.data[1][1] =
        (A.data[0][0] * A.data[2][2] - A.data[0][2] * A.data[2][0]) / det;
    adjoint.data[2][2] =
        (A.data[0][0] * A.data[1][1] - A.data[0][1] * A.data[1][0]) / det;
    adjoint.data[1][0] =
        (A.data[0][1] * AT.data[2][2] - AT.data[1][2] * A.data[2][0]) / det;
    adjoint.data[2][0] =
        (A.data[0][2] * AT.data[1][1] - AT.data[2][1] * A.data[1][0]) / det;
    adjoint.data[0][1] =
        (AT.data[1][2] * A.data[2][0] - AT.data[1][0] * A.data[2][2]) / det;
    adjoint.data[0][2] =
        (AT.data[2][1] * A.data[0][0] - AT.data[0][1] * A.data[2][0]) / det;
    adjoint.data[1][2] =
        (AT.data[2][0] * A.data[0][1] - AT.data[0][2] * AT.data[2][1]) / det;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            A.data[i][j] = adjoint.data[i][j];
        }
    }
    return AT;
}

// 计算矩阵乘积
Matrix multiply(Matrix A, Matrix B) {
    Matrix C;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            C.data[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                C.data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
    return C;
}

// 计算矩阵的特征值和特征向量
Components eigen(Matrix A) {
    Components EV;
    Matrix D, Q, T;
    double d[3][3];
    double p[3];
    double lambda[3];
    int i, j, k;
    D = {{A.data[0][0], 0, 0}, {0, A.data[1][1], 0}, {0, 0, A.data[2][2]}};
    T = {{A.data[0][1], A.data[0][2], 0},
         {A.data[1][2], A.data[1][0], 0},
         {0, 0, A.data[2][0]}};
    Q = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    for (i = 0; i < 3; i++) {
        lambda[i] = d[i][i];
        p[i] = T.data[i][i];
        for (j = i + 1; j < 3; j++) {
            d[i][j] = T.data[i][j] / p[i];
            p[j] = T.data[j][j] - d[i][j] * T.data[i][j];
        }
        EV.values[i] = lambda[i] + d[i][i] * p[i] / 2;
        for (k = i + 1; k < 3; k++) {
            EV.values[i] -= d[i][k] * d[k][i] / (4 * p[k]);
        }
        EV.values[i] = sqrt(EV.values[i]);
        for (j = i + 1; j < 3; j++) {
            Q.data[j][i] = -d[i][j] / (2 * p[j]);
            Q.data[i][j] = Q.data[j][i];
            Q.data[j][j] += d[i][j] * d[j][i] / (4 * p[j]);
        }
        for (k = i + 1; k < 3; k++) {
            for (j = i + 1; j < 3; j++) {
                T.data[k][j] -= Q.data[k][i] * T.data[i][j];
                D.data[k][j] -= Q.data[k][i] * D.data[i][j];
            }
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = i + 1; j < 3; j++) {
            if (fabs(D.data[i][j]) > EPSILON) {
                for (k = i + 1; k < 3; k++) {
                    Q.data[k][j] /= D.data[k][j];
                    Q.data[k][i] /= -D.data[k][j];
                }
                for (k = i + 1; k < 3; k++) {
                    for (l = j + 1; l < 3; l++) {
                        T.data[k][l] -= Q.data[k][i] * D.data[i][l] +
                                        Q.data[k][j] * D.data[j][l];
                    }
                }
            } else if (fabs(T.data[i][j]) > EPSILON) {
                Q.data[i][j] = T.data[i][j];
                for (k = j = i + 1; k < 3; k++) {
                    Q.data[k][j] /= T.data[k][j];
                    Q.data[k][i] /= -T.data[k][j];
                }
                for (k = i + 1; k < 3; k++) {
                    for (l = j + 1; l < 3; l++) {
                        T.data[k][l] -= Q.data[k][i] * T.data[i][l] +
                                        Q.data[k][j] * T.data[j][l];
                    }
                }
            }
        }
    }
    Matrix Q_inv = transpose(Q);
    Matrix D_inv = inverse(D);
    Matrix T_inv = inverse(T);
    Matrix P = multiply(multiply(Q, multiply(D_inv, T)), Q_inv);
    EV.values[2] = P.data[0][0];
    EV.values[1] = P.data[1][1];
    EV.values[0] = P.data[2][2];
    return EV;
}

int main() {
    Matrix A = {{3, -1, 0}, {-1, 3, -1}, {0, -1, 3}};
    Components EV = eigen(A);
    printf("Eigenvalues: %f, %f, %f\n", EV.values[0], EV.values[1],
           EV.values[2]);
    return 0;
}
