TARGETIP=192.168.0.28
OLDPROGRAMPATH=/home/pi/guarani
EXECUTABLE=build/guarani

cd build && ninja
cd ..

echo copying to $TARGETIP
# remove old executable on target
ssh pi@$TARGETIP rm $OLDPROGRAMPATH
# copy over new executable
scp $EXECUTABLE pi@$TARGETIP:/home/pi/guarani