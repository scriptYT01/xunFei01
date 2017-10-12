#include "ymList01.h"

_STitemX _listA1[] = {
{ "01"  , "v down set_power 1" } , /* kai ji */
{ "02"  , "v down set_power 1" } ,
{ "03"  , "v down set_power 1" } ,
{ "04"  , "v down set_power 1" } ,
{ "05"  , "v down set_power 1" } ,

{ "06"  , "v down set_power 0" } , /* guan ji */
{ "07"  , "v down set_power 0" } ,
{ "08"  , "v down set_power 0" } ,
{ "09"  , "v down set_power 0" } ,
{ "10"  , "v down set_power 0" } ,

{ "11"  , "v down set_wind 1" } ,
{ "12"  , "v down set_wind 1" } ,
{ "13"  , "v down set_wind 1" } ,
{ "14"  , "v down set_wind 1" } ,

{ "15"  , "v down set_wind 16" } ,
{ "17"  , "v down set_wind 16" } ,
{ "19"  , "v down set_wind 16" } ,
{ "20"  , "v down set_wind 4" } ,
{ "21"  , "v down set_wind 4" } ,
{ "22"  , "v down set_wind 4" } ,
{ "23"  , "v down set_wind 4" } ,
{ "24"  , "v down set_wind 4" } ,
{ "25"  , "v down set_light 1" } ,

{ "26"  , "v down set_wind 16" } ,
{ "27"  , "v down set_light 1" } ,
{ "28"  , "v down set_light 1" } ,
{ "29"  , "v down set_light 1" } ,
{ "30"  , "v down set_light 0" } ,

{ "31"  , "v down set_light 0" } ,
{ "32"  , "v down set_light 0" } ,
{ "33"  , "v down set_light 0" } ,
{ "34"  , "v down set_light 0" } ,
{ "38"  , "v down timer_setting 10" } ,
{ "39"  , "v down timer_setting 15" } ,

{ "40"  , "v down timer_setting 20" } ,
{ "41"  , "v down timer_setting 30" } ,
{ "42"  , "v down timer_setting 40" } ,

{ "43"  , "v down timer_setting 60" } ,
{ "44"  , "v down timer_setting 30" } ,
{ "45"  , "v down timer_setting 30" } ,

{ "46"  , "v down timer_setting 60" } ,
{ "47"  , "v down timer_setting 60" } ,
{ "48"  , "v down timer_setting 90" } ,

{ "49"  , "v down timer_setting 120" } ,
{ "50"  , "v down timer_setting 120" } ,
{ "51"  , "v down timer_setting 150" } ,

{ "52"  , "v down timer_setting 180" } ,
{ "53"  , "v down timer_setting 180" } ,
{ "54"  , "v down timer_setting 210" } ,

{ "55"  , "v down timer_setting 10" } ,
{ "56"  , "v down timer_setting 15" } ,
{ "57"  , "v down timer_setting 20" } ,

{ "58"  , "v down timer_setting 30" } ,
{ "59"  , "v down timer_setting 40" } ,
{ "60"  , "v down timer_setting 60" } ,

{ "61"  , "v down timer_setting 30" } ,
{ "62"  , "v down timer_setting 30" } ,
{ "63"  , "v down timer_setting 60" } ,

{ "64"  , "v down timer_setting 60" } ,
{ "65"  , "v down timer_setting 90" } ,
{ "66"  , "v down timer_setting 120" } ,

{ "67"  , "v down timer_setting 120" } ,
{ "68"  , "v down timer_setting 150" } ,
{ "69"  , "v down timer_setting 180" } ,

{ "70"  , "v down timer_setting 180" } ,
{ "71"  , "v down timer_setting 210" } ,

{ "72"  , "v down current_time" } ,
{ "73"  , "v down current_time" } ,
{ "74"  , "v down current_time" } ,
{ "75"  , "v down current_time" } ,

{ "76"  , "v down current_time" } , /* ji_dian_le */
{ "77"  , "v down current_time" } ,

{ "78"  , "v down air_pollution" } ,
{ "79"  , "v down air_pollution" } ,

{ "80"  , "v down air_pollution" } ,
{ "81"  , "v down air_pollution" } ,

{ "88"  , "v down active" } ,
{ "89"  , "v down active" } 
}; /*  _listA1 */

int     _listA1_BSize = sizeof( _listA1 ) ; // the total size in byte
int     _itemSize     = sizeof( _STitemX ) ;
int     _listA1_objSize = (sizeof( _listA1 )) / (sizeof( _STitemX )) ;
int     _listA1_awake1  = (sizeof( _listA1 )) / (sizeof( _STitemX )) - 2 ;


const char* _listG0[] = {
    "05" , "21" , "25" , "75" , "79" , "38" , "10 " , NULL 
} ;/* _listG0[] */

_STlistGX _listGG[]    = { 
    { _listG0 , "0" , 7 } 
} ;

int _listGG_objSize = 1 ;

