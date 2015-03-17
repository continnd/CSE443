#include <stdio.h>

#define REPETITIONS 500000000

void approach1(const int m[], const int v[], int r[]) {
    int i;
    for (i = 0; i < 16; i++) {
       r[i] = m[i] * v[i];
    }
}

void approach2(const int m[], const int v[], int r[]) {
       r[0] = m[0] * v[0];
       r[1] = m[1] * v[1];
       r[2] = m[2] * v[2];
       r[3] = m[3] * v[3];
       r[4] = m[4] * v[4];
       r[5] = m[5] * v[5];
       r[6] = m[6] * v[6];
       r[7] = m[7] * v[7];
       r[8] = m[8] * v[8];
       r[9] = m[9] * v[9];
       r[10] = m[10] * v[10];
       r[11] = m[11] * v[11];
       r[12] = m[12] * v[12];
       r[13] = m[13] * v[13];
       r[14] = m[14] * v[14];
       r[15] = m[15] * v[15];
}

void approach3(const int m[][4], const int v[], int r[]);

int main() {
    const int m[16] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
    int r[16];
    int count, sum = 0;

    for(count = 0; (count < REPETITIONS); count++) {
        int v[16] = {count, count, count, 0, count, count, count, 0, count, count, count, 0, count, count, count, 0};
        approach2(m, v, r);  // <---- Only modify this line!
        sum += r[2];
    }
    printf("Final sum = %d\n", sum);
    return 0;
}
