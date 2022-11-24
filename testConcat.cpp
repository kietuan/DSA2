#include "ConcatStringTree.h"

void test_indexof(const ConcatStringTree* const p)
{
	string s = ( p->toString().substr(18, p->length()) );
	cout << "String: " << s <<endl;
	for(auto it = s.begin(); it != s.end(); it++)
	{
		cout << "Index of "<<*it<<" = " << p->indexOf(*it) << endl;
	}
}

int main()
{
    ConcatStringTree s1("hello");
    ConcatStringTree s3("world");
    ConcatStringTree s2("fucking");

    ConcatStringTree s4 = s1.concat(s2).concat(s3);
    ConcatStringTree s5 = s4.subString(6, 16);

    cout << "Length of s4 " << s4.length() << endl;
    cout << "Length of s5 " << s5.length() << endl;
    cout << "s5 " << s5.toString() << endl;
    cout<<"preorder " << s5.toStringPreOrder() << endl;

    cout << "index of 9 " << s5.get(9) << endl;

    test_indexof(&s4);
    test_indexof(&s5);
//substring có rỗng không, tính xử lý thế nào?

    return 0;
}