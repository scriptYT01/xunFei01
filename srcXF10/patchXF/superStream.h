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

enum _enSsDir {
    _enSsdUnknown,
    _enSsdOut,
    _enSsdIn
}; /* _enSsDir */

enum _enSsType { /* stream type */
    _enSstUnknown,
    _enSstOut,
    _enSstIn,
    _enSstStdout,
    _enSstTcp
}; /* _enSsType */

class _streamBase
{
    protected :
        _enSsType  _ssType ;
        string      _ssPath ;
        string      _ssComment ;
    public :
        _streamBase( string ___path , string ___comment ) {
            _ssType     =   _enSstUnknown ;
            _ssPath     =   ___path ;
            _ssComment  =   ___comment ;
                cerr << " --- _streamBase : " << endl ;
        };
}; /* class _streamBase */


// DoorController
class _streamController 
{
    private :
    protected :
        _enSsDir _ssDir ;
    protected :
        vector <_streamBase*> _sVec ;
    public:
        _streamController():_ssDir(_enSsdUnknown) {};
        virtual ~_streamController(){};
    public:
        bool _addPath2( string ___path , string ___comment ) {
            _sVec . push_back( new _streamBase( ___path , ___comment ) ) ;
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

