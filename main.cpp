
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>
#include <boost/asio.hpp>
#include <boost/timer/timer.hpp>
#include <vector>

using namespace cv;

void VersionPrint ()
{
    std::cout << "boost version:" << BOOST_VERSION << std::endl;
    std::cout << "opencv version:" << CV_VERSION << std::endl;
}

void DrawGround(Mat src,Mat ret)
{
    Mat tempMat;

    cv::GaussianBlur(src,tempMat,Size(17,17),0,0);
    namedWindow("GaussianBlur", 0);
    imshow("GaussianBlur", tempMat);

    cv::adaptiveThreshold(tempMat,tempMat,255,0,0,35,5);
    cv::dilate(tempMat,tempMat,cv::getStructuringElement(MORPH_RECT,Size(10,10)));
    cv::erode(tempMat,tempMat,cv::getStructuringElement(MORPH_RECT,Size(5,5)));
    imshow("threshold", tempMat);

    std::vector<std::vector<cv::Point> > c;
    cv::findContours(t,c,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    double maxArea = 0;
    size_t MaxI = 0;
    std::vector <cv::Point> maxContor;
    for(size_t i = 0;i < c.size();i++)
    {
        double area = cv::contourArea(c[i]);
        if(area > maxArea)
        {
            maxArea = area;
            maxContor = c[i];
            MaxI = i;
        }
    }

    RNG rng(12345);
    Mat drawing = imread("pic/view_mini1.jpg",1);
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( drawing, c, MaxI, color, 2, 8, CV_RETR_EXTERNAL, 0, Point() );
    imshow("maxc",drawing);
}

int main ()
{
    Mat m = imread("pic/m.jpg", 0);//模板
    namedWindow("模板", 0);
    imshow("模板", m);

//    Mat p = imread("pic/car1.jpg",0);//匹配
    Mat p = imread("pic/view_mini1.jpg",0);//匹配
    namedWindow("匹配", 0);
    imshow("匹配", p);



    std::cout << "end" << std::endl;    
    waitKey(0);
    return 0;
}
