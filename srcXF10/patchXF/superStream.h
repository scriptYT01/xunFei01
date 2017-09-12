
#ifndef     __SUPERSTREAM_H
#define     __SUPERSTREAM_H

#include "patchXF/pathcXFbase.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>


using namespace std;

enum _enErrAction {
    _enEreopen,     // tryReopen when err occurs.
    _enEignore,     // ignore when err occurs.
    _enEexit        // exit when err occurs.
}; /* _enErrAction */

enum _enSsDir {
    _enSsdUnknown,
    _enSsdOutIn,
    _enSsdOut,
    _enSsdIn
}; /* _enSsDir */

enum _enSsType { /* stream type */
    _enSstUnknown,
    _enSstTcpConnectTo,
    _enSstTcpListen1,
    _enSstCin,
    _enSstCerr,
    _enSstCout
}; /* _enSsType */

struct _superStreamInfoX {
    uint64_t                _tryCnt     ;
    uint64_t                _tryLen     ;
    uint64_t                _skipCnt    ;
    uint64_t                _skipLen    ;
    uint64_t                _succCnt    ;
    uint64_t                _succLen    ;
} ;
typedef struct _superStreamInfoX _superStreamInfo ; 

class _superStreamBase
{
    protected :
        _enSsDir                _ssDir      ;
        _enSsType               _ssType     ;
        string                  _ssPath     ;
        string                  _ssComment  ;
        _superStreamBase    *   _ssOK       ;
        int                     _ssFD       ;
        _superStreamInfo        _ssInfoW    ; 
        _superStreamInfo        _ssInfoR    ; 

        void _superStreamInit( _enSsType ___ssType , _enSsDir ___ssDir , string ___path , string ___comment ) ;
        static int              _valid_fd_or_errFD( int ___fd ) ;
        static bool             _fd_valid1_invalid0( int ___fd ) ;
        void                    _ssTryReopneIfNeeded( _enErrAction ___eAction ) ;
    public :
        _superStreamBase(){ _memZS( _ssInfoW ) ; _memZS( _ssInfoR ) ; } ;
        ~_superStreamBase( ){} ;
        static _superStreamBase * _genSS( bool _exitIfErr ,  _enSsDir ___ssDir , string ___path , string ___comment ) ;
    public :
        void _ssWriteNonblock(  _enErrAction ___eAction , int ___len , const char * ___buf ) ;
        void _ssWriteBlock(     _enErrAction ___eAction , int ___len , const char * ___buf ) ;
        void _ssReadNonblock(   _enErrAction ___eAction , int ___len , const char * ___buf ) ;
        void _ssReadBlock(      _enErrAction ___eAction , int ___len , const char * ___buf ) ;
    private :
        virtual bool _ssOpenOrReopen() = 0 ;

}; /* class _superStreamBase */

class _ssCin : public _superStreamBase
{
    public : 
        _ssCin( _enSsDir ___ssDir , string ___path , string ___comment ) ;
        ~_ssCin( ){} ;
    private :
        bool _ssOpenOrReopen() ;
} ; /* class _ssCin */

class _ssCout : public _superStreamBase
{
    public :
        _ssCout( _enSsDir ___ssDir , string ___path , string ___comment ) ;
        ~_ssCout( ){} ;
    private :
        bool _ssOpenOrReopen() ;
} ; /* class _ssCout */

class _ssCerr : public _superStreamBase
{
    public :
        _ssCerr( _enSsDir ___ssDir , string ___path , string ___comment ) ;
        ~_ssCerr( ){} ;
    private :
        bool _ssOpenOrReopen() ;
} ; /* class _ssCerr */

class _ssTcpConnectTo : public _superStreamBase
{
    public :
        _ssTcpConnectTo( _enSsDir ___ssDir , string ___path , string ___comment ) ;
        ~_ssTcpConnectTo( ){} ;
    private :
        bool _ssOpenOrReopen() ;
} ; /* class _ssTcpConnectTo */

class _ssListen1 : public _superStreamBase
{
    public :
        _ssListen1( _enSsDir ___ssDir , string ___path , string ___comment ) ;
        ~_ssListen1( ){} ;
    private :
        bool _ssOpenOrReopen() ;
} ; /* class _ssListen1 */



#endif //     __SENDRESULTTOSDOUTORTCP_H

