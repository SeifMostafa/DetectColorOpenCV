#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

void Hist_and_Backproj(int, void* );
Mat image,hsv,hue,frame;
int bins=25;

void bluring(Mat image,int x)
{
    for ( int i = 1; i < x; i = i + 2 )        // 20 for bluring
    {
        medianBlur ( image, image, i );
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    waitKey(0);
}
void histogram(Mat image)
{
    cvtColor( image, hsv, CV_BGR2HSV );

    /// Use only the Hue value
    hue.create( hsv.size(), hsv.depth() );
    int ch[] = { 0, 0 };
    mixChannels( &hsv, 1, &hue, 1, ch, 1 );

    /// Create Trackbar to enter the number of bins
    char* window_image = "Source image";
    namedWindow( window_image, CV_WINDOW_AUTOSIZE );
    createTrackbar("* Hue  bins: ", window_image, &bins, 180, Hist_and_Backproj );
    Hist_and_Backproj(0, 0);

    /// Show the image
    imshow( window_image, image );
    /// Wait until user exits the program
    waitKey(0);
}
Mat detectcolor(Mat image)
{
    Mat3b hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);
    /*Hue values of basic colors

        Orange  0-22
        Yellow 22- 38
        Green 38-75
        Blue 75-130
        Violet 130-160
        Red 160-179
    */
    Mat1b mask1, mask2;
    inRange(hsv, Scalar(0, 70, 50), Scalar(10, 255, 255), mask1);
    inRange(hsv, Scalar(170, 70, 50), Scalar(180, 255, 255), mask2);

    Mat1b mask = mask1 | mask2;
    return mask;
}
int main(int argc, char** argv )
{

    // image = imread( "/home/seifmostafa/Documents/fci/Level3_secTerm/ImageProcessing/project/phases/dataset/smallsets.png", 1 );
    ///bluring(image,20);
    ///histogram(image);
    // imshow("Result",detectcolor(image));
    //waitKey(0);
    /*  VideoCapture cap(0);
      if (!cap.isOpened())
      {
          cout << "Capture could not be opened succesfully" << endl;
          return -1;
      }
      namedWindow("Video", 0);
      while (char(waitKey(100)) != 'q' && cap.isOpened())
      {
          cap >> frame;
          if (frame.empty())
          {
              cout << "Video over" << endl;
              break;
          }
          imshow("Video",detectcolor(frame));
      }*/


    Mat img(255,255, CV_8UC3);
    for(int y=0; y<img.rows; y++)
    {
        for(int x=0; x<img.cols; x++)
        {
            // get pixel
            Vec3b & color =img.at<Vec3b>(y,x);;
            color[0] = x;
            color[1] = y;
            color[2] = 255;
            // ... do something to the color ....

            // set pixel
            img.at<Vec3b>(Point(x,y)) = color;
        }
    }
    imshow("Pic",img);
    waitKey(0);
    return 0;
}
void Hist_and_Backproj(int, void* )
{
    MatND hist;
    int histSize = MAX( bins, 2 );
    float hue_range[] = { 0, 180 };
    const float* ranges = { hue_range };

    /// Get the Histogram and normalize it
    calcHist( &hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
    normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );

    /// Get Backprojection
    MatND backproj;
    calcBackProject( &hue, 1, 0, hist, backproj, &ranges, 1, true );

    /// Draw the backproj
    imshow( "BackProj", backproj );

    /// Draw the histogram
    int w = 400;
    int h = 400;
    int bin_w = cvRound( (double) w / histSize );
    Mat histImg = Mat::zeros( w, h, CV_8UC3 );

    for( int i = 0; i < bins; i ++ )
    {
        rectangle( histImg, Point( i*bin_w, h ), Point( (i+1)*bin_w, h - cvRound( hist.at<float>(i)*h/255.0 ) ), Scalar( 0, 0, 255 ), -1 );
    }

    imshow( "Histogram", histImg );
}
