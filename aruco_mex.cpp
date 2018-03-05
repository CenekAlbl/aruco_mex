#include <opencv2\opencv.hpp>
#include <aruco.h>


#ifndef HAS_OPENCV
#define HAS_OPENCV
#endif

#include "mex.h"
#include "mc_convert.h"
#include "mc_convert.cpp"

using namespace cv;
using namespace aruco;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs,
	const mxArray *prhs[]) {

	if (nrhs == 0){
		mexErrMsgTxt("Three arguments are needed for the function");
	}
	if (nlhs == 0){
		mexErrMsgTxt("Give only one output arguments to the function");
	}

	//Read Matlab image and load it to an IplImage struct
	IplImage* inputImg = mxArr_to_new_IplImage(prhs[0]);
	cv::Mat inMat = cv::cvarrToMat(inputImg);
	
	
	MarkerDetector MDetector;
	vector<Marker> Markers;
	//Ok, let's detect
	MDetector.detect(inMat, Markers);
	// Return coordinates
	plhs[0] = mxCreateDoubleMatrix(1, Markers.size()*2, mxREAL);
	double * data = (double *)mxGetData(plhs[0]);
	int * ids;
	int nMarkers = Markers.size();
	if (nlhs > 1){
		plhs[1] = mxCreateNumericMatrix(1, nMarkers, mxUINT32_CLASS,mxREAL);
		ids = (int *)mxGetData(plhs[1]);
		for (int i = 0; i<Markers.size(); i++)
		{
			ids[i] = Markers[i].id;
		}
	}
	for (int i = 0; i<Markers.size(); i++)
	{
		data[i*2] = Markers[i].getCenter().x;
		data[i*2+1] = Markers[i].getCenter().y;
	}
	//inMat.deallocate();
	cvReleaseImage(&inputImg);
	//cvReleaseImage(&outputImg);
}