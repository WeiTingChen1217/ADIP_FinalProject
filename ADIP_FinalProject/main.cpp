//
//  main.cpp
//  ADIP_FinalProject
//
//  Created by WeiTingChen on 12/8/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


/// Global variables
Mat src, dst_morphology, dst_canny, standard_hough;

int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

int threshold1 = 50;
int threshold2 = 150;
int const max_threshold1 = 255;
int const max_threshold2 = 255;

int min_threshold = 50;
int max_trackbar = 150;
int s_trackbar = max_trackbar;
int p_trackbar = max_trackbar;

const char* morphology_name = "Morphology Transformations Demo";
const char* canny_name = "Canny Transformations Demo";
const char* standard_name = "Standard Hough Lines Demo";


/** Function Headers */
void Morphology_Operations( int, void* );
void Canny_Operations(int, void*);
void Standard_Hough( int, void* );


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "ADIP Final Project.\n";
    
    int c = waitKey(0);
    printf("%d", c);
    
    waitKey(0);
    
    char str_input_image_1[] = "/Users/WeiTing_Chen/Desktop/ADIP_FinalProject/doc/ADIP_Final/nolight_lv1/IMG_2337.JPG";
    
    /// Load an image
    src = imread( str_input_image_1, IMREAD_GRAYSCALE );
    
    if( src.empty() )
    { return -1; }
    
    Size size(src.cols/4,src.rows/4);
    resize(src, src, size);

//=========================================
//  Morphology_Operations
//=========================================
    /// Create window
    namedWindow(morphology_name, WINDOW_NORMAL);
    
    /// Create Trackbar to select Morphology operation
    createTrackbar("Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat", morphology_name, &morph_operator, max_operator, Morphology_Operations );
    
    /// Create Trackbar to select kernel type
    createTrackbar( "Element:\n 0: Rect - 1: Cross - 2: Ellipse", morphology_name,&morph_elem, max_elem,Morphology_Operations );
    
    /// Create Trackbar to choose kernel size
    createTrackbar( "Kernel size:\n 2n +1", morphology_name,
                   &morph_size, max_kernel_size,Morphology_Operations );
    
//=========================================
//  Canny_Operations
//=========================================
    /// Create window
    namedWindow(canny_name, WINDOW_NORMAL);
    
    /// Create Trackbar to select threshold1
    createTrackbar("threshold1: ", canny_name, &threshold1, max_threshold1, Canny_Operations );
    
    /// Create Trackbar to select threshold2
    createTrackbar( "threshold2: ", canny_name,&threshold2, max_threshold2, Canny_Operations );
    
//=========================================
//  Standard_Hough
//=========================================
    /// Create window
    namedWindow( standard_name, WINDOW_AUTOSIZE );
    
    /// Create Trackbar to select s_trackbar
    createTrackbar( "Thres:", standard_name, &s_trackbar, max_trackbar, Standard_Hough);
    
    
    /// Default start
    Morphology_Operations( 0, 0 );
    Canny_Operations(0, 0);
    Standard_Hough(0, 0);

    
    waitKey(0);
    
    return 0;
}

/**
 * @function Morphology_Operations
 */
void Morphology_Operations( int, void* )
{
    // Since MORPH_X : 2,3,4,5 and 6
    int operation = morph_operator + 2;
    
    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    
    /// Apply the specified morphology operation
    morphologyEx( src, dst_morphology, operation, element );
    imshow( morphology_name, dst_morphology );
}

/**
 * @function Canny_Operations
 */
void Canny_Operations( int, void* )
{
    Mat dst_2;
    Canny(dst_morphology, dst_canny, threshold1, threshold2, 3);
    threshold(dst_canny, dst_2, 128, 255, THRESH_BINARY_INV);  //反轉影像，讓邊緣呈現黑線
    imshow(canny_name, dst_2 );
}

/**
 * @function Standard_Hough
 */
void Standard_Hough( int, void* )
{
    vector<Vec2f> s_lines;
    cvtColor( dst_canny, standard_hough, COLOR_GRAY2BGR );
    
    /// 1. Use Standard Hough Transform
    HoughLines( dst_canny, s_lines, 1, CV_PI/180, min_threshold + s_trackbar, 0, 0 );
    
    /// Show the result
    for( size_t i = 0; i < s_lines.size(); i++ )
    {
        float r = s_lines[i][0], t = s_lines[i][1];
        double cos_t = cos(t), sin_t = sin(t);
        double x0 = r*cos_t, y0 = r*sin_t;
        double alpha = 1000;
        
        Point pt1( cvRound(x0 + alpha*(-sin_t)), cvRound(y0 + alpha*cos_t) );
        Point pt2( cvRound(x0 - alpha*(-sin_t)), cvRound(y0 - alpha*cos_t) );
        line( standard_hough, pt1, pt2, Scalar(255,0,0), 3, LINE_AA);
    }
    
    imshow( standard_name, standard_hough );
}
