#include "patchXF/pathcXFbase.h"

#ifndef     __SENDRESULTTOSDOUTORTCP_H
#define     __SENDRESULTTOSDOUTORTCP_H

#include <stdio.h>
#include <assert.h>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>


using namespace std;

enum _enSccType {
    _enSccUnknown,
    _enSccOut,
    _enSccIn
}; /* _enSccType */

enum _enSssType { /* stream type */
    _enSssUnknown,
    _enSssOut,
    _enSssIn,
    _enSssStdout,
    _enSssTcp
}; /* _enSssType */

class _streamBase
{
    protected :
        _enSssType  _ssType ;
        string      _ssPath ;
        string      _ssComment ;
    public :
        _streamBase( _enSssType ___ssType , string ___path , string ___comment ) {
            _ssType     =   _enSssUnknown ;
            _ssPath     =   ___path ;
            _ssComment  =   ___comment ;
                cerr << " --- _streamBase : " << endl ;
        };
}; /* class _streamBase */

class _streamIN : public _streamBase
{
    public :
        _streamIN( string ___path , string ___comment )
            : _streamBase( _enSssIn , ___path , ___comment ) {
                cerr << " --- _streamIN : " << endl ;
        }
    private:
} ; // class _streamIN 

class _streamOUT : public _streamBase
{
    public :
        _streamOUT( string ___path , string ___comment ) 
            : _streamBase( _enSssOut , ___path , ___comment ) {
                cerr << " --- _streamOUT : " << endl ;
        }
    private:
} ; // class _streamOUT 

// DoorController
class _streamController 
{
    private :
    protected :
        _enSccType _sccType ;
    protected :
        //vector <string,string,_streamBase*> _sVec ;
        vector <_streamBase*> _sVec ;
    public:
        _streamController():_sccType(_enSccUnknown) {};
        virtual ~_streamController(){};
    public:
        //_outSC< _streamBase > . _addPath2( _argv[2] , " output-stream " ) ;
        template <class TSS1>
            bool _addPath2( string ___path , string ___comment ) {
                //_sVec . push_back( ___path , ___comment , (_streamBase*) (new TSS1( ___path , ___comment )) ) ;
                _sVec . push_back( (_streamBase*) new TSS1( ___path , ___comment ) ) ;
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
        _streamControllerIN(){ _sccType=_enSccIn; };
        ~_streamControllerIN(){};
} ; // class _streamControllerIN 

class _streamControllerOUT : public _streamController 
{
    public :
        _streamControllerOUT(){ _sccType=_enSccOut; };
        ~_streamControllerOUT(){};
} ; // class _streamControllerOUT 


#endif //     __SENDRESULTTOSDOUTORTCP_H

