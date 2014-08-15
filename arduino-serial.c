/*
 * arduino-serial
 * --------------
 *
 * A simple command-line example program showing how a computer can
 * communicate with an Arduino board. Works on any POSIX system (Mac/Unix/PC)
 *
 *
 * Compile with something like:
 *   gcc -o arduino-serial arduino-serial-lib.c arduino-serial.c
 * or use the included Makefile
 *
 * Mac: make sure you have Xcode installed
 * Windows: try MinGW to get GCC
 *
 *
 * Originally created 5 December 2006
 * 2006-2013, Tod E. Kurt, http://todbot.com/blog/
 *
 *
 * Updated 8 December 2006:
 *  Justin McBride discovered B14400 & B28800 aren't in Linux's termios.h.
 *  I've included his patch, but commented out for now.  One really needs a
 *  real make system when doing cross-platform C and I wanted to avoid that
 *  for this little program. Those baudrates aren't used much anyway. :)
 *
 * Updated 26 December 2007:
 *  Added ability to specify a delay (so you can wait for Arduino Diecimila)
 *  Added ability to send a binary byte number
 *
 * Update 31 August 2008:
 *  Added patch to clean up odd baudrates from Andy at hexapodia.org
 *
 * Update 6 April 2012:
 *  Split into a library and app parts, put on github
 *
 * Update 20 Apr 2013:
 *  Small updates to deal with flushing and read backs
 *  Fixed re-opens
 *  Added --flush option
 *  Added --sendline option
 *  Added --eolchar option
 *  Added --timeout option
 *  Added -q/-quiet option
 *
 */

#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // for usleep()
#include <getopt.h>

#include "arduino-serial-lib.h"


//
void usage(void)
{
    printf("Usage: arduino-serial -b <bps> -p <serialport> [OPTIONS]\n"
    "\n"
    "Options:\n"
    "  -h, --help                 Print this help message\n"
    "  -b, --baud=baudrate        Baudrate (bps) of Arduino (default 9600)\n"
    "  -p, --port=serialport      Serial port Arduino is connected to\n"
    "  -s, --send=string          Send string to Arduino\n"
    "  -S, --sendline=string      Send string with newline to Arduino\n"
    "  -i  --stdinput             Use standard input\n"
    "  -r, --receive              Receive string from Arduino & print it out\n"
    "  -n  --num=num              Send a number as a single byte\n"
    "  -F  --flush                Flush serial port buffers for fresh reading\n"
    "  -d  --delay=millis         Delay for specified milliseconds\n"
    "  -e  --eolchar=char         Specify EOL char for reads (default '\\n')\n"
    "  -t  --timeout=millis       Timeout for reads in millisecs (default 5000)\n"
    "  -q  --quiet                Don't print out as much info\n"
    "\n"
    "Note: Order is important. Set '-b' baudrate before opening port'-p'. \n"
    "      Used to make series of actions: '-d 2000 -s hello -d 100 -r' \n"
    "      means 'wait 2secs, send 'hello', wait 100msec, get reply'\n"
    "\n");
    exit(EXIT_SUCCESS);
}

//
void error(char* msg)
{
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    const int buf_max = 256;

    int fd = -1;
    char serialport[buf_max];
    int baudrate = 9600;  // default
    char quiet=0;
    char eolchar = '\n';
    int timeout = 5000;
    char buf[buf_max];
    int rc,n;

    if (argc==1) {
        usage();
    }

    /* parse options */
    int option_index = 0, opt;
    static struct option loptions[] = {
        {"help",       no_argument,       0, 'h'},
        {"port",       required_argument, 0, 'p'},
        {"baud",       required_argument, 0, 'b'},
        {"send",       required_argument, 0, 's'},
        {"sendline",   required_argument, 0, 'S'},
        {"stdinput",   no_argument,       0, 'i'},
        {"receive",    no_argument,       0, 'r'},
        {"flush",      no_argument,       0, 'F'},
        {"num",        required_argument, 0, 'n'},
        {"delay",      required_argument, 0, 'd'},
        {"eolchar",    required_argument, 0, 'e'},
        {"timeout",    required_argument, 0, 't'},
        {"quiet",      no_argument,       0, 'q'},
        {NULL,         0,                 0, 0}
    };

    while(1) {
        opt = getopt_long (argc, argv, "hp:b:s:S:i:rFn:d:qe:t:",
                           loptions, &option_index);
        if (opt==-1) break;
        switch (opt) {
        case '0': break;
        case 'q':
            quiet = 1;
            break;
        case 'e':
            eolchar = optarg[0];
            if(!quiet) printf("eolchar set to '%c'\n",eolchar);
            break;
        case 't':
            timeout = strtol(optarg,NULL,10);
            if( !quiet ) printf("timeout set to %d millisecs\n",timeout);
            break;
        case 'd':
            n = strtol(optarg,NULL,10);
            if( !quiet ) printf("sleep %d millisecs\n",n);
            usleep(n * 1000 ); // sleep milliseconds
            break;
        case 'h':
            usage();
            break;
        case 'b':
            baudrate = strtol(optarg,NULL,10);
            break;
        case 'p':
            if( fd!=-1 ) {
                serialport_close(fd);
                if(!quiet) printf("closed port %s\n",serialport);
            }
            strcpy(serialport,optarg);
            fd = serialport_init(optarg, baudrate);
            if( fd==-1 ) error("couldn't open port");
            if(!quiet) printf("opened port %s\n",serialport);
            serialport_flush(fd);
            break;
        case 'n':
            if( fd == -1 ) error("serial port not opened");
            n = strtol(optarg, NULL, 10); // convert string to number
            rc = serialport_writebyte(fd, (uint8_t)n);
            if(rc==-1) error("error writing");
            break;
        case 'S':
        case 's':
            if( fd == -1 ) error("serial port not opened");
            sprintf(buf, (opt=='S' ? "%s\n" : "%s"), optarg);

            if( !quiet ) printf("send string:%s\n", buf);
            rc = serialport_write(fd, buf);
            if(rc==-1) error("error writing");
            break;
        case 'i':
            rc=-1;
            if( fd == -1) error("serial port not opened");
            while(fgets(buf, buf_max, stdin)) {
                if( !quiet ) printf("send string:%s\n", buf);
                rc = serialport_write(fd, buf);
            }
            if(rc==-1) error("error writing");
            break;
        case 'r':
            if( fd == -1 ) error("serial port not opened");
            memset(buf,0,buf_max);  //
            serialport_read_until(fd, buf, eolchar, buf_max, timeout);
            if( !quiet ) printf("read string:");
            printf("%s\n", buf);
            break;
        case 'F':
            if( fd == -1 ) error("serial port not opened");
            if( !quiet ) printf("flushing receive buffer\n");
            serialport_flush(fd);
            break;

        }
    }

    exit(EXIT_SUCCESS);
} // end main

