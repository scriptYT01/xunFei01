
#include "patchXF/pathcXFbase.h"

// note : index start from 1 ,
void _paraAtoi( int ___argc , char ** ___argv , int ___index , char * ___varName , int * ___varLP ) {
    char ___str ;

    if ( ___varLP   == NULL  ) { _prExit( " var NULL " ); }
    if ( ___varName == NULL  ) { _prExit( " varName NULL " ); }
    
    if ( ___index < 1 || ___index >= ___argv ) { _prExit( " para index error " ); }

    ___str = ___argv[___index - 1];
    if ( ___str == NULL  ) { _prExit( " para NULL " ); }


    (*___varLP) = atoi( ___str ) ;

} // _paraAtoi
