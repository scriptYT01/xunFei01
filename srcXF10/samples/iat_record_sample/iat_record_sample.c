/*
* 语音听写(iFly Auto Transform)技术能够实时地将语音转换成对应的文字。
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"
#include "speech_recognizer.h"

#define FRAME_LEN	640 
#define	BUFFER_SIZE	4096

//#define _prSF( fmt , ... ) printf( "--debuging: %s %d %s : " fmt "\n" , basename(__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
char * _argv01 = NULL ;
char * _argv02 = NULL ;

/* Upload User words */
static int upload_userwords()
{
	char*			userwords	=	NULL;
	size_t			len			=	0;
	size_t			read_len	=	0;
	FILE*			fp			=	NULL;
	int				__ret			=	-1;

	fp = fopen("userwords.txt", "rb");
	if (NULL == fp)										
	{
		printf("\nopen [userwords.txt] failed! \n");
		goto upload_exit;
	}

	fseek(fp, 0, SEEK_END);
	len = ftell(fp); 
	fseek(fp, 0, SEEK_SET);  					
	
	userwords = (char*)malloc(len + 1);
	if (NULL == userwords)
	{
		printf("\nout of memory! \n");
		goto upload_exit;
	}

	read_len = fread((void*)userwords, 1, len, fp); 
	if (read_len != len)
	{
		printf("\nread [userwords.txt] failed!\n");
		goto upload_exit;
	}
	userwords[len] = '\0';
	
	MSPUploadData("userwords", userwords, len, "sub = uup, dtt = userword", &__ret); //ÉÏ´«ÓÃ»§´Ê±í
	if (MSP_SUCCESS != __ret)
	{
		printf("\nMSPUploadData failed ! errorCode: %d \n", __ret);
		goto upload_exit;
	}
	
upload_exit:
	if (NULL != fp)
	{
		fclose(fp);
		fp = NULL;
	}	
	if (NULL != userwords)
	{
		free(userwords);
		userwords = NULL;
	}
	
	return __ret;
}


static void show_result(char *string, char is_over)
{
	printf("\rResult: [ %s ]", string);
	if(is_over)
		putchar('\n');
}

static char *g_result = NULL;
static unsigned int g_buffersize = BUFFER_SIZE;

void on_result(const char *result, char is_last)
{
	if (result) {
		size_t left = g_buffersize - 1 - strlen(g_result);
		size_t size = strlen(result);
		if (left < size) {
			g_result = (char*)realloc(g_result, g_buffersize + BUFFER_SIZE);
			if (g_result)
				g_buffersize += BUFFER_SIZE;
			else {
				printf("mem alloc failed\n");
				return;
			}
		}
		strncat(g_result, result, size);
		show_result(g_result, is_last);
	}
}
void on_speech_begin()
{
	if (g_result)
	{
		free(g_result);
	}
	g_result = (char*)malloc(BUFFER_SIZE);
	g_buffersize = BUFFER_SIZE;
	memset(g_result, 0, g_buffersize);

	printf("Start Listening...\n");
}
void on_speech_end(int reason)
{
	if (reason == END_REASON_VAD_DETECT)
		printf("\nSpeaking done \n");
	else
		printf("\nRecognizer error %d\n", reason);
}

/* demo send audio data from a file */
static void demo_file(const char* audio_file, const char* session_begin_params)
{
	int	errcode = 0;
	FILE*	f_pcm = NULL;
	char*	p_pcm = NULL;
	unsigned long	pcm_count = 0;
	unsigned long	pcm_size = 0;
	unsigned long	read_size = 0;
	struct speech_rec iat;
	struct speech_rec_notifier recnotifier = {
		on_result,
		on_speech_begin,
		on_speech_end
	};

	if (NULL == audio_file)
		goto iat_exit;

	f_pcm = fopen(audio_file, "rb");
	if (NULL == f_pcm)
	{
		printf("\nopen [%s] failed! \n", audio_file);
		goto iat_exit;
	}

	fseek(f_pcm, 0, SEEK_END);
	pcm_size = ftell(f_pcm);
	fseek(f_pcm, 0, SEEK_SET);

	p_pcm = (char *)malloc(pcm_size);
	if (NULL == p_pcm)
	{
		printf("\nout of memory! \n");
		goto iat_exit;
	}

	read_size = fread((void *)p_pcm, 1, pcm_size, f_pcm);
	if (read_size != pcm_size)
	{
		printf("\nread [%s] error!\n", audio_file);
		goto iat_exit;
	}

	errcode = _sr_init(&iat, session_begin_params, SR_USER, &recnotifier);
	if (errcode) {
		printf("speech recognizer init failed : %d\n", errcode);
		goto iat_exit;
	}

	errcode = _sr_start_listening(&iat);
	if (errcode) {
		printf("\nsr_start_listening failed! error code:%d\n", errcode);
		goto iat_exit;
	}

	while (1)
	{
		unsigned int len = 10 * FRAME_LEN; /* 200ms audio */
		int __ret = 0;

		if (pcm_size < 2 * len)
			len = pcm_size;
		if (len <= 0)
			break;

		__ret = _sr_write_audio_data(&iat, &p_pcm[pcm_count], len);

		if (0 != __ret)
		{
			printf("\nwrite audio data failed! error code:%d\n", __ret);
			goto iat_exit;
		}

		pcm_count += (long)len;
		pcm_size -= (long)len;		
	}

	errcode = _sr_stop_listening(&iat);
	if (errcode) {
		printf("\n_sr_stop_listening failed! error code:%d \n", errcode);
		goto iat_exit;
	}

iat_exit:
	if (NULL != f_pcm)
	{
		fclose(f_pcm);
		f_pcm = NULL;
	}
	if (NULL != p_pcm)
	{
		free(p_pcm);
		p_pcm = NULL;
	}

	_sr_stop_listening(&iat);
	_sr_uninit(&iat);
}

/* demo recognize the audio from microphone */
static void _demo_mic(const char* session_begin_params)
{
	int errcode;
	int i = 0;

	struct speech_rec iat;

	struct speech_rec_notifier recnotifier = {// _demo_mic
		on_result,
		on_speech_begin,
		on_speech_end
	};

	errcode = _sr_init(&iat, session_begin_params, SR_MIC, &recnotifier);// _demo_mic
	if (errcode) {
		printf("speech recognizer init failed\n");
		return;
	}
	errcode = _sr_start_listening(&iat);
	if (errcode) {
		printf("start listen failed %d\n", errcode);// _demo_mic
	}
	/* demo 15 seconds recording */
	while(i++ < 15)
		sleep(1);
	errcode = _sr_stop_listening(&iat);// _demo_mic
	if (errcode) {
		printf("stop listening failed %d\n", errcode);
	}

	_sr_uninit(&iat);
} // _demo_mic


/* main thread: start/stop record ; query the result of recgonization.
 * record thread: record callback(data write)
 * helper thread: ui(keystroke detection)
 */
int main(int argc, char* argv[])
{
	int __ret = MSP_SUCCESS;
	int __upload_on =	0; /* whether upload the user word */
	/* login params, please do keep the appid correct */
	const char* login_params = "appid = 58f4654e, work_dir = .";
	int __aud_src_0file_1mic = 1; /* from mic or file */

    if ( argc >= 2 ) { _argv01 = argv[1] ; }
    if ( argc >= 3 ) { _argv02 = argv[2] ; }

	/*
	* See "iFlytek MSC Reference Manual"
	*/
	const char* session_begin_params = // main
		"sub = iat, domain = iat, language = zh_cn, "
		"accent = mandarin, sample_rate = 16000, "
		"result_type = plain, result_encoding = utf8";

	/* Login first. the 1st arg is username, the 2nd arg is password
	 * just set them as NULL. the 3rd arg is login paramertes 
	 * */
	__ret = MSPLogin(NULL, NULL, login_params); // main
	if (MSP_SUCCESS != __ret)	{
		printf("MSPLogin failed , Error code %d.\n",__ret);
		goto exit; // login fail, exit the program
	}

	printf("Want to upload the user words ? \n0: No.\n1: Yes\n"); // main
    if ( 0 ) {
	    scanf("%d", &__upload_on);
    }
	if (__upload_on)
	{
		printf("Uploading the user words ...\n"); // main
		__ret = upload_userwords();
		if (MSP_SUCCESS != __ret)
			goto exit;	
		printf("Uploaded successfully\n"); // main
	}

	printf("Where the audio comes from?\n"
			"0: From a audio file.\n1: From microphone.\n"); // main
    if ( 0 ) {
	    scanf("%d", &__aud_src_0file_1mic);
    } 
	if(__aud_src_0file_1mic != 0) {
		printf("Demo recognizing the speech from microphone\n"); // main
		printf("Speak in 15 seconds\n");

		_demo_mic(session_begin_params); // main

		printf("15 sec passed\n");
	} else {
		printf("Demo recgonizing the speech from a recorded audio file\n"); // main
		demo_file("wav/iflytek02.wav", session_begin_params); 
	}
exit:
	MSPLogout(); // Logout...

	return 0;
} // main
