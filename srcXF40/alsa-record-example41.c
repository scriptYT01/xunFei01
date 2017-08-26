/* 
  A Minimal Capture Program

  This program opens an audio interface for capture, configures it for
  stereo, 16 bit, 44.1kHz, interleaved conventional read/write
  access. Then its reads a chunk of random data from it, and exits. It
  isn't meant to be a real program.

  From on Paul David's tutorial : http://equalarea.com/paul/alsa-audio.html

  Fixes rate and buffer problems

  sudo apt-get install libasound2-dev
  gcc -o alsa-record-example -lasound alsa-record-example.c && ./alsa-record-example hw:0
*/

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

#include "patchXF/pathcXFbase.h"
	      
char *  _dev_name01 = "default" ;
int     _chAmount = 2 ;
time_t  _Time01 ;
time_t  _Time02 ;
int64_t _Time03 ;
int64_t _Time04 ;

int main (int ___argc, char *___argv[])
{
    int __i01;
    int err;
    char *buffer;
    int buffer_frames = 128;
    //unsigned int rate = 44100;
    unsigned int rate = 16000;
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

        _prEFn( " arg c <%d>" , ___argc ) ; 
        _prEFn( " usage : %s [_dev_name01] [_chAmount] " , ___argv[0] );

    if ( ___argc >=2 && ___argv[1] != NULL && ___argv[1][0] != 0 ) {
        _dev_name01 = ___argv[1] ; 
        if ( 0 ) {
            _prEFn( " arg c <%d>" , ___argc ) ; 
            _prEFn( " arg 1 <%s>" , _dev_name01 ) ; 
            _prEFn( " arg 0 <%s>" , ___argv[0] ) ; 
            _prEFn( " arg 1 <%s>" , ___argv[1] ) ; 
            _prEFn( " arg 2 <%s>" , ___argv[2] ) ; 
        }
    }
    if ( ___argc >=3 && ___argv[2] != NULL && ___argv[2][0] != 0 ) {
        _chAmount = atoi( ___argv[2] ) ;
    }

    _prEFn( " ------ trying snd_pcm_open  <%s>" , _dev_name01 ) ; 

    //int snd_pcm_open( snd_pcm_t **handle, int card, int device, int mode );
    if ((err = snd_pcm_open (&capture_handle, _dev_name01, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        _prEFn( "cannot open audio device %s (%d:%s)", 
                _dev_name01,
                err , snd_strerror (err));
        exit (1);
    }
    _prEFn( "audio interface opened" );


    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
        _prEFn( "cannot allocate hardware parameter structure (%d:%s)",
                err , snd_strerror (err));
        exit (1);
    }
    _prEFn( "hw_params mallocated succeed." );

    if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
        _prEFn( "cannot initialize hardware parameter structure (%d:%s)",
                err , snd_strerror (err));
        exit (1);
    }
    _prEFn( "hw_params initialized" );

    if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        _prEFn( "cannot set access type (%d:%s)",
                err, snd_strerror (err));
        exit (1);
    }
    _prEFn( "hw_params access setted");

    _prEFn( "trying to set hw_params formate to <%d : %s>" , format , 
            format==SND_PCM_FORMAT_S16_LE?"SND_PCM_FORMAT_S16_LE":"unknown format" );
    if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, format)) < 0) {
        _prEFn( "cannot set sample format (%d:%s)",
                err, snd_strerror (err));
        exit (1);
    }
    _prEFn( "hw_params format setted");

    _prEFn( "trying to set hw_params rate to %d" , rate);
    if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
        _prEFn( "cannot set sample rate (%d:%s)",
                err, snd_strerror (err));
        exit (1);
    }
    _prEFn( "hw_params rate setted");

    _prEFn( " ----- trying to set channel amount to :%d" , _chAmount ) ;
    if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, _chAmount )) < 0) {
        _prEFn( "cannot set channel count (%d:%s)",
                err, snd_strerror (err));
        exit (1);
    }
    _prEFn( "hw_params channels setted");

    if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
        _prEFn( "cannot set parameters (%d:%s)",
                err, snd_strerror (err));
        exit (1);
    }
    _prEFn( "hw_params setted");

    snd_pcm_hw_params_free (hw_params);
    _prEFn( "hw_params freed");

    if ((err = snd_pcm_prepare (capture_handle)) < 0) {
        _prEFn( "cannot prepare audio interface for use (%d:%s)",
                err, snd_strerror (err));
        exit (1);
    }
    _prEFn( "audio interface prepared");

    buffer = malloc(128 * snd_pcm_format_width(format) / 8 * 2);
    _prEFn( "buffer allocated");

    _Time01 = time(0) ;
    for (__i01 = 0; __i01 < 100000; ++__i01) {
        if ((err = snd_pcm_readi (capture_handle, buffer, buffer_frames)) != buffer_frames) {
            _prEFn( "read from audio interface failed (%d:%s)",
                    err, snd_strerror (err));
            exit (1);
        }
        if ( __i01 % 1000 == 1 ) {
            _Time02 = time(0) ; _Time03 = (int) (_Time02 - _Time01) ; _Time04 = _Time03 * 1000000 / (__i01+1) ;
            _prEFn( "read %5d done , %lld , %lld us/perREAD ", __i01 , _Time03 , _Time04 );
        }
    }
    free(buffer);
            _Time02 = time(0) ; _Time03 = (int) (_Time02 - _Time01) ; _Time04 = _Time03 * 1000000 / (__i01+1) ;
            _prEFn( "buffer freed : %d end , %lld , %lld us/perREAD " , __i01 , _Time03 , _Time04 );

    snd_pcm_close (capture_handle);
    _prEFn( "audio interface closed");

    exit (0);
} /* main */

