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
    _dpS1( " %s \n" ,  _eventToStr( _lastEventType00 ).c_str() ) ;
    _dpS1( " %s \n" ,  _stateToStr( _lastEventType11 ).c_str() ) ;
    _dpS1( " %s %s %s \n" ,  " =--7= sleep : " , SSTR( _lastEventType31 ).c_str() , " =--8= " ) ;
    _dpS1( " %s \n" ,  _vadToStr( _lastEventType41 ).c_str() ) ;
    _dpS1( " %s %s %s \n" ,  " =--8= error : " , SSTR( _lastEventType61 ).c_str() , " =--0= " ) ;
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
    time_t __t01 ;
    time_t __t02 ;

    if ( 0 ) { 
        _dbgS1 . _scSelfTest01() ; 
        _prExit( " debuging SS " );
    } 

    _dpS1( " supply with argv[1] , then use it as the input as wav file name and deal with it. " ) ;
    _prEFn( " supply with argv[1] , then use it as the input as wav file name and deal with it. " ) ;


    if ( _argc != 2 && _argc != 3 ) {
	    _dpS1( "\n useage : " " %s %d " " <wave_input.wav>\n\n\n"  , _argv[0] , _argc ) ;
        exit(33);
    }

	_dpS1( "\n _waveCMDauto01 start : %d " , _argc );
    _dumpStatus();
	_dpS1( "\n _waveCMDauto01 0121  : %d " , _argc );

    createAgent();
    //sleep(1);
    __i01 = _sleepWaitForState01( 100000 , 10 , &_lastEventType11 , AIUIConstant::STATE_READY ) ;
	_dpS1 ( " 11 repeate (%d) time , result : %d --> %s \n" , __i01 , _lastEventType11 , _stateToStr( _lastEventType11 ).c_str() ) ;
    if ( 0 ) { exit(11); }

    //stopWriteThread();
    wakeup();
    __i01 = _sleepWaitForState01( 100000 , 100 , &_lastEventType11 , AIUIConstant::STATE_WORKING ) ;
	_dpS1 ( " 22 repeate (%d) time , result : %d --> %s \n" , __i01 , _lastEventType11 , _stateToStr( _lastEventType11 ).c_str() ) ;
    if ( 0 ) { exit(22); }

    _lastNlp01 = -1 ;
    _lastNlp02 = -1 ;
    writeAiui(false);

    __t01 = time( 0 ) ;
    while ( 1 ) {
        __t02 = time( 0 ) ;
        _dpS1 ( 
                " ======= 1 aiui start unit it is killed. : %s , %s : %d \n" 
                , _argv[0]
                , basename(_argv[0])
                , (__t02 - __t01) 
                ) ;
        sleep( 100 ) ;
    }

    if ( 0 ) {
        __i01 = _sleepWaitForState02( 100000 , 200 , &_lastEventType11 , AIUIConstant::STATE_WORKING ) ;
	    _dpS1 ( " 33 repeate (%d) time , result : %d --> %s \n" , __i01 , _lastEventType11 , _stateToStr( _lastEventType11 ).c_str() ) ;
    } 
    if ( _fname01 != "-" ) 
    {
        __i01 = _sleepWaitForState11( 100000 , 200 , &_lastNlp01 , &_lastNlp02 , 4 ) ;
	    _dpS1 ( " 44 NLP result (%d) time , result : %d -- %d --> %s \n" , __i01 , _lastNlp01 , _lastNlp02 , time(0) ) ;
    }
    if ( _fname01 == "-" ) 
    {
        __i01 = _sleepWaitForState11( 100 * 1000000 , 200 , &_lastNlp01 , &_lastNlp02 , 4 ) ;
	    _dpS1 ( " 44 NLP result (%d) time , result : %d -- %d --> %s \n" , __i01 , _lastNlp01 , _lastNlp02 , time(0) ) ;
    }

    if ( 1 ) { _prEFn( " nothing got " ) ; exit(33); }

    wakeup();
    sleep(1);
    start();
    sleep(1);
    wakeup();
    sleep(1);
    writeAiui(false);

	_dpS1( "\n _waveCMDauto01 end : %d \n\n\n" , _argc ) ;

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

int __answerCNT_all = 0 ;
int __answerCNT_ask = 0 ;
int __answerCNT_answer = 0 ;
int __answerCNT_unEnv = 0 ;
int __answerCNT_NotFound = 0 ;
int __answerCNT_NULL = 0 ;
int __answerCNT_EE = 0 ;

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
                            _aiui -> _dpS1n( "EVENT_STATE:%s" , "IDLE" );
                        } 
                        break;

                    case AIUIConstant::STATE_READY:
                        {
                            _aiui -> _dpS1n( "EVENT_STATE:%s" , "STATE_READY" );
                        } 
                        break;

                    case AIUIConstant::STATE_WORKING:
                        {
                            _aiui -> _dpS1n( "EVENT_STATE:%s" , "WORKING" );
                        } 
                        break;
                }
            } 
            break;

        case AIUIConstant::EVENT_WAKEUP: // onEvent    2
            {
                _aiui -> _dpS1n( "EVENT_WAKEUP:%s" , event.getInfo() );
            } 
            break;

        case AIUIConstant::EVENT_SLEEP: // onEvent 3
            {
                _lastEventType31 = event.getArg1() ;
                _aiui -> _dpS1n( "EVENT_SLEEP:arg1=:%d" , _lastEventType31 ) ;// onEvent 31
            } 
            break;

        case AIUIConstant::EVENT_VAD: // onEvent 4 
            {
                _lastEventType41 = event.getArg1() ;
                switch ( _lastEventType41 ) { // onEvent 41
                    case AIUIConstant::VAD_BOS:
                        {
                            _aiui -> _dpS1n( "EVENT_VAD:%s" , "BOS" ) ;// onEvent 41
                        } 
                        break;

                    case AIUIConstant::VAD_EOS:
                        {
                            _aiui -> _dpS1n( "EVENT_VAD:%s" , "EOS" ) ;// onEvent 41
                        } 
                        break;

                    case AIUIConstant::VAD_VOL:
                        {
                            if(0) { 
                                _aiui -> _dpS1n( "EVENT_VAD:%s" , "VOL" ) ;// onEvent 41
                            }
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
                    _aiui -> _dpS1n( "parse error! %s" , event.getInfo() ) ;
                    break;
                }
                Json::Value data = (bizParamJson["data"])[0];
                Json::Value params = data["params"];
                Json::Value content = (data["content"])[0];
                string sub = params["sub"].asString();
                _aiui -> _dpS1n( "EVENT_RESULT:start:%s %s %d " , sub.c_str() , __func__ , __LINE__ );

                if (sub == "nlp")
                {
                    Json::Value empty;
                    Json::Value contentId = content.get("cnt_id", empty);

                    _lastNlp01 = _lastNlp02 ;
                    _lastNlp02 = time(0);

                    if (contentId.empty())
                    {
                        _aiui -> _dpS1n( "Content Id is empty" ) ;
                        break;
                    }

                    string cnt_id = contentId.asString();
                    Buffer* buffer = event.getData()->getBinary(cnt_id.c_str()); // onEvent 52
                    string resultStr;
                    string __answer1 ;
                    string __answer2 ;

                    if (NULL != buffer)
                    {
                        __answerCNT_all ++ ;
                        resultStr = string((char*)buffer->data());

                        _aiui -> _dpS1n( "==get json : begin" ) ;
                        _aiui -> _dpS1n( "%s" , resultStr.c_str() ) ;
                        _aiui -> _dpS1n( "==get json : end" ) ;
                        __answer1 = _jsonGetResult14( resultStr , "intent" , "answer" , "text" , "" ) ;
                        if ( "" !=  __answer1 ) {
                            //if ( 0 != strncmp( __answer1.c_str() , "jsonError" , strlen( "jsonError" ) ) ) {
                            if ( 0 != __answer1 . find( "jsonError" ) ) {

                                __answerCNT_ask++ ;
                                __answerCNT_answer++ ;

                                __answer2 = _jsonGetResult14( resultStr , "intent" , "text" , "" , "" ) ;
                                _aiui -> _dpS1n( " %d : %d : %d " ":get_result81:ask11:%s" 
                                        , __answerCNT_ask ,  __answerCNT_ask + __answerCNT_unEnv 
                                        , __answerCNT_all , __answer2.c_str() ) ;

                                _aiui -> _dpOUT( "%s\n" , __answer1.c_str() ) ;
                                _aiui -> _dpS1n( " %d : %d : " "get_result02:answer02:%s" 
                                        , __answerCNT_answer , __answerCNT_all , __answer1.c_str() );
                            } else {
                                __answer2 = _jsonGetResult14( resultStr , "intent" , "text" , "" , "" ) ;
                                if ( 0 != __answer2 . find( "jsonError" ) ) {
                                    __answerCNT_unEnv ++ ;
                                    _aiui -> _dpS1n( "%d : %d " ":get_result89:unENV:%s" 
                                            , __answerCNT_unEnv , __answerCNT_all , __answer2.c_str() );
                                } else {
                                    _aiui -> _dpS1n( "%d : %d " ":get_result97 : jsonError found found. <%s>"
                                            , __answerCNT_NotFound , __answerCNT_all , __answer2.c_str() );
                                }
                            }
                        } else {
                            _aiui -> _dpS1n( "%d : %d " ":get_result99 : NULL found." , __answerCNT_NULL , __answerCNT_all ) ;
                        }
                        _aiui -> _dpS1n( "%d : %d " ":get_resultEE:%d" , __answerCNT_EE , __answerCNT_all , __answerCNT_all ) ;
                    }
                }
                _aiui -> _dpS1n( "EVENT_RESULT:end:%s : %s %d , %d" , sub.c_str() , __func__ , __LINE__ , time(0) );

            }
            break;

        case AIUIConstant::EVENT_ERROR: // onEvent 6
            {
                _lastEventType61 = event.getArg1() ;
                _aiui -> _dpS1n( "EVENT_ERROR:%d " , _lastEventType61 ) ;
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
        _dpS1n( "\n open file <%s> as input \n\n\n" , _fname01.c_str() );
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
    
   _dpS1n( "input cmd:" ); 

	string cmd;
	while (true)
	{
		cin >> cmd;

		if (cmd == "c")
		{
			_dpS1n( "createAgent" ); // AIUITester::readCmd
			createAgent();
		} else if (cmd == "w") {
			_dpS1n( "wakeup" );
			wakeup();
		} else if (cmd == "s") {
			_dpS1n( "start" );
			start();
		} else if (cmd == "st") {
			_dpS1n( "stop" );
			stop();
		} else if (cmd == "d") {
			_dpS1n( "destroy" ); // AIUITester::readCmd
			destory();
		} else if (cmd == "r") {
			_dpS1n( "reset" );
			reset();
		} else if (cmd == "e") {
			_dpS1n( "exit" );
			break;
		} else if (cmd == "wr") {
			_dpS1n( "writeAiui" );
			writeAiui(false);
		} else if (cmd == "wrr") {
			_dpS1n( "writeAiui repeatly" ); // AIUITester::readCmd
			writeAiui(true);
		} else if (cmd == "swrt" || cmd == "stwr"|| cmd == "S") {
			_dpS1n( "stopWriteThread" );
			stopWriteThread();
		} else if (cmd == "wrt") {
			_dpS1n( "writeText" );
			writeText();
		} else if (cmd == "q") {
			destory();
			break;
		} else if (cmd == "ww") {
			_dpS1n( "stop-wakeup-writeAiui" ); // AIUITester::readCmd
			stopWriteThread();
			wakeup();
			start();
			wakeup();
			writeAiui(false);
			;
		} else if (cmd == "h") { // AIUITester::readCmd
            dbmemDebugShow();


			_dpS1n( 
                    "c    : createAgent"             "\n"
                    "w    : wakeup"                  "\n"
                    "s    : start"                   "\n"
                    "st   : stop"                    "\n"
                    "d    : destroy"                 "\n"
                    "r    : reset"                   "\n"
                    "e    : exit"                    "\n"
                    "wr   : writeAiui"               "\n"
                    "wrr  : writeAiui repeatly"      "\n"
                    "swrt : stopWriteThread"         "\n"
                    "wrt  : writeText"               "\n"
                    "ww   : stop-wakeup-writeAiui"   "\n"
                    ) ;
        } else {
			_dpS1n( "invalid cmd, input again." ); // AIUITester::readCmd
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
    extern string  _progNameS ;
    string  __ss ;

	//		AIUISetting::setSaveDataLog(true);
	AIUISetting::setAIUIDir(TEST_ROOT_DIR);
	AIUISetting::initLogger(LOG_DIR);

    usage() ;

	cerr 
        << " input argc: " 
        << _argc 
        << endl
        ;

    __ss = ">:/tmp/log.01." + _progNameS + ".txt"    ;
    _zExit( _dbgS1 . _addPath2( __ss . c_str()      , " debug-text-01 " ) 
            , "debug stream error : <%s> <%s> " , _argv[1] , _argv[2] ) ;

    __ss = ">:/tmp/log.02." + _progNameS + ".txt"    ;
    _zExit( _dbgS2 . _addPath2( __ss . c_str()      , " debug-text-02 " ) 
            , "debug stream error : <%s> <%s> " , _argv[1] , _argv[2] ) ;

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

            _zExit( _outSC . _addPath2( _argv[2] , " output-text-to-tts_1 " ) 
                    , "output stream error : <%s> <%s> " , _argv[1] , _argv[2] ) ;

            _nExit( 0 , " debuging output-text-to-tts_2 " ) 

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
