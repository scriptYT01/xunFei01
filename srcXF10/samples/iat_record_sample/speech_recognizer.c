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
	SR_STATE_INIT,
	SR_STATE_STARTED
};


#define SR_MALLOC malloc
#define SR_MFREE  free
#define SR_MEMSET	memset


static void _Sleep(size_t ms)
{
	usleep(ms*1000);
} // _Sleep


static void _end_sr_on_error(struct speech_rec *___sr, int errcode)
{
	if(___sr->aud_src == SR_MIC)
		stop_record(___sr->recorder);
	
	if (___sr->session_id) {
		if (___sr->notif.on_speech_end)
			___sr->notif.on_speech_end(errcode); // _end_sr_on_error

		QISRSessionEnd(___sr->session_id, "err");
		___sr->session_id = NULL;
	}
	___sr->state = SR_STATE_INIT;
} // _end_sr_on_error

static void _end_sr_on_vad(struct speech_rec *___sr)
{
	int errcode;
	const char *rslt;

	if (___sr->aud_src == SR_MIC)
		stop_record(___sr->recorder);	

	while(___sr->rec_stat != MSP_REC_STATUS_COMPLETE ){ // _end_sr_on_vad
		rslt = QISRGetResult(___sr->session_id, &___sr->rec_stat, 0, &errcode);
		if (rslt && ___sr->notif.on_result)
			___sr->notif.on_result(rslt, ___sr->rec_stat == MSP_REC_STATUS_COMPLETE ? 1 : 0);

		_Sleep(100); /* for cpu occupy, should sleep here */
	}

	if (___sr->session_id) {
		if (___sr->notif.on_speech_end)
			___sr->notif.on_speech_end(END_REASON_VAD_DETECT); // _end_sr_on_vad
		QISRSessionEnd(___sr->session_id, "VAD Normal");
		___sr->session_id = NULL;
	}
	___sr->state = SR_STATE_INIT;
} // _end_sr_on_vad

/* the record call back */
static void _iat_cb(char *data, unsigned long len, void *user_para)
{
	int errcode;
	struct speech_rec *__sr;

	if(len == 0 || data == NULL)
		return;

	__sr = (struct speech_rec *)user_para;

	if(__sr == NULL || __sr->ep_stat >= MSP_EP_AFTER_SPEECH) // _iat_cb
		return;
	if (__sr->state < SR_STATE_STARTED)
		return; /* ignore the data if error/vad happened */
	
	errcode = _sr_write_audio_data(__sr, data, len); // _iat_cb
	if (errcode) {
		_end_sr_on_error(__sr, errcode);
		return;
	}
} // _iat_cb

static char * _skip_space(char *s)
{
	while (s && *s != ' ' && *s != '\0')
		s++;
	return s;
} // _skip_space
static int _update_format_from_sessionparam(const char * session_para, WAVEFORMATEX *wavefmt)
{
	char *s;
	if ((s = strstr(session_para, "sample_rate"))) {
		s = strstr(s, "=");
		if (s && *s) {
			s = _skip_space(s);
			if (s && *s) {
				wavefmt->nSamplesPerSec = atoi(s);
				wavefmt->nAvgBytesPerSec = wavefmt->nBlockAlign * wavefmt->nSamplesPerSec;
			}
		}
		else
			return -1; // _update_format_from_sessionparam
	}
	else {
		return -1;
	}

	return 0;
} // _update_format_from_sessionparam

/* devid will be ignored if aud_src is not SR_MIC ; use get_default_dev_id
 * to use the default input device. Currently the device list function is
 * not provided yet. 
 */

int _sr_init_ex(struct speech_rec * ___sr, const char * session_begin_params, 
			enum sr_audsrc ___aud_src, record_dev_id devid, 
				struct speech_rec_notifier * notify)
{
	int errcode;
	size_t param_size;
	WAVEFORMATEX wavfmt = DEFAULT_FORMAT;

    _prSFn( " start : %s " , record_dev_id.u.name ) ;

	if (___aud_src == SR_MIC && get_input_dev_num() == 0) { // _sr_init_ex
		return -E_SR_NOACTIVEDEVICE;
	}

	if (!___sr)
		return -E_SR_INVAL;

	if (session_begin_params == NULL) {
		session_begin_params = DEFAULT_SESSION_PARA; // _sr_init_ex
	}

	SR_MEMSET(___sr, 0, sizeof(struct speech_rec));
	___sr->state = SR_STATE_INIT;
	___sr->aud_src = ___aud_src;
	___sr->ep_stat = MSP_EP_LOOKING_FOR_SPEECH; // _sr_init_ex
	___sr->rec_stat = MSP_REC_STATUS_SUCCESS;
	___sr->audio_status = MSP_AUDIO_SAMPLE_FIRST;

	param_size = strlen(session_begin_params) + 1;
	___sr->session_begin_params = (char*)SR_MALLOC(param_size); // _sr_init_ex
	if (___sr->session_begin_params == NULL) {
		sr_dbg("mem alloc failed\n");
		return -E_SR_NOMEM;
	}
	strncpy(___sr->session_begin_params, session_begin_params, param_size); // _sr_init_ex

	___sr->notif = *notify;
	
	if (___aud_src == SR_MIC) {
		errcode = create_recorder(&___sr->recorder, _iat_cb, (void*)___sr); // _sr_init_ex
		if (___sr->recorder == NULL || errcode != 0) {
			sr_dbg("create recorder failed: %d\n", errcode);
			errcode = -E_SR_RECORDFAIL;
			goto fail;
		}
		_update_format_from_sessionparam(session_begin_params, &wavfmt); // _sr_init_ex
	
		errcode = _open_recorder5(___sr->recorder, devid, &wavfmt);
		if (errcode != 0) {
			sr_dbg("recorder open failed: %d\n", errcode); // _sr_init_ex
			errcode = -E_SR_RECORDFAIL;
			goto fail;
		}
	}

    _prSFn( " end normal " ) ;
	return 0;

fail:
	if (___sr->recorder) {
		destroy_recorder(___sr->recorder); // _sr_init_ex
		___sr->recorder = NULL;
	}

	if (___sr->session_begin_params) {
		SR_MFREE(___sr->session_begin_params); // _sr_init_ex
		___sr->session_begin_params = NULL;
	}
	SR_MEMSET(&___sr->notif, 0, sizeof(___sr->notif));

    _prSFn( " end error : %d" , errcode ) ;
	return errcode;
} // _sr_init_ex

/* use the default input device to capture the audio. see _sr_init_ex */
int _sr_init(struct speech_rec * ___sr, const char * session_begin_params, 
		enum sr_audsrc ___aud_src, struct speech_rec_notifier * notify)
{
	return _sr_init_ex(___sr, session_begin_params, ___aud_src, 
			get_default_input_dev(), notify);
} // _sr_init

int _sr_start_listening(struct speech_rec *___sr)
{
	int ret;
	const char*		session_id = NULL;
	int				errcode = MSP_SUCCESS;

	if (___sr->state >= SR_STATE_STARTED) { // _sr_start_listening
		sr_dbg("already STARTED.\n");
		return -E_SR_ALREADY;
	} 

	session_id = QISRSessionBegin(NULL, ___sr->session_begin_params, &errcode); //听写不需要语法，第一个参数为NULL
	if (MSP_SUCCESS != errcode)
	{
		sr_dbg("\nQISRSessionBegin failed! error code:%d\n", errcode); // _sr_start_listening
		return errcode;
	}
	___sr->session_id = session_id;
	___sr->ep_stat = MSP_EP_LOOKING_FOR_SPEECH;
	___sr->rec_stat = MSP_REC_STATUS_SUCCESS;
	___sr->audio_status = MSP_AUDIO_SAMPLE_FIRST; // _sr_start_listening

	if (___sr->aud_src == SR_MIC) {
		ret = start_record(___sr->recorder);
		if (ret != 0) {
			sr_dbg("start record failed: %d\n", ret);
			QISRSessionEnd(session_id, "start record fail"); // _sr_start_listening
			___sr->session_id = NULL;
			return -E_SR_RECORDFAIL;
		}
	}

	___sr->state = SR_STATE_STARTED; // _sr_start_listening

	if (___sr->notif.on_speech_begin)
		___sr->notif.on_speech_begin();

	return 0;
} // _sr_start_listening

/* after stop_record, there are still some data callbacks */
static void _wait_for_rec_stop(struct recorder *rec, unsigned int timeout_ms)
{
	while (!is_record_stopped(rec)) {
		_Sleep(1);
		if (timeout_ms != (unsigned int)-1)
			if (0 == timeout_ms--)
				break;
	}
} // _wait_for_rec_stop

int _sr_stop_listening(struct speech_rec *___sr)
{
	int ret = 0;
	const char * rslt = NULL;

	if (___sr->state < SR_STATE_STARTED) {
		sr_dbg("Not started or already stopped.\n");
		return 0;
	}

	if (___sr->aud_src == SR_MIC) { // _sr_stop_listening
		ret = stop_record(___sr->recorder);
		if (ret != 0) {
			sr_dbg("Stop failed! \n");
			return -E_SR_RECORDFAIL;
		}
		_wait_for_rec_stop(___sr->recorder, (unsigned int)-1); // _sr_stop_listening
	}
	___sr->state = SR_STATE_INIT;
	ret = QISRAudioWrite(___sr->session_id, NULL, 0, MSP_AUDIO_SAMPLE_LAST, &___sr->ep_stat, &___sr->rec_stat);
	if (ret != 0) {
		sr_dbg("write LAST_SAMPLE failed: %d\n", ret);
		QISRSessionEnd(___sr->session_id, "write err"); // _sr_stop_listening
		return ret;
	}

	while (___sr->rec_stat != MSP_REC_STATUS_COMPLETE) {
		rslt = QISRGetResult(___sr->session_id, &___sr->rec_stat, 0, &ret); // _sr_stop_listening
		if (MSP_SUCCESS != ret)	{
			sr_dbg("\nQISRGetResult failed! error code: %d\n", ret);
			_end_sr_on_error(___sr, ret);
			return ret;
		}
		if (NULL != rslt && ___sr->notif.on_result)
			___sr->notif.on_result(rslt, ___sr->rec_stat == MSP_REC_STATUS_COMPLETE ? 1 : 0); // _sr_stop_listening
		_Sleep(100);
	}

	QISRSessionEnd(___sr->session_id, "normal");
	___sr->session_id = NULL;
	return 0;
} // _sr_stop_listening

int _sr_write_audio_data(struct speech_rec *___sr, char *data, unsigned int len)
{
	const char *rslt = NULL;
	int ret = 0;
	if (!___sr )
		return -E_SR_INVAL;
	if (!data || !len)
		return 0;

	ret = QISRAudioWrite(___sr->session_id, data, len, ___sr->audio_status, &___sr->ep_stat, &___sr->rec_stat); // _sr_write_audio_data
	if (ret) {
		_end_sr_on_error(___sr, ret);
		return ret;
	}
	___sr->audio_status = MSP_AUDIO_SAMPLE_CONTINUE;

	if (MSP_REC_STATUS_SUCCESS == ___sr->rec_stat) { //已经有部分听写结果
		rslt = QISRGetResult(___sr->session_id, &___sr->rec_stat, 0, &ret); // _sr_write_audio_data
		if (MSP_SUCCESS != ret)	{
			sr_dbg("\nQISRGetResult failed! error code: %d\n", ret);
			_end_sr_on_error(___sr, ret);
			return ret;
		}
		if (NULL != rslt && ___sr->notif.on_result)
			___sr->notif.on_result(rslt, ___sr->rec_stat == MSP_REC_STATUS_COMPLETE ? 1 : 0); // _sr_write_audio_data
	}

	if (MSP_EP_AFTER_SPEECH == ___sr->ep_stat)
		_end_sr_on_vad(___sr);

	return 0;
} // _sr_write_audio_data

void _sr_uninit(struct speech_rec * ___sr)
{
	if (___sr->recorder) {
		if(!is_record_stopped(___sr->recorder))
			stop_record(___sr->recorder);
		close_recorder(___sr->recorder);
		destroy_recorder(___sr->recorder);      // _sr_uninit
		___sr->recorder = NULL;
	}

	if (___sr->session_begin_params) {
		SR_MFREE(___sr->session_begin_params);
		___sr->session_begin_params = NULL;
	}
} // _sr_uninit
