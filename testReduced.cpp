#include "ReducedConcatStringTree.h"

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

int main()
{
    HashConfig config(3 ,1.1, 2.3, 0.9, 1.5, 10);
    auto hasTable = new LitStringHash(config);
    auto s1 = new ReducedConcatStringTree("Hello", hasTable);
    auto s2 = new ReducedConcatStringTree ("world", hasTable);
    ReducedConcatStringTree s3 = s1->concat(*s2);
    delete s1;
    delete s2;
    cout << s3.toString() << endl;
    return 0;
}