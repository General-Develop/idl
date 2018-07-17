#include <iostream>
using namespace std;
#include <windows.h>

#include <boost/lexical_cast.hpp>

#include "smartptr.h"
using namespace Vdl;

int main(int argc, char** argv)
{
	class Node
	{
	public:
		int i;
		std::string str;
	};
	typedef iSmartPtr<Node> NodePtr;

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

	// correct: smart pointer auto release
	
	NodePtr nodePtr;
	for(int i = 0;i < testCount;++i)
	{
		nodePtr = new Node;
		nodePtr->i = i;
		nodePtr->str = boost::lexical_cast<std::string>(i);
	}
	

	// fatal: smart point donot have reference count. so donot use "ptr2 = ptr1.get()"
	//  if you really want to use.  please use iIntrusivePtr. see file intrusiveptr.h
	/*
	{
		NodePtr ptr1;
		ptr1 = new Node;
		ptr1->i = 111;
		ptr1->str = "111";
		{
			NodePtr ptr2 = ptr1.get();
		}
	} // release ptr1 error
	*/

	system("pause");

	return 0;
}