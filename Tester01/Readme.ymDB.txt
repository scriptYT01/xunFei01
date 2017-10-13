
########################## 1.
the target file is : /bin/ymDbg01.x1000.bin 

########################## 2.
when running it with "-h" parameter , the prog will show the following and exit.

adb -s 0123456789ABCDE2 shell /bin/ymDbg01.x1000.bin -h
argc : 2
argV 0: len <22> , content </bin/ymDbg01.x1000.bin>
argV 1: len <2> , content <-h>
option -h




Usage :
 /bin/ymDbg01.x1000.bin [<-d> | <-h> | <-p XX> | <-l YY> ]
     -h , --help   : show debug info / usage only.
     -d , --debug  : listen only , waiting the use play the awake / words
     -p , --play   : awake , then play the specified word item only.
     -pp           : list all single test can be use
     -l , --list   : awake , then play the specified word list.
     -ll           : list all list can be use
     no parameter  : play the default all word items , use about 10 - 15 minuts.

########################## 3.
if no parameter given , the testbench will try to use the all 76 words to take a full test,
use about 10-15 minuts.

########################## 4.
when manual test is need ( the person to awake and speak the word ),
run it with "-d" parameter.
then , it will listen only and report active/inactive state, 
or report the recognized-words if received.

########################## 5.
if running with "-p 11" then the testbench will try awake and test the NO.11 wave only.
"--play 11" can be use at the same time.
if running with "-pp" , it will show all the wav files can be used as parameter.

########################## 6.
if running with "-l 0" then the testbench will try awake and test the "Group.0" WAV only.
"--list 0" can be use at the same time.
if running with "-ll" , it will show all the WAV groups can be used as parameter.

########################## 7.
the factary test command :
adb -s 0123456789ABCDE2 shell "/bin/ymDbg01.x1000.bin -l 0"



