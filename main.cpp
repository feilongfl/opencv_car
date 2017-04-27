
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

    Mat g;
    double gus;
    //std::cout << "gus:" << std::endl;
    //std::cin >> gus; 
    gus = 17;
    cv::GaussianBlur(p,g,Size(gus,gus),0,0);
    namedWindow("GaussianBlur", 0);
    imshow("GaussianBlur", g);
    //waitKey(0);


    Mat t;
    //cv::threshold(g,t,150,255,0);
    cv::adaptiveThreshold(g,t,255,0,0,35,5);
    cv::dilate(t,t,cv::getStructuringElement(MORPH_RECT,Size(10,10)));
    cv::erode(t,t,cv::getStructuringElement(MORPH_RECT,Size(5,5)));
    imshow("threshold", t);
    waitKey(0);

    //Mat c;
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
    //Mat drawing = Mat::zeros( p.size(), CV_8UC3 );
    Mat drawing = imread("pic/view_mini1.jpg",1);
    /*
    for( int i = 0; i< c.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, c, i, color, 2, 8, CV_RETR_EXTERNAL, 0, Point() );
     }
     */
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( drawing, c, MaxI, color, 2, 8, CV_RETR_EXTERNAL, 0, Point() );
    imshow("maxc",drawing);
     
    /*
    cv::Rect maxRect = cv::boundingRect(maxContor);
    cv::rectangle(p,maxRect,cv::Scalar(255));
    imshow("maxc",p);

    Mat result_erase(p.size(), CV_8U, Scalar(255));  
    drawContours(result_erase, maxContor,  
        1, //绘制所有轮廓  
        Scalar(0),  //颜色为黑色  
        2); //轮廓线的绘制宽度为2  
    imshow("maxc2",result_erase);
*/
/*
    Mat c;
    double th1,th2;
    std::cout << "th1:" << std::endl;
    std::cin >> th1;
    std::cout << "th2:" << std::endl;
    std::cin >> th2;
    cv::Canny(g,c,th1,th2);
    cv::imshow("canny",c);
*/

    cv::waitKey(0);




/*
    Mat img_1 = m;
    Mat img_2 = p;
    std::cout << "ready" << std::endl;
    //waitKey(0);

    // 
    // now, you can no more create an instance on the 'stack', like in the tutorial
    // (yea, noticed for a fix/pr).
    // you will have to use cv::Ptr all the way down:
    //
    cv::Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
    //cv::Ptr<Feature2D> f2d = xfeatures2d::SURF::create();
    //cv::Ptr<Feature2D> f2d = ORB::create();
    // you get the picture, i hope..

    //-- Step 1: Detect the keypoints:
    std::std::vector<KeyPoint> keypoints_1, keypoints_2;    
    f2d->detect( img_1, keypoints_1 );
    f2d->detect( img_2, keypoints_2 );

    //-- Step 2: Calculate descriptors (feature std::vectors)    
    Mat descriptors_1, descriptors_2;    
    f2d->compute( img_1, keypoints_1, descriptors_1 );
    f2d->compute( img_2, keypoints_2, descriptors_2 );

    //-- Step 3: Matching descriptor std::vectors using BFMatcher :
    BFMatcher matcher;
    std::std::vector< DMatch > matches;
    matcher.match( descriptors_1, descriptors_2, matches );

    //-- Step 4: Drawing the pics
    Mat imgMatchs;
    cv::drawMatches(img_1,keypoints_1,img_2,keypoints_2,matches,imgMatchs);
    namedWindow("match", 0);
    //cv::imshow("match",imgMatchs);
    cv::imwrite("match.jpg",imgMatchs);
*/
    std::cout << "end" << std::endl;    
    //waitKey(0);
    return 0;
}
