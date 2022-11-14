#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"

class ConcatStringTree {
    class node; //forward declare

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
        friend class ConcatStringTree;

        std::string data{};
        int length{};
        int leftLength{};
        int rightLength{}; 

        node *left{};
        node *right{};

        node(string str=""): data{str}, leftLength{0},rightLength{0}, left{nullptr}, right{nullptr}
        {
            length = str.length();
        }

        node (node* &other);

        int getLength(node*) const; //return the total length
        void setLength(node* root); //cho cả cây
        void setLength()
        {
            setLength(this);
        }
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