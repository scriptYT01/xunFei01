#include "match01.h"

_Match01::_Match01( char ___sep , int ___len ) 
{
    _maLen      = ___len ;
    _maSep      = ___sep ;

    _Match01();
} /* _Match01::_Match01 */

_Match01::_Match01( ) 
{
} /* _Match01::_Match01 */

_Match01* _Match01::_ok( ) 
{
    if ( _maBuf == NULL ) {
        return NULL ;
    } 

    return this ;
} /* _Match01::_ok */
