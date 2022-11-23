#include "ConcatStringTree.h"

int main()
{

    ConcatStringTree s1("hello");
    ConcatStringTree s2("world");
    ConcatStringTree s3 = s1.concat(s2);
    return 0;
}