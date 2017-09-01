#include "patchXF/pathcXFbase.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"

static int text_to_speech_from_file_continuE_loop( int ___len , const char * ___src_text , const char* params)
{
	int          __ret          = -1;
	const char*  __sessionID    = NULL;
	unsigned int __audio_len    = 0;
	//wave_pcm_hdr __wav_hdr      = default_wav_hdr;
	int          __synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;


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
		const void* data = QTTSAudioGet(__sessionID, &__audio_len, &__synth_status, &__ret);
		if (MSP_SUCCESS != __ret)
			break;
		if (NULL != data)
		{
			fwrite(data, __audio_len, 1, fp);
		    __wav_hdr.data_size += __audio_len; //计算data_size大小
		}
		if (MSP_TTS_FLAG_DATA_END == __synth_status)
			break;
		_prEFn(">");
		usleep(150*1000); //防止频繁占用CPU
	}
	_prEFn("");
	if (MSP_SUCCESS != __ret)
	{
		_prEFn("QTTSAudioGet failed, error code: %d.",__ret);
		QTTSSessionEnd(__sessionID, "AudioGetError");
		fclose(fp);
		return __ret;
	}
	/* 修正wav文件头数据的大小 */
	__wav_hdr.size_8 += __wav_hdr.data_size + (sizeof(__wav_hdr) - 8);
	
	/* 将修正过的数据写回文件头部,音频文件为wav格式 */
	fseek(fp, 4, 0);
	fwrite(&__wav_hdr.size_8,sizeof(__wav_hdr.size_8), 1, fp); //写入size_8的值
	fseek(fp, 40, 0); //将文件指针偏移到存储data_size值的位置
	fwrite(&__wav_hdr.data_size,sizeof(__wav_hdr.data_size), 1, fp); //写入data_size的值
	fclose(fp);
	fp = NULL;
	/* 合成完毕 */
	__ret = QTTSSessionEnd(__sessionID, "Normal");
	if (MSP_SUCCESS != __ret)
	{
		_prEFn("QTTSSessionEnd failed, error code: %d.",__ret);
	}

	return __ret;
} // text_to_speech_from_file_continuE_loop

//int text_to_speech_from_file_continue(const char* ___src_text, const char* des_path, const char* params)
int text_to_speech_from_file_continue(FILE * ___fd , const char* params)
	int             __ret          = -1;
    char *          __src_text     ;
    ssize_t         __len           ;

    if ( NULL == ___fd ) {
		_prEFn(" input source ERROR. " );
        return -1 ;
    }

    while ( 1 ) {
        __src_text      = NULL ;
        __len           = 0 ;
        __len           = getline( & __src_text , & __len , ___fd ) ;

	    if ( __len < 0 ) {
		    _prEFn(" input source read error :  %s " , strerror( errno ) );
		    if ( ___fd ) { fclose( ___fd); }
            return __ret ;
        }

	    __ret = 
            text_to_speech_from_file_continuE_loop( __len , __src_text , params ) ;

	    if ( MSP_SUCCESS != __ret ) {
		    if ( ___fd ) { fclose( ___fd); }
            return __ret ;
        }
    }
} // text_to_speech_from_file_continue
