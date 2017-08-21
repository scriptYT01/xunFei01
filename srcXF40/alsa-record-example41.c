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

#include <libgen.h>
#define _prSF( fmt , ... ) printf( "--debuging: %s %d %s : " fmt , basename(__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
#define _prSFn( fmt , ... ) _prSF( fmt "\n" , ## __VA_ARGS__ )
	      
char * _dev_name01 = "default" ;

int main (int ___argc, char *___argv[])
{
    int i;
    int err;
    char *buffer;
    int buffer_frames = 128;
    unsigned int rate = 44100;
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

        _prSFn( " arg c <%d>" , ___argc ) ; 

    if ( ___argc >=2 && ___argv[1] != NULL && ___argv[1][0] != 0 ) {
        _dev_name01 = ___argv[1] ; 
        if ( 0 ) {
            _prSFn( " arg c <%d>" , ___argc ) ; 
            _prSFn( " arg 1 <%s>" , _dev_name01 ) ; 
            _prSFn( " arg 0 <%s>" , ___argv[0] ) ; 
            _prSFn( " arg 1 <%s>" , ___argv[1] ) ; 
            _prSFn( " arg 2 <%s>" , ___argv[2] ) ; 
        }
    }

    _prSFn( " trying snd_pcm_open  <%s>" , _dev_name01 ) ; 

    if ((err = snd_pcm_open (&capture_handle, _dev_name01, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        _prSFn( "cannot open audio device %s (%s)\n", 
                _dev_name01,
                snd_strerror (err));
        exit (1);
    }
    _prSFn( "audio interface opened\n" );


    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
        _prSFn( "cannot allocate hardware parameter structure (%s)\n",
                snd_strerror (err));
        exit (1);
    }
    _prSFn( "hw_params mallocated succeed.\n" );

    if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
        _prSFn( "cannot initialize hardware parameter structure (%s)\n",
                snd_strerror (err));
        exit (1);
    }
    _prSFn( "hw_params initialized\n" );

    if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        _prSFn( "cannot set access type (%s)\n",
                snd_strerror (err));
        exit (1);
    }
    _prSFn( "hw_params access setted\n");

    if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, format)) < 0) {
        _prSFn( "cannot set sample format (%s)\n",
                snd_strerror (err));
        exit (1);
    }
    _prSFn( "hw_params format setted\n");

    if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
        _prSFn( "cannot set sample rate (%s)\n",
                snd_strerror (err));
        exit (1);
    }
    _prSFn( "hw_params rate setted\n");

    if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 2)) < 0) {
        _prSFn( "cannot set channel count (%s)\n",
                snd_strerror (err));
        exit (1);
    }
    _prSFn( "hw_params channels setted\n");

    if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
        _prSFn( "cannot set parameters (%s)\n",
                snd_strerror (err));
        exit (1);
    }
    _prSFn( "hw_params setted\n");

    snd_pcm_hw_params_free (hw_params);
    _prSFn( "hw_params freed\n");

    if ((err = snd_pcm_prepare (capture_handle)) < 0) {
        _prSFn( "cannot prepare audio interface for use (%s)\n",
                snd_strerror (err));
        exit (1);
    }
    _prSFn( "audio interface prepared\n");

    buffer = malloc(128 * snd_pcm_format_width(format) / 8 * 2);
    _prSFn( "buffer allocated\n");

    for (i = 0; i < 10; ++i) {
        if ((err = snd_pcm_readi (capture_handle, buffer, buffer_frames)) != buffer_frames) {
            _prSFn( "read from audio interface failed (%d:%s)\n",
                    err, snd_strerror (err));
            exit (1);
        }
        _prSFn( "read %d done\n", i);
    }
    free(buffer);
    _prSFn( "buffer freed\n");

    snd_pcm_close (capture_handle);
    _prSFn( "audio interface closed\n");

    exit (0);
} /* main */

