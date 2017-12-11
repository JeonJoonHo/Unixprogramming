//
// Created by 전준호 on 2017. 11. 8..
//
#include <iostream>
#include <time.h>
using namespace std;

void my_func();

int main()
{

    float start = clock();

    for (int i = 0; i < 100000; i ++)
    {
        my_func();
    }

    float end = clock();
    cout << "Ending time of clock: " << (end - start) / ((double)CLOCKS_PER_SEC);
}

void my_func()
{
    int **array;
    array = new int *[10];

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            array[i] = new int[10];
        }
    }
}