
#ifndef     __SENDRESULTTOSDOUTORTCP_H
#define     __SENDRESULTTOSDOUTORTCP_H

#include "patchXF/pathcXFbase.h"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

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

class _superStreamBase
{
    protected :
        _enSsDir                _ssDir      ;
        _enSsType               _ssType     ;
        string                  _ssPath     ;
        string                  _ssComment  ;
        _superStreamBase    *   _ssOK       ;
        int                     _ssFD       ;
        void _superStreamInit( _enSsType ___ssType , _enSsDir ___ssDir , string ___path , string ___comment ) ;
    public :
        _superStreamBase(){} ;
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

