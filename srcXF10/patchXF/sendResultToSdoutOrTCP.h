
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

enum _enOccType {
    _enOccUnknown,
    _enOccOut,
    _enOccIn
};
enum _enOssType {
    _enOssUnknown,
    _enOssStdout,
    _enOssTcp
};

class _sendResultToSdoutOrTCP
{
    private :
        string _outPath ;
        _enOssType _ossType ;
        _sendResultToSdoutOrTCP * _nowObj ;
    public :
        _sendResultToSdoutOrTCP( string ___outPath ) ;
        ~_sendResultToSdoutOrTCP();
        _sendResultToSdoutOrTCP * _Obj() ;
        void _SSo1( string ___msg1 ) ;
        void _SSo2( string ___msg1 , string ___msg2 ) ;

} ; // class _sendResultToSdoutOrTCP

class _streamBase
{
    protected :
        _enOssType  _ssType ;
        string      _ssPath ;
        string      _ssComment ;
    public :
        _streamBase( string ___path , string ___comment ) {
            _ssType     =   _enOssUnknown ;
            _ssPath     =   ___path ;
            _ssComment  =   ___comment ;
                cerr << " --- _streamBase : " << endl ;
        };
}; // class _streamBase

class _streamIN : public _streamBase
{
    public :
        _streamIN( string ___path , string ___comment )
            : _streamBase( ___path , ___comment ) {
                cerr << " --- _streamIN : " << endl ;
        }
    private:
} ; // class _streamIN 

class _streamOUT : public _streamBase
{
    public :
        _streamOUT( string ___path , string ___comment ) 
            : _streamBase( ___path , ___comment ) {
                cerr << " --- _streamOUT : " << endl ;
        }
    private:
} ; // class _streamOUT 

// DoorController
class _streamController 
{
    private :
    protected :
        _enOccType _scType ;
    protected :
        //vector <string,string,_streamBase*> _sVec ;
        vector <_streamBase*> _sVec ;
    public:
        _streamController():_scType(_enOccUnknown) {};
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
        _streamControllerIN(){ _scType=_enOccIn; };
        ~_streamControllerIN(){};
} ; // class _streamControllerIN 

class _streamControllerOUT : public _streamController 
{
    public :
        _streamControllerOUT(){ _scType=_enOccOut; };
        ~_streamControllerOUT(){};
} ; // class _streamControllerOUT 


#endif //     __SENDRESULTTOSDOUTORTCP_H

