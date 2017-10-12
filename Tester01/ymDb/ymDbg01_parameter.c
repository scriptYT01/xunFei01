
#include "ymList01.h"

static int _verbose_flag;

void _showUsageExit()
{
    _P1n( "\n\n\n" 
            "Usage : "                                                                                        "\n"
            " %s [<-d> | <-h> | <-p XX> | <-l YY> ]"                                                          "\n"
            "     -h , --help   : show debug info / usage only. "                                             "\n"

            "     -d , --debug  : listen only , waiting the use play the awake / words "                      "\n"
            "     -p , --play   : awake , then play the specified word item only."                            "\n"
            "     -l , --list   : awake , then play the specified word list. "                                "\n"

            "     no parameter  : play the default all word items , use about 10 - 15 minuts. "               "\n"
            "\n"
            , _argV[0] 
        );
    exit(33) ;
} /* _showUsageExit */

void _listUse_1_all()
{
    _listAA             = _listA1 ;
    _listAAbyteSize     = _listA1_BSize ;
    _itemSize           = sizeof( _STitemX ) ;
} /* _listUse_1_all */

int     _ymDbgMode = _ymMode_all ; 
char *  _ymDbgModeStr = NULL ;
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
            break ;
        case _ymDbgMode_debuging :
            break ;
        case _ymDbgMode_play_single :
            break ;
        case _ymDbgMode_play_list :
            break ;
        default:
            _P1n( "\n\n unknow mode %d \n" , _ymDbgMode ) ;
            exit( 88 ) ;
            break ;
    }

} /* _paraAnalyzeYmDbg */
