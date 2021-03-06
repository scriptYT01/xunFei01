
#ifndef     __SUPERSTREAM_H
#define     __SUPERSTREAM_H

#include "base01.h"

//#include <string>
//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <functional>


using namespace std;

enum _enErrAction {
    _enEreopen          ,   // tryReopen when err occurs.
    _enEignore          ,   // ignore when err occurs.
    _enEexit                // exit when err occurs.
}; /* _enErrAction */

enum _enSsDir {
    _enSsdUnknown           ,
    _enSsdOutIn             ,
    _enSsdOut               ,
    _enSsdIn                
}; /* _enSsDir */

enum _enSsType { /* stream type */
    _enSstUnknown           ,
    _enSstFileOut           ,
    _enSstFileIn            ,
    _enSstTcpConnectTo      ,
    _enSstTcpListen1        ,
    _enSstCin               ,
    _enSstCerr              ,
    _enSstCout                
}; /* _enSsType */

struct _superStreamInfoX {
    uint64_t                _tryCnt     ;
    uint64_t                _tryLen     ;
    uint64_t                _skipCnt    ;
    uint64_t                _skipLen    ;
    uint64_t                _succCnt    ;
    uint64_t                _succLen    ;
} ;
typedef struct _superStreamInfoX _superStreamInfo ; 

int                 S_valid_fd_or_errFD( int * ___fd ) ;
bool                S_fd_canWrite( int *___fd , int * ___retryCNT ) ;
bool                S_fd_canRead(  int *___fd , int * ___retryCNT ) ;
int                 S_setNonblocking(int ___fd) ;
bool                S_fd_valid1_invalid0_close( int * ___fd ) ;

class _TTcp {
    public :
        int                         _ttListenFD           ;
        int                         _ttClientFD     ;
    public :
                                    _TTcp() { _ttClientFD = -1000002 ; _ttListenFD = -1000001 ; _ttpath = NULL ; }
    public :
        bool                        _ttTryListen01(     const char * ___ttPath ) ;
        bool                        _ttTryConnect01( const char * ___ttPath ) ;
        bool                        _ttTryAcceptClient( );
    public :
        bool                        _ttAnalyzeL1(       const char * ___tcpPath ) ;
        bool                        _ttAnalyzeL3();
        bool                        _ttAnalyzeT1(       const char * ___tcpPath ) ;
        bool                        _ttAnalyzeT3();
        void                        _dumpSelf( const char * ___file , int ___lineno , const char * ___func ) ;
    public :
        bool                        _ttCanWrite1(  ) ;
        bool                        _ttCanRead1(   ) ;
        bool                        _ttCanWrite2(  ) ;
        bool                        _ttCanRead2(   ) ;
    private :
        int                         _ttBd           ;
        int                         _ttLd           ;
        struct sockaddr_in          _ttSaddr        ;
        char                      * _ttpath         ;
        char                      * _tthost         ;
        char                      * _ttport         ;
        int                         _ttplen         ;
        struct sockaddr_storage     _ttRemoteaddr   ;
        socklen_t                   _ttAddrlen      ;
        int                         _ttCntR ;
        int                         _ttCntW ;
        struct addrinfo             _ttTwantedInfo ;
        struct addrinfo           * _ttTdnsResultInfo ;
} ; /* _TTcp */

class _superStreamBase
{
    protected :
        _enErrAction                _ssErrAction    ;
        _enSsDir                    _ssDir          ;
        _enSsType                   _ssType         ;
        const char              *   _ssPath         ;
        const char              *   _ssComment      ;
        _superStreamBase        *   _ssOK           ;
        _TTcp                       _tTcp           ;
        _superStreamInfo            _ssInfoW        ; 
        _superStreamInfo            _ssInfoR        ; 
        int                         _ssBufAlignSize ;
        int                         _ssBufDeep      ;
        const char              *   _ssBufLP        ;
    public :
        int                         _ssFD           ;
                                    _superStreamBase(){ _memZS( _ssInfoW ) ; _memZS( _ssInfoR ) ; _ssBufLP = NULL ; } ;
                                    ~_superStreamBase( ){} ;
        static _superStreamBase *   _genSS( bool _exitIfErr ,  _enSsDir ___ssDir , const char * ___path , const char * ___comment ) ;

    protected :
        void                        _superStreamInit( _enSsType ___ssType , _enSsDir ___ssDir , const char * ___path , const char * ___comment ) ;
        int                     *   _getDataFD( ) ;
        int                     *   _getTcpListenFD( ) ;
        int                         _ssCntR ;
        int                         _ssCntW ;
    public :
        bool                        _canWrite(  bool ___reopen ) ;
        bool                        _canRead(   bool ___reopen ) ;
    public :
        bool                        _isOK() { return _ssOK?true:false ; } ;
        void                        _ssTryReopneIfNeeded( ) ;
        void                        _dumpSelf( const char * ___file , int ___lineno , const char * ___func ) ;
    public :
        void                        _ssSetErrAction( _enErrAction ___eAction ) ;
    public :
        void                        _ssBufSet(  int ___ssBufAlignSize    ,   int ___ssBufDeep ) ;
        int                         _ssBufR(    int ___len          ,   const char * ___buf ) ;
        int                         _ssBufW(    int ___len          ,         char * ___buf ) ;
    public :
        int                         _ssWriteNonblock(  int ___len , const char * ___buf ) ;
        int                         _ssWriteBlock(     int ___len , const char * ___buf ) ;
        int                         _ssReadNonblock(   int ___len ,       char * ___buf ) ;
        int                         _ssReadBlock(      int ___len ,       char * ___buf ) ;
    private :
        int                         _ssReadReal( int ___len , char * ___buf ) ;
    private :
        virtual bool                _ssOpenOrReopen() = 0 ;

}; /* class _superStreamBase */

#define ssOK( aa )  (( aa && aa -> _isOK() )?true:false)

class _ssCin : public _superStreamBase
{
    public : 
        _ssCin( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) ;
        ~_ssCin( ){} ;
    private :
        bool _ssOpenOrReopen() ;
} ; /* class _ssCin */

class _ssCout : public _superStreamBase
{
    public :
        _ssCout( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) ;
        ~_ssCout( ){} ;
    private :
        bool _ssOpenOrReopen() ;
} ; /* class _ssCout */

class _ssCerr : public _superStreamBase
{
    public :
        _ssCerr( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) ;
        ~_ssCerr( ){} ;
    private :
        bool _ssOpenOrReopen() ;
} ; /* class _ssCerr */

class _ssTcpConnectTo : public _superStreamBase
{
    public :
        _ssTcpConnectTo( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) ;
        ~_ssTcpConnectTo( ){} ;
    public :
        bool                        _canWrite(  bool ___reopen ) ;
        bool                        _canRead(   bool ___reopen ) ;
    private :
        bool _ssOpenOrReopen() ;
        bool _ssOpenTCPconnectToSock();
} ; /* class _ssTcpConnectTo */

class _ssListen1 : public _superStreamBase
{
    public :
        _ssListen1( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) ;
        ~_ssListen1( ){} ;
    public :
        bool                        _canWrite(  bool ___reopen ) ;
        bool                        _canRead(   bool ___reopen ) ;
    private :
        bool _ssOpenOrReopen() ;
        bool _ssOpenTCPListenServerPortAcceptSock() ;
} ; /* class _ssListen1 */

class _ssFileOut : public _superStreamBase
{
    public :
        _ssFileOut( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) ;
        ~_ssFileOut( ){} ;
    private :
        bool _ssOpenOrReopen() ;
} ; /* class _ssFileOut */

class _ssFileIn : public _superStreamBase
{
    public :
        _ssFileIn( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) ;
        ~_ssFileIn( ){} ;
    private :
        bool _ssOpenOrReopen() ;
} ; /* class _ssFileIn */


#define _dbSS1   _prEFn( " %d , %d , %s, %s" , _exitIfErr , ___ssDir , ___path , ___comment )
#define _dbSS2   _prEFn( " %d , %d , %s, %s" , _ssDir , _ssType , _ssPath , _ssComment )
#define dumpExit(ena)    if(ena) {dumpSelfX(); _prExit( "debuging." );}

#define _ssWriteNonblockStr( s1 ) _ssWriteNonblock( strlen(s1) , s1 ) 

extern int _recFD01 ;
#define debugFD01( aa )    if( (aa -> _ssFD) != _recFD01 ) { _prEFn( " before %d , after %d " , _recFD01 , (aa -> _ssFD) ) ; _recFD01 = (aa -> _ssFD) ; }

extern _ssFileIn   *   _SSin   ;
extern _ssFileOut  *   _SSout  ;
extern _ssFileOut  *   _SSerr  ;
extern _ssListen1  *   _SSexit ;

void         _paraAnalyzeSS( int ___argc , char ** ___argv , uint16_t ___exitPort ) ;
void        _readStdinAndTryHexDump( _superStreamBase * ___ss , int ___len , int ___hexLen ) ;


#endif //     __SENDRESULTTOSDOUTORTCP_H

