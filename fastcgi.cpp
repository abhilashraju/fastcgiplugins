#include <alloca.h>
#include <fcgiapp.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#define LISTENSOCK_FILENO 0
#define LISTENSOCK_FLAGS 0
int main(int argc, char **argv) {
  openlog("testfastcgi", LOG_CONS | LOG_NDELAY, LOG_USER);
  // int socket_fd = FCGX_OpenSocket("/tmp/hello.sock", 8081);

  int err = FCGX_Init(); /* call before Accept in multithreaded apps */
  if (err) {
    syslog(LOG_INFO, "FCGX_Init failed: %d", err);
    return 1;
  }
  FCGX_Request cgi;
  err = FCGX_InitRequest(&cgi, LISTENSOCK_FILENO, LISTENSOCK_FLAGS);
  if (err) {
    syslog(LOG_INFO, "FCGX_InitRequest failed: %d", err);
    return 2;
  }

  while (1) {
    err = FCGX_Accept_r(&cgi);
    if (err) {
      syslog(LOG_INFO, "FCGX_Accept_r stopped: %d", err);
      auto m = strerror(err);
      std::cout << m;
      return 0;
    }
    char **envp;
    int size = 200;
    for (envp = cgi.envp; *envp; ++envp)
      size += strlen(*envp) + 11;
    char *result = (char *)alloca(size);
    strcpy(result, "Status: 200 OK\r\nContent-Type: text/html\r\n\r\n");
    strcat(result, "<html><head><title>testcgi</title></head><body><ul>\r\n");

    for (envp = cgi.envp; *envp; ++envp) {
      strcat(result, "<li>");
      strcat(result, *envp);
      strcat(result, "</li>\r\n");
    }

    strcat(result, "</ul></body></html>\r\n");
    FCGX_PutStr(result, strlen(result), cgi.out);
  }

  return 0;
}