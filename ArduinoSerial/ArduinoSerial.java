/**
 *
 *
 *
 */


import gnu.io.*;
import java.io.*;
import java.util.*;



public class ArduinoSerial implements SerialPortEventListener
{
    public boolean debug = false;

    static int           rate = 57600;
    static final int databits = 8;
    static final int parity   = SerialPort.PARITY_NONE;
    static final int stopbits = SerialPort.STOPBITS_1;

    public SerialPort port    = null;
    public String portname    = "/dev/tty.usbmodem000014FD1";

    /** the serial input stream, normally you don't need access to this */
    public InputStream input;
    /** the serial output stream, normally you don't need access to this */
    public OutputStream output;

    
    public static void main(String[] args) {
        new ArduinoSerial(args);
    }

    public ArduinoSerial(String[] args) {

        boolean connected = open_port();

        if( connected ) {
            
            while( true ) {
                try {
                    String msg = "abcdefghijklmnopqrstuvwxyz\r\n";
                    //{ 'a','b','c','d','e','f','g','i','j'};
                    byte[] msgbytes = msg.getBytes();
                    //System.out.println("writing...");
                    output.write('h');
                    output.write('e');
                    output.write('l');
                    output.write('l');
                    output.write('o');
                    output.write(msgbytes);
                    Thread.sleep(100);
                } catch (Exception e) { // null pointer or serial port dead
                    e.printStackTrace();
                }
            }
        }
        else { 
            logmsg("not connected");
        }
    }

    /**
     *
     */
    private boolean open_port() {
        boolean success = false;
        try {
            Enumeration portList = CommPortIdentifier.getPortIdentifiers();
            while (portList.hasMoreElements()) {
                CommPortIdentifier portId =
                    (CommPortIdentifier) portList.nextElement();
                if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
                    //System.out.println("found " + portId.getName());
                    if (portId.getName().equals(portname)) {
                        //logmsg("open_port:"+ portId);
                        port = (SerialPort)portId.open("arduino serial", 2000);
                        //port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
                        port.disableReceiveThreshold();
                        port.enableReceiveTimeout(3000);
                        port.setSerialPortParams(rate,
                                                 SerialPort.DATABITS_8,
                                                 SerialPort.STOPBITS_1,
                                                 SerialPort.PARITY_NONE); 
                        input  = port.getInputStream();
                        output = port.getOutputStream();
                        //port.setSerialPortParams(rate,databits,stopbits,parity);
                        port.addEventListener(this);
                        port.notifyOnDataAvailable(true);
                        logmsg("port "+portname+" opened successfully");
                        success = true;
                    }
                }
            }
      
        } catch (Exception e) {
            logmsg("connect failed: "+e);
            port = null;
            input = null;
            output = null;
        }
                        
        return success;
    }


    /**
     * callback for SerialPortEventListener
     * (from processing.serial.Serial)
     */
    synchronized public void serialEvent(SerialPortEvent serialEvent) {
        try {
            logmsg("serialEvent:"+serialEvent+", nvail:"+input.available());
            if (serialEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
                while (input.available() > 0) {
                    System.out.print( (char)input.read() );  // HACK
                } // while
            }
        } catch (IOException e) {
            logmsg("serialEvent " + e);
        }
    }
    
    /**
     * just a little debug 
     */
    public void logmsg(String msg) {
        if(debug) 
            System.err.println("ArduinoSerial("+System.currentTimeMillis()+
                               "):"+ msg);
    }


}
