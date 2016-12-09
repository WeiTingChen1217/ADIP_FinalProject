//
//  Morphology_2-3.cpp
//  ADIP_FinalProject
//
//  Created by WeiTingChen on 12/8/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

/// Global variables
Mat src, dst;

int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

const char* window_name = "Morphology Transformations Demo";


/** Function Headers */
void Morphology_Operations( int, void* );

/**
 * @function main
 */
int main( int, char** argv )
{
    char str_input_image_1[] = "/Users/WeiTing_Chen/Desktop/ADIP_FinalProject/output/test_morphology/IMG_2335_1_0_04.jpg";
    
    /// Load an image
    src = imread( str_input_image_1, IMREAD_GRAYSCALE );
//    Size size(src.cols/4,src.rows/4);
    Size size(src.cols,src.rows);
    resize(src, src, size);
    
    
    if( src.empty() )
    { return -1; }
    
    /// Create window
    namedWindow(window_name, WINDOW_NORMAL);
    
    /// Create Trackbar to select Morphology operation
    createTrackbar("Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat", window_name, &morph_operator, max_operator, Morphology_Operations );
    
    /// Create Trackbar to select kernel type
    createTrackbar( "Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_name,&morph_elem, max_elem,Morphology_Operations );
    
    /// Create Trackbar to choose kernel size
    createTrackbar( "Kernel size:\n 2n +1", window_name,
        &morph_size, max_kernel_size,Morphology_Operations );
    
    /// Default start
    Morphology_Operations( 0, 0 );
    
    waitKey(0);
    
    // Define save file name
    char str_save_name[100];
    char str_do_number[30];
    strcpy(str_save_name,
           "../../output/test_morphology/IMG_2335-2");
    
    sprintf(str_do_number, "_%d", morph_operator);
    strcat(str_save_name, str_do_number);
    sprintf(str_do_number, "_%d", morph_elem);
    strcat(str_save_name, str_do_number);
    if (morph_size < 10) {
        sprintf(str_do_number, "_0%d", morph_size);
        strcat(str_save_name, str_do_number);
    }else{
        sprintf(str_do_number, "_%d", morph_size);
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
void Morphology_Operations( int, void* )
{
    // Since MORPH_X : 2,3,4,5 and 6
    int operation = morph_operator + 2;
    
    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    
    /// Apply the specified morphology operation
    morphologyEx( src, dst, operation, element );
    imshow( window_name, dst );
}
