#include "patchXF/pathcXFbase.h"

#ifndef     __SENDRESULTTOSDOUTORTCP_H
#define     __SENDRESULTTOSDOUTORTCP_H

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>


using namespace std;

enum _enEaction {
    _enEreopen,     // tryReopen when err occurs.
    _enEignore,     // ignore when err occurs.
    _enEexit        // exit when err occurs.
}; /* _enEaction */

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
        int                     _ssFP       ;
    public :
        _superStreamBase( _enSsDir ___ssDir , string ___path , string ___comment ) ;
        ~_superStreamBase( ){} ;
        static _superStreamBase * _genSS( bool _exitIfErr ,  _enSsDir ___ssDir , string ___path , string ___comment ) ;
    public :
        void _ssOut(            _enEaction ___eAction , const char *___fmt , ... ) ;
        void _ssWriteNonblock(  _enEaction ___eAction , int ___len , const char * ___buf ) ;
        void _ssWriteBlock(     _enEaction ___eAction , int ___len , const char * ___buf ) ;

}; /* class _superStreamBase */

class _ssCin : public _superStreamBase
{
    public : 
        _ssCin( _enSsDir ___ssDir , string ___path , string ___comment ) ;
        ~_ssCin( ){} ;
} ; /* class _ssCin */

class _ssCout : public _superStreamBase
{
    public :
        _ssCout( _enSsDir ___ssDir , string ___path , string ___comment ) ;
        ~_ssCout( ){} ;
} ; /* class _ssCout */

class _ssCerr : public _superStreamBase
{
    public :
        _ssCerr( _enSsDir ___ssDir , string ___path , string ___comment ) ;
        ~_ssCerr( ){} ;
} ; /* class _ssCerr */

// DoorController
class _streamController 
{
    private :
    protected :
        _enSsDir _ssDir ;
    protected :
        vector <_superStreamBase*> _sVec ;
    public:
        _streamController():_ssDir(_enSsdUnknown) {};
        virtual ~_streamController(){};
    public:
        bool _addPath2( string ___path , string ___comment ) {
            _sVec . push_back( _superStreamBase::
                    _genSS( 1 , _ssDir , ___path , ___comment ) ) ;
            return true ;
        }
        void _sendMsg1( string ___msg1 ) {
            cout << ___msg1 ;
        }
        void _sendMsg1n( string ___msg1 ) { _sendMsg1( ___msg1 + "\n" ); }
}; // class _streamController 

class _streamControllerIN : public _streamController 
{
    public :
        _streamControllerIN(){ _ssDir=_enSsdIn; };
        ~_streamControllerIN(){};
} ; // class _streamControllerIN 

class _streamControllerOUT : public _streamController 
{
    public :
        _streamControllerOUT(){ _ssDir=_enSsdOut; };
        ~_streamControllerOUT(){};
} ; // class _streamControllerOUT 


#endif //     __SENDRESULTTOSDOUTORTCP_H

