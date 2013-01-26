pihat
=====
This is cloned from original pihat http://www.skagmo.com/page.php?p=projects/22_pihat

Status support is added according to http://www.picaxeforum.co.uk/showthread.php?16509-El-cheapo-mains-remote&p=147355&viewfull=1#post147355

to run it download pihat to your pi and run

chmod +x ./pihat

To pair
./pihat --brand=5 --repeats=50 --id=0 --channel=7 --state=1

To turn on
./pihat --brand=5 --id=0 --channel=7 --state=1

To turn off
./pihat --brand=5 --id=0 --channel=7 --state=0


id is the remote id between 0 - 262143. 65536 - 131071 for Morrison remote and 196608 - 262143 for Status remote.

State takes value from 0 (off) to 1 (on). Note for ALL button (aka channel 0) either states will results off action since that's there is no ALL On button on Status Remotes. 

channel takes value from 0 to 7 with the following mappings:

0 -> ALL

7 -> button 1

6 -> button 4

5 -> button 3

3 -> button 2

If you want to use your FM radio to record the transmission you can specify --frequency=1 to the above commands and turn your radio to 100 MHz.
