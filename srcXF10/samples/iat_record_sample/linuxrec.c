/*
@file
@brief  record demo for linux

@author		taozhang9
@date		2016/05/27
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

////#include <alsa/asoundlib.h>
////#include <tinyalsa/asoundlib.h>
//#ifdef  __i386__
//#include <alsa/asoundlib.h>
//#else  
////#include <asoundlib.h>
//#include <alsa/asoundlib.h>
//#endif
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

//#include "linuxrec.h"

static int show_xrun = 1;
static int _start_record_internal(snd_pcm_t *pcm)
{
	return snd_pcm_start(pcm);
} // _start_record_internal

static int _stop_record_internal(snd_pcm_t *pcm)
{
	return snd_pcm_drop(pcm);
} // _stop_record_internal


static int _is_stopped_internal(struct recorder *___rec)
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
	
} // _is_stopped_internal

static int _format_ms_to_alsa(const WAVEFORMATEX * wavfmt, 
						snd_pcm_format_t * format)
{
	snd_pcm_format_t tmp;
	tmp = snd_pcm_build_linear_format(wavfmt->wBitsPerSample, 
			wavfmt->wBitsPerSample, wavfmt->wBitsPerSample == 8 ? 1 : 0, 0);
	if ( tmp == SND_PCM_FORMAT_UNKNOWN )
		return -EINVAL;
	*format = tmp;
	return 0;
} // _format_ms_to_alsa

/* set hardware and software params */
static int _set_hwparams3(struct recorder * ___rec,  const WAVEFORMATEX *wavfmt,
			unsigned int buffertime, unsigned int periodtime)
{
	snd_pcm_hw_params_t *params;
	int err;
	unsigned int rate;
	snd_pcm_format_t format;
	snd_pcm_uframes_t __size;
	snd_pcm_t *__handle = (snd_pcm_t *)___rec->wavein_hdl; // _set_hwparams3

	___rec->buffer_time = buffertime;
	___rec->period_time = periodtime;

	snd_pcm_hw_params_alloca(&params);
	err = snd_pcm_hw_params_any(__handle, params);
	if (err < 0) {
		dbg("Broken configuration for this PCM");
		return err;
	}
	err = snd_pcm_hw_params_set_access(__handle, params, // _set_hwparams3
					   SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err < 0) {
		dbg("Access type not available");
		return err;
	}
	err = _format_ms_to_alsa(wavfmt, &format); // _set_hwparams3
	if (err) {
		dbg("Invalid format");
		return - EINVAL;
	}
	err = snd_pcm_hw_params_set_format(__handle, params, format); // _set_hwparams3
	if (err < 0) {
		dbg("Sample format non available");
		return err;
	}
	err = snd_pcm_hw_params_set_channels(__handle, params, wavfmt->nChannels); // _set_hwparams3
	if (err < 0) {
		dbg("Channels count non available");
		return err;
	}

	rate = wavfmt->nSamplesPerSec;
	err = snd_pcm_hw_params_set_rate_near(__handle, params, &rate, 0); // _set_hwparams3
	if (err < 0) {
		dbg("Set rate failed");
		return err;
	}
	if(rate != wavfmt->nSamplesPerSec) {
		dbg("Rate mismatch");
		return -EINVAL;
	}
	if (___rec->buffer_time == 0 || ___rec->period_time == 0) { // _set_hwparams3
		err = snd_pcm_hw_params_get_buffer_time_max(params,
						    &___rec->buffer_time, 0);
		assert(err >= 0);
		if (___rec->buffer_time > 500000)
			___rec->buffer_time = 500000;
		___rec->period_time = ___rec->buffer_time / 4;
	}
	err = snd_pcm_hw_params_set_period_time_near(__handle, params, // _set_hwparams3
					     &___rec->period_time, 0);
	if (err < 0) {
		dbg("set period time fail");
		return err;
	}
	err = snd_pcm_hw_params_set_buffer_time_near(__handle, params, // _set_hwparams3
					     &___rec->buffer_time, 0);
	if (err < 0) {
		dbg("set buffer time failed");
		return err;
	}
	err = snd_pcm_hw_params_get_period_size(params, &__size, 0); // _set_hwparams3
	if (err < 0) {
		dbg("get period __size fail");
		return err;
	}
	___rec->period_frames = __size; 
	err = snd_pcm_hw_params_get_buffer_size(params, &__size); // _set_hwparams3
	if (__size == ___rec->period_frames) {
		_prSF("Can't 1 use period equal to buffer __size (%u == %u)", (unsigned int) __size, (unsigned int) ___rec->period_frames);
		return -EINVAL;
	} else {
		_prSF("  use period not equal to buffer __size (%u != %u)", (unsigned int) __size, (unsigned int) ___rec->period_frames);
    }
	___rec->buffer_frames = __size;
	___rec->bits_per_frame = wavfmt->wBitsPerSample; // _set_hwparams3

	/* set to driver */
	err = snd_pcm_hw_params(__handle, params);
	if (err < 0) {
		dbg("Unable to install hw params:");
		return err;
	}
	return 0;
} // _set_hwparams3

static int _set_swparams2(struct recorder * ___rec)
{
	int err;
	snd_pcm_sw_params_t *swparams;
	snd_pcm_t * __handle = (snd_pcm_t*)(___rec->wavein_hdl);
	/* sw para */
	snd_pcm_sw_params_alloca(&swparams);
	err = snd_pcm_sw_params_current(__handle, swparams); // _set_swparams2
	if (err < 0) {
		dbg("get current sw para fail");
		return err;
	}

	err = snd_pcm_sw_params_set_avail_min(__handle, swparams, 
						___rec->period_frames);
	if (err < 0) {
		dbg("set avail min failed");
		return err;
	}
	/* set a value bigger than the buffer frames to prevent the auto start.
	 * we use the snd_pcm_start to explicit start the pcm */
	err = snd_pcm_sw_params_set_start_threshold(__handle, swparams,  // _set_swparams2
			___rec->buffer_frames * 2);
	if (err < 0) {
		dbg("set start threshold fail");
		return err;
	}

	if ( (err = snd_pcm_sw_params(__handle, swparams)) < 0) { // _set_swparams2
		dbg("unable to install sw params:");
		return err;
	}
	return 0;
} // _set_swparams2

static int _set_params1(struct recorder *___rec, WAVEFORMATEX *fmt,
		unsigned int buffertime, unsigned int periodtime)
{
	int err;
	WAVEFORMATEX defmt = DEFAULT_FORMAT;
	
	if (fmt == NULL) {
		fmt = &defmt;
	}
	err = _set_hwparams3(___rec, fmt, buffertime, periodtime); // _set_params1
	if (err)
		return err;
	err = _set_swparams2(___rec);
	if (err)
		return err;
	return 0;
} // _set_params1

/*
 *   Underrun and suspend recovery
 */
 
static int xrun_recovery(snd_pcm_t *___handle, int err)
{
	if (err == -EPIPE) {	/* over-run */
		if (show_xrun)
			printf("!!!!!!overrun happend!!!!!!");

		err = snd_pcm_prepare(___handle);
		if (err < 0) {
			if (show_xrun)
				//printf("Can't 2 recovery from overrun,"
				_prSF("Can't 2 recovery from overrun,"
				"prepare failed: %s\n", snd_strerror(err));
			return err;
		}
		return 0;
	} else if (err == -ESTRPIPE) {
		while ((err = snd_pcm_resume(___handle)) == -EAGAIN)
			usleep(200000);	/* wait until the suspend flag is released */
		if (err < 0) {
			err = snd_pcm_prepare(___handle);
			if (err < 0) {
				if (show_xrun)
					//printf("Can't 3 recovery from suspend,"
					_prSF("Can't 3 recovery from suspend,"
					"prepare failed: %s\n", snd_strerror(err));
				return err;
			}
		}
		return 0;
	}
	return err;
}
static ssize_t pcm_read(struct recorder *___rec, size_t rcount)
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
			if(xrun_recovery(__handle, r) < 0) {
				return -1;
			}
		} 

		if (r > 0) {
			count -= r;
			data += r * ___rec->bits_per_frame / 8;
		}
	}
	return rcount;
}

static void * record_thread_proc(void * para)
{
	struct recorder * ___rec = (struct recorder *) para;
	size_t frames, bytes;
	sigset_t mask, oldmask;


	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTERM);
	pthread_sigmask(SIG_BLOCK, &mask, &oldmask);

	while(1) {
		frames = ___rec->period_frames;
		bytes = frames * ___rec->bits_per_frame / 8; 

		/* closing, exit the thread */
		if (___rec->state == RECORD_STATE_CLOSING)
			break;

		if(___rec->state < RECORD_STATE_RECORDING)
			usleep(100000);

		if (pcm_read(___rec, frames) != frames) {
			return NULL;
		}

		if (___rec->on_data_ind)
			___rec->on_data_ind(___rec->audiobuf, bytes, 
					___rec->user_cb_para);
	}
	return ___rec;

}
static int create_record_thread(void * para, pthread_t * tidp)
{
	int err;
	err = pthread_create(tidp, NULL, record_thread_proc, (void *)para);
	if (err != 0)
		return err;

	return 0;
}

#if 0 /* don't use it now... cuz only one buffer supported */
static void free_rec_buffer(struct recorder * ___rec)
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
}

static int prepare_rec_buffer(struct recorder * ___rec )
{
	struct bufinfo *buffers;
	unsigned int i;
	int err;
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
	free_rec_buffer(___rec);
	return -ENOMEM;
}
#else
static void free_rec_buffer(struct recorder * ___rec)
{
	if (___rec->audiobuf) {
		free(___rec->audiobuf);
		___rec->audiobuf = NULL;
	}
}

static int prepare_rec_buffer(struct recorder * ___rec )
{
	/* the read and QISRWrite is blocked, currently only support one buffer,
	 * if overrun too much, need more buffer and another new thread
	 * to write the audio to network */
	size_t sz = (___rec->period_frames * ___rec->bits_per_frame / 8);
	___rec->audiobuf = (char *)malloc(sz);
	if(!___rec->audiobuf)
		return -ENOMEM;
	return 0;
}
#endif

static int open_recorder_internal(struct recorder * ___rec, 
		record_dev_id ___dev, WAVEFORMATEX * fmt)
{
	int err = 0;

    _prSF( " trying open <%s>" "\n" , ___dev.u.name ) ;
	err = snd_pcm_open((snd_pcm_t **)&___rec->wavein_hdl, ___dev.u.name, 
			SND_PCM_STREAM_CAPTURE, 0);
	if(err < 0) {
        _prSF( " open failed <%s>" "\n" , ___dev.u.name ) ;
		goto fail;
    }

    _prSF( " trying set para <%s>" "\n" , ___dev.u.name ) ;
	err = _set_params1(___rec, fmt, DEF_BUFF_TIME, DEF_PERIOD_TIME);
	if(err) {
        _prSF( " set para <%s>" "\n" , ___dev.u.name ) ;
		goto fail;
    }

	assert(___rec->bufheader == NULL);

    _prSF( " trying prepare rec buffer <%s>" "\n" , ___dev.u.name ) ;
	err = prepare_rec_buffer(___rec);
	if(err) {
        _prSF( " prepare rec buffer faile : <%s>" "\n" , ___dev.u.name ) ;
		goto fail;
    }

    _prSF( " trying create rec thread <%s>" "\n" , ___dev.u.name ) ;
	err = create_record_thread((void*)___rec, 
			&___rec->rec_thread);
	if(err) {
        _prSF( " create rec thread failed :<%s>" "\n" , ___dev.u.name ) ;
		goto fail;
    }
	

    _prSF( " rec open normal end : <%s>" "\n" , ___dev.u.name ) ;
	return 0;
fail:
	if(___rec->wavein_hdl)
		snd_pcm_close((snd_pcm_t *) ___rec->wavein_hdl);
	___rec->wavein_hdl = NULL;
	free_rec_buffer(___rec);
    _prSF( " rec open ERROR end : <%s>" "\n" , ___dev.u.name ) ;
	return err;
}

static void close_recorder_internal(struct recorder *___rec)
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
	free_rec_buffer(___rec);
}
/* return the count of pcm device */
/* list all cards */
static int get_pcm_device_cnt(snd_pcm_stream_t stream)
{
	void **hints, **n;
	char *io, *filter, *name;
	int cnt = 0;

    _prSFn( " ---- start " ) ;

	if (snd_device_name_hint(-1, "pcm", &hints) < 0) {
        _prSFn( " ---- init end " ) ;
		return 0;
    }

	n = hints;
	filter = stream == SND_PCM_STREAM_CAPTURE ? "Input" : "Output";
	while (*n != NULL) {
		io = snd_device_name_get_hint(*n, "IOID");
		name = snd_device_name_get_hint(*n, "NAME");
		if (name && (io == NULL || strcmp(io, filter) == 0)) {
            _prSFn( " ==--== cmp pcm:  %d : %s -- %s -- %s " , cnt , filter , io , name ) ;
			cnt ++;
        }
		if (io != NULL)
			free(io);
		if (name != NULL)
			free(name);
		n++;
	}
	snd_device_name_free_hint(hints);
    _prSFn( " ---- end %d " , cnt ) ;
	return cnt;
}

static void _free_name_desc(char **name_or_desc) 
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
} // _free_name_desc

/* return success: total count, need free the name and desc buffer 
 * fail: -1 , *name_out and *desc_out will be NULL */
static int list_pcm(snd_pcm_stream_t stream, char**name_out, 
						char ** desc_out)
{
	void **hints, **n;
	char **name, **descr;
	char *io;
	const char *filter; // list_pcm
	int cnt = 0;
	int i = 0;

    _prSFn( " ---- start " ) ;

	if (snd_device_name_hint(-1, "pcm", &hints) < 0) { // list_pcm
        _prSFn( " ---- init end " ) ;
		return 0;
    }

	n = hints;
	cnt = get_pcm_device_cnt(stream);
	if(!cnt) {
        _prSFn( " ---- ERROR end 1" ) ;
		goto fail; 
	}
    _prSFn( " ---- pcm amount : %d " , cnt ) ;

	*name_out = calloc(sizeof(char *) , (1+cnt)); // list_pcm
	if (*name_out == NULL) {
        _prSFn( " ---- ERROR end 3" ) ;
		goto fail;
    }

	*desc_out = calloc(sizeof(char *) , (1 + cnt));
	if (*desc_out == NULL) {
        _prSFn( " ---- ERROR end 5" ) ;
		goto fail;
    }

	/* the last one is a flag, NULL */
	name_out[cnt] = NULL;
	desc_out[cnt] = NULL;
	name = name_out;
	descr = desc_out;

	filter = stream == SND_PCM_STREAM_CAPTURE ? "Input" : "Output"; // list_pcm
	while (*n != NULL && i < cnt) {
		*name = snd_device_name_get_hint(*n, "NAME");
		*descr = snd_device_name_get_hint(*n, "DESC");
		io = snd_device_name_get_hint(*n, "IOID");
		if (name == NULL || 
			(io != NULL && strcmp(io, filter) != 0) ){ // list_pcm
			if (*name) free(*name);
			if (*descr) free(*descr);
		} else {
			if (*descr == NULL) {
				*descr = malloc(4);
				memset(*descr, 0, 4); // list_pcm
			}
			name++;
			descr++;
			i++;
		}
		if (io != NULL)
			free(io);
		n++;
	}
	snd_device_name_free_hint(hints); // list_pcm
    _prSFn( " ---- normal end %d " , cnt ) ;
	return cnt;
fail:
	_free_name_desc(name_out);
	_free_name_desc(desc_out);
	snd_device_name_free_hint(hints);
    _prSFn( " ---- ERROR end " ) ;
	return -1;
} // list_pcm

/* -------------------------------------
 * Interfaces 
 --------------------------------------*/ 
/* the device id is a pcm string name in linux */
record_dev_id  get_default_input_dev()
{
	record_dev_id id; 

    extern char * _argv01 ;
    if ( _argv01 != NULL ) {
	    id.u.name = _argv01 ;
    } else {
        #ifdef __i386__
	        id.u.name = "default" ; // arecord -L :
	        id.u.name = "sysdefault" ; // arecord -L :
	        //id.u.name = "front" ; // arecord -L :
	        id.u.name = "hw:0,0" ; // arecord -L :
        #else
            if ( 1 ) {
	            id.u.name = "default";
        	    id.u.name = "sysdefault" ;
            } else {
	            id.u.name = "hw:0,2";
            }
        #endif
    }

    _prSFn( "------ return id.u.name %s" , id.u.name );
	return id;
} // get_default_input_dev

record_dev_id * list_input_device() 
{
	// TODO: unimplemented
	return NULL;
} // list_input_device

int get_input_dev_num()
{
	return get_pcm_device_cnt(SND_PCM_STREAM_CAPTURE);
} // list_input_device


/* callback will be run on a new thread */
int create_recorder(struct recorder ** out_rec, 
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
} // create_recorder

void destroy_recorder(struct recorder *___rec)
{
	if(!___rec)
		return;

	free(___rec);
} // destroy_recorder

int _open_recorder5(struct recorder * ___rec, record_dev_id dev, WAVEFORMATEX * fmt)
{
	int ret = 0;
	if(!___rec )
		return -RECORD_ERR_INVAL;
	if(___rec->state >= RECORD_STATE_READY) {
		return 0;
    }

	ret = open_recorder_internal(___rec, dev, fmt);
	if(ret == 0) {
		___rec->state = RECORD_STATE_READY;
    }
	return 0;

} // _open_recorder5

void close_recorder(struct recorder *___rec)
{
	if(___rec == NULL || ___rec->state < RECORD_STATE_READY)
		return;
	if(___rec->state == RECORD_STATE_RECORDING)
		stop_record(___rec);

	___rec->state = RECORD_STATE_CLOSING;

	close_recorder_internal(___rec);

	___rec->state = RECORD_STATE_CREATED;	
} // close_recorder

int start_record(struct recorder * ___rec)
{
	int ret;
	if(___rec == NULL) {
		return -RECORD_ERR_INVAL;
    }
	if( ___rec->state < RECORD_STATE_READY) {
        _prSFn( "------ why not ready : %d , %d " , ___rec->state , RECORD_STATE_READY );
		return -RECORD_ERR_NOT_READY;
    }
	if( ___rec->state == RECORD_STATE_RECORDING) {
		return 0;
    }

	ret = _start_record_internal((snd_pcm_t *)___rec->wavein_hdl);
	if(ret == 0)
		___rec->state = RECORD_STATE_RECORDING;
	return ret;
} // start_record

int stop_record(struct recorder * ___rec)
{
	int ret;
	if(___rec == NULL)
		return -RECORD_ERR_INVAL;
	if( ___rec->state < RECORD_STATE_RECORDING)
		return 0;

	___rec->state = RECORD_STATE_STOPPING;
	ret = _stop_record_internal((snd_pcm_t *)___rec->wavein_hdl);
	if(ret == 0) {		
		___rec->state = RECORD_STATE_READY;
	}
	return ret;
} // stop_record

int is_record_stopped(struct recorder *___rec)
{
	if(___rec->state == RECORD_STATE_RECORDING)
		return 0;

	return _is_stopped_internal(___rec);
} // is_record_stopped
