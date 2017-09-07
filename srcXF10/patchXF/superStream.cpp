#include "superStream.h"


/* _superStream */

_superStreamBase * 
_superStreamBase::_genSS( bool _exitIfErr , _enSsDir ___ssDir , string ___path , string ___comment ) {
    _superStreamBase * __ss ;
    __ss = (new _superStreamBase( ___ssDir , ___path , ___comment ) ) -> _ssOK ;

    if ( _exitIfErr ) {
        _zExit( __ss , " create error , exit when error met. exit." ) ;
    }

    return __ss ;

} /*_superStreamBase::_genSS */

_superStreamBase::_superStreamBase( _enSsDir ___ssDir , string ___path , string ___comment ) {
    _ssDir      =   ___ssDir        ;
    _ssType     =   _enSstUnknown   ;
    _ssPath     =   ___path         ;
    _ssComment  =   ___comment      ;
    cerr << " --- _superStreamBase : " << endl ;

    _ssOK = this ;
};
