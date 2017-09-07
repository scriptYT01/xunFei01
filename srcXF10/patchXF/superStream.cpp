#include "superStream.h"


_superStream::_superStream( string ___outPath )
    : 
        _outPath( ___outPath ) 
{
    if ( "-" == _outPath ) {
        _ossType = _enSssStdout ;
    } 

    assert ( _ossType != _enSssUnknown ) ;
    _nowObj = this ;
} // _superStream::_superStream

_superStream::~_superStream( )
{
} // _superStream::~_superStream

class _superStream * _superStream::_Obj() 
{
    return  _nowObj ;
} // _superStream::_Obj

void _superStream::_SSo1( string ___msg1 ) 
{
} // _superStream::_SSo1

void _superStream::_SSo2( string ___msg1 , string ___msg2 ) 
{
    _SSo1( ___msg1 + ___msg2 ) ;
} // _superStream::_SSo2


