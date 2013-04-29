
arduino-serial -- C code to talk to Arduino
===========================================

Original URL: http://todbot.com/blog/2006/12/06/arduino-serial-c-code-to-talk-to-arduino/

Usage is:

<pre>
% ./arduino-serial
Usage: arduino-serial -p <serialport> [OPTIONS]

Options:
  -h, --help                   Print this help message
  -p, --port=serialport        Serial port Arduino is on
  -b, --baud=baudrate          Baudrate (bps) of Arduino (default 9600)
  -s, --send=string            Send string to Arduino
  -S, --sendline=string        Send string with newline to Arduino
  -r, --receive                Receive string from Arduino & print it out
  -n  --num=num                Send a number as a single byte
  -F  --flush                  Flush serial port buffers for fresh reading
  -d  --delay=millis           Delay for specified milliseconds
  -q  --quiet                  Don't print out as much info

Note: Order is important. Set '-b' baudrate before opening port'-p'.
      Used to make series of actions: '-d 2000 -s hello -d 100 -r'
      means 'wait 2secs, send 'hello', wait 100msec, get reply'
</pre>

