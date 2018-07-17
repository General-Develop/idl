#include <iostream>
using namespace std;
#include <windows.h>

#include "intrusiveptr.h"
using namespace Vdl;

int main(int argc, char** argv)
{
	class Node : public ReferenceCounter
	{
	public:
		int i;
		std::string str;
	};
	typedef iIntrusivePtr<Node> NodePtr;

	const int testCount = 1000000;

	system("pause");

	// wrong: memory increase with no release;
	/*
	Node* pNode = NULL;
	for(int i = 0;i < testCount;++i)
	{
		pNode = new Node;
		pNode->i = i;
		pNode->str = boost::lexical_cast<std::string>(i);
	}
	*/

	// correct: intrusive smart pointer auto release
	/*
	NodePtr nodePtr;
	for(int i = 0;i < testCount;++i)
	{
		nodePtr = new Node;
		nodePtr->i = i;
		nodePtr->str = boost::lexical_cast<std::string>(i);
	}
	*/

	// correct: intrusive smart point surpport reference count. 
	
	{
		NodePtr ptr1;
		ptr1 = new Node;
		ptr1->i = 111;
		ptr1->str = "111";
		{
			NodePtr ptr2 = ptr1.get();
		}
	} // release ptr1 correct
	

	system("pause");

	return 0;
}