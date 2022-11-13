#include "ConcatStringTree.h"

ConcatStringTree::ConcatStringTree(const char* c)
{
    root =  new node{};
    root->data = c;
    root->length = root->data.length();
}