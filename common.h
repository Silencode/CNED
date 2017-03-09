/*
This code is intended for academic use only.
You are free to use and modify the code, at your own risk.

If you use this code, or find it useful, please refer to the paper:


The comments in the code refer to the abovementioned paper.
If you need further details about the code or the algorithm, please contact me at:

michele.fornaciari@unimore.it

last update: 23/12/2014
*/

#pragma once
typedef vector<Point>	VP;
typedef vector< VP >	VVP;
typedef unsigned int uint;

#define _INFINITY 1024


int inline sgn(float val) {
    return (0.f < val) - (val < 0.f);
};

bool inline isInf(float x)
{
	union
	{
		float f;
		int	  i;
	} u;

	u.f = x;
	u.i &= 0x7fffffff;
	return !(u.i ^ 0x7f800000);
};

float inline Slope(float x1, float y1, float x2, float y2)
{
	//reference slope
		float den = float(x2 - x1);
		float num = float(y2 - y1);
		if(den != 0)
		{
			return (num / den);
		}
		else
		{
			return ((num > 0) ? float(_INFINITY) : float(-_INFINITY));
		}
};

//void cvCanny2(	const void* srcarr, void* dstarr,
//				double low_thresh, double high_thresh,
//				void* dxarr, void* dyarr,
//                int aperture_size );
//
//void cvCanny3(	const void* srcarr, void* dstarr,
//				void* dxarr, void* dyarr,
//                int aperture_size );

void Canny2(	InputArray image, OutputArray _edges,
				OutputArray _sobel_x, OutputArray _sobel_y,
                double threshold1, double threshold2,
                int apertureSize, bool L2gradient );

void Canny3(	InputArray image, OutputArray _edges,
				OutputArray _sobel_x, OutputArray _sobel_y,
                int apertureSize, bool L2gradient );


float inline ed2(const Point& A, const Point& B)
{
	return float(((B.x - A.x)*(B.x - A.x) + (B.y - A.y)*(B.y - A.y)));
}

float inline ed2f(const Point2f& A, const Point2f& B)
{
	return (B.x - A.x)*(B.x - A.x) + (B.y - A.y)*(B.y - A.y);
}

void Labeling(Mat1b& image, vector<vector<Point> >& segments, int iMinLength);
void LabelingRect(Mat1b& image, VVP& segments, int iMinLength, vector<Rect>& bboxes);
void Thinning(Mat1b& imgMask, uchar byF=255, uchar byB=0);

bool SortBottomLeft2TopRight(const Point& lhs, const Point& rhs);
bool SortTopLeft2BottomRight(const Point& lhs, const Point& rhs);

bool SortBottomLeft2TopRight2f(const Point2f& lhs, const Point2f& rhs);

float GetMinAnglePI(float alpha, float beta);



