#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"

#define FRAME_LEN	640

//#define _prD( aa ) printf( "--debuging: %s %d %s : %d \n" , basename(__FILE__), __LINE__, __func__ , aa )
//#define _prDS( dd , ss ) printf( "--debuging: %s %d %s : %d <%s> \n" , basename(__FILE__), __LINE__, __func__ , dd , ss )
//#define _prS( ss ) printf( "--debuging: %s %d %s : <%s> \n" , basename(__FILE__), __LINE__, __func__ , ss )
//#define _prSF( fmt , ... ) printf( "--debuging: %s %d %s : " fmt "\n" , basename(__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
//#define _prSFn( fmt , ... ) _prSF( fmt "\n" ) 

#include <libgen.h>
#define _prSF(  fmt , ... ) printf( "--debuging: %s %d %s : " fmt  , basename(__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
#define _prSFn( fmt , ... ) _prSF( fmt "\n" , ## __VA_ARGS__ ) 

int main(int argc, char* argv[])
{

	const char *  __lgi_param     = "appid = 58f4654e";
	const char *  __ise_ssb_param = NULL;
	const char *  __sessionID     = NULL;
	long          __pcmSize       = 0;
	long          __pcmCount      = 0;
	long          __txtSize       = 0;
	FILE *        __f_pcm         = NULL;
	FILE *        __f_txt         = NULL;
	FILE *        __f_output      = NULL;
	char *        __pPCM          = NULL;
	char *        __text          = NULL;
	unsigned int  __rlstLen       = 0;
	int           __audStat       = MSP_AUDIO_SAMPLE_CONTINUE;
	int           __epStatus      = MSP_EP_LOOKING_FOR_SPEECH;
	int           __recStatus     = MSP_REC_STATUS_SUCCESS;
	int           __ret           = 0;
	enum _category
	{
		read_syllable_cn,
		read_word_cn,
		read_sentence_cn,
		read_word_en,
		read_sentence_en,
	}category;

	__ret = MSPLogin(NULL, NULL, __lgi_param);
	if (MSP_SUCCESS != __ret)
	{
		printf("MSPLogin failed error = %d\n",__ret);
	}

	category = read_sentence_cn ;
	switch(category)
	{
	case read_syllable_cn:
        _prD( category ) ;
		__ise_ssb_param = "sub=ise,category=read_syllable,language=zh_cn,aue=speex-wb;7,auf=audio/L16;rate=16000";
		//__f_txt = fopen("./ise_cn/cn_syll.txt", "rb+");
		__f_txt = fopen("./ise_cn/cn_syll2.txt", "rb+");//无拼音标注
		__f_pcm = fopen("./ise_cn/cn_syll.wav", "rb+");
		break;
	case read_word_cn:
        _prD( category ) ;
		__ise_ssb_param = "sub=ise,category=read_word,language=zh_cn,aue=speex-wb;7,auf=audio/L16;rate=16000";
		__f_txt = fopen("./ise_cn/cn_word.txt", "rb+");
		__f_pcm = fopen("./ise_cn/cn_word.wav", "rb+");
		break;
	case read_sentence_cn: /* ----------------*/
        _prD( category ) ;
		__ise_ssb_param = "sub=ise,category=read_sentence,language=zh_cn,aue=speex-wb;7,auf=audio/L16;rate=16000";
		__f_txt = fopen("./ise_cn/cn_sentence.txt", "rb+");
		__f_pcm = fopen("./ise_cn/cn_sentence.wav", "rb+");
		break;
	case read_word_en:
        _prD( category ) ;
		__ise_ssb_param = "sub=ise,category=read_word,language=en_us,aue=speex-wb;7,auf=audio/L16;rate=16000";
		__f_txt = fopen("./ise_en/en_word.txt", "rb+");
		__f_pcm = fopen("./ise_en/en_word.wav", "rb+");
		break;
	case read_sentence_en:
        _prD( category ) ;
		__ise_ssb_param = "sub=ise,category=read_sentence,language=en_us,aue=speex-wb;7,auf=audio/L16;rate=16000";
		__f_txt = fopen("./ise_en/en_sentence.txt", "rb+");
		__f_pcm = fopen("./ise_en/en_sentence.wav", "rb+");
		break;
	}
	__sessionID = QISESessionBegin(__ise_ssb_param, NULL, &__ret);
	if (MSP_SUCCESS != __ret)
	{
        _prD( __ret ) ;
		printf("QISESessionBegin failed error = %d\n",__ret);
	}
	if (NULL != __f_txt)
	{
		fseek(__f_txt, 0, SEEK_END);
		__txtSize = ftell(__f_txt);
		fseek(__f_txt, 0, SEEK_SET);
		__text = (char *)malloc(__txtSize);
		fread((void *)__text, __txtSize, 1, __f_txt);
		__text[__txtSize] = '\0';
		fclose(__f_txt);
		__f_txt = NULL;
	}
    _prDS( __sessionID , __text ) ;
	__ret = QISETextPut(__sessionID, __text, (unsigned int)strlen(__text), NULL);
	if (MSP_SUCCESS != __ret)
	{
		printf("QISETextPut failed error = %d\n",__ret);
	}
	if (NULL != __f_pcm) {
		fseek(__f_pcm, 0, SEEK_END);
		__pcmSize = ftell(__f_pcm);
		fseek(__f_pcm, 0, SEEK_SET);
		__pPCM = (char *)malloc(__pcmSize);
		fread((void *)__pPCM, __pcmSize, 1, __f_pcm);
		fclose(__f_pcm);
		__f_pcm = NULL;
	}
    _prSF("open to file :<%s>" , "output.txt" ) ;
	__f_output = fopen("output.txt","wb+");
	if (__f_output == NULL)
	{
		printf("open output.txt failed\n");
		goto ise_exit;
	}

	while (1) 
	{
		unsigned int len = 10 * FRAME_LEN;// 每次写入200ms音频(16k，16bit)：1帧音频20ms，10帧=200ms。16k采样率的16位音频，一帧的大小为640Byte
		int __ret;

		if (__pcmSize <= 2*len) 
		{
			len = __pcmSize;
		}
		if (len <= 0)
		{
			break;
		}
		__audStat = MSP_AUDIO_SAMPLE_CONTINUE;
		if (0 == __pcmCount)
		{
			__audStat = MSP_AUDIO_SAMPLE_FIRST;
		}
		printf(">");
		__ret = QISEAudioWrite(__sessionID, (const void *)&__pPCM[__pcmCount], len, __audStat, &__epStatus, &__recStatus);
		if (MSP_SUCCESS != __ret)
		{
			printf("QISEAudioWrite failed error = %d\n",__ret);
			goto ise_exit;
		}	

		__pcmCount += (long)len;
		__pcmSize -= (long)len;
		if (MSP_REC_STATUS_SUCCESS == __recStatus) 
		{
			const char *rslt = QISEGetResult(__sessionID, &__rlstLen,&__recStatus, &__ret);
			if (MSP_SUCCESS != __ret)
			{
				printf("QISEGetResult failed ,error = %d\n",__ret);
				goto ise_exit;
			}
			
			if (NULL != rslt)
			{
				printf("\nrslt = %s\n,__rlstLen =%u \n", rslt,__rlstLen);
				fwrite(rslt,__rlstLen,1,__f_output);
			}
			
		}
		if (MSP_EP_AFTER_SPEECH == __epStatus)
		{
			break;
		}
		usleep(200*1000);
	}
	__ret = QISEAudioWrite(__sessionID, (const void *)NULL, 0, 4, &__epStatus, &__recStatus);
	if (MSP_SUCCESS != __ret)
	{
		printf("QISEAudioWrite failed error = %d\n",__ret);
		goto ise_exit;
	}
		
	while (MSP_REC_STATUS_COMPLETE != __recStatus) 
	{
		const char *rslt = QISEGetResult(__sessionID, &__rlstLen,&__recStatus, &__ret);
		if (MSP_SUCCESS != __ret)
		{
			printf("QISEGetResult failed ,error = %d\n",__ret);
			goto ise_exit;
		}
		if (NULL != rslt)
		{
			printf("\nrslt = %s\n,__rlstLen =%u \n", rslt,__rlstLen);
			fwrite(rslt,__rlstLen,1,__f_output);
		}
		usleep(150*1000);
	}
	
	printf("=============================================================\n");
	printf("=============================================================\n");

ise_exit:
	if (NULL != __pPCM)
	{
		free(__pPCM);
		__pPCM = NULL;
	}
	if (__f_output!=NULL)
	{
		fclose(__f_output);
		__f_output = NULL;
	}
	__ret = QISESessionEnd(__sessionID, NULL);
	if (MSP_SUCCESS != __ret)
	{
		printf("QISESessionEnd failed error = %d\n",__ret);
	}
	__ret = MSPLogout();
	if (MSP_SUCCESS != __ret)
	{
		printf("MSPLogout failed error = %d\n",__ret);
	}
    if ( 0 ) {
	    printf("press any key to exit ...\n");
	    getchar();
    }
	return 0;
} // main
