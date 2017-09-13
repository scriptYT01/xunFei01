/*
* AIUIAgentTest.h
*
*  Created on: 2017年3月9日
*      Author: hj
*/

/* #include "patchXF/superStream.h" */
#include "patchXF/streamController.h"

#ifndef AIUIAGENTTEST_H_
#define AIUIAGENTTEST_H_

#include "aiui/AIUI.h"


#include <string>
#include <iostream>

#include "FileUtil.h"

#ifdef _MSC_VER
#include <windows.h>
#include <process.h>
#define TEST_ROOT_DIR ".\\AIUI"
#define CFG_FILE_PATH TEST_ROOT_DIR##"\\cfg\\aiui.cfg"
#define TEST_AUDIO_PATH TEST_ROOT_DIR##"\\audio\\test.pcm"
#define LOG_DIR TEST_ROOT_DIR##"\\log"
#else
#include <unistd.h>
#include <pthread.h>
#define TEST_ROOT_DIR "./AIUI/"
#define CFG_FILE_PATH "./AIUI/cfg/aiui.cfg"
#define TEST_AUDIO_PATH "./AIUI/audio/test.pcm"
#define LOG_DIR "./AIUI/log"
#endif

#include "patchXF/pathcXFbase.h"

using namespace aiui;
using namespace std;


class WriteAudioThread
{
private:
	IAIUIAgent* mAgent;

	string mAudioPath;

	bool mRepeat;

	bool mRun;

	FileUtil::DataFileHelper* mFileHelper;

  pthread_t thread_id;
  bool thread_created;

private:
	bool threadLoop();

  static void* thread_proc(void * paramptr);

public:
	WriteAudioThread(IAIUIAgent* agent, const string& audioPath, bool repeat);

	~WriteAudioThread();

	void stopRun();

	bool runWAT();

}; // class WriteAudioThread


class AIUITester;
class TestListener : public IAIUIListener
{
private:
    AIUITester * _aiui ;
public:
	void onEvent(IAIUIEvent& event);
    void _setCaller1( AIUITester * ___aiui ) { _aiui = ___aiui ; };
}; // class TestListener 

class AIUITester
{
private:
	IAIUIAgent* agent;

	TestListener listener;

	WriteAudioThread * writeThread;

    //string _fname01 = TEST_AUDIO_PATH ;
    string _fname01 ;
public:
    _streamControllerIN     _inSC     ;
    _streamControllerOUT    _outSC    ;
    _streamControllerOUT    _dbgS1    ;
    _streamControllerOUT    _dbgS2    ;
public:
    AIUITester() ;
    ~AIUITester();
private:

	void createAgent();
	void wakeup();
	void start();
	void stop();
	void writeAiui(bool repeat);
	void stopWriteThread();
	void reset();
	void writeText();
	void destory();

	void _waveCMDauto01();
    string _cmdToStr( int ) ;
    string _eventToStr( int ) ;
    string _stateToStr( int ) ;
    string _vadToStr( int ) ;
	void _dumpStatus();
    int _sleepWaitForState01( int ___gapUS, int ___gapMax , int * ___dst , int ___wanted );
    int _sleepWaitForState02( int ___gapUS, int ___gapMax , int * ___dst , int ___wanted );
    int _sleepWaitForState11( int ___gapUS, int ___gapMax , int * ___lastNLP01 , int *___lastNLP02 , int ___idleWait ) ;
    //string _jsonGetResult12( string ___inStr , string ___node1 , string ___node2 );
    void usage();


public:
	void readCmd();
	void testAiui();
}; // class AIUITester



#endif /* AIUIAGENTTEST_H_ */
