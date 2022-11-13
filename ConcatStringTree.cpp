#include "ConcatStringTree.h"

ConcatStringTree::ConcatStringTree(const char* c)
{
    root =  new node{};
    root->data = c;
    root->length = root->data.length();
    size = root->length; //phân biệt độ dài của root và độ dài của chuỗi
}

int ConcatStringTree::length() const
{
    return size; //cần tạo 1 hàm cho size
}

char ConcatStringTree::get(int index)
{
    
}