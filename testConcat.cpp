#include "ConcatStringTree.h"

int main()
{

    auto s1 = new ConcatStringTree ("hello");
    auto s2 =  new ConcatStringTree("world");
    ConcatStringTree s3 = s1->concat(*s2);
    delete s1;
    delete s2;
    return 0;
}