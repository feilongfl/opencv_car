
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>
#include <boost/asio.hpp>
#include <boost/timer/timer.hpp>
#include <vector>
using namespace cv;

#if !defined WATERSHS
#define WATERSHS

class WatershedSegmenter {

  private:

	  cv::Mat markers;

  public:

	  void setMarkers(const cv::Mat& markerImage) {

		// Convert to image of ints
		markerImage.convertTo(markers,CV_32S);
	  }

	  cv::Mat process(const cv::Mat &image) {

		// Apply watershed
		cv::watershed(image,markers);

		return markers;
	  }

	  // Return result in the form of an image
	  cv::Mat getSegmentation() {
		  
		cv::Mat tmp;
		// all segment with label higher than 255
		// will be assigned value 255
		markers.convertTo(tmp,CV_8U);

		return tmp;
	  }

	  // Return watershed in the form of an image以图像的形式返回分水岭
	  cv::Mat getWatersheds() {
	
		cv::Mat tmp;
		//在变换前，把每个像素p转换为255p+255（在conertTo中实现）
		markers.convertTo(tmp,CV_8U,255,255);

		return tmp;
	  }
};
#endif


int main ()
{
    // Read input image  
    cv::Mat image1= cv::imread("pic/view_mini1.jpg");  
    if (!image1.data)  
        return 0;   
// Display the color image  
    cv::resize(image1, image1, cv::Size(), 0.7, 0.7);  
    cv::namedWindow("Original Image1");  
    cv::imshow("Original Image1",image1);  

    // Identify image pixels with object  
      
    Mat binary;  
    cv::cvtColor(image1,binary,COLOR_BGRA2GRAY);  
    cv::threshold(binary,binary,30,255,THRESH_BINARY_INV);//阈值分割原图的灰度图，获得二值图像  
    // Display the binary image  
    cv::namedWindow("binary Image1");  
    cv::imshow("binary Image1",binary);  
    waitKey();  
      
    // CLOSE operation  
    cv::Mat element5(10,10,CV_8U,cv::Scalar(1));//5*5正方形，8位uchar型，全1结构元素  
    cv::Mat fg1;  
    cv::morphologyEx(binary, fg1,cv::MORPH_CLOSE,element5,Point(-1,-1),1);// 闭运算填充物体内细小空洞、连接邻近物体  
  
    // Display the foreground image  
    cv::namedWindow("Foreground Image");  
    cv::imshow("Foreground Image",fg1);  
    waitKey();  
// Identify image pixels without objects  
      
    cv::Mat bg1;  
    cv::dilate(binary,bg1,cv::Mat(),cv::Point(-1,-1),4);//膨胀4次，锚点为结构元素中心点  
    cv::threshold(bg1,bg1,1,128,cv::THRESH_BINARY_INV);//>=1的像素设置为128（即背景）  
    // Display the background image  
    cv::namedWindow("Background Image");  
    cv::imshow("Background Image",bg1);  
    waitKey();  

    //Get markers image  
  
    Mat markers1 = fg1 + bg1; //使用Mat类的重载运算符+来合并图像。  
    cv::namedWindow("markers Image");  
    cv::imshow("markers Image",markers1);  
    waitKey();  

    // Apply watershed segmentation  
  
    WatershedSegmenter segmenter1;  //实例化一个分水岭分割方法的对象  
    segmenter1.setMarkers(markers1);//设置算法的标记图像，使得水淹过程从这组预先定义好的标记像素开始  
    segmenter1.process(image1);     //传入待分割原图  
       
    // Display segmentation result  
    cv::namedWindow("Segmentation1");  
    cv::imshow("Segmentation1",segmenter1.getSegmentation());//将修改后的标记图markers转换为可显示的8位灰度图并返回分割结果（白色为前景，灰色为背景，0为边缘）  
    waitKey();  
        // Display watersheds  
    cv::namedWindow("Watersheds1");  
    cv::imshow("Watersheds1",segmenter1.getWatersheds());//以图像的形式返回分水岭（分割线条）  
    waitKey();  

    // Get the masked image  
    Mat maskimage = segmenter1.getSegmentation();  
    cv::threshold(maskimage,maskimage,250,1,THRESH_BINARY);  
    cv::cvtColor(maskimage,maskimage,COLOR_GRAY2BGR);  
  
    maskimage = image1.mul(maskimage);  
    cv::namedWindow("maskimage");  
    cv::imshow("maskimage",maskimage);  
    waitKey();  
  
    // Turn background (0) to white (255)  
    int nl= maskimage.rows; // number of lines  
    int nc= maskimage.cols * maskimage.channels(); // total number of elements per line  
  
    for (int j=0; j<nl; j++) {  
         uchar* data= maskimage.ptr<uchar>(j);  
        for (int i=0; i<nc; i++)   
        {  
            // process each pixel ---------------------  
            if (*data==0) //将背景由黑色改为白色显示  
                *data=255;  
            data++;//指针操作：如为uchar型指针则移动1个字节，即移动到下1列  
        }  
     }  
    cv::namedWindow("result");  
    cv::imshow("result",maskimage);  

    cv::boundingRect()
    waitKey();  

}