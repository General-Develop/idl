#include <iostream>
using namespace std;
#include <windows.h>

#include "timer.h"
using namespace Vdl;

int main(int argc, char** argv)
{
	Timer time;

	Sleep(1234);
	cout << "now time elapsed: " << time.Elapsed() << endl;

	return 0;
}
