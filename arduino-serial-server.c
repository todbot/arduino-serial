//
// arduino-serial-server -- web-based service for serial port 
// 
// This is just an idea. It doesn't work yet.
// 
//
//

#include <stdio.h>    // Standard input/output definitions 
#include <stdlib.h> 
#include <string.h>   // String function definitions 
#include <unistd.h>   // for usleep()
#include <getopt.h>

#include "mongoose.h"

#include "arduino-serial-lib.h"

static int fd;

//
static void get_qsvar(const struct mg_request_info *request_info,
                      const char *name, char *dst, size_t dst_len) {
  const char *qs = request_info->query_string;
  mg_get_var(qs, strlen(qs == NULL ? "" : qs), name, dst, dst_len);

}

//
static void *callback(enum mg_event event,
                      struct mg_connection *conn,
                      const struct mg_request_info *request_info)
{
    const int buf_max = 1000;
    char result[buf_max] = "none";

    if (event == MG_NEW_REQUEST) {
        //const char* uri = request_info->uri;

        // get query args
        char cmd[32];
        char str[256];
        get_qsvar(request_info, "cmd", cmd, sizeof(cmd));
        get_qsvar(request_info, "str", str, sizeof(str));

        if( ! cmd[0] ) { // cmd is empty 
            /// do something useful here
        }
        else {
            if( strcasecmp(cmd, "read")==0 ) { 
                serialport_flush(fd);
                serialport_read_until(fd, str, '\n', buf_max);
                sprintf(result, "read:\n%s\n",str); // debug
            }
            else if( strcasecmp(cmd, "send")==0 ) {
                serialport_write(fd, str);
                sprintf(result, "send:\n%s\n",str);
            }
            else {
                sprintf(result, "uknown cmd '%s'",cmd);
            }
        }

        printf("result:%s\n",result);
        // Echo requested URI back to the client
        mg_printf(conn, "HTTP/1.1 200 OK\r\n"
                  "Content-Type: text/plain\r\n\r\n"
                  "%s"
                  "\n", 
                  result
                  );
 
        return "";  // Mark as processed

    } else {
        return NULL;
    }
}

void error(char* msg)
{
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

//
void usage(void)
{
    printf("Usage: arduino-serial -p <serialport> [OPTIONS]\n"
    "\n"
    "Options:\n"
    "  -h, --help                   Print this help message\n"
    "  -P, --httpport=httpport      HTTP port to listen on (default 8080)\n"
    "  -p, --serialport=serialport  Serial port Arduino is on\n"
    "  -b, --baud=baudrate          Baudrate (bps) of Arduino (default 9600)\n" 
    "\n");
    exit(EXIT_SUCCESS);
}

//
int main(int argc, char *argv[]) 
{
    struct mg_context *ctx;

    int baudrate = 9600;  // default
    char httpport[16] = "8080"; 
    char serialport[256];
    
    /* parse options */
    int option_index = 0, opt;
    static struct option loptions[] = {
        {"help",       no_argument,       0, 'h'},
        {"httpport",   required_argument, 0, 'P'},
        {"serialport", required_argument, 0, 'p'},
        {"baud",       required_argument, 0, 'b'},
        {"quiet",      no_argument,       0, 'q'},
        {NULL,         0,                 0, 0}
    };
  

    while(1) {
        opt = getopt_long (argc, argv, "hP:p:b:q",
                           loptions, &option_index);
        if (opt==-1) break;
        switch (opt) {
        case '0': break;
        case 'h':
            usage();
            break;
        case 'b':
            baudrate = strtol(optarg,NULL,10);
            break;
        case 'p':
            strcpy(serialport,optarg);
            fd = serialport_init(optarg, baudrate);
            if( fd==-1 ) error("couldn't open port");
            serialport_flush(fd);
            break;
        case 'P':
            strcpy(httpport,optarg);
            break;
        }
    }
    
    char *options[] = {"listening_ports", httpport, NULL};

  
    //fd = serialport_init("/dev/tty.usbserial-A800f8ib", 19200);
    fd = serialport_init( serialport, baudrate );
    if( fd==-1 ) fprintf(stderr, "couldn't open port");
    printf("opened serialport %s @ %d bps\n", serialport, baudrate);

    ctx = mg_start(&callback, NULL, (const char**) options);
    printf("arduino-serial-server: running on port %s\n",
           mg_get_option(ctx, "listening_ports"));
    getchar();  // Wait until user hits "enter"
    
    mg_stop(ctx);
    
    return 0;
}
