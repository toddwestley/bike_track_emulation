# bike_track_emulation
Using bluetooth low energy Wahoo speed sensor to emulate riding on a track 

I wrote this program to dispaly graphically the output from a Wahoo speed sensor mounted to my rear wheel.  The bike is mounted on
a Yaheetech Premium Steel Bike Bicycle Indoor Exercise Bike Stationary Workout Trainer Stand that I got from Amazon.  

To debug the program I used an old front wheel with an idential Wahoo speed sensor.

The command to start the program is:

for i in {3..1}
do
	echo $i
	sleep 1
done
echo "GO GO GO GO GO GO GO!!!!!!!!!!!!!!!!!!!!!"
echo "GO GO GO GO GO GO GO!!!!!!!!!!!!!!!!!!!!!"
echo "GO GO GO GO GO GO GO!!!!!!!!!!!!!!!!!!!!!"
echo "GO GO GO GO GO GO GO!!!!!!!!!!!!!!!!!!!!!"
echo "GO GO GO GO GO GO GO!!!!!!!!!!!!!!!!!!!!!"
echo "GO GO GO GO GO GO GO!!!!!!!!!!!!!!!!!!!!!"
#gatttool -b E5:97:EE:07:27:F2 -t random --char-write-req --handle=0x0025 --value=0100 --listen >gattoutput.txt
gatt_todd_tool -b E5:97:EE:07:27:F2 -t random --char-write-req --handle=0x0025 --value=0100 --listen 2>&1 | tee gattoutput.txt
#atttool -b D5:1A:DE:86:93:E9 -t random --char-write-req --handle=0x0025 --value=0100 --listen 2>&1 | tee gattoutput.tx
#	D5:1A:DE:86:93:E9	on truing stand
#	E5:97:EE:07:27:F2	TRAINER
# start pedalling
# then activate MyWorkout

I need time to walk from my computer to the bike.  [I wear cycling shoes with SPD cleats.]
