#include "sendResultToSdoutOrTCP.h"

_sendResultToSdoutOrTCP::_sendResultToSdoutOrTCP( string ___outDST )
    : 
        _outDST( ___outDST ) 
{
} // _sendResultToSdoutOrTCP::_sendResultToSdoutOrTCP

_sendResultToSdoutOrTCP::~_sendResultToSdoutOrTCP( )
{
} // _sendResultToSdoutOrTCP::~_sendResultToSdoutOrTCP

class _sendResultToSdoutOrTCP * _sendResultToSdoutOrTCP::_Obj() 
{
    //return this;
    return  _nowOBJ ;
} // _sendResultToSdoutOrTCP::_Obj
