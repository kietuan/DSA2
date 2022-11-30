#include "ConcatStringTree.h"

void test(const ConcatStringTree* const p)
{
	string s = ( p->toString().substr(18, p->length()) );
	cout << "String: \"" << s << "\" have the length: "<<p->length() <<endl;
    cout<<"preorder " << p->toStringPreOrder() << endl;
	for(auto it = s.begin(); it != s.end(); it++)
	{
		cout << "Index of "<<*it<<" = " << p->indexOf(*it) << " get "<< p->get(p->indexOf(*it)) <<endl;
	}
}

void test(const ReducedConcatStringTree* const p)
{
    string s = ( p->toString().substr(18, p->length()) );
	cout << "String: \"" << s << "\" have the length: "<<p->length() <<endl;
    cout<<"preorder " << p->toStringPreOrder() << endl;
	for(auto it = s.begin(); it != s.end(); it++)
	{
		cout << "Index of "<<*it<<" = " << p->indexOf(*it) << " get "<< p->get(p->indexOf(*it)) <<endl;
	}
}

void tcHoc()
{
    HashConfig h(1, 1, 0, 0.8, 2, 2);
    LitStringHash *litStringHash = new LitStringHash(h);
    ReducedConcatStringTree *test1 = new ReducedConcatStringTree("1", litStringHash);
    ReducedConcatStringTree *test2 = new ReducedConcatStringTree("2", litStringHash);
    ReducedConcatStringTree *test3 = new ReducedConcatStringTree("3", litStringHash);
    ReducedConcatStringTree *test4 = new ReducedConcatStringTree("4", litStringHash);
    ReducedConcatStringTree *test5 = new ReducedConcatStringTree("5", litStringHash);
    ReducedConcatStringTree *test6 = new ReducedConcatStringTree("6", litStringHash);
    ReducedConcatStringTree *test7 = new ReducedConcatStringTree("7", litStringHash);
    ReducedConcatStringTree *test8 = new ReducedConcatStringTree("8", litStringHash);
    ReducedConcatStringTree *test9 = new ReducedConcatStringTree("9", litStringHash);
    ReducedConcatStringTree *test10 = new ReducedConcatStringTree("10", litStringHash);

    ReducedConcatStringTree *concat1 = new ReducedConcatStringTree(test1->concat(*test2));
    ReducedConcatStringTree *concat2 = new ReducedConcatStringTree(test3->concat(*test4));
    ReducedConcatStringTree *concat3 = new ReducedConcatStringTree(concat1->concat(*concat2));
    ReducedConcatStringTree *concat4 = new ReducedConcatStringTree(test1->concat(*test5));
    ReducedConcatStringTree *concat5 = new ReducedConcatStringTree(test1->concat(*test6));
    ReducedConcatStringTree *concat6 = new ReducedConcatStringTree(test1->concat(*test7));
    ReducedConcatStringTree *concat7 = new ReducedConcatStringTree(test1->concat(*test8));
    ReducedConcatStringTree *concat8 = new ReducedConcatStringTree(test1->concat(*test9));
    ReducedConcatStringTree *concat9 = new ReducedConcatStringTree(test1->concat(*test10));
    cout << litStringHash->toString() << endl;
    cout << litStringHash->getLastInsertedIndex() << endl;
    cout << test1->getParTreeStringPreOrder("") << " " << test1->getParTreeSize("") << endl;
    cout << test1 -> toString();
    cout << concat3->getParTreeStringPreOrder("ll") << " " << concat3->getParTreeSize("ll") << endl;
    cout << concat3->getParTreeStringPreOrder("rr") << " " << concat3->getParTreeSize("rr") << endl;
    delete test2;
    cout << litStringHash->toString() << " " << litStringHash->getLastInsertedIndex() << endl;
    delete test3;
    cout << litStringHash->toString() << " " << litStringHash->getLastInsertedIndex() << endl;
    delete test4;
    cout << litStringHash->toString() << " " << litStringHash->getLastInsertedIndex() << endl;
    delete test5;
    cout << litStringHash->toString() << " " << litStringHash->getLastInsertedIndex() << endl;
    delete concat3;
    delete concat4;
    cout << litStringHash->toString() << " " << litStringHash->getLastInsertedIndex() << endl;
    delete concat5;
    delete concat6;
    delete test6;
    cout << litStringHash->toString() << " " << litStringHash->getLastInsertedIndex() << endl;
    delete test7;
    cout << litStringHash->toString() << " " << litStringHash->getLastInsertedIndex() << endl;
    delete test8;
    cout << litStringHash->toString() << " " << litStringHash->getLastInsertedIndex() << endl;
    delete test9;
    cout << litStringHash->toString() << " " << litStringHash->getLastInsertedIndex() << endl;
    delete test10;

    delete concat1;
    cout << test1->getParTreeStringPreOrder("") << " " << test1->getParTreeSize("") << endl;
    delete concat2;
    cout << test1->getParTreeStringPreOrder("") << " " << test1->getParTreeSize("") << endl;
    delete concat7;
    //test1->toString();
    cout << litStringHash->toString() << " " << litStringHash->getLastInsertedIndex() << endl;
    //test1->toString();
    delete concat8;
    cout << test1->getParTreeStringPreOrder("") << " " << test1->getParTreeSize("") << endl;
    delete concat9;
    cout << test1->getParTreeStringPreOrder("") << " " << test1->getParTreeSize("") << endl;
    delete test1;
    cout << litStringHash->toString() << " " << litStringHash->getLastInsertedIndex() << endl;
    delete litStringHash;
}
//int main(int argc, char *argv[])
int main()
{	
	tcHoc();

    
    return 0;
}