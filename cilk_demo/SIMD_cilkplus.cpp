#include <iostream>
#include <cilk/cilk.h>

__declspec(vector) int mul2(int a)
{
  return 2 * a;
}

int main()
{
  int* A = new int[100];
  for (int i = 0; i < 100; i++) {
    A[i] = i;
  }
  int* B = new int[100];
  
  B[:] = mul2(A[:]);
}
