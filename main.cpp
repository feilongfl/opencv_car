
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>
#include <boost/asio.hpp>
#include <boost/timer/timer.hpp>
#include <vector>

using namespace cv;

//打印版本号
void VersionPrint ()
{
    std::cout << "boost version:" << BOOST_VERSION << std::endl;
    std::cout << "opencv version:" << CV_VERSION << std::endl;
}

//画赛道
void DrawGround(Mat src,Mat ret)
{
    Mat grayMat;
    //灰度
    cv::cvtColor(src,grayMat,CV_BGR2GRAY);

    //高斯模糊
    cv::GaussianBlur(grayMat,grayMat,Size(17,17),0,0);

    //动态二值
    Mat binMat;
    cv::adaptiveThreshold(grayMat,binMat,255,0,0,35,5);
    cv::dilate(binMat,binMat,cv::getStructuringElement(MORPH_RECT,Size(10,10)));
    cv::erode(binMat,binMat,cv::getStructuringElement(MORPH_RECT,Size(5,5)));

    //计算连通区域
    std::vector<std::vector<cv::Point> > c;
    cv::findContours(binMat,c,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point> > ca;
    cv::approxPolyDP(c,ca,1,true);

    //搜索最大连通区
    double maxArea = 0;
    size_t MaxI = 0;
    for(size_t i = 0;i < c.size();i++)
    {
        double area = cv::contourArea(c[i]);
        if(area > maxArea)
        {
            maxArea = area;
            MaxI = i;
        }
    }

    //标记
    RNG rng(12345);
    ret = src;
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( ret, c, MaxI, color, 2, 8, CV_RETR_EXTERNAL, 0, Point() );
    drawContours( ret, ca, MaxI, color, 2, 8, CV_RETR_EXTERNAL, 0, Point() );
    
    

    imshow("赛道",ret);
}


//画赛道,canny法
void DrawGround2(Mat src,Mat ret)
{
    Mat grayMat;
    //灰度
    cv::cvtColor(src,grayMat,CV_BGR2GRAY);

    //动态二值
    Mat binMat;
    cv::Canny(grayMat,binMat,15,40);
    cv::dilate(binMat,binMat,cv::getStructuringElement(MORPH_RECT,Size(10,10)));
    //cv::erode(binMat,binMat,cv::getStructuringElement(MORPH_RECT,Size(5,5)));

    imshow("canny",binMat);

    cv::waitKey(0);
    cv::adaptiveThreshold(grayMat,binMat,255,0,0,35,5);
    cv::dilate(binMat,binMat,cv::getStructuringElement(MORPH_RECT,Size(10,10)));
    cv::erode(binMat,binMat,cv::getStructuringElement(MORPH_RECT,Size(20,20)));

    //计算连通区域
    std::vector<std::vector<cv::Point> > c;
    cv::findContours(binMat,c,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

    //搜索最大连通区
    double maxArea = 0;
    size_t MaxI = 0;
    for(size_t i = 0;i < c.size();i++)
    {
        double area = cv::contourArea(c[i]);
        if(area > maxArea)
        {
            maxArea = area;
            MaxI = i;
        }
    }

    //标记
    RNG rng(12345);
    ret = src;
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( ret, c, MaxI, color, 2, 8, CV_RETR_EXTERNAL, 0, Point() );
    imshow("赛道2",ret);
}

int main ()
{
    Mat p = imread("pic/view_mini1.jpg",1);//匹配
    //namedWindow("原始", 0);
    imshow("原始", p);

    Mat r;
    DrawGround(p,r);
    //DrawGround2(p,r);

    std::cout << "end" << std::endl;    
    waitKey(0);
    return 0;
}
