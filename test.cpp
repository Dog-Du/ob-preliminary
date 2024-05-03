#define _CRT_SECURE_NO_WARNINGS 1
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <vector>

using namespace std;
#define ll long long

struct A
{
  // bool a;
  union
  {
    int   b;
    float c;
    char  d[4];
  } value;
};

int main()
{
  A a;
  // a.a       = true;
  a.value.b = 10;
  cout << sizeof(a) << endl;
  cout << (ll)&a.value   << ' ' << (ll)a.value.d << ' ' << (ll)&a.value.b << ' '
       << (ll)&a.value.d[1] << ' ' << *(int *)(&a.value) <<endl;

  system("pause");
  return 0;
}
