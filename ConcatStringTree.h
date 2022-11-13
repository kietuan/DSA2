#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"

class ConcatStringTree {
    class node; //forward declare
    node *root{};
    int size{};


public:
    ConcatStringTree(const char * s);
    int length() const;
    char get(int index);
    int indexOf(char c);
    string toStringPreOrder() const;
    string toString() const;
    ConcatStringTree concat(const ConcatStringTree & otherS) const;
    ConcatStringTree subString(int from, int to) const;
    ConcatStringTree reverse() const;

    int getParTreeSize(const string & query) const;
    string getParTreeStringPreOrder(const string & query) const;
private:
    int recursiveLength(node* ) const;

private:
    class node
    {
        friend class ConcatStringTree;

        int length{};
        int leftLength{};
        std::string data{};

        node *left{};
        node *right{};

        node(): length{0}, leftLength{0}, data{""}, left{nullptr}, right{nullptr}
        {}


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