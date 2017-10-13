#include "ymList01.h"

void _showPlayNow()
{
    int __i01 ;
    _prOOn( "\n\n\n" 
            " Test content is (%p , %d) :"    "\n" 
            , _listAA
            , _testSize
        );
    for ( __i01 = 0 ; __i01 < _testSize ; __i01 ++ ) {
        if ( __i01 == 0 ) {
            if(0) _prOOn( "%p" , &(_listAA[ __i01 ]) ) ;
            _prOO( "%s" , _listAA[ __i01 ] . _fname ) ;
        } else if ( __i01 % 20 == 0 ) {
            if(0) _prOOn( "%p" , &(_listAA[ __i01 ]) ) ;
            _prOO( "\n%s" , _listAA[ __i01 ] . _fname ) ;
        } else if ( __i01 % 10 == 0 ) {
            if(0) _prOOn( "%p" , &(_listAA[ __i01 ]) ) ;
            _prOO( "    %s" , _listAA[ __i01 ] . _fname ) ;
        } else {
            if(0) _prOOn( "%p" , &(_listAA[ __i01 ]) ) ;
            _prOO( " %s" , _listAA[ __i01 ] . _fname ) ;
        }
    }
    _prOOn( "\n\n") ;
} /* _showPlayNow */

void _showPlaySingle()
{
    int __i01 ;
    _prOOn( "\n\n\n" 
            " avaiable single test is "                                                                     "\n" 
        );
    for ( __i01 = 0 ; __i01 < _listA1_awake1 ; __i01 ++ ) {
        if ( __i01 == 0 ) {
            _prOO( "%s" , _listA1[ __i01 ] . _fname ) ;
        } else if ( __i01 % 20 == 0 ) {
            _prOO( "\n%s" , _listA1[ __i01 ] . _fname ) ;
        } else if ( __i01 % 10 == 0 ) {
            _prOO( "    %s" , _listA1[ __i01 ] . _fname ) ;
        } else {
            _prOO( " %s" , _listA1[ __i01 ] . _fname ) ;
        }
    }
    _prOOn( "\n\n") ;
    exit( 91 ) ;
} /* _showPlaySingle */

void _showPlayListG()
{
    int __i01 ;
    _prOOn( "\n\n\n" 
            " avaiable list Group is "                                                                     "\n" 
        );
    for ( __i01 = 0 ; __i01 < _listGG_objSize ; __i01 ++ ) {
        if ( __i01 == 0 ) {
            _prOO( "%2d" , __i01 ) ;
        } else if ( __i01 % 20 == 0 ) {
            _prOO( "\n%2d" , __i01 ) ;
        } else if ( __i01 % 10 == 0 ) {
            _prOO( "    %2d" , __i01 ) ;
        } else {
            _prOO( " %2d" , __i01 ) ;
        }
    }
    _prOOn( "\n\n") ;
    exit( 92 ) ;
} /* _showPlayListG */

void _showUsageExit()
{
    _prOOn( "\n\n\n" 
            "Usage : "                                                                                        "\n"
            " %s [<-d> | <-h> | <-p XX> | <-l YY> ]"                                                          "\n"
            "     -h , --help   : show debug info / usage only. "                                             "\n"

            "     -d , --debug  : listen only , waiting the use play the awake / words "                      "\n"
            "     -p , --play   : awake , then play the specified word item only."                            "\n"
            "     -pp           : list all single test can be use "                                           "\n"
            "     -l , --list   : awake , then play the specified word list. "                                "\n"
            "     -ll           : list all list can be use "                                                  "\n"

            "     no parameter  : play the default all word items , use about 10 - 15 minuts. "               "\n"
            "\n"
            , _argV[0] 
        );
    exit(33) ;
} /* _showUsageExit */

