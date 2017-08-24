/*
@file
@brief  record demo for linux

@author		taozhang9
@date		2016/05/27
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <alsa/asoundlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <pthread.h>
#include "formats.h"
#include "linuxrec.h"

#define DBG_ON 1

#if DBG_ON
#define dbg  printf
#else
#define dbg
#endif

#include "patchXF/pathcXFbase.h"
#include "patchXF/pathcXFiatRec.h"

/* Do not change the sequence */
enum {
	RECORD_STATE_CREATED,	/* Init		*/
	RECORD_STATE_CLOSING,
	RECORD_STATE_READY,		/* Opened	*/
	RECORD_STATE_STOPPING,	/* During Stop	*/
	RECORD_STATE_RECORDING,	/* Started	*/
};

#define SAMPLE_RATE  16000
#define SAMPLE_BIT_SIZE 16
#define FRAME_CNT   10
//#define BUF_COUNT   1
#define DEF_BUFF_TIME  500000
#define DEF_PERIOD_TIME 100000

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
#if 0
struct bufinfo {
	char *data;
	unsigned int bufsize;
};
#endif


static int _Show_xrun = 1;
static int _Start_record_internal(snd_pcm_t *pcm)
{
	return snd_pcm_start(pcm);
} // _Start_record_internal

static int _Stop_record_internal(snd_pcm_t *pcm)
{
	return snd_pcm_drop(pcm);
} // _Stop_record_internal


static int _Is_stopped_internal(struct recorder *___rec)
{
	snd_pcm_state_t state;

	state =  snd_pcm_state((snd_pcm_t *)___rec->wavein_hdl);
	switch (state) {
	case SND_PCM_STATE_RUNNING:
	case SND_PCM_STATE_DRAINING:
		return 0;
	default: break;
	}
	return 1;
	
} // _Is_stopped_internal

static int _Format_ms_to_alsa(const WAVEFORMATEX * wavfmt, 
						snd_pcm_format_t * format)
{
	snd_pcm_format_t tmp;
	tmp = snd_pcm_build_linear_format(wavfmt->wBitsPerSample, 
			wavfmt->wBitsPerSample, wavfmt->wBitsPerSample == 8 ? 1 : 0, 0);
	if ( tmp == SND_PCM_FORMAT_UNKNOWN )
		return -EINVAL;
	*format = tmp;
	return 0;
} // _Format_ms_to_alsa

/* set hardware and software params */
static int _Set_hwparams(struct recorder * ___rec,  const WAVEFORMATEX *wavfmt,
			unsigned int buffertime, unsigned int periodtime)
{
	snd_pcm_hw_params_t *params;
	int __err;
	unsigned int rate;
	snd_pcm_format_t format;
	snd_pcm_uframes_t size;
	snd_pcm_t *__handle = (snd_pcm_t *)___rec->wavein_hdl;

	___rec->buffer_time = buffertime;
	___rec->period_time = periodtime;

	snd_pcm_hw_params_alloca(&params);
	__err = snd_pcm_hw_params_any(__handle, params);
	if (__err < 0) {
		dbg("Broken configuration for this PCM");
		return __err;
	}
	__err = snd_pcm_hw_params_set_access(__handle, params, // _Set_hwparams
					   SND_PCM_ACCESS_RW_INTERLEAVED);
	if (__err < 0) {
		dbg("Access type not available");
		return __err;
	}
	__err = _Format_ms_to_alsa(wavfmt, &format);
	if (__err) {
		dbg("Invalid format");
		return - EINVAL;
	}
	__err = snd_pcm_hw_params_set_format(__handle, params, format); // _Set_hwparams
	if (__err < 0) {
		dbg("Sample format non available");
		return __err;
	}
	__err = snd_pcm_hw_params_set_channels(__handle, params, wavfmt->nChannels);
	if (__err < 0) {
		dbg("Channels count non available");
		return __err;
	}

	rate = wavfmt->nSamplesPerSec;
	__err = snd_pcm_hw_params_set_rate_near(__handle, params, &rate, 0); // _Set_hwparams
	if (__err < 0) {
		dbg("Set rate failed");
		return __err;
	}
	if(rate != wavfmt->nSamplesPerSec) {
		dbg("Rate mismatch");
		return -EINVAL;
	}
	if (___rec->buffer_time == 0 || ___rec->period_time == 0) {
		__err = snd_pcm_hw_params_get_buffer_time_max(params,
						    &___rec->buffer_time, 0);
		assert(__err >= 0);
		if (___rec->buffer_time > 500000)
			___rec->buffer_time = 500000;
		___rec->period_time = ___rec->buffer_time / 4;
	}
	__err = snd_pcm_hw_params_set_period_time_near(__handle, params, // _Set_hwparams
					     &___rec->period_time, 0);
	if (__err < 0) {
		dbg("set period time fail");
		return __err;
	}
	__err = snd_pcm_hw_params_set_buffer_time_near(__handle, params,
					     &___rec->buffer_time, 0);
	if (__err < 0) {
		dbg("set buffer time failed");
		return __err;
	}
	__err = snd_pcm_hw_params_get_period_size(params, &size, 0); // _Set_hwparams
	if (__err < 0) {
		dbg("get period size fail");
		return __err;
	}
	___rec->period_frames = size; 
	__err = snd_pcm_hw_params_get_buffer_size(params, &size);
	if (size == ___rec->period_frames) {
		_prSFn("Can't use period equal to buffer size (%d == %d)",
				      (int) size, ___rec->period_frames);
		return -EINVAL;
	}
	___rec->buffer_frames = size;
	___rec->bits_per_frame = wavfmt->wBitsPerSample; // _Set_hwparams

	/* set to driver */
	__err = snd_pcm_hw_params(__handle, params);
	if (__err < 0) {
		dbg("Unable to install hw params:");
		return __err;
	}
	return 0;
} // _Set_hwparams

static int _Set_swparams(struct recorder * ___rec)
{
	int __err;
	snd_pcm_sw_params_t *swparams;
	snd_pcm_t * __handle = (snd_pcm_t*)(___rec->wavein_hdl);
	/* sw para */
	snd_pcm_sw_params_alloca(&swparams);
	__err = snd_pcm_sw_params_current(__handle, swparams);
	if (__err < 0) {
		dbg("get current sw para fail");
		return __err;
	}

	__err = snd_pcm_sw_params_set_avail_min(__handle, swparams, 
						___rec->period_frames);
	if (__err < 0) {
		dbg("set avail min failed");
		return __err;
	}
	/* set a value bigger than the buffer frames to prevent the auto start.
	 * we use the snd_pcm_start to explicit start the pcm */
	__err = snd_pcm_sw_params_set_start_threshold(__handle, swparams,  // _Set_swparams
			___rec->buffer_frames * 2);
	if (__err < 0) {
		dbg("set start threshold fail");
		return __err;
	}

	if ( (__err = snd_pcm_sw_params(__handle, swparams)) < 0) {
		dbg("unable to install sw params:");
		return __err;
	}
	return 0;
} // _Set_swparams

static int _Set_params(struct recorder *___rec, WAVEFORMATEX *___fmt,
		unsigned int buffertime, unsigned int periodtime)
{
	int __err;
	WAVEFORMATEX defmt = DEFAULT_FORMAT;
	
	if (___fmt == NULL) {
		___fmt = &defmt;
	}
	__err = _Set_hwparams(___rec, ___fmt, buffertime, periodtime);
	if (__err)
		return __err;
	__err = _Set_swparams(___rec);
	if (__err)
		return __err;
	return 0;
} // _Set_params

/*
 *   Underrun and suspend recovery
 */
 
static int _Xrun_recovery(snd_pcm_t *__handle, int ___err)
{
	if (___err == -EPIPE) {	/* over-run */
		if (_Show_xrun)
			printf("!!!!!!overrun happend!!!!!!");

		___err = snd_pcm_prepare(__handle);
		if (___err < 0) {
			if (_Show_xrun)
				printf("Can't recovery from overrun,"
				"prepare failed: %s\n", snd_strerror(___err));
			return ___err;
		}
		return 0;
	} else if (___err == -ESTRPIPE) {
		while ((___err = snd_pcm_resume(__handle)) == -EAGAIN) // _Xrun_recovery
			usleep(200000);	/* wait until the suspend flag is released */
		if (___err < 0) {
			___err = snd_pcm_prepare(__handle);
			if (___err < 0) {
				if (_Show_xrun)
					printf("Can't recovery from suspend,"
					"prepare failed: %s\n", snd_strerror(___err));
				return ___err;
			}
		}
		return 0;
	}
	return ___err;
} // _Xrun_recovery

static ssize_t _Pcm_read(struct recorder *___rec, size_t rcount)
{
	ssize_t r;
	size_t count = rcount;
	char *data;
	snd_pcm_t *__handle = (snd_pcm_t *)___rec->wavein_hdl;
	if(!__handle)
		return -EINVAL;

	data = ___rec->audiobuf;
	while (count > 0) {
		r = snd_pcm_readi(__handle, data, count);
		if (r == -EAGAIN || (r >= 0 && (size_t)r < count)) {
			snd_pcm_wait(__handle, 100);
		} else if (r < 0) {
			if(_Xrun_recovery(__handle, r) < 0) { // _Pcm_read
				return -1;
			}
		} 

		if (r > 0) {
			count -= r;
			data += r * ___rec->bits_per_frame / 8;
		}
	}
	return rcount;
} // _Pcm_read

static void * _Record_thread_proc(void * para)
{
	struct recorder * __rec = (struct recorder *) para;
	size_t frames, bytes;
	sigset_t mask, oldmask;


	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTERM);
	pthread_sigmask(SIG_BLOCK, &mask, &oldmask);

	while(1) {
		frames = __rec->period_frames;
		bytes = frames * __rec->bits_per_frame / 8; 

		/* closing, exit the thread */
		if (__rec->state == RECORD_STATE_CLOSING)
			break;

		if(__rec->state < RECORD_STATE_RECORDING) // _Record_thread_proc
			usleep(100000);

		if (_Pcm_read(__rec, frames) != frames) {
			return NULL;
		}

		if (__rec->on_data_ind)
			__rec->on_data_ind(__rec->audiobuf, bytes, 
					__rec->user_cb_para);
	}
	return __rec;

} // _Record_thread_proc

static int _Create_record_thread(void * para, pthread_t * tidp)
{
	int __err;
	__err = pthread_create(tidp, NULL, _Record_thread_proc, (void *)para);
	if (__err != 0)
		return __err;

	return 0;
} // _Create_record_thread

#if 0 /* don't use it now... cuz only one buffer supported */
static void _Free_rec_buffer(struct recorder * ___rec)
{
	if (___rec->bufheader) {
		unsigned int i;
		struct bufinfo *info = (struct bufinfo *) ___rec->bufheader;

		assert(___rec->bufcount > 0);
		for (i = 0; i < ___rec->bufcount; ++i) {
			if (info->data) {
				free(info->data);
				info->data = NULL;
				info->bufsize = 0;
				info->audio_bytes = 0;
			}
			info++;
		}
		free(___rec->bufheader);
		___rec->bufheader = NULL;
	}
	___rec->bufcount = 0;
} // _Free_rec_buffer

static int _Prepare_rec_buffer(struct recorder * ___rec )
{
	struct bufinfo *buffers;
	unsigned int i;
	int __err;
	size_t sz;

	/* the read and QISRWrite is blocked, currently only support one buffer,
	 * if overrun too much, need more buffer and another new thread
	 * to write the audio to network */
	___rec->bufcount = 1;
	sz = sizeof(struct bufinfo)*___rec->bufcount;
	buffers=(struct bufinfo*)malloc(sz);
	if (!buffers) {
		___rec->bufcount = 0;
		goto fail;
	}
	memset(buffers, 0, sz);
	___rec->bufheader = buffers;

	for (i = 0; i < ___rec->bufcount; ++i) {
		buffers[i].bufsize = 
			(___rec->period_frames * ___rec->bits_per_frame / 8);
		buffers[i].data = (char *)malloc(buffers[i].bufsize);
		if (!buffers[i].data) {
			buffers[i].bufsize = 0;
			goto fail;
		}
		buffers[i].audio_bytes = 0;
	}
	return 0;
fail:
	_Free_rec_buffer(___rec);
	return -ENOMEM;
} // _Free_rec_buffer
#else
static void _Free_rec_buffer(struct recorder * ___rec)
{
	if (___rec->audiobuf) {
		free(___rec->audiobuf);
		___rec->audiobuf = NULL;
	}
} // _Free_rec_buffer

static int _Prepare_rec_buffer(struct recorder * ___rec )
{
	/* the read and QISRWrite is blocked, currently only support one buffer,
	 * if overrun too much, need more buffer and another new thread
	 * to write the audio to network */
	size_t sz = (___rec->period_frames * ___rec->bits_per_frame / 8);
	___rec->audiobuf = (char *)malloc(sz);
	if(!___rec->audiobuf)
		return -ENOMEM;
	return 0;
} // _Prepare_rec_buffer
#endif

static int _Open_recorder_internal(struct recorder * ___rec,  // call to snd_pcm_open , _Set_params , _Create_record_thread
		record_dev_id ___dev, WAVEFORMATEX * ___fmt)
{
	int __err = 0;

    _prSFn( " -- snd_pcm_open <%s>" , ___dev.u.name ) ;
	__err = snd_pcm_open((snd_pcm_t **)&___rec->wavein_hdl, ___dev.u.name, 
			SND_PCM_STREAM_CAPTURE, 0);
	if(__err < 0) {
        _prSFn( " -- snd_pcm_open ERROR <%d>" , __err ) ;
		goto fail;
    }

    _prSFn( " -- _Set_params <%d,%d>" , DEF_BUFF_TIME , DEF_PERIOD_TIME ) ;
	__err = _Set_params(___rec, ___fmt, DEF_BUFF_TIME, DEF_PERIOD_TIME);
	if(__err) {
        _prSFn( " -- _Set_params ERROR <%d>" , __err ) ;
		goto fail;
    }

    _prSFn( " -- _Prepare_rec_buffer " );
	assert(___rec->bufheader == NULL);
	__err = _Prepare_rec_buffer(___rec); // _Open_recorder_internal
	if(__err) {
        _prSFn( " -- _Prepare_rec_buffer ERROR <%d>" , __err ) ;
		goto fail;
    }

    _prSFn( " -- _Create_record_thread " );
	__err = _Create_record_thread((void*)___rec, 
			&___rec->rec_thread);
	if(__err) {
        _prSFn( " -- _Create_record_thread ERROR <%d>" , __err ) ;
		goto fail;
    }

    _prSFn( " -- end normal " );
	return 0;
fail:
	if(___rec->wavein_hdl)
		snd_pcm_close((snd_pcm_t *) ___rec->wavein_hdl);
	___rec->wavein_hdl = NULL;
	_Free_rec_buffer(___rec);

    _prSFn( " -- end ERROR : %d " , __err );
	return __err;
} // _Open_recorder_internal

static void _Close_recorder_internal(struct recorder *___rec)
{
	snd_pcm_t * __handle;

	__handle = (snd_pcm_t *) ___rec->wavein_hdl;

	/* may be the thread is blocked at read, cancel it */
	pthread_cancel(___rec->rec_thread);
	
	/* wait for the pcm thread quit first */
	pthread_join(___rec->rec_thread, NULL);

	if(__handle) {
		snd_pcm_close(__handle);
		___rec->wavein_hdl = NULL;
	}
	_Free_rec_buffer(___rec);
} // _Close_recorder_internal

/* return the count of pcm device */
/* list all cards */
static int _get_pcm_device_cnt(snd_pcm_stream_t stream)
{
	void **hints, **n;
	char *io, *filter, *name;
	int cnt = 0;
	int _amount = 0;
	char * __descr ; 

	if (snd_device_name_hint(-1, "pcm", &hints) < 0)
		return 0;
	n = hints;
	filter = stream == SND_PCM_STREAM_CAPTURE ? "Input" : "Output";
	while (*n != NULL) {
		io = snd_device_name_get_hint(*n, "IOID");
		name = snd_device_name_get_hint(*n, "NAME");
		__descr = snd_device_name_get_hint(*n, "DESC");
		if (name && (io == NULL || strcmp(io, filter) == 0)) {
            if ( 0 ) { _prSFn( " %d : %s : <%s> : [%s] : {%s} " , _amount + 1 , filter , io , name , __descr ) ; } 
            if ( 1 ) { _prSFn( " %d : %s : <%s> : [%s] "        , _amount + 1 , filter , io , name ) ; } 
			cnt ++;
        }
		if (io != NULL)
			free(io);
		if (name != NULL)
			free(name);
		if (__descr != NULL)
			free(__descr);
		n++;
		_amount ++ ;
	}
	snd_device_name_free_hint(hints);
    if ( 0 ) { _prSFn( " _get_pcm_device_cnt -> %d of %d" , cnt , _amount) ; }
	return cnt;
} // _get_pcm_device_cnt

static void free_name_desc(char **name_or_desc) 
{
	char **ss;
	ss = name_or_desc;
	if(NULL == name_or_desc)
		return;
	while(*name_or_desc) {
		free(*name_or_desc);
		*name_or_desc = NULL;
		name_or_desc++;
	}
	free(ss);
}

/* return success: total count, need free the name and desc buffer 
 * fail: -1 , *name_out and *desc_out will be NULL */
int _list_pcm(snd_pcm_stream_t stream, char**name_out, 
						char ** desc_out)
{
	void **hints, **n;
	char **__name, **__descr;
	char *io;
	const char *filter;
	int cnt = 0;
	int i = 0;

	if (snd_device_name_hint(-1, "pcm", &hints) < 0)
		return 0;
	n = hints;
	cnt = _get_pcm_device_cnt(stream);
	if(!cnt) {
		goto fail; 
	}

	*name_out = calloc(sizeof(char *) , (1+cnt));
	if (*name_out == NULL)
		goto fail;
	*desc_out = calloc(sizeof(char *) , (1 + cnt));
	if (*desc_out == NULL)
		goto fail;

	/* the last one is a flag, NULL */
	name_out[cnt] = NULL;
	desc_out[cnt] = NULL;
	__name = name_out;
	__descr = desc_out;

	filter = stream == SND_PCM_STREAM_CAPTURE ? "Input" : "Output";
	while (*n != NULL && i < cnt) {
		*__name = snd_device_name_get_hint(*n, "NAME");
		*__descr = snd_device_name_get_hint(*n, "DESC");
		io = snd_device_name_get_hint(*n, "IOID");
		if (__name == NULL || 
			(io != NULL && strcmp(io, filter) != 0) ){
			if (*__name) free(*__name);
			if (*__descr) free(*__descr);
		} else {
			if (*__descr == NULL) {
				*__descr = malloc(4);
				memset(*__descr, 0, 4);
			}
            if ( 1 ) { _prSFn( " %s : <%s> : [%s] " , filter , *__name , *__descr ) ; } 
			__name++;
			__descr++;
			i++;
		}
		if (io != NULL)
			free(io);
		n++;
	}
	snd_device_name_free_hint(hints);
	return cnt;
fail:
	free_name_desc(name_out);
	free_name_desc(desc_out);
	snd_device_name_free_hint(hints);
	return -1;
} // _list_pcm 

/* -------------------------------------
 * Interfaces 
 --------------------------------------*/ 
/* the device id is a pcm string name in linux */
record_dev_id  _get_default_input_dev()
{
	record_dev_id id; 
	id.u.name = "default";
	id.u.name = _micDevName ;

    if ( 1 ) { _prSFn( " init record_dev_id's name to : %s" , id.u.name ) ; } 
	return id;
} // _get_default_input_dev

record_dev_id * list_input_device() 
{
	// TODO: unimplemented
	return NULL;
}

int _get_input_dev_num()
{
    int __rt ;
	__rt = _get_pcm_device_cnt(SND_PCM_STREAM_CAPTURE);

    if ( 1 ) { _prSFn( " _get_pcm_device_cnt -> %d" , __rt ); }
    if ( 0 ) { 
        char* name_out ;
        char * desc_out ;
        _list_pcm( SND_PCM_STREAM_CAPTURE , & name_out , & desc_out ) ; 
    }

	return __rt; 
} // _get_input_dev_num


/* callback will be run on a new thread */
int _create_recorder(struct recorder ** out_rec, 
				void (*on_data_ind)(char *data, unsigned long len, void *user_cb_para), 
				void* user_cb_para)
{
	struct recorder * myrec;
	myrec = (struct recorder *)malloc(sizeof(struct recorder));
	if(!myrec)
		return -RECORD_ERR_MEMFAIL;

	memset(myrec, 0, sizeof(struct recorder));
	myrec->on_data_ind = on_data_ind;
	myrec->user_cb_para = user_cb_para;
	myrec->state = RECORD_STATE_CREATED;

	*out_rec = myrec;
	return 0;
} // _create_recorder

void _destroy_recorder(struct recorder *___rec)
{
	if(!___rec)
		return;

	free(___rec);
} // _destroy_recorder

int _open_recorder(struct recorder * ___rec, record_dev_id ___dev, WAVEFORMATEX * ___fmt) // internal -> snd_pcm_open , _Set_params , _Create_record_thread
{
	int ret = 0;
	if(!___rec )
		return -RECORD_ERR_INVAL;
	if(___rec->state >= RECORD_STATE_READY)
		return 0;

	ret = _Open_recorder_internal(___rec, ___dev, ___fmt);
	if(ret == 0)
		___rec->state = RECORD_STATE_READY;
	return 0;

} // _open_recorder

void _close_recorder(struct recorder *___rec)
{
	if(___rec == NULL || ___rec->state < RECORD_STATE_READY)
		return;
	if(___rec->state == RECORD_STATE_RECORDING)
		_stop_record(___rec);

	___rec->state = RECORD_STATE_CLOSING;

	_Close_recorder_internal(___rec);

	___rec->state = RECORD_STATE_CREATED;	
} // _close_recorder

int _start_record(struct recorder * ___rec)
{
	int ret;
	if(___rec == NULL)
		return -RECORD_ERR_INVAL;
	if( ___rec->state < RECORD_STATE_READY)
		return -RECORD_ERR_NOT_READY;
	if( ___rec->state == RECORD_STATE_RECORDING)
		return 0;

	ret = _Start_record_internal((snd_pcm_t *)___rec->wavein_hdl);
	if(ret == 0)
		___rec->state = RECORD_STATE_RECORDING;
	return ret;
} // _start_record

int _stop_record(struct recorder * ___rec)
{
	int ret;
	if(___rec == NULL)
		return -RECORD_ERR_INVAL;
	if( ___rec->state < RECORD_STATE_RECORDING)
		return 0;

	___rec->state = RECORD_STATE_STOPPING;
	ret = _Stop_record_internal((snd_pcm_t *)___rec->wavein_hdl);
	if(ret == 0) {		
		___rec->state = RECORD_STATE_READY;
	}
	return ret;
} // _stop_record

int is_record_stopped(struct recorder *___rec)
{
	if(___rec->state == RECORD_STATE_RECORDING)
		return 0;

	return _Is_stopped_internal(___rec);
}
