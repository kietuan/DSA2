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
    ConcatStringTree *test1 = new ConcatStringTree("01");
    ConcatStringTree *test2 = new ConcatStringTree("23");
    ConcatStringTree *test3 = new ConcatStringTree("45");
    ConcatStringTree *test4 = new ConcatStringTree("A");
    ConcatStringTree *test5 = new ConcatStringTree("B");
    ConcatStringTree *test6 = new ConcatStringTree("C");
    ConcatStringTree *test7 = new ConcatStringTree("D");
    ConcatStringTree *test8 = new ConcatStringTree("E");
    ConcatStringTree *test9 = new ConcatStringTree("F");
    ConcatStringTree *test10 = new ConcatStringTree("G");

    ConcatStringTree *concat1 = new ConcatStringTree(test1->concat(*test2));
    ConcatStringTree *concat2 = new ConcatStringTree(test3->concat(*test4));
    ConcatStringTree *concat3 = new ConcatStringTree(concat1->concat(*concat2));
    ConcatStringTree *concat4 = new ConcatStringTree(test1->concat(*test5));
    ConcatStringTree *concat5 = new ConcatStringTree(test1->concat(*test6));
    ConcatStringTree *concat6 = new ConcatStringTree(test1->concat(*test7));
    ConcatStringTree *concat7 = new ConcatStringTree(test1->concat(*test8));
    ConcatStringTree *concat8 = new ConcatStringTree(test1->concat(*test9));
    ConcatStringTree *concat9 = new ConcatStringTree(test1->concat(*test10));
    cout << test1->getParTreeStringPreOrder("") << " " << test1->getParTreeSize("") << endl;
    cout << concat3->getParTreeStringPreOrder("ll") << " " << concat3->getParTreeSize("ll") << endl;
    cout << concat3->getParTreeStringPreOrder("rr") << " " << concat3->getParTreeSize("rr") << endl;
    delete test2;
    delete test3;
    delete test4;
    delete test5;
    delete concat3;
    delete concat4;
    delete concat5;
    delete concat6;
    delete test6;
    delete test7;
    delete test8;
    delete test9;
    delete test10;

    delete concat1;
    cout << test1->getParTreeStringPreOrder("") << " " << test1->getParTreeSize("") << endl;
    delete concat2;
    cout << test1->getParTreeStringPreOrder("") << " " << test1->getParTreeSize("") << endl;
    delete concat7;
    delete concat8;
    cout << test1->getParTreeStringPreOrder("") << " " << test1->getParTreeSize("") << endl;
    delete concat9;
    cout << test1->getParTreeStringPreOrder("") << " " << test1->getParTreeSize("") << endl;
    delete test1;
}

//int main(int argc, char *argv[])
int main()
{	
	tcHoc();

    
    return 0;
}