#include <iostream>
using namespace std;

#include <time.h>

#include "datetime.h"
using namespace Vdl;


int main(int argc, char** argv)
{
	DateTime localTime = DateTimeHelper::CurrentDateTime();
	cout << "localTime: " << localTime << endl;

	std::string strLocalTime = DateTimeHelper::ToString(localTime);
	cout << "strLocalTime: " << strLocalTime << endl;

	DateTime localTimeNew = DateTimeHelper::FromString(strLocalTime);
	cout << "localTimeNew: " << localTimeNew << endl;

	return 0;

}