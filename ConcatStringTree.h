#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__
#define MAX 10000000UL

#include "main.h"

class ConcatStringTree {
    class node; //forward declare
    class ParentsTree;

    node *root{};
    int size{};
    int numOfnodes{};

public:
    ConcatStringTree(const char * s);
    ConcatStringTree(ConcatStringTree const &other) = delete; //copy constructor
    ConcatStringTree(ConcatStringTree &&other);
    ~ConcatStringTree();
public:
    ConcatStringTree(): root(nullptr), size(0), numOfnodes(0) {}

    int length() const;
    char get(int index) const;
    int indexOf(char c) const;
    string toStringPreOrder() const;
    string toString() const;
    ConcatStringTree concat(const ConcatStringTree & otherS) const;
    ConcatStringTree subString(int from, int to) const;
    ConcatStringTree reverse() const;
    int getParTreeSize(const string & query) const;
    string getParTreeStringPreOrder(const string & query) const;
private:
    int recursiveLength(node* ) const;
    char recursiveGet (int index, ConcatStringTree::node*) const;
    void setSize();
    void recursiveFind (char c, node* node, int currIndex, int& found) const;
    string recursivetoStringPre(node* root) const;
    string recursivetoString(node* root) const;
    static node* recursiveReverse(node* const &root) ; //trả về 1 node mới và 1 cây mới từ đó
    static node* recursiveSubstr(node* const &root, int from, int to);
    ConcatStringTree& operator= (ConcatStringTree &&); //move assignment
    ConcatStringTree& operator= (ConcatStringTree const &other) = delete;

private:
    class node
    {
        //friend class ConcatStringTree;
    public: //có thể truy cập bằng enclosed nhưng không thể truy cập được ở bên ngoài

        //Variables
        std::string const data{};
        int               length{};
        int               leftLength{};
        int               rightLength{}; 
        ConcatStringTree::ParentsTree* parents{}; //chứa các node chỉ đến nó?, chỉ có thể là con trỏ chứ không thể là 1 class hoàn chỉnh được
        long id{};
        static long maxID;
    private:
        node *left{};
        node *right{};

        //Constructors and Destructor
    public:
        node(string str): data{str}, leftLength{0},rightLength{0}, left{nullptr}, right{nullptr}, length{str.length()}
        {
            if (maxID < MAX) id = ++maxID;
            else throw overflow_error("Id is overflow!");

            parents = new ParentsTree(); //khởi tạo từ 1 string, data, do đó không có gì thì không có PArrents, cây parents rỗng, root là nullptr
        }

        node()                        = delete;
        node(node const &other)       = delete;
        node& operator= (node &other) = delete;
        //node& operator= (node &&other);
        ~node();

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
        void assignLeft(node* p);
        void assignRight(node* p);
        node* const& getLeft() {return this->left;} ;
        node* const& getRight() {return this->right;};
        void removeParent(node*);
    };

    class ParentsTree
    {
        //friend class node;
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
        ParentsTree(): root(nullptr)
        {}


        //Method
    public:
        int size() const;
        int getHeight() const;
        string toStringPreOrder() const;
        void insert(ConcatStringTree::node* const node);
        bool isEmpty() const;
        void remove(ConcatStringTree::node* const node);

    private:
        
        static void rotateLeft(parentsNode* &node);
        static void rotateRight(parentsNode* &node);
        static void leftBalance(parentsNode*& node, bool &taller);
        static void rightBalance(parentsNode*& node, bool &taller);
        static void removeLeftBalance(parentsNode*& node, bool &shorter);
        static void removeRightBalance(parentsNode*& node, bool &shorter);

        static void insertRec(parentsNode*& node , ConcatStringTree::node* const & p, bool& taller);
        static void removeRec(parentsNode*& node , ConcatStringTree::node* const &p, bool &shorter, bool &success);
        static string nodelistPre(const parentsNode* const &node );
        static int getHeight(parentsNode*); //tính theo node
        static int getSize(parentsNode*);

    private:
        class parentsNode
        {
        public:
            ConcatStringTree::node* data{}; //các parentsNode sẽ phân biệt, xếp thứ tự với nhau dựa vào data->id;
            BalanceValue balance{E}; 
            parentsNode* left{};
            parentsNode* right{};

            parentsNode(ConcatStringTree::node* p = nullptr): data(p), left(nullptr), right(nullptr), balance(E)
            {}
        };
    };

};




//----------------------------------------------------------------------Bài 3----------------------------------------------------------------------------------



#endif // __CONCAT_STRING_TREE_H__