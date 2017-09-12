
#include <iostream>

#include "AIUITest.h"

using namespace std;
using namespace aiui;

int     _argc = -1 ;
char ** _argv = NULL ;
char *  _progNameB = NULL ;
//string  _progNameS = "xxkj" ;
string  _progNameS ;

int main(int ___argc , char ** ___argv )
{

	AIUITester t;

    _argc   = ___argc ;
    _argv   = ___argv ;
    _progNameB = strdup( basename( _argv[0] ) ) ;
    _progNameS = string( _progNameB ) ;
    //_progNameS = string( "xxkjwjws" ) ;

	t.testAiui();
	return 0;
}
