#include "ConcatStringTree.h"
unsigned long ConcatStringTree::node::maxID{0}; //because there is no before


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
    //return size; //cần tạo 1 hàm cho size
	return node::getLength(this->root);
}
void ConcatStringTree::setSize()
{
    if (root)
	{
		node::setLength(this->root);
		this->size = root->length + root->rightLength + root->leftLength;
	}
    else this->size = 0;
}
int ConcatStringTree::node::getLength(node* node) 
{
    if (!node) return 0;
    
    //node->length = node->data.length();
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


char ConcatStringTree::get(int index) const
{
    if (index < 0 || index >= size) throw out_of_range("Index of string is invalid!");
    else 
    {
        return recursiveGet(index, this->root);
    }
}
char ConcatStringTree::recursiveGet(int index, ConcatStringTree::node* node) const
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



int ConcatStringTree::indexOf(char c) const
{
    int found = -1;
    recursiveFind(c, this->root, this->root->leftLength, found);
    return found;
}
void ConcatStringTree::recursiveFind (char c, node* node, int currIndex, int &found) const
{   
    if (found == -1)
    {   
        if (!node) return;

        if (node->getLeft()) 
			recursiveFind(c, node->getLeft(), currIndex - node->getLeft()->rightLength - node->getLeft()->length, found);
        auto i = node->data.find(c);
        if (i != string::npos && found == -1) //found
        {
            found = (int)i + currIndex;
        }
        if(node->getRight()) 
			recursiveFind(c, node->getRight(), currIndex + node->length + node->getRight()->leftLength, found);
    }
}


string ConcatStringTree::toStringPreOrder() const
{	
	string res = "ConcatStringTree[";
	string s = recursivetoStringPre(this->root);
	if (s != "" ) s.pop_back(); //bỏ đi dấu ; sau cùng
    return res + s +"]";
}
string ConcatStringTree::recursivetoStringPre(node* root) const
{
    if (!root) return "";

    string s1 = root->data;
	//(LL=0,L=5,"Hello");
	if (s1 != "") 
		s1 = "(LL=" + to_string(root->leftLength) + ",L=" + to_string(root->length + root->leftLength + root->rightLength) +",\"" + s1 + "\");" ;
	else s1 = "(LL=" + to_string(root->leftLength) + ",L=" + to_string(root->length + root->leftLength + root->rightLength) +",<NULL>);";

    string s2 = recursivetoStringPre(root->getLeft());
    string s3 = recursivetoStringPre(root->getRight());
    return s1 + s2 + s3;
}


string ConcatStringTree::toString() const 
{
	string res = "ConcatStringTree[\"";
    return res + recursivetoString(this->root) + "\"]";
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
//precodition: from and to in true form and all the internal nodes are null string
ConcatStringTree::node* ConcatStringTree:: recursiveSubstr( node* const &root, int from, int to) //trả về  địa chỉ của gốc mới của 1 cây mới từ đó, to ở đây là vị trí chính xác không phải vị trí sau
{
	if (!root) return nullptr;

	if (!root->getLeft() && !root->getRight())
	{
		auto newnode = new node( root->data.substr(from, to - from + 1) );
		return newnode;
	}
	else
	{
		node *newnode = new node("");
		if 		(from <= root->leftLength-1 && to <= root->leftLength-1) 
			newnode->assignLeft (recursiveSubstr(root->getLeft(), from, to));
		else if (from <= root->leftLength-1 && to >= root->leftLength  )
		{
 			newnode->assignLeft (recursiveSubstr(root->getLeft(), from, root->leftLength - 1));
			newnode->assignRight (recursiveSubstr(root->getRight(), 0, to - root->leftLength));
		}
		else if (from >= root->leftLength 	&& to >= root->leftLength  )
		{
			newnode->assignRight( recursiveSubstr(root->getRight(), from - root->leftLength, to - root->leftLength));
		}
		return newnode;
	}
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
	//if (success) print();
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
			rotateLeft(node);
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
	        //here, shorter = ? and success = true.....
			//lúc này đã xóa thành công 1 node cây con bên trái
			if (shorter)
			{
				if (node->balance == E)
				{
					node->balance = R;
					shorter = false;
				}
				else if (node->balance == L)
				{
					node->balance = E;
					shorter = true;
				}
				else if (node->balance == R)
				{
					removeRightBalance(node, shorter);
				}
			}
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
/*ConcatStringTree::node:: ~node()
{
	//test
	cout << "Destroy "<<id << " while max = " << maxID <<endl;
}*/

//help function cho việc hủy cây, ~Concatstringtree
void ConcatStringTree::node:: removeParent(node* p) //xóa node p ra khỏi node cha của this
{
	//có thể p đang trỏ tới this
	//if (p->left == this) 	   p->left = nullptr;
	//else if (p->right == this) p->right = nullptr;
	//if(!this) return;
	this->parents->remove(p); //có thể không thành công nếu p không tồn tại trong cây
	if (this->parents->isEmpty())
	{
		//bắt đầu xóa p ra khỏi, lần lượt lần đến 2 cây con 
		if (this->left) this->left->removeParent(this);
		if(this->right) this->right->removeParent(this);
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

	string s = "(id=" + std::to_string(root->data->id) +");" ;
	return s + nodelistPre(root->left) + nodelistPre(root->right);
}

int ConcatStringTree::getParTreeSize(const string & query) const
{
	node *temp = root;
	for(int i = 0; i < static_cast<int>(query.length()); i++)
	{	 
		if (query[i] != 'l' && query[i] != 'r') throw runtime_error("Invalid character of query");
		if (!temp) 								throw runtime_error("Invalid query: reaching NULL");

		if 		(query[i] == 'l') temp = temp->getLeft();
		else if (query[i] == 'r') temp = temp->getRight();
	}

	return temp->parents->size();
}
string ConcatStringTree::getParTreeStringPreOrder(const string & query) const
{
	node *temp = root;
	for(int i = 0; i < static_cast<int>(query.length()); i++)
	{	 
		if (!temp) throw runtime_error("Invalid query: reaching NULL");
		if (query[i] == 'l') temp = temp->getLeft();
		else if (query[i] == 'r') temp = temp->getRight();
		else throw runtime_error("Invalid character of query");

		if (!temp) throw runtime_error("Invalid query: reaching NULL");
	}
	return temp->parents->toStringPreOrder();
}






//--------------------------------------------------------Bài 3------------------------------------------------------------------
unsigned long ReducedConcatStringTree::node::maxID{0}; //because there is no before

//--------------------------------------------Hàm cho Hash-----------------------------------------
int LitStringHash::hashFucntion(string const key) const
{
    if (key == "") throw ("Cannot hash a null string");
    int power = 1;
    int res = 0;
    for (int i = 0; i < static_cast<int>(key.length()); i++)
    {
        res += (int)key[i] * power;
        power *= config.p;
    }
    res %= this->size;
    return res;
}
int LitStringHash::probe(std::string const key, int i) const
{
	return (int)(hashFucntion(key) + i*config.c1 + i*i*config.c2) % size; //có thể sai vì đã ép kiểu
}
int LitStringHash::search (std::string const s) const
{
	int index{};
	//LitString newS(s);
	for (int i = 0; i < size; i++)
	{
		index = probe(s, i);
		if (status[index] == NIL) return -1;

		else if (status[index] == FILLED)
            if (data[index].str == s) return index;
	}
	return -1;
}
int  LitStringHash:: insert(string const s) //không cho phép insert trùng nhau
{
	if (!data) allocateMem();

	int temp = search(s);
	if (temp != -1) return temp; //đã có chuỗi s->str, không insert nữa

	int index{};
	for (int i = 0; i < size; i++)
	{
		index = probe(s, i);
		if (status[index] !=  FILLED)
		{
			data[index] = LitString(s);
			status[index] =  FILLED;
			this->lastInserted = index;
            //return index;
			break;
		}
		if (i == size-1 && status[index] == FILLED) throw runtime_error("No possible slot");
	}
	//đến đây chắc chắn phải thêm được
	int currSize = 0;
	for (int i = 0; i < size; i++)	if (status[i] == FILLED) currSize++;
	if ( (double)currSize/size > config.lambda) {
		rehash();
		index = lastInserted = this->search(s); //tìm lại cái vừa thêm
	}
	return index;
}
int  LitStringHash::remove (string const s)
{
	if (search(s) == -1) throw ("Not found!");

	int index{};
	for (int i = 0; i < size; i++)
	{
		index = probe(s,i);
		if  (status[index] == NIL)  throw ("Not found!");
		else if ( status[index] == FILLED)
			if (data[index].str == s)
			{
				status[index] = DELETED;
				break;
			}
	}

	int currSize = 0;
	for (int i = 0; i < size; i++)	if (status[i] == FILLED) currSize++;
	if (currSize == 0) dellocateMem();
	return index; //vị trí vừa xóa
}



void LitStringHash::allocateMem()
{
	if (!data)
	{
		size = config.initSize;
		data = new LitString[size]{};
		status = new STATUS_TYPE[size]{};
		for (int i = 0; i < size; i++) status[i] = NIL;

	}
}
void LitStringHash::dellocateMem()
{
	if (data)
	{
		delete[] data;
		delete[] status;
		data = nullptr;
		status = nullptr;
		size = 0;

	}
}
void LitStringHash::rehash()
{
	auto oldsize = size;
	auto olddata = data;
	auto oldstatus = status;

	size = int(size * config.alpha);
	this->data   = new LitString   [size]{};
	this->status = new STATUS_TYPE [size]{};
	for (int i = 0; i <size; i++) status[i] = NIL;

	for (int i = 0; i < oldsize; i++)
	{
		if (oldstatus[i] == FILLED) this->insert(olddata[i].str);
	}

	delete[] olddata;
	delete[] oldstatus;
}

bool LitString:: operator==(LitString const &other) const
{
	if (this->str == other.str) return true;
	return false;
}

//-----------------------------------------------------------------------Constructors cho Reduced---------------------------------------------------------
ReducedConcatStringTree::ReducedConcatStringTree(const char* s, LitStringHash *litStringHash): litStringHash{litStringHash}
{
	if (static_cast<int>(s[0]) == 0)
	{
		root = new node (nullptr, nullptr);
	}
	else
	{
		int index = litStringHash->insert(s);
		//litStringHash->data[index] chứa phần tử mà ta mới thêm vô
		root = new node(&(litStringHash->data[index]), litStringHash);
	}
	size = root->length;
	this->root->parents->insert(this->root);
}

ReducedConcatStringTree::ReducedConcatStringTree(ReducedConcatStringTree &&other):
	root(other.root), size(other.size)
{
	other.root = nullptr;
}

ReducedConcatStringTree& ReducedConcatStringTree::operator= (ReducedConcatStringTree  &&other)
{
	if (this == &other) return *this;
	
	delete this->root;
	this->root = other.root;
	other.root = nullptr;
	return *this;
}

ReducedConcatStringTree:: ~ReducedConcatStringTree()
{
	this->root->removeParent(this->root);
}

ReducedConcatStringTree::node:: node(LitString* p, LitStringHash* hash) :  //node này trỏ tới p, nhưng p đã có sẵn nên quan tâm là thêm p vào như thế nào
    data{p}, hashTable{hash}, leftLength{0}, rightLength{0}, left{nullptr}, right{nullptr}
{
	if (!this->data && this->hashTable) throw ("Wrong Initializion of node!");
	if (data) this->length = (int)p->str.length();
	else this->length = 0;

    if (data) data->numofLink += 1; //có thể có trường hợp khởi tạo data và hash đều là null, tức chuỗi rỗng

    if (maxID < MAX) id = ++maxID;
    else throw overflow_error("Id is overflow!");

    parents = new ParentsTree();
}

ReducedConcatStringTree::node:: ~node()
{
	if (this->data)
	{
		this->data->numofLink -= 1;
		if (this->data->numofLink == 0) hashTable->remove(data->str);
	}
}


//-------------------------------------------------------------------------Hàm cho Reduced----------------------------------------------------------------
int ReducedConcatStringTree::length() const
{
	return node::getLength(this->root);
}
void ReducedConcatStringTree::setSize()
{
    if (root)
	{
		node::setLength(this->root);
		this->size = root->length + root->rightLength + root->leftLength;
	}
    else this->size = 0;
}

int ReducedConcatStringTree::node::getLength(node* node) 
{
    if (!node) return 0;
    
    //node->length = node->data.length();
    return node->length + getLength(node->left) + getLength(node->right);
}
void ReducedConcatStringTree::node::setLength(node* root) //đặt cho cả cây từ gốc, preorder
{
    if (!root) return;
	if (!root->data) root->length = 0;
    else root->length = root->data->length();

    root->leftLength = getLength(root->left);
    root->rightLength = getLength(root->right);

    setLength (root->left);
    setLength(root->right);
}


char ReducedConcatStringTree::get(int index) const
{
    if (index < 0 || index > size) throw out_of_range("Index of string is invalid!");
    else
    {
        return recursiveGet(index, this->root);
    }
}
char ReducedConcatStringTree::recursiveGet(int index, ReducedConcatStringTree::node* node) const
{
    if (index <= node->leftLength - 1)
    {
        return recursiveGet(index, node->getLeft());
    }
    else if (index >= node->leftLength && index <= node->leftLength + node->length -1)
    {
        return node->data->str[index - node->leftLength];
    }
    else return recursiveGet(index - node->leftLength - node->length, node->getRight());
}



int ReducedConcatStringTree::indexOf(char c) const
{
    int found = -1;
    recursiveFind(c, this->root, this->root->leftLength, found);
    return found;
}
void ReducedConcatStringTree::recursiveFind (char c, node* node, int currIndex, int &found) const
{   
    if (found == -1)
    {   
        if (!node) return;

       	if (node->getLeft()) 
			recursiveFind(c, node->getLeft(), currIndex - node->getLeft()->rightLength - node->getLeft()->length, found);
        size_t 			i =  string::npos;
		if (node->data) i = node->data->str.find(c);
        if (i != string::npos && found == -1) //found
        {
            found = (int)i + currIndex;
        }
        if(node->getRight()) 
			recursiveFind(c, node->getRight(), currIndex + node->length + node->getRight()->leftLength, found);
    }
}


string ReducedConcatStringTree::toStringPreOrder() const
{	
	string res = "ConcatStringTree[";
	string s = recursivetoStringPre(this->root);
	if (s != "" ) s.pop_back(); //bỏ đi dấu ; sau cùng
    return res + s +"]";
}
string ReducedConcatStringTree::recursivetoStringPre(node* root) const
{
    if (!root) return "";

	string s1 = "";
    if (root->data) s1 = root->data->str;
	//(LL=0,L=5,"Hello");
	if (s1 != "") 
		s1 = "(LL=" + to_string(root->leftLength) + ",L=" + to_string(root->length + root->leftLength + root->rightLength) +",\"" + s1 + "\");" ;
	else s1 = "(LL=" + to_string(root->leftLength) + ",L=" + to_string(root->length + root->leftLength + root->rightLength) +",<NULL>);";

    string s2 = recursivetoStringPre(root->getLeft());
    string s3 = recursivetoStringPre(root->getRight());
    return s1 + s2 + s3;
}


string ReducedConcatStringTree::toString() const 
{
    string res = "ConcatStringTree[\"";
    return res + recursivetoString(this->root) + "\"]";
}
string ReducedConcatStringTree::recursivetoString(node* root) const
{
    if (!root) return "";

	string s1 = "";
    if (root->data) s1 = root->data->str;
    string s2 = recursivetoString(root->getLeft());
    string s3 = recursivetoString(root->getRight());
    return s2 + s1 + s3;
}


ReducedConcatStringTree ReducedConcatStringTree::concat(const ReducedConcatStringTree & otherS) const //có tránh được copy constructor?
{
    ReducedConcatStringTree newstr("", this->litStringHash); //có root chứa nullptr và nullptr
    newstr.root->assignLeft (this->root);
    newstr.root->assignRight (otherS.root);

    newstr.root->leftLength = this->root->getLength(this->root);
    newstr.root->rightLength = otherS.root->getLength(otherS.root);
    newstr.setSize();

    return newstr;
}


void ReducedConcatStringTree::node::assignLeft(node* p)
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
void ReducedConcatStringTree::node::assignRight(node* p) //tương tự assignleft
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


int ReducedConcatStringTree::ParentsTree::getHeight() const
{
    return getHeight(this->root);
}
int ReducedConcatStringTree::ParentsTree::size() const
{
    return getSize(this->root);
}
int ReducedConcatStringTree::ParentsTree::getSize (parentsNode* root)
{
    if (!root) return 0;
    return 1 + getSize(root->left) + getSize(root->right);
}

int ReducedConcatStringTree::ParentsTree::getHeight (parentsNode* root) //tính theo node
{
    if (!root) return 0;
    if (!root->left && !root->right) return 1;

    int a = getHeight(root->left);
    int b = getHeight(root->right);
    return 1 + ((a>b) ? a : b);
}

void ReducedConcatStringTree::ParentsTree:: insert(node* const node)
{   
    bool taller = false; //chỉ khi insert thành công thì mới true
    insertRec(this->root, node, taller);
}
void ReducedConcatStringTree::ParentsTree:: remove(node* const node)
{
    bool success = false, shorter = false;
    removeRec(this->root, node, shorter, success);
}


void ReducedConcatStringTree::ParentsTree::rotateRight(parentsNode *&node) //xoay phải
{
    parentsNode* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    node = temp;
}
void ReducedConcatStringTree::ParentsTree::rotateLeft(parentsNode *&node)
{
    parentsNode* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    node = temp;
}
void ReducedConcatStringTree::ParentsTree::rightBalance(parentsNode *&node, bool &taller)
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
			rotateLeft(node);
	    }
	    taller = false;
	}
}
void ReducedConcatStringTree::ParentsTree::leftBalance(parentsNode *&node, bool &taller)
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
void ReducedConcatStringTree::ParentsTree::removeRightBalance(parentsNode *&node, bool &shorter)
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
void ReducedConcatStringTree::ParentsTree::removeLeftBalance(parentsNode *&node, bool &shorter)
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

void ReducedConcatStringTree::ParentsTree::insertRec(parentsNode *&node,ReducedConcatStringTree::node* const& p, bool &taller)
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

void ReducedConcatStringTree::ParentsTree::removeRec(parentsNode*& node ,  ReducedConcatStringTree::node* const &p, bool &shorter, bool &success)
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
	        //here, shorter = ?? and success = true.....
			if (shorter)
			{
				if (node->balance == E)
				{
					node->balance = R;
					shorter = false;
				}
				else if (node->balance == L)
				{
					node->balance = E;
					shorter = true;
				}
				else if (node->balance == R)
				{
					removeRightBalance(node, shorter);
				}
			}
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
	        else if (!shorter){;} //cây bên trái không thay đổi gì -> không ảnh hưởng tới sự cân bằng ở node đó
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
	        else if (!shorter) {;} //cây bên trái không thay đổi gì -> không ảnh hưởng tới sự cân bằng ở node đó
	    }
	}
}



bool ReducedConcatStringTree::ParentsTree:: isEmpty() const
{
	if (root) return false;
	else return true;
}

void ReducedConcatStringTree::node:: removeParent(node* p) //xóa node p ra khỏi node cha của this
{
	//if(!this) return;
	this->parents->remove(p); //có thể không thành công nếu p không tồn tại trong cây
	if (this->parents->isEmpty())
	{
		//bắt đầu xóa p ra khỏi, lần lượt lần đến 2 cây con 
		if (this->left) this->left->removeParent(this);
		if(this->right) this->right->removeParent(this);
		delete this;
	}
}


//----------------------------------------------------------------------Hàm LitStringHash đề bài yêu cầu-------------------------------------------------------------
int LitStringHash:: getLastInsertedIndex() const
{
	if (!this->data) return -1;
	else			 return lastInserted;
}

string LitStringHash::toString() const
{
	std::string s = "LitStringHash[";
	std::string slot_list{};
	for (int i = 0; i < size; i++)
	{
		if (status[i] == FILLED)
		{
			slot_list += "(litS=\"" + data[i].str + "\");" ;
		}
		else
		{
			slot_list += "();" ;
		}
	}
	if (slot_list != "") slot_list.pop_back();
	return s + slot_list + "]" ;
}


int ReducedConcatStringTree::getParTreeSize(const string & query) const
{
	node *temp = root;
	for(int i = 0; i < static_cast<int>(query.length()); i++)
	{	 
		if (query[i] != 'l' && query[i] != 'r') throw runtime_error("Invalid character of query");
		if (!temp) 								throw runtime_error("Invalid query: reaching NULL");

		if 		(query[i] == 'l') temp = temp->getLeft();
		else if (query[i] == 'r') temp = temp->getRight();
	}

	return temp->parents->size();
}
string ReducedConcatStringTree::getParTreeStringPreOrder(const string & query) const
{
	node *temp = root;
	for(int i = 0; i < static_cast<int>(query.length()); i++)
	{	 
		if (!temp) throw runtime_error("Invalid query: reaching NULL");
		if (query[i] == 'l') temp = temp->getLeft();
		else if (query[i] == 'r') temp = temp->getRight();
		else throw runtime_error("Invalid character of query");

		if (!temp) throw runtime_error("Invalid query: reaching NULL");
	}
	return temp->parents->toStringPreOrder();
}

string ReducedConcatStringTree::ParentsTree::toStringPreOrder() const
{
	string res = "ParentsTree[";
	string nodelist = nodelistPre(this->root);
	if (nodelist != "") nodelist.pop_back(); //xóa ký tự ; cuối cùng 
	return res + nodelist + "]";
}

string ReducedConcatStringTree::ParentsTree::nodelistPre(const parentsNode* const &root)
{
	if (!root) return "";

	string s = "(id=" + std::to_string(root->data->id) +");" ;
	return s + nodelistPre(root->left) + nodelistPre(root->right);
}