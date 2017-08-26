
#include <iostream>

#include "AIUITest.h"

using namespace std;
using namespace aiui;

int     _argc = -1 ;
char ** _argv = NULL ;

int main(int ___argc , char ** ___argv )
{

	AIUITester t;

    _argc   = ___argc ;
    _argv   = ___argv ;

	t.test();
	return 0;
}
