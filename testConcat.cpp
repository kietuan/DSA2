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

int main(int argc, char *argv[])
{
    
    return 0;
}