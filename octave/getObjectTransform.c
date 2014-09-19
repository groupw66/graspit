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
	int i,j,numRobots;
	double *pvals,*pvals2,*pind;
	int sockd;
	mxArray *vals,*vals2;

	/* Check for proper number of arguments */

	if (nrhs > 1) {
		mexErrMsgTxt("getObjectTransform takes at most one input argument.");
	} else if (nlhs > 2) {
		mexErrMsgTxt("getObjectTransform takes at most two output argument.");
	}

	sockd = ConnectTo("localhost",4765);

	if (sockd < 0)
		mexErrMsgTxt("Could not connect");

	strcpy(buf,"getObjectTransform ");

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

		if (numRobots > 1){
			for (i=0;i<nlhs;i++)
				plhs[i] = mxCreateCellArray(1,&numRobots);
		}

		for (i=0;i<numRobots;i++) {
			if(nlhs==1)
				vals = mxCreateDoubleMatrix(7,1,mxREAL);
			else{
				vals = mxCreateDoubleMatrix(4,1,mxREAL);
				vals2 = mxCreateDoubleMatrix(3,1,mxREAL);
				pvals2 = mxGetPr(vals2);
			}
			pvals = mxGetPr(vals);

			Readline(sockd,buf,MAXLENGTH);
			if(nlhs==1){
				sscanf(buf,"%lf%lf%lf%lf%lf%lf%lf",&pvals[0],&pvals[1],&pvals[2],&pvals[3],&pvals[4],&pvals[5],&pvals[6]);
				if (numRobots == 1)
					plhs[0] = vals;
				else
					mxSetCell(plhs[0],i,vals);
			}
			else{
				sscanf(buf,"%lf%lf%lf%lf%lf%lf%lf",&pvals[0],&pvals[1],&pvals[2],&pvals[3],&pvals2[0],&pvals2[1],&pvals2[2]);
				if (numRobots == 1) {
					plhs[0] = vals;
					plhs[1] = vals2;
				} else {
					mxSetCell(plhs[0],i,vals);
					mxSetCell(plhs[1],i,vals2);
				}
			}
		}
	}

	CloseConnection(sockd);
	return;
}
