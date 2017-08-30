
#ifndef     __SENDRESULTTOSDOUTORTCP_H
#define     __SENDRESULTTOSDOUTORTCP_H

#include <string>

using namespace std;


class _sendResultToSdoutOrTCP
{
    private :
        string _outDST ;
        class _sendResultToSdoutOrTCP * _nowOBJ = NULL ;
    public :
        _sendResultToSdoutOrTCP( string ___outDST ) ;
        ~_sendResultToSdoutOrTCP();
        class _sendResultToSdoutOrTCP * _getOBJ() ;

} ; // class _sendResultToSdoutOrTCP


#endif //     __SENDRESULTTOSDOUTORTCP_H

