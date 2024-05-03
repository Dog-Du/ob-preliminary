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

struct A {
  bool a;
  union {
    int b;
    float c;
  } value;
};

int main() {
  A a;
  a.a = true;
  a.value.b = 10;

  char *c = (char *)&a.a;

  for (int i = 0; i < 5; ++i) {
    cout << (int)(*c) << ' ';
    ++c;
  }
  system("pause");
  return 0;
}
