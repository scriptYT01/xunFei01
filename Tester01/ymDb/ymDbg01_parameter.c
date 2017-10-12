
#include "ymList01.h"

static int _verbose_flag;
int     _ymDbgMode = _ymMode_all ; 
static char *  _ymDbgModeStr = NULL ;

static _STitemX _listP3single[3] ;
void _listUse_3_play_single()
{
    if ( 0 == strncmp( "p" , _ymDbgModeStr , 2 ) ) {
        _showPlaySingle();
    }

    _listP3single[1] = _listA1[ _testSize ] ;
    _listP3single[2] = _listA1[ _testSize + 1 ] ;

    _listAA             = _listP3single ;
    _listAA_BSize       = sizeof( _listP3single )  ;
} /* _listUse_3_play_single */

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
        {"play",  required_argument,    0, 'p'},
        {"list",  required_argument,    0, 'l'},
        {0, 0, 0, 0}
    };

    while (1)
    {
        /* getopt_long stores the option index here. */
        int __option_index = 0;

        __i01  = getopt_long (___argc, ___argv, "dhp:l:",
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
            break ;
        default:
            _P1n( "\n\n unknow mode %d \n" , _ymDbgMode ) ;
            exit( 99 ) ;
            break ;
    }
    exit( 88 ) ;

} /* _paraAnalyzeYmDbg */
