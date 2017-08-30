#include "sendResultToSdoutOrTCP.h"

_sendResultToSdoutOrTCP::_sendResultToSdoutOrTCP( string ___outPath )
    : 
        _outPath( ___outPath ) 
{
    if ( "-" == _outPath ) {
        _ossType = _enOssStdout ;
    } else {
        _ossType = _enOssUnknown ;
    }
} // _sendResultToSdoutOrTCP::_sendResultToSdoutOrTCP

_sendResultToSdoutOrTCP::~_sendResultToSdoutOrTCP( )
{
} // _sendResultToSdoutOrTCP::~_sendResultToSdoutOrTCP

class _sendResultToSdoutOrTCP * _sendResultToSdoutOrTCP::_Obj() 
{
    //return this;
    return  _nowOBJ ;
} // _sendResultToSdoutOrTCP::_Obj
