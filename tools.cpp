/*
This code is intended for academic use only.
You are free to use and modify the code, at your own risk.

If you use this code, or find it useful, please refer to the paper:

The comments in the code refer to the abovementioned paper.
If you need further details about the code or the algorithm, please contact me at:

lianbosong@foxmail.com

last update: 
*/

#include "EDLibCommon.h"
#include "tools.h"

// Tools for our methods
Point2f lineCrossPoint(Point2f l1p1,Point2f l1p2,Point2f l2p1,Point2f l2p2 )
{
	Point2f crossPoint;
	float k1,k2,b1,b2;
	if (l1p1.x==l1p2.x&&l2p1.x==l2p2.x){
		crossPoint=Point2f(0,0);//无效点
		return crossPoint;
	}
	if (l1p1.x==l1p2.x)
	{
		crossPoint.x=l1p1.x;
		k2=(l2p2.y-l2p1.y)/(l2p2.x-l2p1.x);
		b2=l2p1.y-k2*l2p1.x;
		crossPoint.y=k2*crossPoint.x+b2;
		return crossPoint;
	}
	if (l2p1.x==l2p2.x)
	{
		crossPoint.x=l2p1.x;
		k2=(l1p2.y-l1p1.y)/(l1p2.x-l1p1.x);
		b2=l1p1.y-k2*l1p1.x;
		crossPoint.y=k2*crossPoint.x+b2;
		return crossPoint;
	}

	k1=(l1p2.y-l1p1.y)/(l1p2.x-l1p1.x);
	k2=(l2p2.y-l2p1.y)/(l2p2.x-l2p1.x);
	b1=l1p1.y-k1*l1p1.x;
	b2=l2p1.y-k2*l2p1.x;
	if (k1==k2)
	{
		crossPoint=Point2f(0,0);//无效点
	}
	else
	{
		crossPoint.x=(b2-b1)/(k1-k2);
		crossPoint.y=k1*crossPoint.x+b1;
	}
	return crossPoint;
}
void point2Mat(Point2f p1,Point2f p2,float mat[2][2])
{
	mat[0][0]=p1.x;
	mat[0][1]=p1.y;
	mat[1][0]=p2.x;
	mat[1][1]=p2.y;
}
float value4SixPoints( V2SP )
{
	float result=1;
	Mat A,B,C;
	float matB[2][2],matC[2][2];
	Point2f v,w,u;
	v=lineCrossPoint(p1,p2,p3,p4);
	w=lineCrossPoint(p5,p6,p3,p4);
	u=lineCrossPoint(p5,p6,p1,p2);

	point2Mat(u,v,matB);
	point2Mat(p1,p2,matC);
	B=Mat(2,2,CV_32F,matB);
	C=Mat(2,2,CV_32F,matC);
	A=C*B.inv();
	/*cout<<"u:\t"<<u<<endl;
	cout<<"v:\t"<<v<<endl;
	cout<<"B:\t"<<B<<endl;
	cout<<A<<endl;*/
	result*=A.at<float>(0,0)*A.at<float>(1,0)/(A.at<float>(0,1)*A.at<float>(1,1));

	point2Mat(p3,p4,matC);
	point2Mat(v,w,matB);
	B=Mat(2,2,CV_32F,matB);
	C=Mat(2,2,CV_32F,matC);
	A=C*B.inv();
	result*=A.at<float>(0,0)*A.at<float>(1,0)/(A.at<float>(0,1)*A.at<float>(1,1));

	point2Mat(p5,p6,matC);
	point2Mat(w,u,matB);
	B=Mat(2,2,CV_32F,matB);
	C=Mat(2,2,CV_32F,matC);
	A=C*B.inv();
	result*=A.at<float>(0,0)*A.at<float>(1,0)/(A.at<float>(0,1)*A.at<float>(1,1));
	return result;
}

// Resize The Mat
Mat matResize(Mat src,double scale){
	Mat img2;
	bool showtimeandpic=false;
	if(!showtimeandpic){
		Size dsize = Size(int(src.cols*scale),int(src.rows*scale));
		img2 = Mat(dsize,CV_32S);
		resize(src, img2,dsize,CV_INTER_CUBIC);
	}
	else{
		clock_t start_time=clock();
		{
			Size dsize = Size(int(src.cols*scale),int(src.rows*scale));
			img2 = Mat(dsize,CV_32S);
			resize(src, img2,dsize,CV_INTER_CUBIC);
		}
		clock_t end_time=clock();
		cout<< "Running time is: "<<static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000<<"ms"<<endl;//输出运行时间

		//CV_INTER_NN - 最近邻差值,
		//CV_INTER_LINEAR -  双线性差值 (缺省使用) 
		//CV_INTER_AREA -  使用象素关系重采样。当图像缩小时候，该方法
		//可以避免波纹出现。当图像放大时，类似于  CV_INTER_NN  方法.. 
		//CV_INTER_CUBIC -  立方差值. 
		namedWindow("WindowOrg",CV_WINDOW_AUTOSIZE);
		namedWindow("WindowNew",CV_WINDOW_AUTOSIZE);
		imshow("WindowOrg",src);
		imshow("WindowNew",img2);

		waitKey(1000);
	}
	return img2;
}

// file operation
int writeFile(string fileName_cpp,vector<string> vsContent){
	string line="";
	vector<string> data;
	vector<string> data_split;
	ofstream out(fileName_cpp);
	if(!out)
	{
		cout<<"读写文件失败"<<endl;
		return -1;
	}
	for(vector<string>::iterator i=vsContent.begin();i<vsContent.end();i++){
		out<<*i<<endl;
	}
	out.close();
	return 1;
}
int readFile(string fileName_cpp){
	string line="";
	vector<string> data;
	ifstream in(fileName_cpp);
	if(!in)
	{
		cout<<"读写文件失败"<<endl;
		return -1;
	}
	while(getline(in,line))
	{
		data.push_back(line);     //读取文件每一行数据，并放到“容器”里面
	}
	in.close();
	/******遍历data里面的内容******/
	for(unsigned int i=0;i<data.size();i++)
	{
		cout<<data.at(i)<<endl;
	}
	return 0;
	/******遍历data里面的内容******/
}

int readFileByChar(string fileName_split){

	string line="";
	vector<string> data;
	vector<string> data_split;
	ifstream in_split(fileName_split);
	if(!in_split)
	{
		cout<<"读写文件失败"<<endl;
		return -1;
	}
	while(getline(in_split,line))
	{
		data_split.push_back(line);     //读取文件每一行数据，并放到“容器”里面
	}
	in_split.close();
	/******读文件******/
	/******提取split.txt文件里面的数据******/

	/******遍历data_split里面的内容(数据分离)******/
	for(unsigned int i=0;i<data_split.size();i++)
	{
		cout<<"--------------------"<<endl;
		for(unsigned int j=0;j<getStr(data_split.at(i)).size();j++)
		{
			cout<<getStr(data_split.at(i)).at(j)<<endl;
		}
	}
	/******遍历data_split里面的内容(数据分离)******/
	return 0;
}
void Trim(string &str)
{
	int s=str.find_first_not_of(" \t\n");
	int e=str.find_last_not_of(" \t\n");
	str=str.substr(s,e-s+1);
}
vector<string> getStr(string str)
{
	int j=0;
	string a[100];
	vector<string> v_a;
	//Split()
	for(unsigned int i=0;i<str.size();i++)
	{
		if((str[i]!=',')&&str[i]!='\0')
		{
			a[j]+=str[i];
		}
		else j++;
	}

	for(int k=0;k<j+1;k++)
	{
		v_a.push_back(a[k]);
	}
	return v_a;
}


//Tools for Validate
void DrawDetectedEllipses(Mat3b& output, vector<Ellipse>& ellipses, int iTopN, int thickness)
{
	int sz_ell = int(ellipses.size());
	int n = (iTopN == 0) ? sz_ell : min(iTopN, sz_ell);
	for (int i = 0; i < n; ++i)
	{
		Ellipse& e = ellipses[n - i - 1];
		int g = cvRound(e._score * 255.f);
		Scalar color(0, g, 0);
		ellipse(output, Point(cvRound(e._xc), cvRound(e._yc)), Size(cvRound(e._a), cvRound(e._b)), e._rad*180.0 / CV_PI, 0.0, 360.0, color, thickness);
	}
}
void SaveEllipses(const string& fileName, const vector<Ellipse>& ellipses){
	unsigned n = ellipses.size();
	vector<string> resultString;
	stringstream resultsitem;
	// Save number of ellipses
	resultsitem << n ;
	resultString.push_back(resultsitem.str());
	// Save ellipses
	for (unsigned i = 0; i<n; ++i)
	{
		const Ellipse& e = ellipses[i];
		resultsitem.str("");
		resultsitem << e._xc << "\t" << e._yc << "\t" 
			<< e._a << "\t" << e._b << "\t" 
			<< e._rad << "\t" << e._score;
		resultString.push_back(resultsitem.str());
	}
	writeFile(fileName,resultString);
	/*for (int i=0;i<resultString.size();i++){
		cout<<resultString[i]<<endl;
	}*/
}
void LoadGT(vector<Ellipse>& gt, const string& sGtFileName, bool bIsAngleInRadians)
{
	ifstream in(sGtFileName);
	if (!in.good())
	{
		cout << "Error opening: " << sGtFileName << endl;
		return;
	}

	unsigned n;
	in >> n;

	gt.clear();
	gt.reserve(n);

	while (in.good() && n--)
	{
		Ellipse e;
		in >> e._xc >> e._yc >> e._a >> e._b >> e._rad;

		if (!bIsAngleInRadians)
		{
			// convert to radians
			e._rad = float(e._rad * CV_PI / 180.0);
		}

		if (e._a < e._b)
		{
			float temp = e._a;
			e._a = e._b;
			e._b = temp;

			e._rad = e._rad + float(0.5*CV_PI);
		}

		e._rad = fmod(float(e._rad + 2.f*CV_PI), float(CV_PI));
		e._score = 1.f;
		gt.push_back(e);
	}
	in.close();
}
bool LoadTest(vector<Ellipse>& ellipses, const string& sTestFileName, vector<double>& times, bool bIsAngleInRadians)
{
	ifstream in(sTestFileName);
	if (!in.good())
	{
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

	while (in.good() && n--)
	{
		Ellipse e;
		in >> e._xc >> e._yc >> e._a >> e._b >> e._rad >> e._score;

		if (!bIsAngleInRadians)
		{
			e._rad = e._rad * float(CV_PI / 180.0);
		}

		e._rad = fmod(float(e._rad + 2.0*CV_PI), float(CV_PI));

		if ((e._a > 0) && (e._b > 0) && (e._rad >= 0))
		{
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
vector<float> Evaluate(const vector<Ellipse>& ellGT, const vector<Ellipse>& ellTest, const float th_score, const Mat3b& img)
{
	vector<float> result(6);
	float threshold_overlap = 0.8f;
	//float threshold = 0.95f;

	unsigned sz_gt = ellGT.size();
	unsigned size_test = ellTest.size();

	unsigned sz_test = unsigned(min(1000, int(size_test)));

	vector<Mat1b> gts(sz_gt);
	vector<Mat1b> tests(sz_test);
	//绘制每个目标椭圆
	for (unsigned i = 0; i<sz_gt; ++i)
	{
		const Ellipse& e = ellGT[i];

		Mat1b tmp(img.rows, img.cols, uchar(0));
		ellipse(tmp, Point((int)e._xc, (int)e._yc), Size((int)e._a, (int)e._b), e._rad * 180.0 / CV_PI, 0.0, 360.0, Scalar(255), -1);
		gts[i] = tmp;
	}
	//绘制检测的椭圆
	for (unsigned i = 0; i<sz_test; ++i)
	{
		const Ellipse& e = ellTest[i];
		
		Mat1b tmp(img.rows, img.cols, uchar(0));
		ellipse(tmp, Point((int)e._xc, (int)e._yc), Size((int)e._a, (int)e._b), e._rad * 180.0 / CV_PI, 0.0, 360.0, Scalar(255), -1);
		
		tests[i] = tmp;
	}

	Mat1b overlap(sz_gt, sz_test, uchar(0));
	for (int r = 0; r < overlap.rows; ++r)
	{
		for (int c = 0; c < overlap.cols; ++c)
		{
			//重叠区域占真个区域的比例 与比上并大于阈值 为255
			overlap(r, c) = TestOverlap(gts[r], tests[c], threshold_overlap) ? uchar(255) : uchar(0);
		}
	}
	vector<bool> vec_gt(sz_gt, false);
	//矩阵每行有一个就代表找到
	for (unsigned int i = 0; i < sz_test; ++i)
	{
		const Ellipse& e = ellTest[i];
		for (unsigned int j = 0; j < sz_gt; ++j)
		{
			if (vec_gt[j]) { continue; }

			bool bTest = overlap(j, i) != 0;

			if (bTest)
			{
				vec_gt[j] = true;
				break;
			}
		}
	}

	int tp = std::count(vec_gt.begin(), vec_gt.end(), true);//True Positive 
	int fn = int(sz_gt) - tp;//
	int fp = size_test - tp; // !!!!
	// test = positive = tp + fp
	// gt = tp + fn
	float pr(0.f);
	float re(0.f);
	float fmeasure(0.f);

	if (tp == 0)
	{
		if (fp == 0)
		{
			pr = 1.f;
			re = 0.f;
			fmeasure = (2.f * pr * re) / (pr + re);
		}
		else
		{
			pr = 0.f;
			re = 0.f;
			fmeasure = 0.f;
		}
	}
	else
	{
		pr = float(tp) / float(tp + fp);
		re = float(tp) / float(tp + fn);
		fmeasure = (2.f * pr * re) / (pr + re);
	}
	result[0] = sz_test;
	result[1] = sz_gt;
	result[2] = tp;
	result[3] = pr;
	result[4] = re;
	result[5] = fmeasure;
	return result;
}
bool TestOverlap(const Mat1b& gt, const Mat1b& test, float th)
{
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
	vector<double> times_CC_EC(timesAndRecognize.begin(),timesAndRecognize.begin()+8);
	vector<double> RecognizeResult(timesAndRecognize.begin()+8,timesAndRecognize.end());
	cout << "--------------------------------" << endl;
	cout << "Execution Time: " << endl;
	cout << "Edge Detection: \t" << times_CC_EC[0] << endl;
	cout << "Pre Processing: \t" << times_CC_EC[1] << endl;
	cout << "Grouping:       \t" << times_CC_EC[2] << endl;
	cout << "Estimation:     \t" << times_CC_EC[3] << endl;
	cout << "Validation:     \t" << times_CC_EC[4] << endl;
	cout << "Clustering:     \t" << times_CC_EC[5] << endl;
	cout << "--------------------------------" << endl;
	cout << "countsOfFindEllipse   \t"<<times_CC_EC[6]<<endl;
	cout << "countsOfGetFastCenter \t"<<times_CC_EC[7]<<endl;
	cout << "--------------------------------" << endl;
	cout << "Test Ellipses Number: \t" << RecognizeResult[0] << endl;
	cout << "GT Ellipses Number:   \t" << RecognizeResult[1] << endl;
	cout << "TP Ellipses Number:   \t" << RecognizeResult[2] << endl;
	cout << "Precision:   \t" << RecognizeResult[3] << endl;
	cout << "Recall:      \t" << RecognizeResult[4] << endl;
	cout << "--------------------------------" << endl;
	cout << "Total Time:     \t" << accumulate(times_CC_EC.begin(),times_CC_EC.begin()+6,0.0)<<"ms"<<endl;
	cout << "F-Measure:      \t" << RecognizeResult[5] << endl;
	cout << "------------- END --------------" << endl;
}

vector<float> getRecognizeResult(string sWorkingDir,string imagename,vector<Ellipse> ellsCned,float fThScoreScore, bool showpic){
	vector<Ellipse> gt;
	LoadGT(gt, sWorkingDir + "/gt/" + "gt_" + imagename + ".txt", false);
	string filename = sWorkingDir + "/images/" + imagename;
	Mat3b image = imread(filename);
	Mat3b resultImage = image.clone();

	Mat3b res = image.clone();
	float fmeasure = 0;
	vector<float> recognizeResult = Evaluate(gt, ellsCned, fThScoreScore, res);
	if(showpic){
		DrawDetectedEllipses(resultImage, ellsCned);
		string saveName="result/parameter/r_"+imagename+".txt";
		SaveEllipses(saveName, ellsCned);
		saveName="result/resultImages/r_"+imagename;
		cvSaveImage(saveName.c_str(),&IplImage(resultImage));
	}
	return recognizeResult;
}

// validate
