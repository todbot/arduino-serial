arduino-serial -- C code to talk to Arduino
===========================================

Original URL: http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino/

Post about changes: http://todbot.com/blog/2013/04/29/arduino-serial-updated/


Usage
------
<pre>
laptop%  ./arduino-serial
Usage: arduino-serial -b <bps> -p <serialport> [OPTIONS]

Options:
  -h, --help                 Print this help message
  -b, --baud=baudrate        Baudrate (bps) of Arduino (default 9600)
  -p, --port=serialport      Serial port Arduino is connected to
  -s, --send=string          Send string to Arduino
  -S, --sendline=string      Send string with newline to Arduino
  -r, --receive              Receive string from Arduino & print it out
  -n  --num=num              Send a number as a single byte
  -F  --flush                Flush serial port buffers for fresh reading
  -d  --delay=millis         Delay for specified milliseconds
  -e  --eolchar=char         Specify EOL char for reads (default '\n')
  -t  --timeout=millis       Timeout for reads in millisecs (default 5000)
  -q  --quiet                Don't print out as much info

Note: Order is important. Set '-b' baudrate before opening port'-p'.
      Used to make series of actions: '-d 2000 -s hello -d 100 -r'
      means 'wait 2secs, send 'hello', wait 100msec, get reply'

</pre>


Downloads
---------
For convenience, here's some pre-built versions of arduino-serial. 
They may not be updated regularly, so compile it yourself if you can.
Click the "view raw" to get the actual zip file.

- https://github.com/todbot/arduino-serial/blob/master/arduino-serial-macosx.zip
- https://github.com/todbot/arduino-serial/blob/master/arduino-serial-linux.zip


Compilation
-----------
arduino-serial should compile on any POSIX-compatible system.
Tested on Mac OS X, Ubuntu Linux, Raspian Linux, Beaglebone Linux

To build, just check it out, make, and run it like:
<pre>
% git clone https://github.com/todbot/arduino-serial.git
% cd arduino-serial
% make
% ./arduino-serial
</pre>

For more details on the build process, see the Makefile.
