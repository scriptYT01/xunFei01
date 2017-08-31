#include "sendResultToSdoutOrTCP.h"

_streamController       _outSC    ;
_streamController       _inSC     ;

_sendResultToSdoutOrTCP::_sendResultToSdoutOrTCP( string ___outPath )
    : 
        _outPath( ___outPath ) 
{
    if ( "-" == _outPath ) {
        _ossType = _enOssStdout ;
    } 

    assert ( _ossType != _enOssUnknown ) ;
    _nowObj = this ;
} // _sendResultToSdoutOrTCP::_sendResultToSdoutOrTCP

_sendResultToSdoutOrTCP::~_sendResultToSdoutOrTCP( )
{
} // _sendResultToSdoutOrTCP::~_sendResultToSdoutOrTCP

class _sendResultToSdoutOrTCP * _sendResultToSdoutOrTCP::_Obj() 
{
    return  _nowObj ;
} // _sendResultToSdoutOrTCP::_Obj

void _sendResultToSdoutOrTCP::_SSo1( string ___msg1 ) 
{
} // _sendResultToSdoutOrTCP::_SSo1

void _sendResultToSdoutOrTCP::_SSo2( string ___msg1 , string ___msg2 ) 
{
    _SSo1( ___msg1 + ___msg2 ) ;
} // _sendResultToSdoutOrTCP::_SSo2


