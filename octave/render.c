#include "connectToServer.h"
/*#include <stdio.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#endif
*/
#define MAXLENGTH 500

#include "mex.h"

void mexFunction(int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
  int sockd;
   
  /* Check for proper number of arguments */
  
  if (nrhs != 0) {
    mexErrMsgTxt("render takes no arguments.");
  } else if (nlhs != 0) {
    mexErrMsgTxt("render takes no output arguments.");
  }

  sockd = ConnectTo("localhost",4765);

  if (sockd < 0)
    mexErrMsgTxt("Could not connect");

  Writeline(sockd,"render\n",7);

  CloseConnection(sockd);
  return;
}
