#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__
#define MAX 10000000

#include "main.h"

class ConcatStringTree {
    class node; //forward declare
    class ParentsTree;

    node *root{};
    int size{};
    int numOfnodes{};

public:
    ConcatStringTree(): root(nullptr), size(0), numOfnodes(0)
    {}
    ConcatStringTree(const char * s);
    //ConcatStringTree(ConcatStringTree& other); //copy constructor

    int length() const;
    char get(int index);
    int indexOf(char c) ;
    string toStringPreOrder() const;
    string toString() const;
    ConcatStringTree& concat(const ConcatStringTree & otherS) const;
    ConcatStringTree subString(int from, int to) const;
    ConcatStringTree reverse() const;
    int getParTreeSize(const string & query) const;
    string getParTreeStringPreOrder(const string & query) const;
private:
    int recursiveLength(node* ) const;
    char recursiveGet (int index, ConcatStringTree::node*);
    void setSize();
    void recursiveFind (char c, node* node, int currIndex, int& found);
    string recursivetoStringPre(node* root) const;
    string recursivetoString(node* root) const;
    static node* recursiveReverse(node* root) ; //trả về 1 node mới và 1 cây mới từ đó
    static node* recursiveSubstr(const node* root, int from, int to);

private:
    class node
    {
        //friend class ConcatStringTree;
    public: //có thể truy cập bằng enclosed nhưng không thể truy cập được ở bên ngoài
        std::string data{};
        int length{};
        int leftLength{};
        int rightLength{}; 

        node *left{};
        node *right{};

        ConcatStringTree::ParentsTree* parents{}; //chứa các node chỉ đến nó?, chỉ có thể là con trỏ chứ không thể là 1 class hoàn chỉnh được
        long id{};
        static long maxID;

        node(string str=""): data{str}, leftLength{0},rightLength{0}, left{nullptr}, right{nullptr}
        {
            length = str.length();
            
            if (maxID < MAX) id = ++maxID;
            else throw overflow_error("Id is overflow!");
            parents = new ParentsTree(); //khởi tạo từ 1 string, data, do đó không có gì thì không có PArrents, cây parents rỗng, root là nullptr
        }
        node (node* &other);

        int getLength(node*) const; //return the total length
        void setLength(node* root); //cho cả cây
        void setLength()
        {
            setLength(this);
        }

        //bởi vì trong mỗi node sẽ chứa 1 cái ParentsTrees chỉ đến các node mà là node cha của nó, tức là chỉ thẳng đến nó, nên ta
        //sẽ không gán con trỏ left, right một cách tường minh nữa, ta sẽ tạo một function gán left right cho nó và làm hết mọi việc thay đổi cái ParentsTreee
        //
        void assignLeft(node* p);
        void assignRight(node* p);
    };

    class ParentsTree
    {
    public:
        class parentsNode;

        parentsNode *root{};
        int size{};

        ParentsTree(): size(0), root(nullptr)
        {}
        //void insert(const node* node); //khi có node mới được trỏ vào node đang chứa ParentsTree hiện tại
        //void recursiveInsert(const node* node, parentsNode* root);
        int getSize() const;
        static int getSize(parentsNode*);
        int getHeight() const;
        static int getHeight(parentsNode*); //tính theo node
        static void leftRotate(parentsNode* &node); //we ensure that this is valid case
        static void rightRotate(parentsNode* &node);
        void insert(const ConcatStringTree::node* node);
        void recursiveInsert(parentsNode*& root , const ConcatStringTree::node* node, bool& done);
        static void balanceTree(parentsNode*& node);
        static void updateFactor( parentsNode*& node);
        void remove(const node* node); //khi có node mới không trỏ vào NODE đang chứa ParentsTree hiện tại nữa


        class parentsNode
        {
        public:
            ConcatStringTree::node* data{}; //các parentsNode sẽ phân biệt, xếp thứ tự với nhau dựa vào data->id;
            int balanceFactor{}; //right - left
            parentsNode* left{};
            parentsNode* right{};

            parentsNode(node* p = nullptr): data(p), left(nullptr), right(nullptr), balanceFactor{0}
            {}
        };
    };

};




class ReducedConcatStringTree; // forward declaration

class HashConfig {
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;

    friend class ReducedConcatStringTree;
};

class ReducedConcatStringTree /* */ 
{
public:
    class LitStringHash {
    public:
        LitStringHash(const HashConfig & hashConfig);
        int getLastInsertedIndex() const;
        string toString() const;
    };

public:
    static LitStringHash litStringHash;
};

#endif // __CONCAT_STRING_TREE_H__