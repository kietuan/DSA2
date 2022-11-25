#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__
#define MAX 10000000UL

#include "main.h"

class ConcatStringTree;

class ConcatStringTree {

    class node; //forward declare
    class ParentsTree;

    node *root{};
    int size{};
    int numOfnodes{};

public:
    ConcatStringTree(const char * s);
    ConcatStringTree(ConcatStringTree const &other)            = delete; //copy constructor
    ConcatStringTree(ConcatStringTree &&other);
    ~ConcatStringTree();
private:
    ConcatStringTree(): root(nullptr), size(0), numOfnodes(0) {} //Cẩn thận với default vì khó kiểm soát, không được dùng tự tiện

public:
    int length()                                                const;
    char get(int index)                                         const;
    int indexOf(char c)                                         const;
    string toStringPreOrder()                                   const;
    string toString()                                           const;
    ConcatStringTree concat(const ConcatStringTree & otherS)    const;
    ConcatStringTree subString(int from, int to)                const;
    ConcatStringTree reverse()                                  const;
    int getParTreeSize(const string & query)                    const;
    string getParTreeStringPreOrder(const string & query)       const;

private:
    int recursiveLength(node* )                                 const;
    char recursiveGet (int index, ConcatStringTree::node*)      const;
    void setSize();
    void recursiveFind (char c, node* node, int currIndex, int& found) const;
    string recursivetoStringPre(node* root)                     const;
    string recursivetoString(node* root)                        const;
    static node* recursiveReverse(node* const &root) ;                  //trả về 1 node mới và 1 cây mới từ đó
    static node* recursiveSubstr(node* const &root, int from, int to);
    ConcatStringTree& operator= (ConcatStringTree &&);                  //move assignment
    ConcatStringTree& operator= (ConcatStringTree const &other) = delete;


private:
    class node
    {
        //friend class ConcatStringTree;
    public: //có thể truy cập bằng enclosed nhưng không thể truy cập được ở bên ngoài

        //Variables
        std::string       data{};
        int               length{};
        int               leftLength{};
        int               rightLength{}; 
        ConcatStringTree::ParentsTree* parents{}; //chứa các node chỉ đến nó?, chỉ có thể là con trỏ chứ không thể là 1 class hoàn chỉnh được
        unsigned long id{};
        static unsigned long       maxID;
    private:
        node *            left{};
        node *            right{};

        //Constructors and Destructor
    public:
        node(string str): data{str}, length{(int)str.length()}, leftLength{0},rightLength{0}, left{nullptr}, right{nullptr}
        {
            if (maxID < MAX) id = ++maxID;
            else throw overflow_error("Id is overflow!");
            parents = new ParentsTree(); //khởi tạo từ 1 string, data, do đó không có gì thì không có PArrents, cây parents rỗng, root là nullptr
        }

        node()                              = delete;
        node            (node const &other) = delete;
        node& operator= (node const &other) = delete;
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

            parentsNode(ConcatStringTree::node* p = nullptr): data(p), balance(E), left(nullptr), right(nullptr)
            {}
        };
    };

};




//----------------------------------------------------------------------Bài 3----------------------------------------------------------------------------------
class ReducedConcatStringTree; // forward declaration
class LitStringHash; // forward declaration
class LitString;

class LitString // một loại string mới dùng cho hash lưu trữ
{
    friend class ReducedConcatStringTree;
    friend class LitStringHash;
    
private:
    std::string  str{};
    int          numofLink{}; //chỉ có duy nhất ở đây bất thường, chỉ chính là số node đang trỏ tới nó

public:
    int length() const {return str.length();}

    bool operator==(LitString const &other) const;

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

    ReducedConcatStringTree(ReducedConcatStringTree const &other)             = delete;
    ReducedConcatStringTree()                                                 = delete;
    ReducedConcatStringTree& operator= (ReducedConcatStringTree const &other) = delete;
    //Methods
public:
    int     length()                                                       const;
    char    get(int index)                                                 const;
    int     indexOf(char c)                                                const;
    string  toStringPreOrder()                                             const;
    string  toString()                                                     const;
    ReducedConcatStringTree concat(const ReducedConcatStringTree & otherS) const;
    //ReducedConcatStringTree subString(int from, int to)                    const;
    //ReducedConcatStringTree reverse()                                      const;
    int     getParTreeSize(const string & query)                           const;
    string  getParTreeStringPreOrder(const string & query)                 const;
private:
    int recursiveLength(node* )                                            const;
    char recursiveGet (int index, ReducedConcatStringTree::node*)          const;
    void setSize();
    void recursiveFind (char c, node* node, int currIndex, int& found)     const;
    string recursivetoStringPre(node* root)                                const;
    string recursivetoString(node* root)                                   const;
    //static node* recursiveReverse(node* const &root) ; //trả về 1 node mới và 1 cây mới từ đó
    //static node* recursiveSubstr(node* const &root, int from, int to);
    ReducedConcatStringTree& operator= (ReducedConcatStringTree &&); //move assignment
    
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

        parentsNode(ReducedConcatStringTree::node* p = nullptr): data(p), balance(E), left(nullptr), right(nullptr)
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
    unsigned long id{};
    static unsigned long maxID;
private:
    node *left{};
    node *right{};

public:
    node(LitString* p, LitStringHash* hash);  //node này trỏ tới p, nhưng p đã có sẵn nên quan tâm là thêm p vào như thế nào
    ~node(); //khi xóa node phải tháo ra 

    node()                              = delete;
    node            (node const &other) = delete;
    node& operator= (node const &other) = delete;
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
    int    const p;
    double const c1;
    double const c2;
    double const lambda;
    double const alpha;
    int    const initSize;

public:
    HashConfig(int p, double c1, double c2, double lambda, double alpha, int initSize):
        p(p), c1(c1), c2(c2), lambda(lambda), alpha(alpha), initSize(initSize)
    {
        if (alpha < 1.0) throw ("Cannot create a smaller tabler after rehashing!");
        if (lambda > 1.0) throw ("Invalid load factor");
    }
    HashConfig() = delete;

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
    HashConfig const  config;
    int               lastInserted{-1};

    //Constructors
public:
    LitStringHash(const HashConfig & hashConfig): 
        data{nullptr}, status{nullptr}, size{0}, config{hashConfig}, lastInserted{-1} {} //bảng hash vẫn chưa có gì đến khi được thêm vô

    LitStringHash()                                  = delete;
    LitStringHash(LitStringHash const &)             = delete;
    LitStringHash& operator= (LitStringHash const &) = delete;

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



#endif // __CONCAT_STRING_TREE_H__