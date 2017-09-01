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
#include "cJSON.h"

int _lastEventType00 = -1 ;
int _lastEventType11 = -1 ;
int _lastEventType31 = -1 ;
int _lastEventType41 = -1 ;
int _lastEventType61 = -1 ;
int _lastNlp01 = -1 ;
int _lastNlp02 = -1 ;

char _numstr[21]; // enough to hold all numbers up to 64-bits

// #define _strX( bb ) # bb
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
    cerr << _eventToStr( _lastEventType00 ) << endl ;
    cerr << _stateToStr( _lastEventType11 ) << endl ;
    cerr << " =--7= sleep : " << SSTR( _lastEventType31 ) << " =--8= " << endl ;
    cerr << _vadToStr( _lastEventType41 ) << endl ;
    cerr << " =--8= error : " << SSTR( _lastEventType61 ) << " =--0= " << endl ;

} // AIUITester::_dumpStatus

// wait the ___wanted event equal to ___dst , first appears, then exit;or timeout will exit.
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

// wait the ___wanted event equal to ___dst , first appears, or timeout then exit.
// the next step , wait untile the state change to different than dst, then exit, or timeout exit.
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

//string AIUITester::_jsonGetResult14( string ___inStr , string ___node1 , string ___node2 )
string _jsonGetResult14( string ___inStr , string ___node1 , string ___node2 , string ___node3 , string ___node4 )
{
#if 1
    string __rtStr ;
    cJSON * __jN0 ;
    cJSON * __jN1 ;
    cJSON * __jN2 ;
    cJSON * __jN3 ;
    cJSON * __jN4 ;

    __jN0 = cJSON_Parse( ___inStr . c_str() );
    if ( NULL == __jN0 ) return "jsonError00" ;

    if ( ___node1 == "" ) { __jN1 = __jN0 ; } else {
        __jN1 = cJSON_GetObjectItemCaseSensitive(   __jN0 ,       ___node1 . c_str() ) ;
        if ( NULL == __jN1 ) return "jsonError01:<" + ___node1 + ">";
    }
    if ( ___node2 == "" ) { __jN2 = __jN1 ; } else {
        __jN2 = cJSON_GetObjectItemCaseSensitive(   __jN1 ,       ___node2 . c_str() ) ;
        if ( NULL == __jN2 ) return "jsonError02:<" + ___node2 + ">";
    }
    if ( ___node3 == "" ) { __jN3 = __jN2 ; } else {
        __jN3 = cJSON_GetObjectItemCaseSensitive(   __jN2 ,       ___node3 . c_str() ) ;
        if ( NULL == __jN3 ) return "jsonError03:<" + ___node3 + ">";
    }
    if ( ___node4 == "" ) { __jN4 = __jN3 ; } else {
        __jN4 = cJSON_GetObjectItemCaseSensitive(   __jN3 ,       ___node4 . c_str() ) ;
        if ( NULL == __jN4 ) return "jsonError04:<" + ___node4 + ">";
    }

    //__rtStr = cJSON_Print( __jN4 ) ;
    __rtStr = cJSON_PrintUnformatted( __jN4 ) ;
    //__rtStr = *(__jN4 -> string) ;
    return __rtStr ;
#else
    return "" ;
#endif
} // AIUITester::_jsonGetResult14

extern int      _argc ;
extern char **  _argv ;
void AIUITester::_waveCMDauto01()
{
    int __i01 ;

    _prEFn( " supply with argv[1] , then use it as the input as wav file name and deal with it. " ) ;

    if ( _argc != 2 && _argc != 3 ) {
	    cerr << "\n useage : " << _argv[0] << " <wave_input.wav>\n\n\n" << _argc << endl;
        exit(33);
    }

	//cerr << "\n _waveCMDauto01 start "  << _argc << "\n\n\n"<< endl;
    _dumpStatus();
	//cerr << "\n _waveCMDauto01 0121 "   << _argc << "\n\n\n" << endl;

    createAgent();
    //sleep(1);
    __i01 = _sleepWaitForState01( 100000 , 10 , &_lastEventType11 , AIUIConstant::STATE_READY ) ;
	cerr << " 11 repeate ("  << __i01 << ") time , result : " << SSTR( _lastEventType11 ) << " --> " << _stateToStr( _lastEventType11 ) << endl;
    if ( 0 ) { exit(11); }

    //stopWriteThread();
    wakeup();
    __i01 = _sleepWaitForState01( 100000 , 100 , &_lastEventType11 , AIUIConstant::STATE_WORKING ) ;
	cerr << " 22 repeate ("  << __i01 << ") time , result : " << SSTR( _lastEventType11 ) << " --> " << _stateToStr( _lastEventType11 ) << endl;
    if ( 0 ) { exit(22); }

    _lastNlp01 = -1 ;
    _lastNlp02 = -1 ;
    writeAiui(false);
    if ( 0 ) {
        __i01 = _sleepWaitForState02( 100000 , 200 , &_lastEventType11 , AIUIConstant::STATE_WORKING ) ;
	    cerr << " 33 repeate ("  << __i01 << ") time , result : " << SSTR( _lastEventType11 ) << " --> " << _stateToStr( _lastEventType11 ) << endl;
    } 
    if ( _fname01 != "-" ) 
    {
        __i01 = _sleepWaitForState11( 100000 , 200 , &_lastNlp01 , &_lastNlp02 , 4 ) ;
	    cerr << " 34 NLP result ("  << __i01 << ") time , result : " << SSTR( _lastNlp01 ) 
            << " -- " << SSTR( _lastNlp02 ) << " -- " << TSTR << endl ;
    }
    if ( _fname01 == "-" ) 
    {
        __i01 = _sleepWaitForState11( 100 * 1000000 , 200 , &_lastNlp01 , &_lastNlp02 , 4 ) ;
	    cerr << " 35 NLP result ("  << __i01 << ") time , result : " << SSTR( _lastNlp01 ) 
            << " -- " << SSTR( _lastNlp02 ) << " -- " << TSTR << endl ;
    }

    if ( 1 ) { _prEFn( " nothing got " ) ; exit(33); }

    wakeup();
    sleep(1);
    start();
    sleep(1);
    wakeup();
    sleep(1);
    writeAiui(false);

	cerr << "\n _waveCMDauto01 end \n\n\n" << _argc << endl;

} // AIUITester::_waveCMDauto01





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

bool WriteAudioThread::runWAT()
{
    if (thread_created == false) {
        int rc = pthread_create(&thread_id, NULL, thread_proc, this);
        if (rc != 0) {
            _prEFn( " create pthread error " ) ;
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
                            cerr << "EVENT_STATE:" << "IDLE" << endl;
                        } 
                        break;

                    case AIUIConstant::STATE_READY:
                        {
                            cerr << "EVENT_STATE:" << "READY" << endl;
                        } 
                        break;

                    case AIUIConstant::STATE_WORKING:
                        {
                            cerr << "EVENT_STATE:" << "WORKING" << endl;
                        } 
                        break;
                }
            } 
            break;

        case AIUIConstant::EVENT_WAKEUP: // onEvent    2
            {
                cerr << "EVENT_WAKEUP:" << event.getInfo() << endl;
            } 
            break;

        case AIUIConstant::EVENT_SLEEP: // onEvent 3
            {
                _lastEventType31 = event.getArg1() ;
                cerr << "EVENT_SLEEP:arg1=" << _lastEventType31 << endl; // onEvent 31
            } 
            break;

        case AIUIConstant::EVENT_VAD: // onEvent 4 
            {
                _lastEventType41 = event.getArg1() ;
                switch ( _lastEventType41 ) { // onEvent 41
                    case AIUIConstant::VAD_BOS:
                        {
                            cerr << "EVENT_VAD:" << "BOS" << endl;
                        } 
                        break;

                    case AIUIConstant::VAD_EOS:
                        {
                            cerr << "EVENT_VAD:" << "EOS" << endl;
                        } 
                        break;

                    case AIUIConstant::VAD_VOL:
                        {
                            if(0) { cerr << "EVENT_VAD:" << "VOL" << endl;}
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
                    cerr << "parse error!" << endl << event.getInfo() << endl;
                    break;
                }
                Json::Value data = (bizParamJson["data"])[0];
                Json::Value params = data["params"];
                Json::Value content = (data["content"])[0];
                string sub = params["sub"].asString();
                cerr << "EVENT_RESULT:start:" << sub << __func__ << " " << __LINE__ << endl; // onEvent 51

                if (sub == "nlp")
                {
                    Json::Value empty;
                    Json::Value contentId = content.get("cnt_id", empty);

                    _lastNlp01 = _lastNlp02 ;
                    _lastNlp02 = time(0);

                    if (contentId.empty())
                    {
                        cerr << "Content Id is empty" << endl;
                        break;
                    }

                    string cnt_id = contentId.asString();
                    Buffer* buffer = event.getData()->getBinary(cnt_id.c_str()); // onEvent 52
                    string resultStr;
                    string __answer ;

                    if (NULL != buffer)
                    {
                        resultStr = string((char*)buffer->data());

                        cerr << "==get json : begin" << endl;
                        cerr << resultStr << endl;
                        cerr << "==get json : end" << endl;
                        //__answer = _jsonGetResult14( resultStr , "intent" , "answer" , "text" ) ;
                        __answer = _jsonGetResult14( resultStr , "intent" , "answer" , "text" , "" ) ;
                        if ( "" !=  __answer ) {
                            cerr << "get_answer01:" << __answer << endl ;
                            //if ( 0 != strncmp( __answer.c_str() , "jsonError" , strlen( "jsonError" ) ) ) {
                            if ( 0 != _strcmpX1( "jsonError" , __answer.c_str() ) ) {
                                _aiui -> _outSC . _sendMsg1n( __answer ) ;
                            }
                        } else {
                            cerr << "no get_answer01 found." << endl ;
                        }
                    }
                }
                cerr << "EVENT_RESULT:end:" << sub << ":" << __func__ << " " << __LINE__ << TSTR << endl;

            }
            break;

        case AIUIConstant::EVENT_ERROR: // onEvent 6
            {
                _lastEventType61 = event.getArg1() ;
                cerr << "EVENT_ERROR:" << _lastEventType61 << endl; // onEvent 61
            } 
            break;
    }
} // TestListener::onEvent

AIUITester::AIUITester() 
    : 
    agent(NULL)
    , writeThread(NULL)
    , _fname01 ( TEST_AUDIO_PATH )
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

	string paramStr = FileUtil::readFileAsString( CFG_FILE_PATH );
    listener . _setCaller1( this ) ;
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


void AIUITester::writeAiui(bool repeat)
{
	if (agent == NULL)
		return;

	if (writeThread == NULL) {
	    cerr << "\n open file <" << _fname01 << "> as input \n\n\n" << endl;
		//writeThread = new WriteAudioThread(agent, TEST_AUDIO_PATH,  repeat);
		writeThread = new WriteAudioThread(agent, _fname01,  repeat);
		writeThread->runWAT();
	} else {
        // if , writeThread is already created, skip ..... to prevent double thread is created.
    }
} // AIUITester::writeAiui



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

    _prEFn( " supply no argv[1] , then wait for the stdin's cmd " );
    
	cerr << "input cmd:" << endl;

	string cmd;
	while (true)
	{
		cin >> cmd;

		if (cmd == "c")
		{
			cerr << "createAgent" << endl; // AIUITester::readCmd
			createAgent();
		} else if (cmd == "w") {
			cerr << "wakeup" << endl;
			wakeup();
		} else if (cmd == "s") {
			cerr << "start" << endl;
			start();
		} else if (cmd == "st") {
			cerr << "stop" << endl;
			stop();
		} else if (cmd == "d") {
			cerr << "destroy" << endl; // AIUITester::readCmd
			destory();
		} else if (cmd == "r") {
			cerr << "reset" << endl;
			reset();
		} else if (cmd == "e") {
			cerr << "exit" << endl;
			break;
		} else if (cmd == "wr") {
			cerr << "writeAiui" << endl;
			writeAiui(false);
		} else if (cmd == "wrr") {
			cerr << "writeAiui repeatly" << endl; // AIUITester::readCmd
			writeAiui(true);
		} else if (cmd == "swrt" || cmd == "stwr"|| cmd == "S") {
			cerr << "stopWriteThread" << endl;
			stopWriteThread();
		} else if (cmd == "wrt") {
			cerr << "writeText" << endl;
			writeText();
		} else if (cmd == "q") {
			destory();
			break;
		} else if (cmd == "ww") {
			cerr << "stop-wakeup-writeAiui" << endl; // AIUITester::readCmd
			stopWriteThread();
			//cerr << "111" << endl;
            //usleep(1000);
            //sleep(5);
			//cerr << "222" << endl;
			wakeup();
			start();
			wakeup();
			//cerr << "333" << endl;
            //usleep(1000000);
            //sleep(10);
			//cerr << "444" << endl;
			writeAiui(false);
			//cerr << "555" << endl;
			;
		} else if (cmd == "h") { // AIUITester::readCmd
            dbmemDebugShow();


			cerr 
			<< "c    : createAgent"             << endl 
			<< "w    : wakeup"                  << endl 
			<< "s    : start"                   << endl 
			<< "st   : stop"                    << endl 
			<< "d    : destroy"                 << endl 
			<< "r    : reset"                   << endl 
			<< "e    : exit"                    << endl 
			<< "wr   : writeAiui"               << endl 
			<< "wrr  : writeAiui repeatly"      << endl 
			<< "swrt : stopWriteThread"         << endl 
			<< "wrt  : writeText"               << endl 
			<< "ww   : stop-wakeup-writeAiui"   << endl
            ;
		} else {
			cerr << "invalid cmd, input again." << endl; // AIUITester::readCmd
		}
        _dumpStatus();
	} // while
} // AIUITester::readCmd

void AIUITester::usage()
{
	cerr 
        << endl
        << " usage : "
        << _argv[0] 
        << " [input.wav | listenIP listenPort | file filename | file -] "
        << endl
        << endl
        ;
} // AIUITester::usage

void AIUITester::testAiui()
{
	//		AIUISetting::setSaveDataLog(true);
	AIUISetting::setAIUIDir(TEST_ROOT_DIR);
	AIUISetting::initLogger(LOG_DIR);

    usage() ;

	cerr 
        << " input argc: " 
        << _argc 
        << endl
        ;

    switch ( _argc ) {
        case 1 : // 0 para -- > wait for stdin to input cmd
            readCmd();
            break ;
        case 2 : // 1 para -- > deal with the para 1 as the input-wave-file-name , using the automaticly process.
            _fname01 = _argv[1] ;
            _waveCMDauto01();
            break ;
        case 3 : // 2 para -- > deal with the para 1 as the input-wave-file-name , using the automaticly process.
                 // 2 para -- > deal with the para 2 as the outputProtocol , using the automaticly process.
            _fname01 = _argv[1] ;

            //_zExit( _outSC . _addPath2<_streamBase>( _argv[2] , " output-text-to-tts" ) 
            _zExit( _outSC . _addPath2<_streamOUT>( _argv[2] , " output-text-to-tts" ) 
                    , "output stream error : <%s> <%s> " , _argv[1] , _argv[2] ) ;

            _waveCMDauto01();
            break ;
        default : 
            cerr 
                << endl
                << " unknow parameter amount : " 
                << _argc
                << endl
                << endl
                ;
            break;
    } 
} // AIUITester::testAiui
