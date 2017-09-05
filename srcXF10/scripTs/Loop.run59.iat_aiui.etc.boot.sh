#!/bin/sh

cd /tmp/

bb1=/ch/xf10/Loop.run51.tts_service_for_aiui.sh
bb2=/ch/xf10/Loop.run52.iat_aiui.sh

SS1="$1"

[ -n "${SS1}" ] || SS1=start

case "$1" in
    start)

        echo "$0 begin."

        if [ ! -f ${bb1} ]
        then
            echo
            echo " file <${bb1}> don't exist. exit. "
            echo
            exit
        fi
        if [ ! -f ${bb2} ]
        then
            echo
            echo " file <${bb2}> don't exist. exit. "
            echo
            exit
        fi


        nohup ${bb1} > log51.txt &
        sleep 3 ;

        nohup ${bb2} > log52.txt &

        echo "$0 end."
        ;;
    stop)
        ;;
    restart|reload)
        ;;
    *)
        echo "Usage: $0 {start|stop|restart}"
        exit 1
esac

exit $?


