//
// Created by 전준호 on 2017. 11. 8..
//

#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;

void staticMatrix();
void stackDynamicMatrix();

int main() {
    staticMatrix(); //Ignore this result
    staticMatrix();
    stackDynamicMatrix(); //Ignore this result
    stackDynamicMatrix();
    return 0;
}

void staticMatrix(){
    time_t begin_s_a;
    time(&begin_s_a);

    static int a[300][300];
    static int b[300][300];
    static int c[300][300];

    time_t begin_s;
    time(&begin_s);
    // Fill with random numbers
    for (int i = 0; i < 300; i++)
        for (int j = 0; j < 300; j++) {
            a[i][j] = rand() % 100 + 1;
            b[i][j] = rand() % 100 + 1;
        }
    time_t end_s;
    time(&begin_s);

    cout<<"Static make Time : " << difftime(end_s, begin_s)<<endl;

    time_t begin;
    time(&begin);
    // Do large number of matrix multiplications
    for (double k = 0; k < 300000; k++)
        for (int i = 0; i < 300; i++)
            for (int j = 0; j < 300; j++) {
                c[i][j] = a[i][j] * b[i][j];
            }
    time_t end;
    time(&end);

    cout<<"Static sum Time : " << difftime(end, begin)<<endl;

    time_t end_s_a;
    time(&end_s_a);
    cout<<"Static excute Time : " << difftime(end_s_a, begin_s_a)<<endl;
    return;
}

void stackDynamicMatrix(){
    time_t begin2_s_a;
    time(&begin2_s_a);

    int d[300][300];
    int e[300][300];
    int f[300][300];

    time_t begin2_s;
    time(&begin2_s);
    // Fill with random numbers
    for (int i = 0; i < 300; i++)
        for (int j = 0; j < 300; j++) {
            d[i][j] = rand() % 100 + 1;
            e[i][j] = rand() % 100 + 1;
        }
    time_t end2_s;
    time(&end2_s);
    cout<<"Stack-Dynamic make Time : " << difftime(end2_s, begin2_s)<<endl;

    time_t begin2;
    time(&begin2);
    // Do large number of matrix multiplications
    for (double k = 0; k < 300000; k++)
        for (int i = 0; i < 300; i++)
            for (int j = 0; j < 300; j++) {
                f[i][j] = d[i][j] * e[i][j];
            }
    time_t end2;
    time(&end2);
    cout<<"Stack-Dynamic sum Time : " << difftime(end2, begin2)<<endl;

    time_t end2_s_a;
    time(&end2_s_a);
    cout<<"Stack-Dynamic excute Time : " << difftime(end2_s_a, begin2_s_a)<<endl;
    return;
}