//
//  main.cpp
//  test_findContours
//
//  Created by WeiTingChen on 12/9/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//
//  Reference : http://monkeycoding.com/?p=622
//


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

/// Global variables
Mat src, dst;

int threshold1 = 50;
int threshold2 = 150;
int const max_threshold1 = 255;
int const max_threshold2 = 255;

const char* window_name = "Canny Transformations Demo";

/** Function Headers */
void Canny_Operations(int, void*);

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    char str_input_image_1[] = "/Users/WeiTing_Chen/Desktop/ADIP_FinalProject/output/test_morphology/IMG_2335-2_0_0_21.jpg";
    
    /// Load an image
    src = imread( str_input_image_1, IMREAD_GRAYSCALE );
    
    if( src.empty() )
    { return -1; }
    
    /// Create window
    namedWindow(window_name, WINDOW_NORMAL);
    
    /// Create Trackbar to select threshold1
    createTrackbar("threshold1: ", window_name, &threshold1, max_threshold1, Canny_Operations );
    
    /// Create Trackbar to select threshold2
    createTrackbar( "threshold2: ", window_name,&threshold2, max_threshold2, Canny_Operations );
    
    
    /// Default start
    Canny_Operations(0, 0);
    
    waitKey(0);
    
    
    // Define save file name
    char str_save_name[100];
    char str_do_number[30];
    strcpy(str_save_name,
           "../../output/test_morphology/IMG_2335-3");

    
    if (threshold1 > 100) {
        sprintf(str_do_number, "_%d", threshold1);
        strcat(str_save_name, str_do_number);
    }else if (threshold1 > 10) {
        sprintf(str_do_number, "_0%d", threshold1);
        strcat(str_save_name, str_do_number);
    }else{
        sprintf(str_do_number, "_00%d", threshold1);
        strcat(str_save_name, str_do_number);
    }
    if (threshold2 > 100) {
        sprintf(str_do_number, "_%d", threshold2);
        strcat(str_save_name, str_do_number);
    }else if (threshold2 > 10) {
        sprintf(str_do_number, "_0%d", threshold2);
        strcat(str_save_name, str_do_number);
    }else{
        sprintf(str_do_number, "_00%d", threshold2);
        strcat(str_save_name, str_do_number);
    }
    
    strcat(str_save_name, ".jpg");
    puts(str_save_name);
    
    // Save image
    imwrite(str_save_name, dst);
    
    return 0;
}

/**
 * @function Morphology_Operations
 */
void Canny_Operations( int, void* )
{
    Canny(src, dst, threshold1, threshold2, 3);
    threshold(dst, dst, 128, 255, THRESH_BINARY_INV);  //反轉影像，讓邊緣呈現黑線
    imshow( window_name, dst );
}
