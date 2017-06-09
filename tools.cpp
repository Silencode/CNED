/*
This code is intended for academic use only.
You are free to use and modify the code, at your own risk.

If you use this code, or find it useful, please refer to the paper:
Qi Jia, Xin Fan*, Zhongxuan Luo, Lianbo Song, and Tie Qiu, 
¡°A Fast Ellipse Detector Using Projective Invariant Pruning¡±, 
IEEE Transactions on Image Processing, 26(8): 3665-3679, 2017. 

http://ieeexplore.ieee.org/document/7929406/
Digital Object Identifier: 10.1109/TIP.2017.2704660

The comments in the code refer to the abovementioned paper.
If you need further details about the code or the algorithm, please contact me at:
lianbosong#foxmail.com

last update: 
*/

#include "EDLibCommon.h"
#include "tools.h"

// Tools for CNED methods
Point2f lineCrossPoint(Point2f l1p1, Point2f l1p2, Point2f l2p1, Point2f l2p2) {
	Point2f crossPoint;
	float k1, k2, b1, b2;
	if (l1p1.x == l1p2.x && l2p1.x == l2p2.x) {
		crossPoint = Point2f(0,0);//Invalid points
		return crossPoint;
	}
	if (l1p1.x == l1p2.x) {
		crossPoint.x = l1p1.x;
		k2 = (l2p2.y - l2p1.y) / (l2p2.x - l2p1.x);
		b2 = l2p1.y - k2 * l2p1.x;
		crossPoint.y = k2 * crossPoint.x + b2;
		return crossPoint;
	}
	if (l2p1.x == l2p2.x) {
		crossPoint.x = l2p1.x;
		k2 = (l1p2.y - l1p1.y) / (l1p2.x - l1p1.x);
		b2 = l1p1.y - k2 * l1p1.x;
		crossPoint.y = k2 * crossPoint.x + b2;
		return crossPoint;
	}

	k1 = (l1p2.y - l1p1.y) / (l1p2.x - l1p1.x);
	k2 = (l2p2.y - l2p1.y) / (l2p2.x - l2p1.x);
	b1 = l1p1.y - k1 * l1p1.x;
	b2 = l2p1.y - k2 * l2p1.x;
	if (k1 == k2) {
		crossPoint = Point2f(0,0);//Invalid points
	} else {
		crossPoint.x = (b2 - b1) / (k1 - k2);
		crossPoint.y = k1 * crossPoint.x + b1;
	}
	return crossPoint;
}
void point2Mat(Point2f p1, Point2f p2, float mat[2][2]) {
	mat[0][0] = p1.x;
	mat[0][1] = p1.y;
	mat[1][0] = p2.x;
	mat[1][1] = p2.y;
}
float value4SixPoints(V2SP) {
	float result = 1;
	Mat A, B, C;
	float matB[2][2], matC[2][2];
	Point2f v, w, u;
	v = lineCrossPoint(p1, p2, p3, p4);
	w = lineCrossPoint(p5, p6, p3, p4);
	u = lineCrossPoint(p5, p6, p1, p2);

	point2Mat(u, v, matB);
	point2Mat(p1, p2, matC);
	B = Mat(2, 2, CV_32F, matB);
	C = Mat(2,2,CV_32F,matC);
	A = C * B.inv();
	result *= A.at<float>(0,0) * A.at<float>(1,0) 
			/ (A.at<float>(0,1) * A.at<float>(1,1));

	point2Mat(p3, p4, matC);
	point2Mat(v, w, matB);
	B = Mat(2, 2, CV_32F, matB);
	C = Mat(2, 2, CV_32F, matC);
	A = C * B.inv();
	result *= A.at<float>(0,0) * A.at<float>(1,0) 
			/ (A.at<float>(0,1) * A.at<float>(1,1));

	point2Mat(p5, p6, matC);
	point2Mat(w, u, matB);
	B = Mat(2, 2, CV_32F, matB);
	C = Mat(2, 2, CV_32F, matC);
	A = C * B.inv();
	result *= A.at<float>(0,0) * A.at<float>(1,0) 
			/ (A.at<float>(0,1) * A.at<float>(1,1));
	return result;
}

// file operation
int writeFile(string fileName_cpp, vector<string> vsContent) {
	string line = "";
	vector<string> data;
	vector<string> data_split;
	ofstream out(fileName_cpp);
	if (!out) {
		cout << "¶ÁÐ´ÎÄ¼þÊ§°Ü" << endl;
		return -1;
	}
	for (vector<string>::iterator i = vsContent.begin(); 
		i < vsContent.end(); i++) {
		out << *i << endl;
	}
	out.close();
	return 1;
}

//Tools for Validate
void DrawDetectedEllipses(Mat3b& output, vector<Ellipse>& ellipses,
						  int iTopN, int thickness) {
	int sz_ell = int(ellipses.size());
	int n = (iTopN == 0) ? sz_ell : min(iTopN, sz_ell);
	for (int i = 0; i < n; ++i) {
		Ellipse& e = ellipses[n - i - 1];
		int g = cvRound(e._score * 255.f);
		Scalar color(0, g, 0);
		ellipse(output, Point(cvRound(e._xc), cvRound(e._yc)), 
			Size(cvRound(e._a), cvRound(e._b)), 
			e._rad*180.0 / CV_PI, 0.0, 360.0, color, thickness);
	}
}
void SaveEllipses(const string& fileName, const vector<Ellipse>& ellipses) {
	unsigned n = (unsigned) ellipses.size();
	vector<string> resultString;
	stringstream resultsitem;
	// Save number of ellipses
	resultsitem << n ;
	resultString.push_back(resultsitem.str());
	// Save ellipses
	for (unsigned i = 0; i<n; ++i) {
		const Ellipse& e = ellipses[i];
		resultsitem.str("");
		resultsitem << e._xc << "\t" << e._yc << "\t" 
			<< e._a << "\t" << e._b << "\t" 
			<< e._rad << "\t" << e._score;
		resultString.push_back(resultsitem.str());
	}
	writeFile(fileName, resultString);
}
void LoadGT(vector<Ellipse>& gt, const string& sGtFileName, 
			bool bIsAngleInRadians) {
	ifstream in(sGtFileName);
	if (!in.good()) {
		cout << "Error opening: " << sGtFileName << endl;
		return;
	}

	unsigned n;
	in >> n;

	gt.clear();
	gt.reserve(n);

	while (in.good() && n--) {
		Ellipse e;
		in >> e._xc >> e._yc >> e._a >> e._b >> e._rad;

		if (!bIsAngleInRadians) {
			// convert to radians
			e._rad = float(e._rad * CV_PI / 180.0);
		}

		if (e._a < e._b) {
			float temp = e._a;
			e._a = e._b;
			e._b = temp;

			e._rad = e._rad + float(0.5*CV_PI);
		}

		e._rad = fmod(float(e._rad + 2.f * CV_PI), float(CV_PI));
		e._score = 1.f;
		gt.push_back(e);
	}
	in.close();
}
bool LoadTest(vector<Ellipse>& ellipses, const string& sTestFileName, 
			  vector<double>& times, bool bIsAngleInRadians) {
	ifstream in(sTestFileName);
	if (!in.good()) {
		cout << "Error opening: " << sTestFileName << endl;
		return false;
	}

	times.resize(6);
	in >> times[0] >> times[1] >> times[2] >> times[3] >> times[4] >> times[5];

	unsigned n;
	in >> n;

	ellipses.clear();

	if (n == 0) return true;

	ellipses.reserve(n);

	while (in.good() && n--) {
		Ellipse e;
		in >> e._xc >> e._yc >> e._a >> e._b >> e._rad >> e._score;

		if (!bIsAngleInRadians) {
			e._rad = e._rad * float(CV_PI / 180.0);
		}

		e._rad = fmod(float(e._rad + 2.0*CV_PI), float(CV_PI));

		if ((e._a > 0) && (e._b > 0) && (e._rad >= 0)) {
			ellipses.push_back(e);
		}
	}
	in.close();

	// Sort ellipses by decreasing score
	sort(ellipses.begin(), ellipses.end());

	return true;
}

/*
result gt test tp p r f
*/
vector<float> Evaluate(const vector<Ellipse>& ellGT, 
					   const vector<Ellipse>& ellTest, 
					   const float th_score, const Mat3b& img) {
	vector<float> result(6);
	float threshold_overlap = 0.8f;
	//float threshold = 0.95f;

	unsigned sz_gt = (unsigned) ellGT.size();
	unsigned size_test = (unsigned) ellTest.size();

	unsigned sz_test = unsigned(min(1000, int(size_test)));

	vector<Mat1b> gts(sz_gt);
	vector<Mat1b> tests(sz_test);
	//Draw GT Ellipses
	for (unsigned i = 0; i<sz_gt; ++i) {
		const Ellipse& e = ellGT[i];

		Mat1b tmp(img.rows, img.cols, uchar(0));
		ellipse(tmp, Point((int)e._xc, (int)e._yc), Size((int)e._a, (int)e._b),
				e._rad * 180.0 / CV_PI, 0.0, 360.0, Scalar(255), -1);
		gts[i] = tmp;
	}
	//Draw Detected Ellipses
	for (unsigned i = 0; i<sz_test; ++i) {
		const Ellipse& e = ellTest[i];
		
		Mat1b tmp(img.rows, img.cols, uchar(0));
		ellipse(tmp, Point((int)e._xc, (int)e._yc), Size((int)e._a, (int)e._b), 
				e._rad * 180.0 / CV_PI, 0.0, 360.0, Scalar(255), -1);
		tests[i] = tmp;
	}

	Mat1b overlap(sz_gt, sz_test, uchar(0));
	for (int r = 0; r < overlap.rows; ++r) {
		for (int c = 0; c < overlap.cols; ++c) {
			overlap(r, c) = TestOverlap(gts[r], tests[c], threshold_overlap) 
				? uchar(255) : uchar(0);
		}
	}
	vector<bool> vec_gt(sz_gt, false);

	for (unsigned int i = 0; i < sz_test; ++i) {
		const Ellipse& e = ellTest[i];
		for (unsigned int j = 0; j < sz_gt; ++j) {
			if (vec_gt[j]) { continue; }

			bool bTest = overlap(j, i) != 0;

			if (bTest) {
				vec_gt[j] = true;
				break;
			}
		}
	}

	int tp = (int) std::count(vec_gt.begin(), vec_gt.end(), true);//True Positive 
	int fn = int(sz_gt) - tp;
	int fp = size_test - tp; 
	float pr(0.f);
	float re(0.f);
	float fmeasure(0.f);

	if (tp == 0) {
		if (fp == 0) {
			pr = 1.f;
			re = 0.f;
			fmeasure = (2.f * pr * re) / (pr + re);
		} else {
			pr = 0.f;
			re = 0.f;
			fmeasure = 0.f;
		}
	} else {
		pr = float(tp) / float(tp + fp);
		re = float(tp) / float(tp + fn);
		fmeasure = (2.f * pr * re) / (pr + re);
	}
	result[0] = (float) sz_test;
	result[1] = (float) sz_gt;
	result[2] = (float) tp;
	result[3] = pr;
	result[4] = re;
	result[5] = fmeasure;
	return result;
}
bool TestOverlap(const Mat1b& gt, const Mat1b& test, float th) {
	float fAND = float(countNonZero(gt & test));
	float fOR = float(countNonZero(gt | test));
	float fsim = fAND / fOR;

	return (fsim >= th);
}
int Count(const vector<bool> v)
{
	int counter = 0;
	for (unsigned i = 0; i<v.size(); ++i)
	{
		if (v[i]) { ++counter; }
	}
	return counter;
}
void showResult(vector<double> timesAndRecognize){
	vector<double> times_CC_EC(timesAndRecognize.begin(), 
								timesAndRecognize.begin() + 8);
	vector<double> RecognizeResult(timesAndRecognize.begin() + 8, 
									timesAndRecognize.end());
	cout << "--------------------------------" << endl;
	cout << "Execution Time: " << endl;
	cout << "Edge Detection: \t" << times_CC_EC[0] << endl;
	cout << "Pre Processing: \t" << times_CC_EC[1] << endl;
	cout << "Grouping:       \t" << times_CC_EC[2] << endl;
	cout << "Estimation:     \t" << times_CC_EC[3] << endl;
	cout << "Validation:     \t" << times_CC_EC[4] << endl;
	cout << "Clustering:     \t" << times_CC_EC[5] << endl;
	cout << "--------------------------------" << endl;
	cout << "countsOfFindEllipse   \t" << times_CC_EC[6] << endl;
	cout << "countsOfGetFastCenter \t" << times_CC_EC[7] << endl;
	cout << "--------------------------------" << endl;
	cout << "Test Ellipses Number: \t" << RecognizeResult[0] << endl;
	cout << "GT Ellipses Number:   \t" << RecognizeResult[1] << endl;
	cout << "TP Ellipses Number:   \t" << RecognizeResult[2] << endl;
	cout << "Precision:   \t" << RecognizeResult[3] << endl;
	cout << "Recall:      \t" << RecognizeResult[4] << endl;
	cout << "--------------------------------" << endl;
	cout << "Total Time:     \t" << accumulate(times_CC_EC.begin(), 
									times_CC_EC.begin() + 6,0.0)<<"ms"<<endl;
	cout << "F-Measure:      \t" << RecognizeResult[5] << endl;
	cout << "------------- END --------------" << endl;
}

vector<float> getRecognizeResult(string sWorkingDir, string imagename,
								 vector<Ellipse> ellsCned, float fThScoreScore, 
								 bool showpic) {
	vector<Ellipse> gt;
	LoadGT(gt, sWorkingDir + "/gt/" + "gt_" + imagename + ".txt", false);
	string filename = sWorkingDir + "/images/" + imagename;
	Mat3b image = imread(filename);
	Mat3b resultImage = image.clone();

	Mat3b res = image.clone();
	float fmeasure = 0;
	vector<float> recognizeResult = Evaluate(gt, ellsCned, fThScoreScore, res);
	if (showpic) {
		DrawDetectedEllipses(resultImage, ellsCned);
		string saveName = "result/parameter/r_" + imagename + ".txt";
		SaveEllipses(saveName, ellsCned);
		saveName = "result/resultImages/r_" + imagename;
		cvSaveImage(saveName.c_str(), &IplImage(resultImage));
	}
	return recognizeResult;
}
