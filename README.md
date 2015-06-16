# SMS from an Arduino using Tropo

This is a really simple system that uses Tropo to create a web service that
allows an IoT device such as an Arduino to send an SMS message to a phone. It
then sets up an Arduino to act as a temperature sensor and any time the
temperature goes above or below a certain range, it sends an SMS to a predefined
user.

![IoT](https://raw.githubusercontent.com/fluffy/TempAlarmSMS/master/Images/all.jpg)

## Create a Tropo account

Go to https://www.tropo.com and create an account. It's all free. (Just as full
disclosure, I work at Cisco and and Tropo is now part of Cisco ).

## Get it authorized for outgoing SMS 

You might have to open a support ticket with Tropo to have them validate you are
a real person and enable you to send outbound SMS.


## Create an Tropo App

* pick the "Scripting API"

* create a "New Script" and create a script that looks like 

    message("House temperature is " + temp + " C" , {
      to: "+14085551212", 
      network:"SMS", 
      callerID:"+14035551212"});

* don't worry about the phone numbers in it for now

* you should have a screen that looks along the lines of this

![Screen0](https://raw.githubusercontent.com/fluffy/TempAlarmSMS/master/Images/screen0.jpg) 


* once you have created it, you should see a screen along lines of following

![Screen1](https://raw.githubusercontent.com/fluffy/TempAlarmSMS/master/Images/screen1.jpg) 

* you need to note a few things here. The first is the the phone number that
Tropo assigned to your applications.

* Scrolling down a bit more you will see something like

![Screen2](https://raw.githubusercontent.com/fluffy/TempAlarmSMS/master/Images/screen2.jpg) 

* The second thing to note is the API key for
  messaging. Mine looks like something like `
  0a70119d927d9c4bbd4dc8f20bee794cfd5c387ddbd7c8798cd5dd273cad590730df8af3999bd5ce8399519a
  `  This is a secret and if others know it, they
  can use your account so it should be treated like a password. Don't worry, I
  changed mine before posting this so the one here is not valid but I wanted to
  show what they looked like.

## Edit your script

From the Tropo page for you App, go and "Edit script" for your TextScript. It
should look like

    message("House temperature is " + temp + " C" , {
       to: "+14085551212", 
       network:"SMS", 
       callerID:"+14035551212"});
     
but now you need to change the phone number in the callerID to be the phone
number that Tropo assigned to your App and you need to change the phone number
in the "to" line to be the number of your cell phone.

That's it - seriosly. That single Javascript function call is all you need to have a cloud server that sends SMS from your IoT device. I love Tropo :-) 

## Test it

Try it by going to the following URL in a browser but replace the token part
with your token

     http://api.tropo.com/1.0/sessions?action=create&token=0a70119d927d9c4bbd4dc8f20bee794cfd5c387ddbd7c8798cd5dd273cad590730df8af3999bd5ce8399519a&temp=12.3"

or getting that URL with curl. 

You should get a SMS to your phone. 

## Get Arduino libraries

Get the library files for OneWire temperature sensor  

* get the DallasTemperature.cpp and DallasTemperature.h file from
https://github.com/milesburton/Arduino-Temperature-Control-Library and put them
in your Arduino Library folder

* Download the OneWire.zip file from
http://www.pjrc.com/teensy/td_libs_OneWire.html and put the files in it in your
Arduino Library folder

## Make Arduino sketch 

Make a new Arduino sketch and put in it the file the stuff from the
TempAlarm.ino file at https://github.com/fluffy/TempAlarmSMS 

Edit this sketch and change several things

* change the mac address to be the one you can find on the sticker on the back
  of your ethernet shield

* change the token to be the API token for your Tropo app

* change the alarmLow and alarmHigh values to what you want. At first for
  testing you probably want them to be something where you can cause the device
  to send an SMS

*  Check it compiles and upload it to your Arduino

![Screen3](https://raw.githubusercontent.com/fluffy/TempAlarmSMS/master/Images/screen3.jpg) 

## Wire up the DS18B20 temperature sensor

* I'm using a DS18B20 but an DS1820, DS1822, DS18S20 should also work fine 

* I'm using pin 2 as the oneWire bus. It needs a pull up resistor and 4.7K is
  suggested but probably anything from 2K to 10K will work out fine. I ran power
  to the DS1820 but it can be run without that.

Schematic is:

![schematic](https://raw.githubusercontent.com/fluffy/TempAlarmSMS/master/Images/schematic.jpg) 

Looking top down on the breadboard so you can see which pins and which looks
like:

![breadboard](https://raw.githubusercontent.com/fluffy/TempAlarmSMS/master/Images/breadboard.jpg) 


## Run it

My setup looks like:

* I'm a big fan of PoE (Power over ethernet) so you can see this board is power
   that way right now




 

