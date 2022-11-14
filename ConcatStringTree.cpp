#include "ConcatStringTree.h"
 
long ConcatStringTree::node::maxID{0}; //because there is no before

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
    if (root) this->size += this->root->length + this->root->leftLength + this->root->rightLength; //đưa công việc tạo size lại cho class node
    else this->size = 0;
}
int ConcatStringTree::node::getLength(node* node) const
{
    if (!node) return 0;
    
    node->length = node->data.length();
    return node->length + getLength(node->left) + getLength(node->right);
}
void ConcatStringTree::node::setLength(node* root) //đặt cho cả cây từ gốc, preorder
{
    if (!root) return;

    root->length = root->data.length();
    root->leftLength = getLength(root->left);
    root->rightLength = getLength(root->right);
    setLength (root->left);
    setLength(root->right);
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
void ConcatStringTree::recursiveFind (char c, node* node, int currIndex, int &found)
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
        recursiveFind(c, node->right, currIndex + node->rightLength, found);
    }
}


string ConcatStringTree::toStringPreOrder() const
{
    return recursivetoStringPre(this->root);
}
string ConcatStringTree::recursivetoStringPre(node* root) const
{
    if (!root) return "";

    string s1 = root->data;
    string s2 = recursivetoStringPre(root->left);
    string s3 = recursivetoStringPre(root->right);
    return s1 + s2 + s3;
}


string ConcatStringTree::toString() const
{
    return recursivetoString(this->root);
}
string ConcatStringTree::recursivetoString(node* root) const
{
    if (!root) return "";
    string s1 = root->data;
    string s2 = recursivetoString(root->left);
    string s3 = recursivetoString(root->right);
    return s2 + s1 + s3;
}


ConcatStringTree& ConcatStringTree::concat(const ConcatStringTree & otherS) const //có tránh được copy constructor?
{
    ConcatStringTree *newstr = new ConcatStringTree("");
    newstr->root->left = this->root;
    newstr->root->right = otherS.root;

    newstr->root->leftLength = this->root->getLength(this->root);
    newstr->root->rightLength = otherS.root->getLength(otherS.root);
    newstr->setSize();

    return *newstr;
}

/*
ConcatStringTree::ConcatStringTree(ConcatStringTree& other): size{other.size}, numOfnodes{other.numOfnodes}
{
    root = new node(this->root); //đưa lại cho hàm dưới
}
ConcatStringTree::node:: node(node*& other)
{
    
}
*/

ConcatStringTree ConcatStringTree::subString(int from, int to) const
{
    if (from < 0 || from > this->size - 1 || to < 1 || to > this->size) throw out_of_range("Index of string is invalid!");
    if (from >= to) throw logic_error("Invalid range!");

    //lúc này chắc chắn from và to đã hợp lệ
    ConcatStringTree *newtree = new ConcatStringTree();
    this->root->setLength(); //cho chắc chắn là tính toán đúng độ dài trái phải
    newtree-> root = recursiveSubstr(this->root, from, to - 1);
    newtree->setSize();
    return *newtree;

}
ConcatStringTree::node* ConcatStringTree:: recursiveSubstr(const node* root, int from, int to) //trả về gốc mới của 1 cây mới từ đó, to ở đây là vị trí chính xác không phải vị trí sau
{
    if (!root) return nullptr;

    //trường hợp đơn giản: from và to nằm trong root gốc
    const int leftside = root->leftLength;
    const int rightside = root->leftLength + root->length -1;

    if (from >= leftside && to <= rightside)
    {
        node *newnode = new node( root->data.substr(from, to - from + 1) );
        return newnode;
    }
    else if (from < leftside && to < leftside) //cùng nằm trái
    {
        return recursiveSubstr(root->left, from, to);
    }
    else if(from > rightside && to > rightside ) //cùng nằm phải
    {   
        return recursiveSubstr(root->right, from - rightside - 1, to -rightside - 1 );
    }
    else if (from < leftside && to > rightside)
    {
        node *newnode = new node(root->data);
        newnode->left = recursiveSubstr(root->left, from, leftside - 1);
        newnode->right = recursiveSubstr (root->right, 0, to - (rightside + 1) );
        return newnode;
    }
    else if (from < leftside && to >= leftside && to <= rightside ) //xem kẽ nhau
    {
        node *newnode = new node( root->data.substr(0, to + 1) );
        newnode->left = recursiveSubstr(root->left, from, root->leftLength - 1);
        return newnode;
    }
    else if (from >= leftside && from <= rightside && to > rightside)
    {
        node *newnode = new node( root->data.substr(from) );
        newnode->right = recursiveSubstr(root->right, 0, to - rightside - 1);
        return newnode;
    }
    else return nullptr; //cho có
}


ConcatStringTree ConcatStringTree::reverse() const
{
    ConcatStringTree *newtree = new ConcatStringTree();
    //lúc này root vẫn là nullptr
    newtree->root = recursiveReverse(this->root);
    newtree->setSize();
    return *newtree;
}
ConcatStringTree::node* ConcatStringTree::recursiveReverse(node* root) //trả về gốc mới của 1 cây mới từ đó
{
    if (!root) return nullptr;

    node *newnode = new node(root->data);
    newnode->data = string(root->data.rbegin(), root->data.rend());
    newnode->right =  recursiveReverse(root->left);
    newnode->left = recursiveReverse(root->right);

    newnode->setLength();
    return newnode;
}


void ConcatStringTree::node::assignLeft(node* p)
{
    if (this->left == nullptr)
    {
        if (!p) return;

        this->left = p;
        .... thay đổi AVL của p -> viết thêm 2 hàm insert và remove cho AVL
    }
    else
    {
        ...thay đổi ParentsTree của this->left trước rồi gán qua p, rồi thay đổi AVL của p 
    }
}
void assignRight(node* p) //tương tự assignleft
{
    if (this->right == nullptr)
    {

    }
    else
    {

    }
}