#ifndef     __STREAMCONTROLLER_H
#define     __STREAMCONTROLLER_H

#include    "superStream.h"

/* DoorController */
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
    public :
        void _scPrintf(         _enEaction ___eAction , const char *___fmt , ... ) ;
}; /* class _streamController */

class _streamControllerIN : public _streamController 
{
    public :
        _streamControllerIN(){ _ssDir=_enSsdIn; };
        ~_streamControllerIN(){};
} ; /* class _streamControllerIN */

class _streamControllerOUT : public _streamController 
{
    public :
        _streamControllerOUT(){ _ssDir=_enSsdOut; };
        ~_streamControllerOUT(){};
} ; /* class _streamControllerOUT */

#endif
