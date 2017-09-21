
#include "base01.h"

// note : index start from 1 ,

void _paraAtoi( int _para_must1_ignore0 , int ___argc , char ** ___argv , int ___index , char * ___varName , int * ___varLP ) {
    char * ___str ;

    if ( ___varLP   == NULL  ) { _prExit( " var NULL " ); }
    if ( ___varName == NULL  ) { _prExit( " varName NULL " ); }
    
    if ( ___index < 1 || ___index >= ___argc ) { if ( _para_must1_ignore0 ) { _prExit( " para index error " ); } ; return ; }

    if ( 0 ) { _prSFn( " --- input  %d : %p : %d , %s , %p " , ___argc , ___argv , ___index , ___varName , ___varLP ) ; }

    ___str = ___argv[___index + 0];
    if ( 0 ) { _prSFn( " --- input  is %s " , ___str ) ; }
    if ( ___str == NULL  ) { _prExit( " para NULL " ); }


    (*___varLP) = atoi( ___str ) ;

    _prSFn( " === get para %d : %s : %d" , ___index , ___varName , *___varLP ) ;

} // _paraAtoi

void _paraAtoS( int _para_must1_ignore0 , int ___argc , char ** ___argv , int ___index , char * ___varName , char ** ___varLP ) {
    char * ___str ;

    if ( ___varLP   == NULL  ) { _prExit( " var NULL " ); }
    if ( ___varName == NULL  ) { _prExit( " varName NULL " ); }
    
    if ( ___index < 1 || ___index >= ___argc ) { if ( _para_must1_ignore0 ) { _prExit( " para index error " ); } ; return ; }

    if ( 0 ) { _prSFn( " --- input  %d : %p : %d , %s , %p " , ___argc , ___argv , ___index , ___varName , ___varLP ) ; }

    ___str = ___argv[___index + 0];
    if ( 0 ) { _prSFn( " --- input  is %s " , ___str ) ; }
    if ( ___str == NULL  ) { _prExit( " para NULL " ); }


    (*___varLP) = ___str ;

    _prSFn( " === get para %d : %s : %s" , ___index , ___varName , *___varLP ) ;

} // _paraAtoS

// 0 -> ok . other -> error , empty, or not equal. compare len of str1
int _strcmpX1( const char * ___preStr , const char * ___bufStr ) 
{ 
    if ( NULL == ___preStr || NULL == ___bufStr ) return -1 ;
    return strncmp( ___preStr , ___bufStr , strlen( ___preStr ) ) ; 
}

// 0 -> ok . other -> error , empty, or not equal. compare len of 4096
int _strcmpXX( const char * ___preStr , const char * ___bufStr ) 
{ 
    if ( NULL == ___preStr || NULL == ___bufStr ) return -1 ;
    return strncmp( ___preStr , ___bufStr , 4096 ) ; 
}

uint64_t _u64_now() {
    uint64_t __u64 = 0 ;
    struct timeval __t ;
    if ( 0 == gettimeofday( & __t , NULL ) ){
        __u64   =   __t . tv_sec ;
        __u64   *=  1000000 ;
        __u64   +=  __t . tv_usec ;
    } else {
        // zero 
    }

    return  __u64 ;
} /* _u64_now */

void _timeLoop( uint32_t ___delayUS , int (*___loopBackFunc)() ) {
    int __i03 ;
    uint64_t __u641 ;
    uint64_t __u642 ;
    uint64_t __u643 ;
    uint64_t __u644 ;
    int64_t __u1 ;
    int64_t __u2 ;
    int64_t __u3 ;
    int64_t __u4 ;
    int64_t __u5 ;
    int64_t __u6 = 0 ;
    int64_t __X1 ;

    __u641  =   _u64_now() ;
    __X1    =   ___delayUS ;
    __u642  =   __u641 - __X1   ;
    while ( 1 ) {
        __u643  =   __u642     ; 
        __u642  =   _u64_now() ;
        if ( (*___loopBackFunc)() <= 0 ) break ;
        if (0) _prEFn( " 1: %lld , 2: %lld , 3: %lld , 4: %lld , 5: %lld , 6: %lld , X1: %lld " 
                , __u1 , __u2 , __u3 , __u4 , __u5 , __u6 , __X1 );
        __u644  =   _u64_now() ;



        __u1  =   __u642 - __u643 ; // gap-time
        __u2  =   __u644 - __u642 ; // run-time
        if ( __u2 > __X1 ) {
            _prEFn( " delay over-run " ) ;
            break;
        }
        if ( __u1 > (__X1 * 2)) {
            _prEFn( " why gap over-run " ) ;
            break;
        }

        __u3 = __X1 - __u2 ; // sleep-time-base
        __u4 = __X1 - __u1 ; // jiffer
        __u5 = __u3 + __u4 ;

        if ( 0 == __u6 ) __u6 = __u1 ;
        __u6 *=   10000000 ;
        __u6 += __u1*10000 ;
        __u6 /=   10010000 ;

        __i03 = __u5 ;
        usleep( __i03 ) ;
    }
} /* _timeLoop */

