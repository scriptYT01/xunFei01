#include "superStream.h"


/* _superStream */

_superStreamBase * 
_superStreamBase::_genSS( _enSsDir ___ssDir , string ___path , string ___comment ) {
    _superStreamBase * __ss ;
    __ss = new _superStreamBase( ___ssDir , ___path , ___comment ) ;

    return __ss -> _ssOK ;

} /*_superStreamBase::_genSS */

_superStreamBase::_superStreamBase( _enSsDir ___ssDir , string ___path , string ___comment ) {
    _ssDir      =   ___ssDir        ;
    _ssType     =   _enSstUnknown   ;
    _ssPath     =   ___path         ;
    _ssComment  =   ___comment      ;
    cerr << " --- _superStreamBase : " << endl ;
};
