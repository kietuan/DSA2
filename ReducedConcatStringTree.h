#define MAX 10000000UL

#include "main.h"
class ReducedConcatStringTree; // forward declaration
class LitStringHash; // forward declaration
class LitString;


class ReducedConcatStringTree /* */ {
    friend class LitString;
    friend class LitStringHash;
    class node;
    class ParentsTree;

    //variables
private:
    LitStringHash* const litStringHash{}; // nắm giữ của bảng has mà nó đang dùng, các node của nó sẽ trỏ tới, mặc định cho const vì không muón thay đổi nhữn giá trị sẽ sử dụng
    node* root{};
    int size{};

    //Constructors
public:
    ReducedConcatStringTree(const char * s, LitStringHash * litStringHash);


    //Methods
public:
    int length() const;
    char get(int index);
    int indexOf(char c) ;
    string toStringPreOrder() const;
    string toString() const;
    ReducedConcatStringTree concat(const ConcatStringTree & otherS) const;
    ReducedConcatStringTree subString(int from, int to) const;
    ReducedConcatStringTree reverse() const;
    int getParTreeSize(const string & query) const;
    string getParTreeStringPreOrder(const string & query) const;
};


class ReducedConcatStringTree::node
{
public:
    LitString* data{}; //chỉ nắm giữa 1 tham chiếu, đến đối tượng nằm trong bảng băm
    int length{};
    int leftLength{};
    int rightLength{}; 
    ReducedConcatStringTree::ParentsTree* parents{}; //chứa các node chỉ đến nó?, chỉ có thể là con trỏ chứ không thể là 1 class hoàn chỉnh được
    long id{};
    static long maxID;
private:
    node *left{};
    node *right{};

    
    node(LitString* p): data{p}, left{nullptr}, right{nullptr} //node này trỏ tới p
    {
        data->numofLink += 1;
    }
    node() = delete;

    node(node const &other) = delete;
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

class ReducedConcatStringTree::ParentsTree
{

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
    LitString *data{}; //bảng chứa các litstring
    STATUS_TYPE *status{};
    int size{};        // kích cỡ của bảng
    HashConfig const &config{};

    //Constructors
    LitStringHash(const HashConfig & hashConfig): config{hashConfig}, size{0}, data{nullptr}, status{nullptr} {} //bảng hash vẫn chưa có gì đến khi được thêm vô
    LitStringHash() = delete;

    //Methods
public:
    int getLastInsertedIndex() const;
    string toString() const;
    
private:
    void allocateMem();
    void dellocateMem();
    int insert(string const s);
    int remove (string const s);
    int search (std::string const s) const;
    int hashFucntion(string const key) const;
    int probe(string const key, int i) const;
    void rehash();
};

class LitString // một loại string mới dùng cho hash lưu trữ
{
    friend class ReducedConcatStringTree;
    friend class ReducedConcatStringTree::node;
    friend class LitStringHash;

    std::string str{};
    int numofLink{}; //chỉ có duy nhất ở đây bất thường, chỉ chính là số node đang trỏ tới nó

    bool operator==(LitString const &other);
    LitString(std::string s=""): str(s), numofLink(0)   
    {}
};
