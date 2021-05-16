#include<iostream>
#include<cilk/cilk.h>

int fib (int n)
{
    int ret = 0;
    if (n < 2) return n;
    else {
        int temp = cilk_spawn fib(n-1);
        ret = temp + fib(n-2);
        cilk_sync;
        return ret;
    }
}

int main(){
    std::cout << fib(10) <<std::endl;
}