# defines
TARGETIP=192.168.0.28
OLDPROGRAMPATH=/home/pi/guarani
EXECUTABLE=build/guarani
# make the target
cd build && make -j4
cd ..
# kill gdbserver on target
ssh pi@$TARGETIP killall gdbserver
# remove old executable on target
ssh pi@$TARGETIP rm $OLDPROGRAMPATH
# copy over new executable
scp $EXECUTABLE pi@$TARGETIP:/home/pi/guarani
# start gdb on target (IS ONE LONG COMMAND)
ssh -n -f pi@$TARGETIP "sh -c 'cd /home/pi; nohup gdbserver localhost:3000 ./guarani > /dev/null 2>&1 &'"