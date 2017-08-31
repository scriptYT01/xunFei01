
#ifndef     __SENDRESULTTOSDOUTORTCP_H
#define     __SENDRESULTTOSDOUTORTCP_H

#include <string>
#include <assert.h>

using namespace std;

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


#endif //     __SENDRESULTTOSDOUTORTCP_H

