/*
@file
@brief a simple demo to recognize speech from microphone

@author		taozhang9
@date		2016/05/27
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "speech_recognizer.h"
#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"
#include "linuxrec.h"

//#include "base01.h"
#include "pathcXFtts01.h"

#define SR_DBGON 1
#if SR_DBGON == 1
#	define sr_dbg printf
#else
#	define sr_dbg
#endif

#define DEFAULT_SESSION_PARA \
	 "sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = utf8"

#define DEFAULT_FORMAT		\
{\
	WAVE_FORMAT_PCM,	\
	1,			\
	16000,			\
	32000,			\
	2,			\
	16,			\
	sizeof(WAVEFORMATEX)	\
}

/* internal state */
enum {
	_SR_STATE_INIT,
	_SR_STATE_STARTED
};


#define SR_MALLOC malloc
#define SR_MFREE  free
#define SR_MEMSET	memset


static void _MsSleep(size_t ms)
{
	usleep(ms*1000);
} // _MsSleep


static void _End_sr_on_error(struct speech_rec *sr, int errcode)
{
	if(sr->aud_src == SR_MIC)
		_stop_record(sr->recorder);
	
	if (sr->session_id) {
		if (sr->notif.on_speech_end)
			sr->notif.on_speech_end(errcode);

		QISRSessionEnd(sr->session_id, "err");
		sr->session_id = NULL;
	}
	sr->state = _SR_STATE_INIT;
} // _End_sr_on_error

static void _End_sr_on_vad(struct speech_rec *sr)
{
	int errcode;
	const char *rslt;

	if (sr->aud_src == SR_MIC)
		_stop_record(sr->recorder);	

	while(sr->rec_stat != MSP_REC_STATUS_COMPLETE ){
		rslt = QISRGetResult(sr->session_id, &sr->rec_stat, 0, &errcode);
		if (rslt && sr->notif.on_result)
			sr->notif.on_result(rslt, sr->rec_stat == MSP_REC_STATUS_COMPLETE ? 1 : 0);

		_MsSleep(100); /* for cpu occupy, should sleep here */
	}

	if (sr->session_id) {
		if (sr->notif.on_speech_end)
			sr->notif.on_speech_end(END_REASON_VAD_DETECT);
		QISRSessionEnd(sr->session_id, "VAD Normal");
		sr->session_id = NULL;
	}
	sr->state = _SR_STATE_INIT;
} // _End_sr_on_vad

/* the record call back */
static void _Iat_cb(char *data, unsigned long len, void *user_para)
{
	int errcode;
	struct speech_rec *sr;

	if(len == 0 || data == NULL)
		return;

	sr = (struct speech_rec *)user_para;

	if(sr == NULL || sr->ep_stat >= MSP_EP_AFTER_SPEECH)
		return;
	if (sr->state < _SR_STATE_STARTED)
		return; /* ignore the data if error/vad happened */
	
	errcode = sr_write_audio_data(sr, data, len);
	if (errcode) {
		_End_sr_on_error(sr, errcode);
		return;
	}
} // _Iat_cb

static char * _Skip_space(char *s)
{
	while (s && *s != ' ' && *s != '\0')
		s++;
	return s;
} // _Skip_space

static int _Update_format_from_sessionparam(const char * session_para, WAVEFORMATEX *wavefmt)
{
	char *s;
	if ((s = strstr(session_para, "sample_rate"))) {
		s = strstr(s, "=");
		if (s && *s) {
			s = _Skip_space(s);
			if (s && *s) {
                _prSFn( "\n" "--- reset : nBlockAlign * nSamplesPerSec == nAvgBytesPerSec == : " );
                _prOOn( "%d * %d == %d" , wavefmt->nBlockAlign , wavefmt->nSamplesPerSec , wavefmt->nAvgBytesPerSec );
				wavefmt->nSamplesPerSec = atoi(s);
				wavefmt->nAvgBytesPerSec = wavefmt->nBlockAlign * wavefmt->nSamplesPerSec;
                _prOOn( "%d * %d == %d" "\n", wavefmt->nBlockAlign , wavefmt->nSamplesPerSec , wavefmt->nAvgBytesPerSec );
			}
		}
		else
			return -1;
	}
	else {
		return -1;
	}

	return 0;
} // _Update_format_from_sessionparam

/* devid will be ignored if aud_src is not SR_MIC ; use get_default_dev_id
 * to use the default input device. Currently the device list function is
 * not provided yet. 
 */

int sr_init_ex(struct speech_rec * ___sr, const char * ___session_begin_params, 
			enum sr_audsrc aud_src, record_dev_id devid, 
				struct speech_rec_notifier * notify)
{
	int __errcode;
	size_t param_size;
	WAVEFORMATEX __wavfmt = DEFAULT_FORMAT;

    _dumpWAVEFORMATEX( "origin_wavfmt\n" , "\n" , __func__ , &__wavfmt ) ;

	if (aud_src == SR_MIC && _get_input_dev_num() == 0) { // sr_init_ex
		return -E_SR_NOACTIVEDEVICE;
	}
    _prSFn( " aud_src %d" , aud_src ) ;

	if (!___sr)
		return -E_SR_INVAL;

	if (___session_begin_params == NULL) {
		___session_begin_params = DEFAULT_SESSION_PARA;
	}
    if ( 0 ) { _prSFn( " ___session_begin_params <%s>" , ___session_begin_params ) ; }

	SR_MEMSET(___sr, 0, sizeof(struct speech_rec)); // sr_init_ex
	___sr->state = _SR_STATE_INIT;
	___sr->aud_src = aud_src;
	___sr->ep_stat = MSP_EP_LOOKING_FOR_SPEECH;
	___sr->rec_stat = MSP_REC_STATUS_SUCCESS;
	___sr->audio_status = MSP_AUDIO_SAMPLE_FIRST;

	param_size = strlen(___session_begin_params) + 1;
	___sr->session_begin_params = (char*)SR_MALLOC(param_size);
	if (___sr->session_begin_params == NULL) {
		sr_dbg("mem alloc failed\n");
		return -E_SR_NOMEM;
	}
    _prSFn( " --- set IAT parameter : \n" "%s" "\n" , ___session_begin_params ) ;
	strncpy(___sr->session_begin_params, ___session_begin_params, param_size); // sr_init_ex

	___sr->notif = *notify;
	
	if (aud_src == SR_MIC) {
        _prSFn(" _create_recorder " );
		__errcode = _create_recorder(&___sr->recorder, _Iat_cb, (void*)___sr);
		if (___sr->recorder == NULL || __errcode != 0) {
			_prSF("create recorder failed: %d\n", __errcode);
			__errcode = -E_SR_RECORDFAIL;
			goto fail;
		}

        _prSFn(" _Update_format_from_sessionparam " );
		_Update_format_from_sessionparam(___session_begin_params, &__wavfmt);
	
        _prSFn("_open_recorder : start " ); // _Open_recorder_internal
		__errcode = _open_recorder(___sr->recorder, devid, &__wavfmt);
        _prSFn("_open_recorder : end " );

		if (__errcode != 0) {
			_prSF("recorder open failed: %d\n", __errcode); // sr_init_ex
			__errcode = -E_SR_RECORDFAIL;
			goto fail;
		}
	}

    _prSFn(" end normal. " );
	return 0;

fail:
	if (___sr->recorder) {
		_destroy_recorder(___sr->recorder);
		___sr->recorder = NULL;
	}

	if (___sr->session_begin_params) {
		SR_MFREE(___sr->session_begin_params);
		___sr->session_begin_params = NULL;
	}
	SR_MEMSET(&___sr->notif, 0, sizeof(___sr->notif));

    _prSFn(" end ERROR : %d. " , __errcode);
	return __errcode;
} // sr_init_ex

/* use the default input device to capture the audio. see sr_init_ex */
int sr_init(struct speech_rec * sr, const char * ___session_begin_params, 
		enum sr_audsrc aud_src, struct speech_rec_notifier * notify)
{
	return sr_init_ex(sr, ___session_begin_params, aud_src, 
			_get_default_input_dev(), notify);
}

int sr_start_listening(struct speech_rec *sr)
{
	int ret;
	const char*		session_id = NULL;
	int				errcode = MSP_SUCCESS;

	if (sr->state >= _SR_STATE_STARTED) {
		sr_dbg("already STARTED.\n");
		return -E_SR_ALREADY;
	}

	session_id = QISRSessionBegin(NULL, sr->session_begin_params, &errcode); //听写不需要语法，第一个参数为NULL
	if (MSP_SUCCESS != errcode)
	{
		sr_dbg("\nQISRSessionBegin failed! error code:%d\n", errcode);
		return errcode;
	}
	sr->session_id = session_id;
	sr->ep_stat = MSP_EP_LOOKING_FOR_SPEECH;
	sr->rec_stat = MSP_REC_STATUS_SUCCESS;
	sr->audio_status = MSP_AUDIO_SAMPLE_FIRST;

	if (sr->aud_src == SR_MIC) {
		ret = _start_record(sr->recorder);
		if (ret != 0) {
			sr_dbg("start record failed: %d\n", ret);
			QISRSessionEnd(session_id, "start record fail");
			sr->session_id = NULL;
			return -E_SR_RECORDFAIL;
		}
	}

	sr->state = _SR_STATE_STARTED;

	if (sr->notif.on_speech_begin)
		sr->notif.on_speech_begin();

	return 0;
}

/* after _stop_record, there are still some data callbacks */
static void wait_for_rec_stop(struct recorder *rec, unsigned int timeout_ms)
{
	while (!is_record_stopped(rec)) {
		_MsSleep(1);
		if (timeout_ms != (unsigned int)-1)
			if (0 == timeout_ms--)
				break;
	}
}

int sr_stop_listening(struct speech_rec *sr)
{
	int ret = 0;
	const char * rslt = NULL;

	if (sr->state < _SR_STATE_STARTED) {
		sr_dbg("Not started or already stopped.\n");
		return 0;
	}

	if (sr->aud_src == SR_MIC) {
		ret = _stop_record(sr->recorder);
		if (ret != 0) {
			sr_dbg("Stop failed! \n");
			return -E_SR_RECORDFAIL;
		}
		wait_for_rec_stop(sr->recorder, (unsigned int)-1);
	}
	sr->state = _SR_STATE_INIT;
	ret = QISRAudioWrite(sr->session_id, NULL, 0, MSP_AUDIO_SAMPLE_LAST, &sr->ep_stat, &sr->rec_stat);
	if (ret != 0) {
		sr_dbg("write LAST_SAMPLE failed: %d\n", ret);
		QISRSessionEnd(sr->session_id, "write err");
		return ret;
	}

	while (sr->rec_stat != MSP_REC_STATUS_COMPLETE) {
		rslt = QISRGetResult(sr->session_id, &sr->rec_stat, 0, &ret);
		if (MSP_SUCCESS != ret)	{
			sr_dbg("\nQISRGetResult failed! error code: %d\n", ret);
			_End_sr_on_error(sr, ret);
			return ret;
		}
		if (NULL != rslt && sr->notif.on_result)
			sr->notif.on_result(rslt, sr->rec_stat == MSP_REC_STATUS_COMPLETE ? 1 : 0);
		_MsSleep(100);
	}

	QISRSessionEnd(sr->session_id, "normal");
	sr->session_id = NULL;
	return 0;
}

int sr_write_audio_data(struct speech_rec *sr, char *data, unsigned int len)
{
	const char *rslt = NULL;
	int ret = 0;
	if (!sr )
		return -E_SR_INVAL;
	if (!data || !len)
		return 0;

	ret = QISRAudioWrite(sr->session_id, data, len, sr->audio_status, &sr->ep_stat, &sr->rec_stat);
	if (ret) {
		_End_sr_on_error(sr, ret);
		return ret;
	}
	sr->audio_status = MSP_AUDIO_SAMPLE_CONTINUE; // sr_write_audio_data

	if (MSP_REC_STATUS_SUCCESS == sr->rec_stat) { //已经有部分听写结果
		rslt = QISRGetResult(sr->session_id, &sr->rec_stat, 0, &ret);
		if (MSP_SUCCESS != ret)	{
			sr_dbg("\nQISRGetResult failed! error code: %d\n", ret);
			_End_sr_on_error(sr, ret);
			return ret;
		}
		if (NULL != rslt && sr->notif.on_result)
			sr->notif.on_result(rslt, sr->rec_stat == MSP_REC_STATUS_COMPLETE ? 1 : 0);
	}

	if (MSP_EP_AFTER_SPEECH == sr->ep_stat)
		_End_sr_on_vad(sr);

	return 0;
} // sr_write_audio_data

void sr_uninit(struct speech_rec * sr)
{
	if (sr->recorder) {
		if(!is_record_stopped(sr->recorder))
			_stop_record(sr->recorder);
		_close_recorder(sr->recorder);
		_destroy_recorder(sr->recorder);
		sr->recorder = NULL;
	}

	if (sr->session_begin_params) {
		SR_MFREE(sr->session_begin_params);
		sr->session_begin_params = NULL;
	}
}
