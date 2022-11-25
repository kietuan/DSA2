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


//int main(int argc, char *argv[])
int main()
{	
	ConcatStringTree s1 ("Hello");
    ConcatStringTree s2 ("world");
    ConcatStringTree s3 = s1.concat(s2);	

    
    return 0;
}