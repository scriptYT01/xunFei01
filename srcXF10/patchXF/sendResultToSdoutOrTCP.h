
#ifndef     __SENDRESULTTOSDOUTORTCP_H
#define     __SENDRESULTTOSDOUTORTCP_H

#include <string>
#include <assert.h>

using namespace std;


class _sendResultToSdoutOrTCP
{
    private :
        string _outDST ;
        class _sendResultToSdoutOrTCP * _nowOBJ = NULL ;
    public :
        _sendResultToSdoutOrTCP( string ___outDST ) ;
        ~_sendResultToSdoutOrTCP();
        class _sendResultToSdoutOrTCP * _Obj() ;

} ; // class _sendResultToSdoutOrTCP


#endif //     __SENDRESULTTOSDOUTORTCP_H

