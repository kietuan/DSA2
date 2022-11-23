#include "ReducedConcatStringTree.h"
long ReducedConcatStringTree::node::maxID{0}; //because there is no before

//--------------------------------------------Hàm cho Hash-----------------------------------------
int LitStringHash::hashFucntion(string const key) const
{
    if (key == "") throw ("Cannot hash a null string");
    int power = 1;
    int res = 0;
    for (int i = 0; i < key.length(); i++)
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
            //return index;
			break;
		}
		if (i == size-1 && status[index] == FILLED) throw runtime_error("No possible slot");
	}
	//đến đây chắc chắn phải thêm được
	int currSize = 0;
	for (int i = 0; i < size; i++)	if (status[i] == FILLED) currSize++;
	if ( (double)currSize/size > config.lambda) rehash();
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
	auto temp = new LitString[size * config.alpha]{};
	for (int i = 0; i <size; i++) temp[i] = data[i];
	delete[] data;
	data = temp;

	auto tempst = new STATUS_TYPE [size * config.alpha]{};
	for (int i = 0; i <size; i++) tempst[i]= status[i];
	for (int i = size; i < size * config.alpha; i++) tempst[i] = NIL;
	delete[] status;
	status = tempst;

	size = size * config.alpha;
}

bool LitString:: operator==(LitString const &other)
{
	if (this->str == other.str) return true;
	return false;
}

//-----------------------------------------------------------------------Constructors cho Reduced---------------------------------------------------------
ReducedConcatStringTree::ReducedConcatStringTree(const char * s, LitStringHash * litStringHash) : litStringHash{litStringHash}
{
	int index = litStringHash->insert(s);
	//litStringHash->data[index] chứa phần tử mà ta mới thêm vô
	root = new node(&(litStringHash->data[index]));
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



//-------------------------------------------------------------------------Hàm cho Reduced----------------------------------------------------------------
int ReducedConcatStringTree::length() const
{
	return node::getLength(this->root);
}
void ReducedConcatStringTree::setSize()
{
    if (root) 
		this->size = this->root->length + this->root->leftLength + this->root->rightLength; //đưa công việc tạo size lại cho class node
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

    root->length = root->data->length();
    root->leftLength = getLength(root->left);
    root->rightLength = getLength(root->right);
    setLength (root->left);
    setLength(root->right);
}


char ReducedConcatStringTree::get(int index) chưa cập nhật hàm size liên tục
{
    if (index < 0 || index > size) throw out_of_range("Index of string is invalid!");
    else
    {
        return recursiveGet(index, this->root);
    }
}
char ReducedConcatStringTree::recursiveGet(int index, ReducedConcatStringTree::node* node)
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



int ReducedConcatStringTree::indexOf(char c) 
{
    int found = -1;
    recursiveFind(c, this->root, this->root->leftLength, found);
    return found;
}
void ReducedConcatStringTree::recursiveFind (char c, node* node, int currIndex, int &found)
{   
    if (found == -1)
    {   
        if (!node) return;

        recursiveFind(c, node->getLeft(), currIndex - node->leftLength, found);
        auto i = node->data->str.find(c);
        if (i != string::npos) //found
        {
            found = (int)i + currIndex;
        }
        recursiveFind(c, node->getRight(), currIndex + node->rightLength, found);
    }
}


string ReducedConcatStringTree::toStringPreOrder() const chưa làm đúng yêu cầu đề bài
{
    return recursivetoStringPre(this->root);
}
string ReducedConcatStringTree::recursivetoStringPre(node* root) const
{
    if (!root) return "";

    string s1 = root->data->str;
    string s2 = recursivetoStringPre(root->getLeft());
    string s3 = recursivetoStringPre(root->getRight());
    return s1 + s2 + s3;
}


string ReducedConcatStringTree::toString() const chưa làm đúng yêu cầu đề bài
{
    return recursivetoString(this->root);
}
string ReducedConcatStringTree::recursivetoString(node* root) const
{
    if (!root) return "";
    string s1 = root->data->str;
    string s2 = recursivetoString(root->getLeft());
    string s3 = recursivetoString(root->getRight());
    return s2 + s1 + s3;
}

/*
ReducedConcatStringTree ReducedConcatStringTree::concat(const ReducedConcatStringTree & otherS) const //có tránh được copy constructor?
{
    ReducedConcatStringTree newstr("");
    newstr.root->assignLeft (this->root);
    newstr.root->assignRight (otherS.root);

    newstr.root->leftLength = this->root->getLength(this->root);
    newstr.root->rightLength = otherS.root->getLength(otherS.root);
    newstr.setSize();

    return newstr;
}
*/

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



bool ReducedConcatStringTree::ParentsTree:: isEmpty() const
{
	if (root) return false;
	else return true;
}

void ReducedConcatStringTree::node:: removeParent(node* p) //xóa node p ra khỏi node cha của this
{
	this->parents->remove(p); //có thể không thành công nếu p không tồn tại trong cây
	if (this->parents->isEmpty())
	{
		//bắt đầu xóa p ra khỏi, lần lượt lần đến 2 cây con 
		this->left->removeParent(this);
		this->right->removeParent(this);
		delete this; ở đây gọi destructor của node được
	}
}

ReducedConcatStringTree::node:: ~node()
{
	this->data->numofLink -= 1;
	if (this->data->numofLink == 0) 
}