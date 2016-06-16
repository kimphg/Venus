#pragma once
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm> 
#include "math.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include <DIRECT.H>

//general auxillary fuction
extern "C" double  fn_Round(double input);
extern "C" double	fn_kthSmallest(double a[], int n, int k);
extern "C" double	fn_median(double a[], int n);
extern "C" double	fn_Abs(double input);
extern "C" char*	fn_DoubleToStr(double input, int precision);
extern "C" void	fn_DoubleToStr2(double input, int precision, char* strOutput);

//RECT operation
//scale a rect by a zoom factor
extern "C" void utl_RectZoom(RECT *ioRect, float inZoomFactor, int imgWidth, int imgHeight);
//check the boundary limit of RECT
extern "C" void utl_RectCheckBound(RECT *ioRect, int imgWidth, int imgHeight);
//rect at the same center with different size
extern "C" void utl_RectSizeAdjust(RECT *ioRect, int dx, int dy, int imgWidth, int imgHeight);
//rect increase size of border
extern "C" void utl_RectSizeIncrease(RECT *ioRect, int border, int imgWidth, int imgHeight);

//check if pixel is valid in bound of image
extern "C" void utl_PixelCheckBound(int *pixelX, int *pixelY, int imgWidth, int imgHeight);
extern "C" void utl_PixelCheckBound_Float(float *pixelX, float *pixelY, int imgWidth, int imgHeight);

extern "C" void utl_IndexNewDir(char *ioNewDirPath);

extern "C" int utl_Min3(int a, int b, int c);
extern "C" int utl_Max3(int a, int b, int c);

//write single channel image value to a txt file
extern "C" void utl_WriteImage(IplImage *imgInput, char* filename);
extern "C" void utl_DrawLine(IplImage *imgInput, POINT pt1, POINT pt2, CvScalar pixval);

void utl_ConvertRectToBox(RECT RectInput, CvRect *CvRectInput);
void utl_ConvertBoxToRect(RECT *RectInput, CvRect CvRectInput);


// For tracking
typedef struct TrkResult
{
	RECT		targetBox;
	IplImage	*FGImage;
	IplImage	*FGMask;
	IplImage	*ObjMask;
	float		score;
	BOOL		occlusion; //flag for occusion
	int			x0; //obj center x
	int			y0; //obj center y
	int			axisA; //ellipse axis A
	int			axisB; //ellipse axis B
	double		angle; //ellipse angle
} TrkResult;
