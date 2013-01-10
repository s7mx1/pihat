pihat
=====
This is cloned from original pihat http://www.skagmo.com/page.php?p=projects/22_pihat

Status support is added according to http://www.picaxeforum.co.uk/showthread.php?16509-El-cheapo-mains-remote&p=147355&viewfull=1#post147355

to run it download pihat to your pi and run

chmod +x ./pihat

To pair
./pihat --repeats=50 --channel=0 --state=15

To turn on
./pihat --channel=0 --state=15

To turn off
./pihat --channel=0 --state=7

channel is the remote id between 0 to 65535

state takes value from 0 to 15. According to the picaxeforum its used for "button code / command ( 4-bits )" but due to limited information I have not firgued out the parren yet. However according to the sample


15 (1111) is on for button 1 


7  (0111) is off for button 1

