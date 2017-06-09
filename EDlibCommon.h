// EDlibCommon.h : 标准系统包含文件的包含文件，
// 
#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types_c.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <numeric>
#include <unordered_map>
#include <vector>

#include <time.h>
#include <iostream>
#include <fstream>
#include <math.h>

#include <sys/stat.h>

using namespace std;using namespace cv;

typedef vector<Point>	VP;
typedef vector< VP >	VVP;
typedef unsigned int uint;
#define PI 3.1415926
struct Ellipse
{
	float _xc;
	float _yc;
	float _a;
	float _b;
	float _rad;
	float _score;

	Ellipse() : _xc(0.f), _yc(0.f), _a(0.f), _b(0.f), _rad(0.f), _score(0.f) {};
	Ellipse(float xc, float yc, float a, float b, float rad, float score = 0.f) : _xc(xc), _yc(yc), _a(a), _b(b), _rad(rad), _score(score) {};
	Ellipse(const Ellipse& other) : _xc(other._xc), _yc(other._yc), _a(other._a), _b(other._b), _rad(other._rad), _score(other._score) {};

	void Draw(Mat& img, const Scalar& color, const int thickness)
	{
		ellipse(img, Point(cvRound(_xc),cvRound(_yc)), Size(cvRound(_a),cvRound(_b)), _rad * 180.0 / CV_PI, 0.0, 360.0, color, thickness);
	};

	void Draw(Mat3b& img, const int thickness)
	{
		Scalar color(0, cvFloor(255.f * _score), 0);
		ellipse(img, Point(cvRound(_xc),cvRound(_yc)), Size(cvRound(_a),cvRound(_b)), _rad * 180.0 / CV_PI, 0.0, 360.0, color, thickness);
	};

	bool operator<(const Ellipse& other) const
	{
		if(_score == other._score)
		{
			float lhs_e = _b / _a;
			float rhs_e = other._b / other._a;
			if(lhs_e == rhs_e)
			{
				return false;
			}
			return lhs_e > rhs_e;
		}
		return _score > other._score;
	};
};
