#include "ConcatStringTree.h"

ConcatStringTree::ConcatStringTree(const char* c)
{
    root =  new node{};
    root->data = c;
    root->length = root->data.length();
    size = root->length; //phân biệt độ dài của root và độ dài của chuỗi
    numOfnodes = 1;
}

int ConcatStringTree::length() const
{
    return size; //cần tạo 1 hàm cho size
}
void ConcatStringTree::setSize()
{
    if (root) this->size += this->root->left + this->root->leftLength + this->root->rightLength; //đưa công việc tạo size lại cho class node
}

char ConcatStringTree::get(int index)
{
    if (index < 0 || index > size) throw out_of_range("Index of string is invalid!");
    else 
    {
        return recursiveGet(index, this->root);
    }
}
char ConcatStringTree::recursiveGet(int index, ConcatStringTree::node* node)
{
    if (index <= node->leftLength - 1)
    {
        return recursiveGet(index, node->left);
    }
    else if (index >= node->leftLength && index <= node->leftLength + node->length -1)
    {
        return node->data[index - node->leftLength];
    }
    else return recursiveGet(index - node->leftLength - node->length, node->right);
}


int ConcatStringTree::indexOf(char c) 
{
    int found = -1;
    recursiveFind(c, this->root, this->root->leftLength, found);
    return found;
}
int ConcatStringTree::recursiveFind (char c, node* node, int currIndex, int &found)
{   
    if (found == -1)
    {   
        if (!node) return;

        recursiveFind(c, node->left, currIndex - node->leftLength, found);
        auto i = node->data.find(c);
        if (i != string::npos) //found
        {
            found = (int)i + currIndex;
        }
        recursiveFind(c, node->left, currIndex + node->rightLength, found);
    }
}