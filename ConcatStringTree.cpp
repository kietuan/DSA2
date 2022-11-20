#include "ConcatStringTree.h"
 
long ConcatStringTree::node::maxID{0}; //because there is no before


//Constructors and Destructor of ConcatStringTree
ConcatStringTree::ConcatStringTree(const char* c)
{
    root =  new node(c);
    //root->data = c;
    root->length = root->data.length();
    size = root->length; //phân biệt độ dài của root và độ dài của chuỗi
    numOfnodes = 1;
	this->root->parents->insert(this->root); //chỉ những node là root của một cây mới chứa chính nó như là dấu hiệu nhận biết.
}
ConcatStringTree::ConcatStringTree(ConcatStringTree &&other):
	root(other.root), size(other.size), numOfnodes(other.numOfnodes)
{
	other.root = nullptr;
}
ConcatStringTree:: ~ConcatStringTree()
{
	this->root->removeParent(this->root);
}
ConcatStringTree& ConcatStringTree::operator= (ConcatStringTree  &&other)
{
	if (this == &other) return *this;
	
	delete this->root;
	this->root = other.root;
	other.root = nullptr;
	return *this;
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
int ConcatStringTree::node::getLength(node* node) 
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
        return recursiveGet(index, node->getLeft());
    }
    else if (index >= node->leftLength && index <= node->leftLength + node->length -1)
    {
        return node->data[index - node->leftLength];
    }
    else return recursiveGet(index - node->leftLength - node->length, node->getRight());
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

        recursiveFind(c, node->getLeft(), currIndex - node->leftLength, found);
        auto i = node->data.find(c);
        if (i != string::npos) //found
        {
            found = (int)i + currIndex;
        }
        recursiveFind(c, node->getRight(), currIndex + node->rightLength, found);
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
    string s2 = recursivetoStringPre(root->getLeft());
    string s3 = recursivetoStringPre(root->getRight());
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
    string s2 = recursivetoString(root->getLeft());
    string s3 = recursivetoString(root->getRight());
    return s2 + s1 + s3;
}


ConcatStringTree ConcatStringTree::concat(const ConcatStringTree & otherS) const //có tránh được copy constructor?
{
    ConcatStringTree newstr("");
    newstr.root->assignLeft (this->root);
    newstr.root->assignRight (otherS.root);

    newstr.root->leftLength = this->root->getLength(this->root);
    newstr.root->rightLength = otherS.root->getLength(otherS.root);
    newstr.setSize();

    return newstr;
}



ConcatStringTree ConcatStringTree::subString(int from, int to) const
{
    if (from < 0 || from > this->size - 1 || to < 1 || to > this->size) throw out_of_range("Index of string is invalid!");
    if (from >= to) throw logic_error("Invalid range!");

    //lúc này chắc chắn from và to đã hợp lệ
	this->root->setLength(); //cho chắc chắn là tính toán đúng độ dài trái phải

    ConcatStringTree newtree;
    newtree.root = recursiveSubstr(this->root, from, to - 1);

	if(newtree.root)
		newtree.root->parents->insert(newtree.root);
    newtree.setSize();
    return newtree;
}
ConcatStringTree::node* ConcatStringTree:: recursiveSubstr( node* const &root, int from, int to) //trả về  địa chỉ của gốc mới của 1 cây mới từ đó, to ở đây là vị trí chính xác không phải vị trí sau
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
        return recursiveSubstr(root->getLeft(), from, to);
    }
    else if(from > rightside && to > rightside ) //cùng nằm phải
    {   
        return recursiveSubstr(root->getRight(), from - rightside - 1, to -rightside - 1 );
    }
    else if (from < leftside && to > rightside)
    {
        node *newnode = new node(root->data);
        newnode->assignLeft(recursiveSubstr(root->getLeft(), from, leftside - 1));
        newnode->assignRight (recursiveSubstr (root->getRight(), 0, to - (rightside + 1) ));
        return newnode;
    }
    else if (from < leftside && to >= leftside && to <= rightside ) //xem kẽ nhau
    {
        node *newnode = new node( root->data.substr(0, to + 1) );
        newnode->assignLeft (recursiveSubstr(root->getLeft(), from, root->leftLength - 1));
        return newnode;
    }
    else if (from >= leftside && from <= rightside && to > rightside)
    {
        node *newnode = new node( root->data.substr(from) );
        newnode->assignRight (recursiveSubstr(root->getRight(), 0, to - rightside - 1));
        return newnode;
    }
    else return nullptr; //cho có
}


ConcatStringTree ConcatStringTree::reverse() const
{
    ConcatStringTree newtree; //= new ConcatStringTree();
    //lúc này root vẫn là nullptr
    newtree.root = recursiveReverse(this->root);

    newtree.setSize();
	if(newtree.root)
		newtree.root->parents->insert(newtree.root);
    return newtree;
}
ConcatStringTree::node* ConcatStringTree::recursiveReverse(node* const &root) //trả về gốc mới của 1 cây mới từ đó
{
    if (!root) return nullptr;

    node *newnode = new node(root->data);
    newnode->data = string(root->data.rbegin(), root->data.rend());
    newnode->assignRight(recursiveReverse(root->getLeft()));
    newnode->assignLeft (recursiveReverse(root->getRight()));

    newnode->setLength();
    return newnode;
}




//---------------------------------------------------------------------------------------------------------------000000000000//
// Bài 2


void ConcatStringTree::node::assignLeft(node* p)
{
    if (this->left == nullptr)
    {
        if (!p) return;

        this->left = p;
		p->parents->insert(this);
    }
    else
    {
        //thay đổi ParentsTree của this->left trước rồi gán qua p, rồi thay đổi AVL của p 
		//this->left->parents->remove(this);
		this->left->removeParent(this);
		this->left = p;
		p->parents->insert(this);
    }
	this->leftLength = getLength(p);
}
void ConcatStringTree::node::assignRight(node* p) //tương tự assignleft
{
    if (this->right == nullptr)
    {
		if (!p) return;

		this->right = p;
		p->parents->insert(this);		
    }
    else
    {
		//thay đổi ParentsTree của this->left trước rồi gán qua p, rồi thay đổi AVL của p 
		//this->right->parents->remove(this);
		this->right->removeParent(this);
		this->right = p;
		p->parents->insert(this);
    }
	this->rightLength = getLength(p);
}

int ConcatStringTree::ParentsTree::getHeight() const
{
    return getHeight(this->root);
}
int ConcatStringTree::ParentsTree::size() const
{
    return getSize(this->root);
}
int ConcatStringTree::ParentsTree::getSize (parentsNode* root)
{
    if (!root) return 0;
    return 1 + getSize(root->left) + getSize(root->right);
}

int ConcatStringTree::ParentsTree::getHeight (parentsNode* root) //tính theo node
{
    if (!root) return 0;
    if (!root->left && !root->right) return 1;

    int a = getHeight(root->left);
    int b = getHeight(root->right);
    return 1 + ((a>b) ? a : b);
}



//hiện thực 2 phương thức cơ bản cho PartentsNode là Insert và Remove
void ConcatStringTree::ParentsTree:: insert(node* const node)
{   
    bool taller = false; //chỉ khi insert thành công thì mới true
    insertRec(this->root, node, taller);
}
void ConcatStringTree::ParentsTree:: remove(node* const node)
{
    bool success = false, shorter = false;
    removeRec(this->root, node, shorter, success);
}


void ConcatStringTree::ParentsTree::rotateRight(parentsNode *&node) //xoay phải
{
    parentsNode* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    node = temp;
}
void ConcatStringTree::ParentsTree::rotateLeft(parentsNode *&node)
{
    parentsNode* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    node = temp;
}
void ConcatStringTree::ParentsTree::rightBalance(parentsNode *&node, bool &taller)
{
	if(taller)
	{
	    if (node->right->balance == R)
	    {
	        rotateLeft(node); //không cần gán nữa vì đã reference
	        node->balance = E;
	        node->left->balance = E;
	    }
	    else if (node->right->balance == L)
	    {
	        //chỉnh sửa sau khi đã rotate
	        if (node->right->left->balance == R)
	        {
	            node->right->left->balance = E;
	            node->balance = L;
	            node->right->balance = E;
	        }
	        else if(node->right->left->balance == E) {
	            node->balance = E;
	            node->right->balance = E; 
	        }
	        else if (node->right->left->balance == L)
	        {
	            node->right->left->balance = E;
	            node->balance =E;
	            node->right->balance =R;
	        }
	        rotateRight(node->right);
	    }
	    taller = false;
	}
}
void ConcatStringTree::ParentsTree::leftBalance(parentsNode *&node, bool &taller)
{
	if(taller)
	{
	    if (node->left->balance == L)
	    {
	        rotateRight(node); //không cần gán nữa vì đã reference
	        node->balance = E;
	        node->right->balance = E;
	        taller = false;
	    }
	    else if (node->left->balance == R)
	    {
	        //chỉnh sửa sau khi đã rotate
	        if (node->left->right->balance == L)
	        {
	            node->left->right->balance = E;
	            node->balance = R;
	            node->left->balance = E;
	        }
	        else if(node->left->right->balance == E) 
	        {
	            node->balance = E;
	            node->left->balance = E; 
	        }
	        else if (node->left->right->balance == R)
	        {
	            node->left->right->balance = E;
	            node->balance =E;
	            node->left->balance =L;
	        }
	        rotateLeft(node->left);
	        rotateRight(node);
	    } 
	    taller = false;
	}
}
void ConcatStringTree::ParentsTree::removeRightBalance(parentsNode *&node, bool &shorter)
{               //khi chưa điều chỉnh, cả toàn bộ cây này chiều cao vẫn không thay đổi,vì thứ thay đổi là bị ngắn đi,đã được đánh giá là gnawsn hơn
	if (shorter) //đều giả định là subtree bên trái bị nhỏ hơn thì mới vào hàm  này và 
	            //node->balance=R, bên trái bị ngắn hơn rồi mà còn bị xóa, cùn g lắm giaảm đi 1
    {
        if (node->right->balance == E)
        {
            node->balance = R;
            node->right->balance = L;
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
}
void ConcatStringTree::ParentsTree::removeLeftBalance(parentsNode *&node, bool &shorter)
{
	if (shorter) 
    {
        if (node->left->balance == E)
        {
            node->balance = L;
            node->left->balance = R;
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
}
void ConcatStringTree::ParentsTree::insertRec(parentsNode *&node,ConcatStringTree::node* const& p, bool &taller)
{
	//ta sẽ thêm một node mang giá trị value vào cây con có nốt gốc là node, taller để chỉ rằng cây con có cao lên không
	if (!node)
	{
	    node = new parentsNode(p);
	    taller = true;
	}
	else
	{
	    if (p->id < node->data->id) //to left
	    {
	        insertRec(node->left, p, taller);
	        
	        //những dòng dưới đây sẽ được thực hiện sau khi return xong,  tức là đã thực hiện insert
	        //taller == true nếu node->left sau khi insert cao hơn.
	        if (taller == true)
	        {
	            if (node->balance == L) //đã cao hơn bên trái rồi mà sau khi insert bên trái lại cao hơn nữa
	            {
	                 leftBalance(node, taller);
	            }
	            else if (node->balance == E)
	            {
	                node->balance = L;
	                taller = true; //vẫn không có gì thay đổi, nhưng chiều cao vẫn tăng thêm, phải nhận định  vậy  để dòng sau thực hiện tăng
	            }
	            else if (node->balance == R)
	            {
	                node->balance = E;
	                taller = false; //cân bằng lại, chiều cao vẫn là max nên không có gì thay đổi, node sau không cần phải xwr lý
	            }
	        }
	    }
	    else if (node->data->id < p->id) //to right
	    {
	        insertRec(node->right, p, taller);
	        if (taller == true)
	        {
	            if (node->balance == R)
	            {
	                rightBalance(node, taller);
	            }
	            else if (node->balance == E)
	            {
	                node->balance = R;
	                taller = true; //vẫn không có gì thay đổi, nhưng chiều cao vẫn tăng thêm, phải nhận định  vậy  để dòng sau thực hiện tăng
	            }   //vì taller là một biến return lại cho những hàm đệ quy sau
	            else if (node->balance == L)
	            {
	                node->balance = E;
	                taller = false; //cân bằng lại, chiều cao vẫn là max nên không có gì thay đổi, node sau không cần phải xwr lý
	            }
	        }
	    }
		else if (node->data->id == p->id)
		{
			taller = false;
			return;
		}
	}	
}

void ConcatStringTree::ParentsTree::removeRec(parentsNode*& node ,  ConcatStringTree::node* const &p, bool &shorter, bool &success)
{
    //success: found, đã tìm thấy và xóa, đây như là 1 kỹ thuật để sau nhậtn biết được là đã  xóa
	//đi tìm như bst tree, và tìm thấy thì xóa, vì chỉ có duy nhất 1 giá trị được tìm
	if (!node)
	{
	    success = false;
	    shorter= false;
	}
	
	else if (node->data->id == p->id) //base
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
	        parentsNode* temp = node->right;
	        delete node;
	        node = temp;
	        success = true;
	        shorter = true; //cây con này mất hẳn đi 1 đơn vị
	    }
	    else if (node->left && !node->right)
	    {
	        parentsNode* temp = node->left;
	        delete node;
	        node = temp;
	        success = true;
	        shorter = true;
	    }
	    else if (node->left && node->right)
	    {
	        parentsNode* temp = node->left;
	        while (temp->right) temp = temp->right;
	        //lúc này temp->right đã bằng null
	        
	        node->data = temp->data;
	        removeRec(node->left, temp->data, shorter, success);
	        //here, shorter = true and success = true.....
	    }
	}
	else if (node->data->id > p->id)
	{
	    removeRec(node->left, p, shorter, success);
	    if (success) //đã tìm được và xóa thì mới vào hàm
	    {
	        if (shorter) //cái ở bên trái đã bị nhỏ hơn sau khi xóa
	        {
	            if (node->balance == R)
	            {
	                removeRightBalance(node, shorter);
	            }
	            else if (node->balance == L) {node->balance = E; shorter = true;}
	            else if (node->balance == E){ node->balance = R; shorter = false;}
	        }
	        else if (!shorter); //cây bên trái không thay đổi gì -> không ảnh hưởng tới sự cân bằng ở node đó
	    }
	    
	}
	else if (p->id > node->data->id)
	{
	    removeRec(node->right, p, shorter, success);
	    if (success) //đã tìm được và xóa thì mới vào hàm
	    {
	        if (shorter) //cái ở bên phải đã bị nhỏ hơn sau khi xóa
	        {
	            if (node->balance == L)
	            {
	                removeLeftBalance(node, shorter);
	            }
	            else if (node->balance == R) {node->balance = E; shorter = true;}
	            else if (node->balance == E){ node->balance = L; shorter = false;}
	        }
	        else if (!shorter); //cây bên trái không thay đổi gì -> không ảnh hưởng tới sự cân bằng ở node đó
	    }
	}
}

/*
Việc cần làm: xử lý thao tác thêm id của chính nó vào trong chính nó khi tạo concatstringtree mà không gây ra memory leak.
nếu làm theo thầy mỗi parents thêm chính nó mà chỉ hủy khi parents rỗng thì gây ra leak, như vậy thì chỉ thêm ref gốc vào chính gốc
xử lý destructor của concatstringtree khi return của hàm reverse, substr, concat.
*/

//xét trường hợp khi hủy cây
bool ConcatStringTree::ParentsTree:: isEmpty() const
{
	if (root) return false;
	else return true;
}
ConcatStringTree::node:: ~node()
{
	if (this->parents->isEmpty())
	{

	}
}

//help function cho việc hủy cây, ~Concatstringtree
void ConcatStringTree::node:: removeParent(node* p) //xóa node p ra khỏi node cha của this
{
	this->parents->remove(p); //có thể không thành công nếu p không tồn tại trong cây
	if (this->parents->isEmpty())
	{
		//bắt đầu xóa p ra khỏi, lần lượt lần đến 2 cây con 
		this->left->removeParent(this);
		this->right->removeParent(this);
		delete this;
	}
}


//một số phương thức đề bài yêu cầu
string ConcatStringTree::ParentsTree::toStringPreOrder() const
{
	string res = "ParentsTree[";
	string nodelist = nodelistPre(this->root);
	if (nodelist != "") nodelist.pop_back(); //xóa ký tự ; cuối cùng 
	return res + nodelist + "]";

}
string ConcatStringTree::ParentsTree::nodelistPre(const parentsNode* const &root)
{
	if (!root) return "";

	string s = "id=(" + std::to_string(root->data->id) +");" ;
	return s + nodelistPre(root->left) + nodelistPre(root->right);
}

int ConcatStringTree::getParTreeSize(const string & query) const
{
	node *temp = root;
	for(int i = 0; i < query.length(); i++)
	{	 
		if (!temp) throw runtime_error("Invalid query: reaching NULL");
		if (query[i] == 'l') temp = temp->getLeft();
		else if (query[i] == 'r') temp = temp->getRight();
		else throw runtime_error("Invalid character of query");

		if (!temp) throw runtime_error("Invalid query: reaching NULL");
	}
	return temp->parents->size();
}
string ConcatStringTree::getParTreeStringPreOrder(const string & query) const
{
	node *temp = root;
	for(int i = 0; i < query.length(); i++)
	{	 
		if (!temp) throw runtime_error("Invalid query: reaching NULL");
		if (query[i] == 'l') temp = temp->getLeft();
		else if (query[i] == 'r') temp = temp->getRight();
		else throw runtime_error("Invalid character of query");

		if (!temp) throw runtime_error("Invalid query: reaching NULL");
	}
	return temp->parents->toStringPreOrder();
}