#include "connectToServer.h"
#include <stdio.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#endif

#define MAXLENGTH 500

#include "mex.h"

void mexFunction(int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
  char buf[MAXLENGTH];
  int errCode;
  double *pind;
  int sockd;

   
  /* Check for proper number of arguments */
  
  if (nrhs != 1) {
    mexErrMsgTxt("computeNewVelocities takes one argument");
  } else if (nlhs != 0) {
    mexErrMsgTxt("computeNewVelocities takes no output arguments.");
  }
  
  sockd = ConnectTo("localhost",4765);

  if (sockd < 0)
    mexErrMsgTxt("Could not connect");

  sprintf(buf,"computeNewVelocities %le\n",mxGetScalar(prhs[0]));
  Writeline(sockd,buf,strlen(buf));

  Readline(sockd,buf,MAXLENGTH);
  sscanf(buf,"%d\n",&errCode);

  if (errCode) mexErrMsgTxt("The LCP could not be solved.");

  CloseConnection(sockd);
  return;
}