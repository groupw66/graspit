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
  double value;
  double *poutvals;
  const double *pvals;
  int j,numDOF;
  int sockd;
  const mxArray *vals,*stepby;
  mxArray *outvals;

  /* Check for proper number of arguments */
  
  if (nrhs != 3) {
	mexErrMsgTxt("moveDOFs takes three input arguments.");
  }
  if (nlhs > 1) {
    mexErrMsgTxt("moveDOFs takes at most one output argument.");
  }
  if (nlhs == 0 ) nlhs=1;

  // more argument checking is needed 

  sockd = ConnectTo("localhost",4765);

  if (sockd < 0)
    mexErrMsgTxt("Could not connect");
 
  sprintf(buf,"moveDOFs %d ",(int)mxGetScalar(prhs[0])-1);
 
  vals = prhs[1];
  stepby = prhs[2];
      
  numDOF = mxGetNumberOfElements(vals);
  sprintf(numStr,"%d ",numDOF);
  strcat(buf,numStr);

  pvals = mxGetPr(vals);
    
  for (j=0;j<numDOF;j++) {
    sprintf(numStr,"%lf ",pvals[j]);
	strcat(buf,numStr);
  }
  pvals = mxGetPr(stepby);

  for (j=0;j<numDOF;j++) {
    sprintf(numStr,"%lf ",pvals[j]);
    strcat(buf,numStr);
  }

  strcat(buf,"\n");
  Writeline(sockd,buf,strlen(buf));

  outvals = mxCreateDoubleMatrix(numDOF,1,mxREAL);
  poutvals = mxGetPr(outvals);

  for (j=0;j<numDOF;j++) {
    Readline(sockd,buf,MAXLENGTH);
    if (!strncmp(buf,"Error",5)) {
	  mexErrMsgTxt(buf);
	}
    sscanf(buf,"%lf\n",&value);
    poutvals[j] = value;
  }
	  
  plhs[0] = outvals;

  CloseConnection(sockd);
  return;
}