#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <getopt.h>
#include <libgen.h>


typedef struct { 
    const char * _fname ;
    const char * _wanted ;
    const char * _pinYin ;
} _STitemX ;

typedef struct { 
    const char   ** _listG_item      ;
    const char    * _listG_name     ;
    int             _listG_objSize  ;
} _STlistGX ;

#define _strX( aa ) # aa
#define _write01( aa )      write( _fd_ttyS1 , aa , strlen(aa) ) 
#define _P1( fmt , ... )    fprintf( stdout , fmt , ## __VA_ARGS__ )
#define _P1n( fmt , ... )   _P1( fmt "\n\n" , ## __VA_ARGS__ )
#define _P1d( ddd )         _P1( "%d" , ddd )
#define _P1D(  ddd )         _P1( _strX(ddd) " %d" , ddd )
#define _P1Dn( ddd )         _P1( _strX(ddd) " %d\n" , ddd )
#define _P2( fmt , ... )    fprintf( stderr , fmt , ## __VA_ARGS__ )
#define _P2n( fmt , ... )   _P1( fmt "\n" , ## __VA_ARGS__ )

#define _Pmsg1() _P1n ( "\n" \
        "line %d , time %d " \
        " : <%d> 0x%02x , <%c>" \
        " get <%s>" \
        "     [%s]" \
        " wanted <%s>" \
        , __LINE__ , _time1 \
        , _buf1020[0] , _buf1020[0] , _buf1020[0] , _buf1020 \
        , _listAA[_itemNO] . _fname  \
        , _listAA[_itemNO] . _wanted  \
        );
#define _Pmsg2( bbb ) _P1n ( "\n" \
        "############## " bbb "1 ##############" \
        " seq1 %d,%d, rec1 %d,%d " \
        " get <%s> : %d " \
        "\n" \
        \
        "############## " bbb "2 ##############" \
        " itemNO %d , ok %d , not-recognized %d , mistake-recognized %d " \
        " , play awake wav %d , play sentence %d " \
        , _seq1 , _seq2 , _rec1 , _rec2 \
        , _buf1020 , _time2 \
        \
        , (_itemNO + 1) , _okCNT , _ngCNT , _diCNT \
        , _plCNT0   \
        , _plCNT1   \
        );
#define _diff1() ( 0 != strncmp( _buf1020 , _listAA[_itemNO] . _wanted , 99 ) ) 
#define _Pmsg3() \
    _P1n ( "\n" \
            "diff3 : %d :" \
            "         [%s]" \
            "    get  <%s>" \
            "    want <%s>" \
            , _itemNO \
            , _listAA[_itemNO] . _fname  \
            , _buf1020 \
            , _listAA[_itemNO] . _wanted  \
            )

#define _Pa0()   {_P1("\n") ; fflush( stdout ) ;}
#define _Pa1()   {_P1(".") ; fflush( stdout ) ;}
#define _Pa2()   {_P1("=") ; fflush( stdout ) ;}

int _setTTY_ymDB01(int fd, int speed);
void _printTTYinfo_ymDB01(char *portname, int speed);
void _paraAnalyzeYmDbg( int ___argc, char ** ___argv);

void _showPlayNow()    ;
void _showPlaySingle() ;
void _showPlayListG()  ;
void _showUsageExit()  ;

extern _STitemX   * _listAA             ;
extern int          _listAA_BSize       ; // the total size in byte
extern _STitemX   * _listBB             ;
extern int          _listBB_objSize     ;

extern _STlistGX    _listGG[]           ;
extern int          _listGG_objSize     ;

extern _STitemX     _listA1[]           ;
extern int          _listA1_BSize       ;
extern int          _listA1_objSize     ;
extern int          _listA1_awake1      ;

extern int          _itemSize           ;
extern int          _testSize           ; // the testing word amount.

extern int          _ymDbgMode          ;
enum _ymDbgModeXX {
    _ymMode_all
        , _ymDbgMode_debuging
        , _ymDbgMode_play_single
        , _ymDbgMode_play_list
} ;

extern int         _argC ;
extern char    **  _argV ;

