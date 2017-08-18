#include "AIUITest.h"

#include <string.h>
#include <stdlib.h>
#include <sstream>
#include "jsoncpp/json/json.h"

#define SSTR( x ) \
    static_cast< std::ostringstream & >( \
            ( std::ostringstream() << std::dec << x ) ).str()

#define TSTR   (" " + SSTR( time(0) ) + " ")

extern "C" {
    void dbmemDebugShow( void );
}

string _fname01 = TEST_AUDIO_PATH ;
int _lastEventType00 = -1 ;
int _lastEventType11 = -1 ;
int _lastEventType31 = -1 ;
int _lastEventType41 = -1 ;
int _lastEventType61 = -1 ;
int _lastNlp01 = -1 ;
int _lastNlp02 = -1 ;

char _numstr[21]; // enough to hold all numbers up to 64-bits

#define _strX( bb ) # bb
#define _id2str( aa ) \
    case (AIUIConstant::aa) : \
    __str += " ==-C= " ; \
    __str += _strX( aa ) ; \
    __str += ":" ; \
    __str += SSTR( (AIUIConstant::aa)+0)  ; \
    __str += " " ; \
    __str += " ==-D= " ; \
    break 
//#define _unStr( saved , msg , id )  snprintf( _numstr , 20 , "%d" , id ) ; saved = msg + _numstr 
#define _unStr( saved , msg , id )  \
    saved += msg ; \
    saved += ":" ; \
    saved += SSTR( id ) ;
string AIUITester::_cmdToStr( int ___eventID ) 
{
    string __str = "" ;
    if ( 1 ) { __str = +" =--A= " ; } else     { __str += " " ; }
    switch ( ___eventID ) {
        _id2str( CMD_RESET ) ;
        _id2str( CMD_START ) ;
        _id2str( CMD_STOP ) ;
        _id2str( CMD_STOP_WRITE ) ;
        _id2str( CMD_WAKEUP ) ;
        _id2str( CMD_WRITE ) ;
        default : _unStr( __str , " unknowCMD " , ___eventID ) ;
    }
    if ( 1 ) { __str = +" =--B= " ; } else     { __str += " " ; }
    return __str ;
} // AIUITester::_cmdToStr
string AIUITester::_eventToStr( int ___eventID ) 
{
    string __str = "" ;
    if ( 1 ) { __str += " =--1= " ; } else     { __str += " " ; }
    switch ( ___eventID ) {
        _id2str( EVENT_ERROR ) ;
        _id2str( EVENT_RESULT ) ;
        _id2str( EVENT_SLEEP ) ;
        _id2str( EVENT_STATE ) ;
        _id2str( EVENT_VAD ) ;
        _id2str( EVENT_WAKEUP ) ;
        default : _unStr( __str , " unknowEvent " , ___eventID ) ;
    }
    if ( 1 ) { __str += " =--2= " ; } else     { __str += " " ; }
    return __str ;
} // AIUITester::_eventToStr
string AIUITester::_stateToStr( int ___eventID ) 
{
    string __str = "" ;
    if ( 1 ) { __str += " =--3= " ; } else     { __str += " " ; }
    switch ( ___eventID ) {
        _id2str( STATE_IDLE ) ;
        _id2str( STATE_READY ) ;
        _id2str( STATE_WORKING ) ;
        default : _unStr( __str , " unknowSTATE " , ___eventID ) ;
    }
    if ( 1 ) { __str += " =--4= " ; } else     { __str += " " ; }
    return __str ;
} // AIUITester::_stateToStr
string AIUITester::_vadToStr( int ___eventID ) 
{
    string __str = "" ;
    if ( 1 ) { __str += " =--5= " ; } else     { __str += " " ; }
    switch ( ___eventID ) {
        _id2str( VAD_BOS ) ;
        _id2str( VAD_EOS ) ;
        _id2str( VAD_VOL ) ;
        default : _unStr( __str , " unknowVAD " , ___eventID ) ;
    }
    if ( 1 ) { __str += " =--6= " ; } else     { __str += " " ; }
    return __str ;
} // AIUITester::_vadToStr

void AIUITester::_dumpStatus()
{
    cout << _eventToStr( _lastEventType00 ) << endl ;
    cout << _stateToStr( _lastEventType11 ) << endl ;
    cout << " =--7= sleep : " << SSTR( _lastEventType31 ) << " =--8= " << endl ;
    cout << _vadToStr( _lastEventType41 ) << endl ;
    cout << " =--8= error : " << SSTR( _lastEventType61 ) << " =--0= " << endl ;

} // AIUITester::_dumpStatus

int AIUITester::_sleepWaitForState01( int ___gapUS, int ___gapMax , int * ___dst , int ___wanted )
{
    int __checkCNT = 0 ;

    if ( ___gapUS <= 0 )    return -10001 ;
    if ( ___gapMax <= 0 )   return -10002 ;
    if ( ___dst == NULL )   return -10003 ;

    while ( __checkCNT < ___gapMax ) {
        if ( (*___dst) == ___wanted ) {
            return __checkCNT ;
        }
        usleep( ___gapUS ) ;
        __checkCNT ++ ;
    }
    if ( __checkCNT >= ___gapMax ) {
        return -(300000 + __checkCNT ) ;
    }
    return -1 ;
} // AIUITester::_sleepWaitForState01
int AIUITester::_sleepWaitForState02( int ___gapUS, int ___gapMax , int * ___dst , int ___wanted )
{
    int __checkCNT = 0 ;

    if ( ___gapUS <= 0 )    return -10001 ;
    if ( ___gapMax <= 0 )   return -10002 ;
    if ( ___dst == NULL )   return -10003 ;

    while ( __checkCNT < ___gapMax ) {
        if ( (*___dst) == ___wanted ) {
            break ;
        }
        usleep( ___gapUS ) ;
        __checkCNT ++ ;
    }
    if ( __checkCNT >= ___gapMax ) { // _sleepWaitForState02
        return -2 ;
    }
    while ( __checkCNT < ___gapMax ) {
        usleep( ___gapUS ) ;
        if ( (*___dst) != ___wanted ) {
            return __checkCNT ;
        }
        __checkCNT ++ ;
    }
    if ( __checkCNT >= ___gapMax ) {
        return -(300000 + __checkCNT ) ;
    }
    return -1 ;
} // AIUITester::_sleepWaitForState02
int AIUITester::_sleepWaitForState11( int ___gapUS, int ___gapMax , int * ___lastNLP01 , int *___lastNLP02 , int ___idleWait )
{
    int __checkCNT = 0 ;

    if ( ___gapUS <= 0 )            return -1000001 ;
    if ( ___gapMax <= 0 )           return -1000002 ;
    if ( ___lastNLP01 == NULL )     return -1000003 ;
    if ( ___lastNLP02 == NULL )     return -1000004 ;
    if ( ___idleWait <= 0 )           return -1000002 ;

    while ( __checkCNT < ___gapMax ) {
        if ( (*___lastNLP01) > 0 && (*___lastNLP02) > 0 ) {
            break ;
        }
        usleep( ___gapUS ) ;
        __checkCNT ++ ;
    }
    if ( __checkCNT >= ___gapMax ) {
        return -2 ;
    }
    while ( __checkCNT < ___gapMax ) { // _sleepWaitForState11
        usleep( ___gapUS ) ;
        if ( (time(0)) - (*___lastNLP02) >= ___idleWait ) {
            return __checkCNT ;
        }
        __checkCNT ++ ;
    }
    if ( __checkCNT >= ___gapMax ) {
        return -(300000 + __checkCNT ) ;
    }
    return -1 ;
} // AIUITester::_sleepWaitForState11

extern int      _argc ;
extern char **  _argv ;
void AIUITester::_autoCmd01()
{
    int __i01 ;
    if ( _argc != 2 ) {
	    cout << "\n useage : " << _argv[0] << " <wave_input.wav>\n\n\n" << _argc << endl;
        exit(33);
    }
    _fname01 = _argv[1] ;

	//cout << "\n _autoCmd01 start "  << _argc << "\n\n\n"<< endl;
    _dumpStatus();
	//cout << "\n _autoCmd01 0121 "   << _argc << "\n\n\n" << endl;

    createAgent();
    //sleep(1);
    __i01 = _sleepWaitForState01( 100000 , 10 , &_lastEventType11 , AIUIConstant::STATE_READY ) ;
	cout << " 11 repeate ("  << __i01 << ") time , result : " << SSTR( _lastEventType11 ) << " --> " << _stateToStr( _lastEventType11 ) << endl;
    if ( 0 ) { exit(11); }

    //stopWriteThread();
    wakeup();
    __i01 = _sleepWaitForState01( 100000 , 100 , &_lastEventType11 , AIUIConstant::STATE_WORKING ) ;
	cout << " 22 repeate ("  << __i01 << ") time , result : " << SSTR( _lastEventType11 ) << " --> " << _stateToStr( _lastEventType11 ) << endl;
    if ( 0 ) { exit(22); }

    _lastNlp01 = -1 ;
    _lastNlp02 = -1 ;
    write(false);
    if ( 0 ) {
        __i01 = _sleepWaitForState02( 100000 , 200 , &_lastEventType11 , AIUIConstant::STATE_WORKING ) ;
	    cout << " 33 repeate ("  << __i01 << ") time , result : " << SSTR( _lastEventType11 ) << " --> " << _stateToStr( _lastEventType11 ) << endl;
    } else {
        __i01 = _sleepWaitForState11( 100000 , 200 , &_lastNlp01 , &_lastNlp02 , 4 ) ;
	    cout << " 34 NLP result ("  
            << __i01 
            << ") time , result : " 
            << SSTR( _lastNlp01 ) 
            << " -- " 
            << SSTR( _lastNlp02 ) 
            << " -- " 
            << TSTR
            << endl ;
    }
    if ( 1 ) { exit(33); }

    wakeup();
    sleep(1);
    start();
    sleep(1);
    wakeup();
    sleep(1);
    write(false);

	cout << "\n _autoCmd01 end \n\n\n" << _argc << endl;

} // AIUITester::_autoCmd01





bool WriteAudioThread::threadLoop()
{
	char audio[1279];
	int len = mFileHelper->read(audio, 1279);

	if (len > 0)
	{
		Buffer* buffer = Buffer::alloc(len);
		memcpy(buffer->data(), audio, len);

		IAIUIMessage * writeMsg = IAIUIMessage::create(AIUIConstant::CMD_WRITE,
			0, 0,  "data_type=audio,sample_rate=16000", buffer);	

		if (NULL != mAgent)
		{
			mAgent->sendMessage(writeMsg);
		}		
		writeMsg->destroy();
		usleep(10 * 1000);
	} else {
		if (mRepeat)
		{
			mFileHelper->rewindReadFile();
		} else {
			IAIUIMessage * stopWrite = IAIUIMessage::create(AIUIConstant::CMD_STOP_WRITE,
				0, 0, "data_type=audio,sample_rate=16000");

			if (NULL != mAgent)
			{
				mAgent->sendMessage(stopWrite);
			}
			stopWrite->destroy();

			mFileHelper->closeReadFile();
			mRun = false;
		}
	}

	return mRun;
}


void* WriteAudioThread::thread_proc(void * paramptr)
{
	WriteAudioThread * self = (WriteAudioThread *)paramptr;

	while (1) {
		if (! self->threadLoop())
			break;
	}
	return NULL;
}

WriteAudioThread::WriteAudioThread(IAIUIAgent* agent, const string& audioPath, bool repeat):
mAgent(agent), mAudioPath(audioPath), mRepeat(repeat), mRun(true), mFileHelper(NULL)
,thread_created(false)
{
	mFileHelper = new FileUtil::DataFileHelper("");
	mFileHelper->openReadFile(mAudioPath, false);
}

WriteAudioThread::~WriteAudioThread()
{
	
}

void WriteAudioThread::stopRun()
{
    if (thread_created) {
        mRun = false;
        void * retarg;
        pthread_join(thread_id, &retarg);
        thread_created = false;
    }
}

bool WriteAudioThread::run()
{
    if (thread_created == false) {
        int rc = pthread_create(&thread_id, NULL, thread_proc, this);
        if (rc != 0) {
            exit(-1);
        }
        thread_created = true;
        return true;
    }

    return false;
}

void TestListener::onEvent(IAIUIEvent& event)
{
    _lastEventType00 = event.getEventType() ;
    switch ( _lastEventType00 ) {
    //switch (event.getEventType()) {
        case AIUIConstant::EVENT_STATE: // onEvent 1 // :3
            {
                _lastEventType11 = event.getArg1() ;
                switch ( _lastEventType11 ) { // onEvent    11
                    case AIUIConstant::STATE_IDLE:
                        {
                            cout << "EVENT_STATE:" << "IDLE" << endl;
                        } 
                        break;

                    case AIUIConstant::STATE_READY:
                        {
                            cout << "EVENT_STATE:" << "READY" << endl;
                        } 
                        break;

                    case AIUIConstant::STATE_WORKING:
                        {
                            cout << "EVENT_STATE:" << "WORKING" << endl;
                        } 
                        break;
                }
            } 
            break;

        case AIUIConstant::EVENT_WAKEUP: // onEvent    2
            {
                cout << "EVENT_WAKEUP:" << event.getInfo() << endl;
            } 
            break;

        case AIUIConstant::EVENT_SLEEP: // onEvent 3
            {
                _lastEventType31 = event.getArg1() ;
                cout << "EVENT_SLEEP:arg1=" << _lastEventType31 << endl; // onEvent 31
            } 
            break;

        case AIUIConstant::EVENT_VAD: // onEvent 4 
            {
                _lastEventType41 = event.getArg1() ;
                switch ( _lastEventType41 ) { // onEvent 41
                    case AIUIConstant::VAD_BOS:
                        {
                            cout << "EVENT_VAD:" << "BOS" << endl;
                        } 
                        break;

                    case AIUIConstant::VAD_EOS:
                        {
                            cout << "EVENT_VAD:" << "EOS" << endl;
                        } 
                        break;

                    case AIUIConstant::VAD_VOL:
                        {
                            if(0) { cout << "EVENT_VAD:" << "VOL" << endl;}
                        } 
                        break;
                }
            } 
            break;

        case AIUIConstant::EVENT_RESULT: // onEvent 5
            {
                using namespace VA;
                Json::Value bizParamJson;
                Json::Reader reader;

                if (!reader.parse(event.getInfo(), bizParamJson, false)) {
                    cout << "parse error!" << endl << event.getInfo() << endl;
                    break;
                }
                Json::Value data = (bizParamJson["data"])[0];
                Json::Value params = data["params"];
                Json::Value content = (data["content"])[0];
                string sub = params["sub"].asString();
                cout << "EVENT_RESULT:start:" << sub << __func__ << " " << __LINE__ << endl; // onEvent 51

                if (sub == "nlp")
                {
                    Json::Value empty;
                    Json::Value contentId = content.get("cnt_id", empty);

                    _lastNlp01 = _lastNlp02 ;
                    _lastNlp02 = time(0);

                    if (contentId.empty())
                    {
                        cout << "Content Id is empty" << endl;
                        break;
                    }

                    string cnt_id = contentId.asString();
                    Buffer* buffer = event.getData()->getBinary(cnt_id.c_str()); // onEvent 52
                    string resultStr;

                    if (NULL != buffer)
                    {
                        resultStr = string((char*)buffer->data());

                        cout << resultStr << endl;
                    }
                }
                cout << "EVENT_RESULT:end:" << sub << ":" << __func__ << " " << __LINE__ << TSTR << endl;

            }
            break;

        case AIUIConstant::EVENT_ERROR: // onEvent 6
            {
                _lastEventType61 = event.getArg1() ;
                cout << "EVENT_ERROR:" << _lastEventType61 << endl; // onEvent 61
            } 
            break;
    }
} // TestListener::onEvent

AIUITester::AIUITester() : agent(NULL), writeThread(NULL)
{

}

AIUITester::~AIUITester()
{
	if (agent) {
		agent->destroy();
		agent = NULL;
	}
}

void AIUITester::createAgent()
{
	ISpeechUtility::createSingleInstance("", "",
		"appid=58f4654e");

	string paramStr = FileUtil::readFileAsString(CFG_FILE_PATH);
	agent = IAIUIAgent::createAgent(paramStr.c_str(), &listener);
}

void AIUITester::wakeup()
{
	if (NULL != agent)
	{
		IAIUIMessage * wakeupMsg = IAIUIMessage::create(AIUIConstant::CMD_WAKEUP);
		agent->sendMessage(wakeupMsg);
		wakeupMsg->destroy();
	}
}

void AIUITester::start()
{
	if (NULL != agent)
	{
		IAIUIMessage * startMsg = IAIUIMessage::create (AIUIConstant::CMD_START);
		agent->sendMessage(startMsg);
		startMsg->destroy();
	}
}

void AIUITester::stop()
{
	if (NULL != agent)
	{
		IAIUIMessage *stopMsg = IAIUIMessage::create (AIUIConstant::CMD_STOP);
		agent->sendMessage(stopMsg);
		stopMsg->destroy();
	}
}


void AIUITester::write(bool repeat)
{
	if (agent == NULL)
		return;

	if (writeThread == NULL) {
		//writeThread = new WriteAudioThread(agent, TEST_AUDIO_PATH,  repeat);
	    cout << "\n open file <" << _fname01 << "> as input \n\n\n" << endl;
		writeThread = new WriteAudioThread(agent, _fname01,  repeat);
		writeThread->run();
	}	
}



void AIUITester::stopWriteThread()
{
	if (writeThread) {
		writeThread->stopRun();
		delete writeThread;
		writeThread = NULL;
	}
}

void AIUITester::reset()
{
	if (NULL != agent)
	{
		IAIUIMessage * resetMsg = IAIUIMessage::create(AIUIConstant::CMD_RESET);
		agent->sendMessage(resetMsg);
		resetMsg->destroy();
	}
}
void AIUITester::writeText()
{
	if (NULL != agent)
	{
		string text = "刘德华的歌。";
		// textData内存会在Message在内部处理完后自动release掉
		Buffer* textData = Buffer::alloc(text.length());
		text.copy((char*) textData->data(), text.length());

		IAIUIMessage * writeMsg = IAIUIMessage::create(AIUIConstant::CMD_WRITE,
			0,0, "data_type=text", textData);	

		agent->sendMessage(writeMsg);
		writeMsg->destroy();		
	}
}

void AIUITester::destory()
{
	stopWriteThread();

	if (NULL != agent)
	{
		agent->destroy();
		agent = NULL;
	}

	//		SpeechUtility::getUtility()->destroy();
}

void AIUITester::readCmd()
{

	cout << "input argc:" << _argc << endl;
    if ( _argc > 1 ) {
        _autoCmd01( );
        return ;
    }
    
	cout << "input cmd:" << endl;

	string cmd;
	while (true)
	{
		cin >> cmd;

		if (cmd == "c")
		{
			cout << "createAgent" << endl; // AIUITester::readCmd
			createAgent();
		} else if (cmd == "w") {
			cout << "wakeup" << endl;
			wakeup();
		} else if (cmd == "s") {
			cout << "start" << endl;
			start();
		} else if (cmd == "st") {
			cout << "stop" << endl;
			stop();
		} else if (cmd == "d") {
			cout << "destroy" << endl; // AIUITester::readCmd
			destory();
		} else if (cmd == "r") {
			cout << "reset" << endl;
			reset();
		} else if (cmd == "e") {
			cout << "exit" << endl;
			break;
		} else if (cmd == "wr") {
			cout << "write" << endl;
			write(false);
		} else if (cmd == "wrr") {
			cout << "write repeatly" << endl; // AIUITester::readCmd
			write(true);
		} else if (cmd == "swrt" || cmd == "stwr"|| cmd == "S") {
			cout << "stopWriteThread" << endl;
			stopWriteThread();
		} else if (cmd == "wrt") {
			cout << "writeText" << endl;
			writeText();
		} else if (cmd == "q") {
			destory();
			break;
		} else if (cmd == "ww") {
			cout << "stop-wakeup-write" << endl; // AIUITester::readCmd
			stopWriteThread();
			//cout << "111" << endl;
            //usleep(1000);
            //sleep(5);
			//cout << "222" << endl;
			wakeup();
			start();
			wakeup();
			//cout << "333" << endl;
            //usleep(1000000);
            //sleep(10);
			//cout << "444" << endl;
			write(false);
			//cout << "555" << endl;
			;
		} else if (cmd == "h") { // AIUITester::readCmd
            dbmemDebugShow();


			cout 
			<< "c    : createAgent"         << endl 
			<< "w    : wakeup"              << endl 
			<< "s    : start"               << endl 
			<< "st   : stop"                << endl 
			<< "d    : destroy"             << endl 
			<< "r    : reset"               << endl 
			<< "e    : exit"                << endl 
			<< "wr   : write"               << endl 
			<< "wrr  : write repeatly"      << endl 
			<< "swrt : stopWriteThread"     << endl 
			<< "wrt  : writeText"           << endl 
			<< "ww   : stop-wakeup-write"   << endl
            ;
		} else {
			cout << "invalid cmd, input again." << endl; // AIUITester::readCmd
		}
        _dumpStatus();
	} // while
} // AIUITester::readCmd
void AIUITester::test()
{
	//		AIUISetting::setSaveDataLog(true);
	AIUISetting::setAIUIDir(TEST_ROOT_DIR);
	AIUISetting::initLogger(LOG_DIR);

	readCmd();
}
