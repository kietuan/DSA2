//this is test file
#include <bits/stdc++.h>
using namespace std;



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
public:
	Node *root;
	static Node *rotateRight(Node *&node);
	static Node *rotateLeft(Node *&node);
	static Node *leftBalance(Node *&node, bool &taller);
	static Node *rightBalance(Node *&node, bool &taller);
	static Node *removeLeftBalance(Node *&node, bool &shorter);
	static Node *removeRightBalance(Node *&node, bool &shorter);
	//TODO Methods
	static Node *insertRec(Node *&node, const int &value, bool &taller);
	static Node *removeRec(Node *&node, const int &value, bool &shorter, bool &success);
	AVL()
	{
		this->root = NULL;
	}
	// TODO Methods
	void insert(const int &value);
	void remove(const int &value);
};

void AVL::remove(const int &value)
{
    bool shorter = false, success = false;
    removeRec(this->root, value, shorter, success);
}

Node *AVL::rotateRight(Node *&node) //xoay phải
{
    Node* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    node = temp;
    return node;
}   


Node *AVL::rotateLeft(Node *&node) //xoay trái
{
    Node* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    node = temp;
    return node;
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
	                 leftBalance(node, taller);
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
	    else if (node->data <= value) //to right
	    {
	        insertRec(node->right, value, taller);
	        if (taller == true)
	        {
	            if (node->balance == RH)
	            {
	                rightBalance(node, taller);
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
	        rotateLeft(node); //không cần gán nữa vì đã reference
	        node->balance = EH;
	        node->left->balance = EH;
	    }
	    else if (node->right->balance == LH)
	    {
	        //chỉnh sửa sau khi đã rotate
	        if (node->right->left->balance == RH)
	        {
	            node->right->left->balance = EH;
	            node->balance = LH;
	            node->right->balance = EH;
	        }
	        else if(node->right->left->balance == EH) {
	            node->balance = EH;
	            node->right->balance = EH; 
	        }
	        else if (node->right->left->balance == LH)
	        {
	            node->right->left->balance = EH;
	            node->balance =EH;
	            node->right->balance =RH;
	        }
	        rotateRight(node->right);
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
	        rotateRight(node); //không cần gán nữa vì đã reference
	        node->balance = EH;
	        node->right->balance = EH;
	        taller = false;
	    }
	    else if (node->left->balance == RH)
	    {
	        //chỉnh sửa sau khi đã rotate
	        if (node->left->right->balance == LH)
	        {
	            node->left->right->balance = EH;
	            node->balance = RH;
	            node->left->balance = EH;
	        }
	        else if(node->left->right->balance == EH) 
	        {
	            node->balance = EH;
	            node->left->balance = EH; 
	        }
	        else if (node->left->right->balance == RH)
	        {
	            node->left->right->balance = EH;
	            node->balance =EH;
	            node->left->balance =LH;
	        }
	        rotateLeft(node->left);
	        rotateRight(node);
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
Node *AVL::removeRec(Node *&node, const int &value, bool &shorter, bool &success)
{
    //success: found, đã tìm thấy và xóa, đây như là 1 kỹ thuật để sau nhậtn biết được là đã  xóa
	//đi tìm như bst tree, và tìm thấy thì xóa, vì chỉ có duy nhất 1 giá trị được tìm
	if (!node)
	{
	    success = false;
	    shorter= false;
	}
	
	else if (node->data == value) //base
	{
	    if (!node->left && !node->right)
	    {
	        delete node;
	        node = nullptr;
	        success = true;
	        shorter = true;
	    }
	    else if (!node->left && node->right)
	    {
	        Node* temp = node->right;
	        delete node;
	        node = temp;
	        success = true;
	        shorter = true; //cây con này mất hẳn đi 1 đơn vị
	    }
	    else if (node->left && !node->right)
	    {
	        Node* temp = node->left;
	        delete node;
	        node = temp;
	        success = true;
	        shorter = true;
	    }
	    else if (node->left && node->right)
	    {
	        Node *temp = node->left;
	        while (temp->right) temp = temp->right;
	        //lúc này temp->right đã bằng null
	        
	        node->data = temp->data;
	        removeRec(node->left, temp->data, shorter, success);
	        //here, shorter = true and success = true.....
	    }
	}
	else if (node->data > value)
	{
	    removeRec(node->left, value, shorter, success);
	    if (success) //đã tìm được và xóa thì mới vào hàm
	    {
	        if (shorter) //cái ở bên trái đã bị nhỏ hơn sau khi xóa
	        {
	            if (node->balance == RH)
	            {
	                removeRightBalance(node, shorter);
	            }
	            else if (node->balance == LH) {node->balance = EH; shorter = true;}
	            else if (node->balance == EH){ node->balance = RH; shorter = false;}
	        }
	        else if (!shorter); //cây bên trái không thay đổi gì -> không ảnh hưởng tới sự cân bằng ở node đó
	    }
	    
	}
	else if (value > node->data)
	{
	    removeRec(node->right, value, shorter, success);
	    if (success) //đã tìm được và xóa thì mới vào hàm
	    {
	        if (shorter) //cái ở bên phải đã bị nhỏ hơn sau khi xóa
	        {
	            if (node->balance == LH)
	            {
	                removeLeftBalance(node, shorter);
	            }
	            else if (node->balance == RH) {node->balance = EH; shorter = true;}
	            else if (node->balance == EH){ node->balance = LH; shorter = false;}
	        }
	        else if (!shorter); //cây bên trái không thay đổi gì -> không ảnh hưởng tới sự cân bằng ở node đó
	    }
	}
	
	return nullptr;// abundant code
}

//input: cây node bị mất cân bằng nhưng các node con vẫn còn được cân bằng
//output: giữ nguyên tính cân bằng của các cây con vì ta sẽ đi lần lượt về  theo hướng stack
Node *AVL::removeRightBalance(Node *&node, bool &shorter)
{               //khi chưa điều chỉnh, cả toàn bộ cây này chiều cao vẫn không thay đổi,vì thứ thay đổi là bị ngắn đi,đã được đánh giá là gnawsn hơn
	if (shorter) //đều giả định là subtree bên trái bị nhỏ hơn thì mới vào hàm  này và 
	            //node->balance=RH, bên trái bị ngắn hơn rồi mà còn bị xóa, cùn g lắm giaảm đi 1
    {
        if (node->right->balance == EH)
        {
            node->balance = RH;
            node->right->balance = LH;
            rotateLeft(node);
            shorter = false;
        }
        else //right of right and left of right, simillar
        {   
            bool tempVar = true;
            rightBalance(node, tempVar);
            shorter = true; //vì sau khi điều chỉnh, TỔNG chiều cao của cây này tính  từ root bị giảm đi 1
        }
    }
    return node;
}


Node *AVL::removeLeftBalance(Node *&node, bool &shorter)
{
	if (shorter) 
    {
        if (node->left->balance == EH)
        {
            node->balance = LH;
            node->left->balance = RH;
            rotateRight(node);
            shorter = false;
        }
        else
        {   
            bool tempVar = true;
            leftBalance(node, tempVar);
            shorter = true; //vì sau khi điều chỉnh, TỔNG chiều cao của cây này tính  từ root bị giảm đi 1
        }
    }
    return node;
}


//-----------------------------------------------------------------------------------------------------------------------------
int pairMatching(vector<int>& v, int target) {
    int res{};
    
    AVL tree{};
    while (!v.empty())
    {
         int x = v.back();
         tree.insert(x);
         v.pop_back();
    }
    //target = target - x;
    while (tree.root)
    {
        int x = tree.root->data;
        
        int k = target - x;
        tree.remove(x);
		bool shorter = false, success = false;
    	AVL::removeRec(tree.root, k, shorter, success);

        if (success) res++;
    }
    return res;
}


int main()
{
	 	

	int target = 120;
	vector<int>items{66,49,23,57,85,12,30,65,51,34,92,4,35,47,84,13,72,57,75,68,20,58,92,6,16,80,83,73,
					33,39,22,98,39,96,54,24,7,83,88,57,68,31,60,3,77,96,67,49,4,93,68,23,50,59,28,65,91,
					63,38,23,1,11,73,40,6,26,15,12,61,54,20,80,84,80,34,61,27,100,61,30,93,28,52,94,86,
					32,59,28,94,48,51,46,58,23,37,15,100,51,78,12};
	cout << pairMatching(items, target) << endl;
	return 0;
}