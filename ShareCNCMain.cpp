/*
This code is intended for academic use only.
You are free to use and modify the code, at your own risk.

If you use this code, or find it useful, please refer to the paper:


The comments in the code refer to the abovementioned paper.
If you need further details about the code or the algorithm, please contact me at:

lianbosong@foxmail.com
last update: 2017-2-8
*/

#include "EDLibCommon.h"
#include "tools.h"
#include "CNEllipseDetector.h"
#include <direct.h>
//Parameters Settings
float	fThScoreScore = 0.55f;	//
float	fMinReliability	= 0.4f;	// Const parameters to discard bad ellipses 0.4
float	fTaoCenters = 0.05f;//0.05 	
int		ThLength=16;//16
float	MinOrientedRectSide=3.0f;
int 	iNs = 16;//
float	scale=1;

void OnImage()
{
	string sWorkingDir = "C:\\Users\\lianbo\\Documents\\data\\ellipse_dataset\\PrasadImages-DatasetPrasad\\";
	string imagename = "027_0003.jpg";

	string filename = sWorkingDir + "\\images\\" + imagename;

	// Read image
	Mat3b image = imread(filename);
	Size sz = image.size();

	// Convert to grayscale
	Mat1b gray;
	cvtColor(image, gray, CV_BGR2GRAY);

	// Parameters Settings (Sect. )
	int		iThLength = ThLength;
	float	fThObb = MinOrientedRectSide;
	float	fThPos = 1.0f;
	float	fMaxCenterDistance = sqrt(float(sz.width*sz.width + sz.height*sz.height)) * fTaoCenters;

	// Other constant parameters settings.
	// Gaussian filter parameters, in pre-processing
	Size	szPreProcessingGaussKernelSize	= Size(5,5);
	double	dPreProcessingGaussSigma		= 1.0;

	float	fDistanceToEllipseContour		= 0.1f;	// (Sect. - Validation)
	
	CNEllipseDetector cned;
	// Initialize Detector with selected parameters
	cned.SetParameters	(szPreProcessingGaussKernelSize,dPreProcessingGaussSigma,		
		fThPos,fMaxCenterDistance,iThLength,fThObb,fDistanceToEllipseContour,		
		fThScoreScore,fMinReliability,		
		iNs );

	// Detect
	vector<Ellipse> ellsYaed;
	cned.Detect(gray.clone(), ellsYaed);

	vector<double> times = cned.GetTimes();
	cout << "--------------------------------" << endl;
	cout << "Execution Time: " << endl;
	cout << "Edge Detection: \t" << times[0] << endl;
	cout << "Pre processing: \t" << times[1] << endl;
	cout << "Grouping:       \t" << times[2] << endl;
	cout << "Estimation:     \t" << times[3] << endl;
	cout << "Validation:     \t" << times[4] << endl;
	cout << "Clustering:     \t" << times[5] << endl;
	cout << "--------------------------------" << endl;
	cout << "Total:	         \t" << cned.GetExecTime() << endl;
	cout << "--------------------------------" << endl;

	vector<Ellipse> gt;
	LoadGT(gt, sWorkingDir + "/gt/" + "gt_" + imagename + ".txt", false); // Prasad is in radians

	Mat3b resultImage = image.clone();

	// Draw GT ellipses
	for (unsigned i = 0; i < gt.size(); ++i)
	{
		Ellipse& e = gt[i];
		Scalar color(0, 0, 255);
		ellipse(resultImage, Point(cvRound(e._xc), cvRound(e._yc)), Size(cvRound(e._a), cvRound(e._b)), e._rad*180.0 / CV_PI, 0.0, 360.0, color, 3);
	}

	DrawDetectedEllipses(resultImage, ellsYaed);

	Mat3b res = image.clone();
	vector<float> result;
	result=Evaluate(gt, ellsYaed, fThScoreScore, res);
	cout << "F-measure : " << result[5] << endl;
	imshow("CNED", resultImage);
	waitKey();
}

void OnDataset()
{
	string sWorkingDir = "C:\\Users\\lianbo\\Documents\\data\\ellipse_dataset\\PrasadImages-DatasetPrasad\\";
	string out_folder = "C:\\Users\\lianbo\\Documents\\data\\ellipse_dataset\\PrasadImages-DatasetPrasad\\Output\\";

	vector<string> names;

	vector<float> prs;
	vector<float> res;
	vector<float> fms;
	vector<double> tms;

	glob(sWorkingDir + "images\\" + "*.*", names);

	int counter = 0;
	for (const auto& image_name : names)
	{
		cout << double(counter++) / names.size() << "\n";

		string name_ext = image_name.substr(image_name.find_last_of("\\") + 1);
		string name = name_ext.substr(0, name_ext.find_last_of("."));

		Mat3b image = imread(image_name);
		Size sz = image.size();

		// Convert to grayscale
		Mat1b gray;
		cvtColor(image, gray, CV_BGR2GRAY);

		// Parameters Settings (Sect.)
		int		iThLength = ThLength;
		float	fThObb = MinOrientedRectSide;
		float	fThPos = 1.0f;
		float	fMaxCenterDistance = sqrt(float(sz.width*sz.width + sz.height*sz.height)) * fTaoCenters;

		// Other constant parameters settings.
		// Gaussian filter parameters, in pre-processing
		Size	szPreProcessingGaussKernelSize	= Size(5,5);
		double	dPreProcessingGaussSigma		= 1.0;

		float	fDistanceToEllipseContour		= 0.1f;	// (Sect. - Validation)

		// Initialize Detector with selected parameters
		CNEllipseDetector cned;
		cned.SetParameters	(szPreProcessingGaussKernelSize,dPreProcessingGaussSigma,		
			fThPos,fMaxCenterDistance,iThLength,fThObb,fDistanceToEllipseContour,		
			fThScoreScore,fMinReliability,		
			iNs );

		// Detect
		vector<Ellipse> ellsYaed;
		cned.Detect(gray.clone(), ellsYaed);

		tms.push_back(cned.GetExecTime());

		vector<Ellipse> gt;
		LoadGT(gt, sWorkingDir + "gt\\" + "gt_" + name_ext + ".txt", false); // Prasad is in radians,set to true

		vector<float> result;
		result = Evaluate(gt, ellsYaed, fThScoreScore, image);

		prs.push_back(result[3]);
		res.push_back(result[4]);
		fms.push_back(result[5]);
		Mat3b resultImage = image.clone();

		// Draw GT ellipses
		for (unsigned i = 0; i < gt.size(); ++i)
		{
			Ellipse& e = gt[i];
			Scalar color(0, 0, 255);
			ellipse(resultImage, Point(cvRound(e._xc), cvRound(e._yc)), Size(cvRound(e._a), cvRound(e._b)), e._rad*180.0 / CV_PI, 0.0, 360.0, color, 3);
		}

	}

	float N = float(prs.size());
	float sumPR = accumulate(prs.begin(), prs.end(), 0.f);
	float sumRE = accumulate(res.begin(), res.end(), 0.f);
	float sumFM = accumulate(fms.begin(), fms.end(), 0.f);
	double sumTM = accumulate(tms.begin(), tms.end(), 0.0);

	float meanPR = sumPR / N;
	float meanRE = sumRE / N;
	float meanFM = sumFM / N;
	double meanTM = sumTM / N;

	float finalFM = (2.f * meanPR * meanRE) / (meanPR + meanRE);

	cout << "F-measure : " << finalFM << endl;
	cout << "Exec time : " << meanTM << endl;

	getchar();
}
int main(int argc, char** argv)
{
	OnImage();
	//OnDataset();
	return 0;	   
}