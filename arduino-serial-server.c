//
// arduino-serial-server -- web-based service for serial port 
// 
// This is just an idea. It doesn't work yet.
// 
//
//

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

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
    char result[1000] = "none";

    if (event == MG_NEW_REQUEST) {
        //const char* uri = request_info->uri;

        // get "id" query arg
        char cmd[32];
        get_qsvar(request_info, "cmd", cmd, sizeof(cmd));
      
        if( ! cmd[0] ) { // cmd is empty 
            ///
        }
        else {
            if( strcasecmp(cmd, "read")==0 ) { 
                serialport_flush(fd);
                serialport_read_until(fd, result, '\n', buf_max);
                printf("result: %s\n",result); // debug
            }
        }

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

//
int main(void) {
  struct mg_context *ctx;
  const char *options[] = {"listening_ports", "8080", NULL};

  
  //fd = serialport_init("/dev/tty.usbserial-A800f8ib", 19200);
  fd = serialport_init("/dev/tty.usbmodemfa141", 19200);
  if( fd==-1 ) fprintf(stderr, "couldn't open port");

  ctx = mg_start(&callback, NULL, options);
  printf("arduino-serial-server: running on port %s\n",
         mg_get_option(ctx, "listening_ports"));
  getchar();  // Wait until user hits "enter"

  mg_stop(ctx);

  return 0;
}
