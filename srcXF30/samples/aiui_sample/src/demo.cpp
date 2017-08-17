
#include <iostream>

#include "AIUITest.h"

using namespace std;
using namespace aiui;

int     _argc = -1 ;
char ** _argv = NULL ;

int main(int ___argc , char ** ___argv )
{
    _argc   = ___argc ;
    _argv   = ___argv ;

	AIUITester t;
	t.test();
	return 0;
}
