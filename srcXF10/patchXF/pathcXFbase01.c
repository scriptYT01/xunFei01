
#include "patchXF/pathcXFbase.h"

// note : index start from 1 ,

void _paraAtoi( int _para_must1_ignore0 , int ___argc , char ** ___argv , int ___index , char * ___varName , int * ___varLP ) {
    char * ___str ;

    if ( ___varLP   == NULL  ) { _prExit( " var NULL " ); }
    if ( ___varName == NULL  ) { _prExit( " varName NULL " ); }
    
    if ( ___index < 1 || ___index >= ___argc ) { if ( _para_must1_ignore0 ) { _prExit( " para index error " ); } ; return ; }

    if ( 0 ) { _prSFn( " --- input  %d : %p : %d , %s , %p " , ___argc , ___argv , ___index , ___varName , ___varLP ) ; }

    ___str = ___argv[___index + 0];
    if ( 0 ) { _prSFn( " --- input  is %s " , ___str ) ; }
    if ( ___str == NULL  ) { _prExit( " para NULL " ); }


    (*___varLP) = atoi( ___str ) ;

    _prSFn( " === get para %d : %s : %d" , ___index , ___varName , *___varLP ) ;

} // _paraAtoi

void _paraAtoS( int _para_must1_ignore0 , int ___argc , char ** ___argv , int ___index , char * ___varName , char ** ___varLP ) {
    char * ___str ;

    if ( ___varLP   == NULL  ) { _prExit( " var NULL " ); }
    if ( ___varName == NULL  ) { _prExit( " varName NULL " ); }
    
    if ( ___index < 1 || ___index >= ___argc ) { if ( _para_must1_ignore0 ) { _prExit( " para index error " ); } ; return ; }

    if ( 0 ) { _prSFn( " --- input  %d : %p : %d , %s , %p " , ___argc , ___argv , ___index , ___varName , ___varLP ) ; }

    ___str = ___argv[___index + 0];
    if ( 0 ) { _prSFn( " --- input  is %s " , ___str ) ; }
    if ( ___str == NULL  ) { _prExit( " para NULL " ); }


    (*___varLP) = ___str ;

    _prSFn( " === get para %d : %s : %s" , ___index , ___varName , *___varLP ) ;

} // _paraAtoS

