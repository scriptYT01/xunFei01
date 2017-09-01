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

static int text_to_speech_from_file_continuE_loop( int ___len , const char * ___src_text , const char* params)
{
	int             __ret           = -1;
	const char*     __sessionID     = NULL;
	unsigned int    __audio_len     = 0;
	//wave_pcm_hdr  __wav_hdr       = default_wav_hdr;
	int             __synth_status  = MSP_TTS_FLAG_STILL_HAVE_DATA;
	const void*     __data ;
	unsigned int    __totalLEN      = 0;


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
		//_prEFn(">");
		//usleep(150*1000); //防止频繁占用CPU
		usleep(1*1000); //防止频繁占用CPU
	}
	_prEFn( "END(%d)(%d)" , ___len , __totalLEN );

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
    ssize_t         __ilen         ;

    if ( NULL == ___fd ) {
		_prEFn(" input source ERROR. " );
        return -1 ;
    }

    while ( 1 ) {
        __src_text      = NULL ;
        __len           = 0 ;
        __ilen          = getline( & __src_text , & __len , ___fd ) ;

	    if ( __ilen < 0 ) {
		    _prEFn(" input source read error :  %s " , strerror( errno ) );
            if ( NULL != __src_text ) { free( __src_text ) ; }
		    if ( ___fd ) { fclose( ___fd); }
            return __ret ;
        }

	    __ret = 
            text_to_speech_from_file_continuE_loop( __len , __src_text , params ) ;

	    if ( MSP_SUCCESS != __ret ) {
            if ( NULL != __src_text ) { free( __src_text ) ; }
		    if ( ___fd ) { fclose( ___fd); }
            return __ret ;
        }
    }
} // text_to_speech_from_file_continue
