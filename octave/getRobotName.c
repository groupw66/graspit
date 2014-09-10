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
  char numStr[20];
  int i,numRobots;
  double *pind;
  int sockd;
   
  /* Check for proper number of arguments */
  
  if (nrhs > 1) {
    mexErrMsgTxt("getRobotName takes at most one input argument.");
  } else if (nlhs > 1) {
    mexErrMsgTxt("getRobotName takes at most one output argument.");
  }

  sockd = ConnectTo("localhost",4765);

  if (sockd < 0)
    mexErrMsgTxt("Could not connect");

  strcpy(buf,"getRobotName ");

  if (nrhs==0) {
	strcat(buf,"ALL\n");
	Writeline(sockd,buf,strlen(buf));

    Readline(sockd,buf,MAXLENGTH);
    sscanf(buf,"%d\n",&numRobots);
  }
  else {
	  numRobots = mxGetNumberOfElements(prhs[0]);
	  if (numRobots>0) {
	  sprintf(numStr,"%d ",numRobots);
	  strcat(buf,numStr);

	  pind = mxGetPr(prhs[0]);
	  for (i=0;i<numRobots-1;i++) {
	    sprintf(numStr,"%d ",(int)pind[i]-1);
  		strcat(buf,numStr);
	  }
	  sprintf(numStr,"%d\n",(int)pind[i]-1);
	  strcat(buf,numStr);
	  Writeline(sockd,buf,strlen(buf));
	}
  }

  if (numRobots == 0) {
	plhs[0] = NULL;
  }
  else {
    plhs[0] = mxCreateCellArray(1, &numRobots);

    for (i=0;i<numRobots;i++) {
      Readline(sockd,buf,MAXLENGTH);
	  if (!strncmp(buf,"Error",5)) {
		mexErrMsgTxt(buf);
		break;
	  }
      buf[strlen(buf)-1] = '\0';
	  mxSetCell(plhs[0],i,mxCreateString(buf));
	}
  }

  CloseConnection(sockd);
  return;
}
