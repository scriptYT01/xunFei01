#include "ymList01.h"

static int _verbose_flag;
int     _ymDbgMode = _ymMode_all ; 
static char *  _ymDbgModeStr = NULL ;
char *  _ymDbgLogFname = "/tmp/log.ymDbg.txt" ;

int _ymDbsearchListItem( _STitemX * ___list , int ___size , char * ___wantWAVfname ) 
{
    int __i01 ;
    if ( NULL == ___list || NULL == ___wantWAVfname ) {
        return -1;
    }

    for ( __i01 = 0 ; __i01 < ___size ; __i01 ++ ) {
        if ( 0 == strncmp( ___wantWAVfname , ___list[__i01] . _fname , 99 )) {
            return __i01 ; 
        }
    }

    return -2 ;
} /* _ymDbsearchListItem */

static _STitemX _listP3single[3] ;
void _listUse_3_play_single()
{
    int __i01 ;

    if ( 0 == strncmp( "p" , _ymDbgModeStr , 2 ) ) {
        _showPlaySingle();
    }

    __i01 = _ymDbsearchListItem( _listA1 , _listA1_objSize - 2 , _ymDbgModeStr ) ;
    if ( __i01 < 0 ) {
        _showPlaySingle();
    }

    _listP3single[0] = _listA1[ __i01 ] ;
    _listP3single[1] = _listA1[ _listA1_objSize - 2 ] ;
    _listP3single[2] = _listA1[ _listA1_objSize - 1 ] ;

    _listAA             = _listP3single ;
    _listAA_BSize       = sizeof( _listP3single )  ;
} /* _listUse_3_play_single */

void _listUse_4_play_list()
{
    int         __i01 ;
    int         __i02 ;
    int         __i03 ;
    int         __i04 ;
    char        __buf100[100] ;
    int         __gSize = -1 ;
    _STlistGX * __listG ;
    char     ** __str2 ;

    if ( 0 == strncmp( "l" , _ymDbgModeStr , 2 ) ) {
        _showPlayListG();
    }

    __i01 = atoi( _ymDbgModeStr ) ;
    __buf100[0] = 0 ;
    snprintf( __buf100, 99 , "%d" , __i01 ) ;
    if ( 0 != strncmp( __buf100 , _ymDbgModeStr , 99 ) ) {
        _showPlayListG();
    }

    if ( __i01 >= _listGG_objSize ) {
        _showPlayListG();
    }

    __listG = _listGG + __i01 ;
    if ( NULL == __listG ) {
            _prOOn( "Group error 11 =%s %d: " , basename((char*)__FILE__), __LINE__ ) ; exit( 11 ) ;
    }
    __str2 = (char**) __listG -> _listG_item  ;
    if ( NULL == __str2 ) {
            _prOOn( "Group error 22 =%s %d: " , basename((char*)__FILE__), __LINE__ ) ; exit( 11 ) ;
    }

    __i02 = 0 ;
    while ( 1 ) {
        if ( __i02 > 100 ) {
            _prOOn( "Group error 44 =%s %d: " , basename((char*)__FILE__), __LINE__ ) ; exit( 11 ) ;
        }
        if ( __str2[__i02] == NULL ) {
            __gSize = __i02 ;
            break ;
        }
        __i02 ++ ;
    }
    if ( __gSize != __listG -> _listG_objSize ) {
        _prOOn( "Group str amount %d != %d " , __gSize , __listG -> _listG_objSize ) ; exit( 91 ) ;
    }

    if ( NULL == _listBB ) { 
        _listBB  = malloc( _listA1_BSize ) ; 
        if(0) _prOOn( " 11111 81 : %p , %p , %p , size %d " , _listAA , _listAA , _listBB , _listA1_BSize ) ;
    }

    for ( __i03 = 0 ; __i03 < __gSize ; __i03 ++ ) {
        __i04 = _ymDbsearchListItem( _listA1 , _listA1_objSize - 2 , __str2[__i03] ) ;
        if ( __i04 < 0 ) {
            _prOOn( "error str ? %d %d :" , __i03 , __i04 );
            _prOOn( "error str ? %s " , __str2[__i03] ) ;
            exit( 82 ) ;
        }

        //_listBB[__i03] = _listA1[ __i03 ] ;
        _listBB[__i03] = _listA1[ __i04 ] ;
        if(0) _prOO( "%s %p \n" , _listA1[ __i03 ] . _fname , &(_listA1[ __i03 ]) ) ;
        if(0) _prOO( "%s %p \n" , _listBB[ __i03 ] . _fname , &(_listBB[ __i03 ]) ) ;
    }
    _listBB[__gSize]      = _listA1[ _listA1_objSize - 2 ] ;
    _listBB[__gSize+1]    = _listA1[ _listA1_objSize - 1 ] ;

    _listAA             = _listBB ;
    _listAA_BSize       = (sizeof( _STitemX )) * ( __gSize + 2 )  ;
    
    if(0) _prOOn( " 11111 91 : %p , %d , %d " , _listAA , _listAA_BSize , __gSize ) ;
} /* _listUse_4_play_list */

void _listUse_1_all()
{
    _listAA             = _listA1 ;
    _listAA_BSize       = _listA1_BSize ;
} /* _listUse_1_all */

void _listUse_2_debuging()
{
    /* though listen only state , must give a fake list */
    _listUse_1_all();
} /* _listUse_2_debuging */

void _paraAnalyzeYmDbg( int ___argc, char ** ___argv)
{

    int __i01 ;

    static struct option __long_options[] =
    {
        /* These options set a flag. */
        {"verbose", no_argument,       &_verbose_flag, 1},
        {"brief",   no_argument,       &_verbose_flag, 0},
        /* These options don’t set a flag.
         * We distinguish them by their indices. */
        {"help",     no_argument,       0, 'h'},
        {"debug",    no_argument,       0, 'd'},
        {"version",  no_argument,       0, 'V'},
        {"play",    required_argument,  0, 'p'},
        {"list",    required_argument,  0, 'l'},
        {"logfile", required_argument,  0, 'L'},
        {0, 0, 0, 0}
    };

    while (1)
    {
        /* getopt_long stores the option index here. */
        int __option_index = 0;

        __i01  = getopt_long (___argc, ___argv, "dhp:l:VL:",
                __long_options, &__option_index);

        /* Detect the end of the options. */
        if (__i01  == -1)
            break;

        switch (__i01 )
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (__long_options[__option_index].flag != 0)
                    break;
                printf ("option %s", __long_options[__option_index].name);
                if (optarg)
                    printf (" file %s , line %d : with arg %s", __FILE__ , __LINE__ , optarg);
                printf ("\n");
                break;

            case 'h':
                puts ("option -h\n");
                _showUsageExit();
                break;

            case 'V':
                puts ("option -V\n");
                _showVersionExit( 1 );
                break;

            case 'L':
                _ymDbgLogFname = strdup( optarg ) ;
                printf ("option -L ( logfile ) with value `%s'\n", optarg);
                break;

            case 'p':
                _ymDbgMode = _ymDbgMode_play_single ;
                _ymDbgModeStr = strdup( optarg ) ;
                printf ("option -p ( play ) with value `%s'\n", optarg);
                break;
            case 'l':
                _ymDbgMode = _ymDbgMode_play_list ;
                _ymDbgModeStr = strdup( optarg ) ;
                printf ("option -l ( playList ) with value `%s'\n", optarg);
                break;
            case 'd':
                printf ("option -d ( debuging , listen only ) \n" ) ;
                _ymDbgMode = _ymDbgMode_debuging ;
                break;

            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                abort ();
        }
    }

    /* Instead of reporting ‘--verbose’
     * and ‘--brief’ as they are encountered,
     * we report the final status resulting from them. */
    if (_verbose_flag)
        puts ("verbose flag is set");

    /* Print any remaining command line arguments (not options). */
    if (optind < ___argc)
    {
        printf ("non-option ARGV-elements: ");
        while (optind < ___argc)
            printf ("%s ", ___argv[optind++]);
        putchar ('\n');
        if(1) exit(44);
    }

    switch ( _ymDbgMode ) {
        case _ymMode_all:
            _listUse_1_all();
            return ;
            break ;
        case _ymDbgMode_debuging :
            _listUse_2_debuging();
            return ;
            break ;
        case _ymDbgMode_play_single :
            _listUse_3_play_single();
            return ;
            break ;
        case _ymDbgMode_play_list :
            _listUse_4_play_list();
            return ;
            break ;
        default:
            _prOOn( "\n\n unknow mode %d \n" , _ymDbgMode ) ;
            exit( 99 ) ;
            break ;
    }
    exit( 88 ) ;

} /* _paraAnalyzeYmDbg */
