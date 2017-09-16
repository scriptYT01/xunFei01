#ifndef     __STREAMCONTROLLER_H
#define     __STREAMCONTROLLER_H

#include    "superStream.h"

/* DoorController */
class _streamController 
{
    private :
    protected :
        _enSsDir _scDir ;
    protected :
        vector <_superStreamBase*> _sVec ;
    public:
        _streamController():_scDir(_enSsdUnknown) {};
        virtual ~_streamController(){};
    public:
        bool _addPath2( const char * ___path , const char * ___comment ) ;
    public :
        void _scSelfTest01( ) ;
        void _scPrintf(         const char *___fmt , ... ) ;
        void _scDumpSS() ;
}; /* class _streamController */

class _streamControllerIN : public _streamController 
{
    public :
        _streamControllerIN(){ _scDir=_enSsdIn; };
        ~_streamControllerIN(){};
} ; /* class _streamControllerIN */

class _streamControllerOUT : public _streamController 
{
    public :
        _streamControllerOUT(){ _scDir=_enSsdOut; };
        ~_streamControllerOUT(){};
} ; /* class _streamControllerOUT */

#define _dpS1( fmt , ... )  _dbgS1 . _scPrintf( _enEreopen , fmt , ## __VA_ARGS__ )
#define _dpS1n( fmt , ... ) _dpS1( fmt "\n" , ## __VA_ARGS__ )
#define _dpS2( fmt , ... )  _dbgS2 . _scPrintf( _enEreopen , fmt , ## __VA_ARGS__ )
#define _dpS2n( fmt , ... ) _dpS2( fmt "\n" , ## __VA_ARGS__ )
#define _dpS3( fmt , ... )  _dbgS3 . _scPrintf( _enEreopen , fmt , ## __VA_ARGS__ )
#define _dpS3n( fmt , ... ) _dpS3( fmt "\n" , ## __VA_ARGS__ )

#define _dpOUT( fmt , ... )  _outSC . _scPrintf( _enEignore , fmt , ## __VA_ARGS__ )
#define _dpOUTn( fmt , ... ) _dpOUT( fmt "\n" , ## __VA_ARGS__ )

#endif
