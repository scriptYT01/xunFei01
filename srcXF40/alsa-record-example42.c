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

#define _prSF(  fmt , ... ) printf( "--debuging: %s %d %s : " fmt  , basename(__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
#define _prSFn( fmt , ... ) _prSF( fmt "\n" ) 
	      
char * _dev_name01 = "default" ;

main (int ___argc, char *___argv[])
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
        _prSFn( " arg c <%d>" , ___argc ) ; 
        _prSFn( " arg 1 <%s>" , _dev_name01 ) ; 
        _prSFn( " arg 0 <%s>" , ___argv[0] ) ; 
        _prSFn( " arg 1 <%s>" , ___argv[1] ) ; 
        _prSFn( " arg 2 <%s>" , ___argv[2] ) ; 
    }

    _prSFn( " use device named <%s>" , _dev_name01 ) ; 

    return 0 ;
} // main


