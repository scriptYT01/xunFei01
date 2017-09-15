
#ifndef     __WAVE01_H
#define     __WAVE01_H

#include "base01.h"

#ifndef WAVE_FORMAT_PCM  
#define WAVE_FORMAT_PCM  1
typedef struct tWAVEFORMATEX {
	unsigned short	  wFormatTag;
	unsigned short    nChannels;
	unsigned int      nSamplesPerSec;
	unsigned int      nAvgBytesPerSec;
	unsigned short	  nBlockAlign;
	unsigned short    wBitsPerSample;
	unsigned short    cbSize;
} WAVEFORMATEX;
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void _dumpWAVEFORMATEX( char * ___hMsg , char * ___tMsg , const char * ___varName , WAVEFORMATEX * ___wavfmt ) ;
extern int text_to_speech_from_file_continue(FILE * ___fd , const char* params);
#ifdef __cplusplus
}
#endif

#endif
