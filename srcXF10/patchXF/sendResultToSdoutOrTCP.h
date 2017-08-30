
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
        class _sendResultToSdoutOrTCP * _nowOBJ = NULL ;
    public :
        _sendResultToSdoutOrTCP( string ___outPath ) ;
        ~_sendResultToSdoutOrTCP();
        class _sendResultToSdoutOrTCP * _Obj() ;

} ; // class _sendResultToSdoutOrTCP


#endif //     __SENDRESULTTOSDOUTORTCP_H

