#include "sendResultToSdoutOrTCP.h"

_sendResultToSdoutOrTCP::_sendResultToSdoutOrTCP( string ___outPath )
    : 
        _outPath( ___outPath ) 
{
    if ( "-" == _outPath ) {
        _ossType = _enOssStdout ;
    } 

    _nExit ( _ossType != _enOssUnknown ) ;
    _nowObj = this ;
} // _sendResultToSdoutOrTCP::_sendResultToSdoutOrTCP

_sendResultToSdoutOrTCP::~_sendResultToSdoutOrTCP( )
{
} // _sendResultToSdoutOrTCP::~_sendResultToSdoutOrTCP

class _sendResultToSdoutOrTCP * _sendResultToSdoutOrTCP::_Obj() 
{
    //return this;
    return  _nowObj ;
} // _sendResultToSdoutOrTCP::_Obj
