#include <atomic>
#include <cstdint>
#include <iostream>

using pair = struct { std::uint32_t first; std::uint32_t second; };
using bruhpair = struct {std::uint32_t first; std::uint32_t second;};
int main()
{
    std::atomic<bruhpair> A;
    bruhpair haha = {0, 1};
    A.store(haha);
    bruhpair B = A.load();
    //std::cout << B.first.second << " " << B.second.first <<std::endl;
}
