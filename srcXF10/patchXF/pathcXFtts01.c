#include "patchXF/pathcXFbase.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"

#define _outPCM( len , buf ) fwrite( buf , len , 1 , stdout ) 
#define _outPCMzero2( len ) { \
    char __zeroP[2] = { 0  , 1  } ; \
    char __zeroN[2] = { 0  , 0  } ; \
    int __ii ; \
    int __cnt01=0 ; \
    for ( __ii = (len>>1) ; __ii > 0 ; __ii -- ) { \
        if ( __ii & 1 ) { fwrite( &__zeroP , 2 , 1 , stdout ) ; } \
        else            { fwrite( &__zeroN , 2 , 1 , stdout ) ; } \
        __cnt01 ++ ; \
    } ; \
	if ( 1 ) {_prEFn( "cnt(%d)" , __cnt01 ); } \
}
#define _pcmByte_1s     (16000*2)
#define _pcmByte_1ms    (_pcmByte_1s/1000)
#define _pcmByte_4ms    (_pcmByte_1s/250)
#define _pcmByte_10ms   (_pcmByte_1s/100)
#define _pcmByte_100ms  (_pcmByte_1s/10)
#define _pcmByte_200ms  (_pcmByte_1s/5)
#define _pcmBit_1s      (16000*16)
#define _pcmBit_10ms    (_pcmBit_1s/100)
//#define _pcmByteAlign   _pcmByte_200ms
//#define _pcmByteAlign   _pcmByte_10ms
//#define _pcmByteAlign   _pcmByte_1ms
#define _pcmByteAlign   _pcmByte_4ms

static int text_to_speech_from_file_continuE_loop( int ___len , const char * ___src_text , const char* params)
{
	int             __ret           = -1;
	const char*     __sessionID     = NULL;
	unsigned int    __audio_len     = 0;
	//wave_pcm_hdr  __wav_hdr       = default_wav_hdr;
	int             __synth_status  = MSP_TTS_FLAG_STILL_HAVE_DATA;
	const void*     __data ;
	unsigned int    __totalLEN      = 0;
	unsigned int    __patchZeroLen  = 0;



	/* 开始合成 */
	__sessionID = QTTSSessionBegin(params, &__ret);
	if (MSP_SUCCESS != __ret)
	{
		_prEFn("QTTSSessionBegin failed, error code: %d.", __ret);
		return __ret;
	}

	__ret = QTTSTextPut(__sessionID, ___src_text, (unsigned int)strlen(___src_text), NULL);
	if (MSP_SUCCESS != __ret)
	{
		_prEFn("QTTSTextPut failed, error code: %d.",__ret);
		QTTSSessionEnd(__sessionID, "TextPutError");
		return __ret;
	}

#if 0
	{ _prEFn("正在合成 ...");       }
#else
    { _prEFn("正在合成 ...(%d)<%s>" , ___len , ___src_text );   }
#endif

#if 1
    _outPCMzero2( _pcmByteAlign ) ;
#endif

	while (1) 
	{
		/* 获取合成音频 */
		__data = QTTSAudioGet(__sessionID, &__audio_len, &__synth_status, &__ret);
		if (MSP_SUCCESS != __ret) {
			break;
        }
		if (NULL != __data)
		{
			//fwrite(__data, __audio_len, 1, fp);
		    //__wav_hdr.data_size += __audio_len; //计算data_size大小
            _outPCM( __audio_len , __data ) ;
            __totalLEN += __audio_len ;
		}
		if (MSP_TTS_FLAG_DATA_END == __synth_status) {
			break;
        }
#if 0
		_prEFn(">");
		usleep(150*1000); //防止频繁占用CPU
#else
		//usleep(1*1000); //防止频繁占用CPU
		//usleep(10*1000); //防止频繁占用CPU
		//usleep(20*1000); //防止频繁占用CPU
		//usleep(30*1000); //防止频繁占用CPU
		//usleep(50*1000); //防止频繁占用CPU
		//usleep(100*1000); //防止频繁占用CPU
		usleep(150*1000); //防止频繁占用CPU
#endif
	}

    __patchZeroLen = __totalLEN % _pcmByteAlign ;
    __patchZeroLen = _pcmByteAlign - __patchZeroLen ;
    __patchZeroLen += _pcmByteAlign ;

    _outPCMzero2( __patchZeroLen ) ;

	_prEFn( "END(%d)(%d)(%d)" , ___len , __totalLEN , __patchZeroLen );

	if (MSP_SUCCESS != __ret)
	{
		_prEFn(         "QTTSAudioGet failed, error code: %d." ,    __ret);
		QTTSSessionEnd( __sessionID, "AudioGetError"                    );

		return __ret;
	}

	/* 合成完毕 */
	__ret = QTTSSessionEnd(     __sessionID ,   "Normal"   );
	if (MSP_SUCCESS != __ret)
	{
		_prEFn(     "QTTSSessionEnd failed, error code: %d." ,  __ret );
	}

	return __ret;
} // text_to_speech_from_file_continuE_loop

//int text_to_speech_from_file_continue(const char* ___src_text, const char* des_path, const char* params)
int text_to_speech_from_file_continue(FILE * ___fd , const char* params)
{
	int             __ret          = -1;
    char *          __src_text     ;
    size_t          __len          ;
    ssize_t         __size         ;

    if ( NULL == ___fd ) {
		_prEFn(" input source ERROR. " );
        return -1 ;
    }

    while ( 1 ) {
        __src_text      = NULL ;
        __len           = 0 ;
        __size          = getline( & __src_text , & __len , ___fd ) ;

	    if ( __size < 0 ) {
		    _prEFn(" input source read error :  %s " , strerror( errno ) );
            if ( NULL != __src_text ) { free( __src_text ) ; }
		    if ( ___fd ) { fclose( ___fd); }
            return __ret ;
        }

		_prEFn(" input source len %d , size %d" , __len , __size );

        if ( 0 == strncmp( __src_text , "\n" , sizeof( "\n" ) ) ) {
		    _prEFn(" skip NULL line " ) ;
            continue ;
        }

	    __ret = 
            //text_to_speech_from_file_continuE_loop( __len , __src_text , params ) ;
            text_to_speech_from_file_continuE_loop( __size , __src_text , params ) ;

	    if ( MSP_SUCCESS != __ret ) {
            if ( NULL != __src_text ) { free( __src_text ) ; }
		    if ( ___fd ) { fclose( ___fd); }
            return __ret ;
        }
    }
} // text_to_speech_from_file_continue
