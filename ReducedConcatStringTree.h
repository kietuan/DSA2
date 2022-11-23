#define MAX 10000000UL

#include "main.h"
class ReducedConcatStringTree; // forward declaration
class LitStringHash; // forward declaration
class LitString;

class LitString // một loại string mới dùng cho hash lưu trữ
{
    friend class ReducedConcatStringTree;
    friend class LitStringHash;
    
private:
    std::string str{};
    int numofLink{}; //chỉ có duy nhất ở đây bất thường, chỉ chính là số node đang trỏ tới nó

public:
    int length() const {return str.length();}

    bool operator==(LitString const &other);

    LitString(std::string s=""): str(s), numofLink(0) 
    {}
};


class ReducedConcatStringTree /* */ {
    friend class LitString;
    friend class LitStringHash;
private:
    class node;
    class ParentsTree;

    //variables
private:
    LitStringHash* const litStringHash{}; // nắm giữ của bảng has mà nó đang dùng, các node của nó sẽ trỏ tới, mặc định cho const vì không muón thay đổi nhữn giá trị sẽ sử dụng
    node*                 root{};
    int                   size{}; //độ dài của chuỗi

    //Constructors
public:
    ReducedConcatStringTree(const char * s, LitStringHash * litStringHash);
    ReducedConcatStringTree(ReducedConcatStringTree &&other);
   ~ReducedConcatStringTree();

    ReducedConcatStringTree(ReducedConcatStringTree const &other) = delete;

    //Methods
public:
    int     length() const;
    char    get(int index) const;
    int     indexOf(char c) const;
    string  toStringPreOrder() const;
    string  toString() const;
    ReducedConcatStringTree concat(const ReducedConcatStringTree & otherS) const;
    ReducedConcatStringTree subString(int from, int to) const;
    ReducedConcatStringTree reverse() const;
    int     getParTreeSize(const string & query) const;
    string  getParTreeStringPreOrder(const string & query) const;
private:
    int recursiveLength(node* ) const;
    char recursiveGet (int index, ReducedConcatStringTree::node*) const;
    void setSize();
    void recursiveFind (char c, node* node, int currIndex, int& found) const;
    string recursivetoStringPre(node* root) const;
    string recursivetoString(node* root) const;
    static node* recursiveReverse(node* const &root) ; //trả về 1 node mới và 1 cây mới từ đó
    static node* recursiveSubstr(node* const &root, int from, int to);
    ReducedConcatStringTree& operator= (ReducedConcatStringTree &&); //move assignment
    ReducedConcatStringTree& operator= (ReducedConcatStringTree const &other) = delete;
};


class ReducedConcatStringTree::ParentsTree
{
private:
    //Forward Declare
    class parentsNode;
    enum BalanceValue
    {
        L = -1,
        E = 0,
        R = 1
    };


        //varibalers
    parentsNode *root{};
        //int size;

        //constructor
public:
    ParentsTree(): root(nullptr) {}

        //Method
public:
    int size() const;
    int getHeight() const;
    string toStringPreOrder() const;
    void insert(ReducedConcatStringTree::node* const node);
    bool isEmpty() const;
    void remove(ReducedConcatStringTree::node* const node);

private:
        
    static void rotateLeft(parentsNode* &node);
    static void rotateRight(parentsNode* &node);
    static void leftBalance(parentsNode*& node, bool &taller);
    static void rightBalance(parentsNode*& node, bool &taller);
    static void removeLeftBalance(parentsNode*& node, bool &shorter);
    static void removeRightBalance(parentsNode*& node, bool &shorter);

    static void insertRec(parentsNode*& node , ReducedConcatStringTree::node* const & p, bool& taller);
    static void removeRec(parentsNode*& node , ReducedConcatStringTree::node* const &p, bool &shorter, bool &success);
    static string nodelistPre(const parentsNode* const &node );
    static int getHeight(parentsNode*); //tính theo node
    static int getSize(parentsNode*);

private:
    class parentsNode
    {
    public:
        ReducedConcatStringTree::node* data{}; //các parentsNode sẽ phân biệt, xếp thứ tự với nhau dựa vào data->id;
        BalanceValue                   balance{E}; 
        parentsNode*                   left{};
        parentsNode*                   right{};

        parentsNode(ReducedConcatStringTree::node* p = nullptr): data(p), left(nullptr), right(nullptr), balance(E)
        {}
    };
};



class ReducedConcatStringTree::node
{
public:
    LitString* const data{}; //chỉ nắm giữa 1 tham chiếu, đến đối tượng nằm trong bảng băm
    LitStringHash* const hashTable{}; //mỗi node sẽ phải nắm giữ bảng hash mà nó đang dùng dữ liệu
    int  length{};
    int leftLength{};
    int rightLength{}; 
    ReducedConcatStringTree::ParentsTree* parents{}; //chứa các node chỉ đến nó?, chỉ có thể là con trỏ chứ không thể là 1 class hoàn chỉnh được
    long id{};
    static long maxID;
private:
    node *left{};
    node *right{};

public:
    node(LitString* p, LitStringHash* hash) :  //node này trỏ tới p, nhưng p đã có sẵn nên quan tâm là thêm p vào như thế nào
        data{p}, hashTable{hash}, left{nullptr}, right{nullptr},  leftLength{0},rightLength{0}, length{(int)p->str.length()}
    {
        data->numofLink += 1;
        

        if (maxID < MAX) id = ++maxID;
        else throw overflow_error("Id is overflow!");

        parents = new ParentsTree();
    }
    ~node(); //khi xóa node phải tháo ra 

    node()                         = delete;
    node(node const &other)        = delete;
    node& operator= (node &other)  = delete;
        //node& operator= (node &&other);
    

        //Methods
public:
    static int getLength(node*) ; //return the total length
    static void setLength(node* root); //cho cả cây
    void setLength()
    {
        setLength(this);
    }

        //bởi vì trong mỗi node sẽ chứa 1 cái ParentsTrees chỉ đến các node mà là node cha của nó, tức là chỉ thẳng đến nó, nên ta
        //sẽ không gán con trỏ left, right một cách tường minh nữa, ta sẽ tạo một function gán left right cho nó và làm hết mọi việc thay đổi cái ParentsTreee
        //
    void    assignLeft      (node* p);
    void    assignRight     (node* p);
    node*   const& getLeft() {return this->left;} ;
    node*   const& getRight () {return this->right;};
    void    removeParent    (node*);
};



class HashConfig {
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;

    friend class ReducedConcatStringTree;
    friend class LitStringHash;
};

class LitStringHash {
    friend class ReducedConcatStringTree;
    friend class ReducedConcatStringTree::node;
    enum STATUS_TYPE {NIL, FILLED, DELETED};

private:
    //Variables
    LitString        *data{}; //bảng chứa các litstring
    STATUS_TYPE      *status{};
    int               size{};        // kích cỡ của bảng
    HashConfig const &config{};

    //Constructors
    LitStringHash(const HashConfig & hashConfig): 
        config{hashConfig}, size{0}, data{nullptr}, status{nullptr} {} //bảng hash vẫn chưa có gì đến khi được thêm vô
    LitStringHash() = delete;

    //Methods
public:
    int getLastInsertedIndex() const;
    string toString() const;
    
private:
    void allocateMem();
    void dellocateMem();
    int  insert(string const s); //trả về cái được thêm, hoặc ít nhất là cái đã trùng, không cho phép trùng
    int  remove (string const s);
    int  search (std::string const s) const;
    int  hashFucntion(string const key) const;
    int  probe(string const key, int i) const;
    void rehash();
};


