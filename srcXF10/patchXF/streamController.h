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
        bool _addPath2( string ___path , string ___comment ) ;
    public :
        void _scPrintf(         _enErrAction ___eAction , const char *___fmt , ... ) ;
        void _scDumpSS() ;
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
