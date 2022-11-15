//this is test file
#include <bits/stdc++.h>
using namespace std;

/*
template <typename T>
class tree
{
public:
    class node;

    node* root{};
    

};

class con : private  tree<int>
{};

int main(void)
{
    return 0;
}*/

enum BalanceValue
{
	LH = -1,
	EH = 0,
	RH = 1
};
void printNSpace(int n)
{
	for (int i = 0; i < n - 1; i++)
		cout << " ";
}
void printInteger(int &n)
{
	cout << n << " ";
}

class Node
{
public:
	int data;
	Node *left;
	Node *right;
	BalanceValue balance;
	Node(const int &val)
	{
		data = val;
		left = right = NULL;
		balance = EH;
	}
};

class AVL
{
private:
	Node *root;
protected:
	Node *rotateRight(Node *&node);
	Node *rotateLeft(Node *&node);
	Node *leftBalance(Node *&node, bool &taller);
	Node *rightBalance(Node *&node, bool &taller);
	//TODO Methods
	Node *insertRec(Node *&node, const int &value, bool &taller);
public:
	AVL()
	{
		this->root = NULL;
	}
	
	
	// TODO Methods
	void insert(const int &value);
	void remove(const int &value);
};

Node *AVL::rotateRight(Node *&node) //xoay phải
{
    Node* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    //node = temp;
    return temp;
}   


Node *AVL::rotateLeft(Node *&node) //xoay trái
{
    Node* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    //node = temp;
    return temp;
}

Node *AVL::insertRec(Node *&node, const int &value, bool &taller)
{
	//ta sẽ thêm một node mang giá trị value vào cây con có nốt gốc là node, taller để chỉ rằng cây con có cao lên không
	if (!node)
	{
	    node = new Node(value);
	    taller = true;
	    return node; //đều trả về chính nó trong khi đã truyền vào tham khảo thì cần gì trả về  nữa?
	}
	else
	{
	    if (value < node->data) //to left
	    {
	        node->left = insertRec(node->left, value, taller);
	        
	        //những dòng dưới đây sẽ được thực hiện sau khi return xong,  tức là đã thực hiện insert
	        //taller == true nếu node->left sau khi insert cao hơn.
	        if (taller == true)
	        {
	            if (node->balance == LH) //đã cao hơn bên trái rồi mà sau khi insert bên trái lại cao hơn nữa
	            {
	                node = leftBalance(node, taller);
	            }
	            else if (node->balance == EH)
	            {
	                node->balance = LH;
	                taller = true; //vẫn không có gì thay đổi, nhưng chiều cao vẫn tăng thêm, phải nhận định  vậy  để dòng sau thực hiện tăng
	            }
	            else if (node->balance == RH)
	            {
	                node->balance = EH;
	                taller = false; //cân bằng lại, chiều cao vẫn là max nên không có gì thay đổi, node sau không cần phải xwr lý
	            }
	        }
	    }
	    else if (node->data < value) //to right
	    {
	        node->right = insertRec(node->right, value, taller);
	        if (taller == true)
	        {
	            if (node->balance == RH)
	            {
	                node = rightBalance(node, taller);
	            }
	            else if (node->balance == EH)
	            {
	                node->balance = RH;
	                taller = true; //vẫn không có gì thay đổi, nhưng chiều cao vẫn tăng thêm, phải nhận định  vậy  để dòng sau thực hiện tăng
	            }   //vì taller là một biến return lại cho những hàm đệ quy sau
	            else if (node->balance == LH)
	            {
	                node->balance = EH;
	                taller = false; //cân bằng lại, chiều cao vẫn là max nên không có gì thay đổi, node sau không cần phải xwr lý
	            }
	        }
	    }
	    return node;
	}
	
}

Node *AVL::rightBalance(Node *&node, bool &taller)
{
	if(taller)
	{
	    if (node->right->balance == RH)
	    {
	        node = rotateLeft(node); //không cần gán nữa vì đã reference
	        root->balance = EH;
	        root->right->balance = EH;
	    }
	    else if (node->right->balance == RH)
	    {
	        //chỉnh sửa sau khi đã rotate
	        if (node->right->left->balance == RH)
	        {
	            node->balance = LH;
	            node->right->balance = EH;
	        }
	        else if(node->right->left->balance == EH) node->right->balance = EH;
	        else if (node->right->left->balance == LH)
	        {
	            node->balance =EH;
	            node->right->balance =RH;
	        }
	        node->right->left->balance = EH;
	        node->right = rotateRight(node->right);
	        node = rotateLeft(node);
	    }
	    taller = false;
	}
	return node;
}


Node *AVL::leftBalance(Node *&node, bool &taller)
{
	if(taller)
	{
	    if (node->left->balance == LH)
	    {
	        node = rotateRight(node); //không cần gán nữa vì đã reference
	        root->balance = EH;
	        root->left->balance = EH;
	        taller = false;
	    }
	    else if (node->left->balance == RH)
	    {
	        //chỉnh sửa sau khi đã rotate
	        if (node->left->right->balance == LH)
	        {
	            node->balance = RH;
	            node->left->balance = EH;
	        }
	        else if(node->left->right->balance == EH) node->left->balance = EH;
	        else if (node->left->right->balance == RH)
	        {
	            node->balance =EH;
	            node->left->balance =LH;
	        }
	        node->left->right->balance = EH;
	        node->left=rotateLeft(node->left);
	        node = rotateRight(node);
	    } 
	    taller = false;
	}
    return node;

}

void AVL::insert(const int &value)
{   
    bool taller = false;
	this->root = insertRec(this->root, value, taller);
}

int main(void)
{
    AVL tree;
    int array[] = {10,71,48,47,20,76,97,55,54,3,96};
    for (int i : array)
    {
        tree.insert(i);
    }
    return 0;
}