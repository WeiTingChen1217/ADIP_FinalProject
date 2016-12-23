//
//  main.cpp
//  ADIP_FinalProject
//
//  Created by WeiTingChen on 12/8/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#define Morphology_Mode 1
#define Canny_Mode 2
#define Standard_Hough_Mode 3

/// key board define
#define esc 27 // the key esc
#define back_mode 49 // the key 1
#define loop_mode 50 // the key 2
#define next_mode 51 // the key 3
#define reseat_mode 114 // the key R
#define equalization_mode 101 // the key E
#define save_image 115 // the key S



using namespace cv;
using namespace std;


/// Global variables
Mat src, src_row, src_before_equ;
Mat dst_morphology, dst_canny, dst_standard_hough, dst_equalization;

int morph_elem = 0;
int morph_operator = 1;
int morph_size = 9;
int const max_elem = 2;
int const max_operator = 4;
int const max_kernel_size = 21;

int threshold1 = 127;
int threshold2 = 127;
int const max_threshold1 = 255;
int const max_threshold2 = 255;

int min_threshold = 50;
int max_trackbar = 150;
int s_trackbar = max_trackbar;

const char* morphology_name = "Morphology Transformations Demo";
const char* canny_name = "Canny Transformations Demo";
const char* standard_name = "Standard Hough Lines Demo";
const char* Equ_source_name = "Equ_source image";

const char* str_input_image_1 = "/Users/WeiTing_Chen/Desktop/ADIP_FinalProject/doc/ADIP_Final/nolight_lv1/IMG_2334.JPG";
const char* save_name = "../../output/IMG_2334";

vector<Mat> function_ab;


/** Function Headers */
void Morphology_Operations( int, void* );
void Canny_Operations(int, void*);
void Standard_Hough( int, void* );


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "ADIP Final Project.\n";
    
//    char str_input_image_1[] = "/Users/WeiTing_Chen/Desktop/ADIP_FinalProject/doc/ADIP_Final/nolight_lv1/IMG_2334.JPG";
    
    /// Load an image
    src = imread( str_input_image_1, IMREAD_GRAYSCALE );
    
    if( src.empty() )
    { return -1; }
    
    Size size(src.cols/4,src.rows/4);
    resize(src, src, size);
    
    src_row = src.clone();

    int mode = 1, keyIn = 0;
    int His_Equ_Flag = 0;
    while (1) {
        switch (mode) {
            case Morphology_Mode:
//=========================================
//  Morphology_Operations
//=========================================
                /// Create window
                namedWindow(morphology_name, WINDOW_AUTOSIZE);
                
                /// Create Trackbar to select Morphology operation
                createTrackbar("Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat", morphology_name, &morph_operator, max_operator, Morphology_Operations );
                
                /// Create Trackbar to select kernel type
                createTrackbar( "Element:\n 0: Rect - 1: Cross - 2: Ellipse", morphology_name,&morph_elem, max_elem,Morphology_Operations );
                
                /// Create Trackbar to choose kernel size
                createTrackbar( "Kernel size:\n 2n +1", morphology_name, &morph_size, max_kernel_size,Morphology_Operations );
                
                /// Default start
                Morphology_Operations( 0, 0 );
                
                keyIn = waitKey(0);
                if(back_mode == keyIn){
                    keyIn = reseat_mode;
                    break;
                }
                else if (loop_mode == keyIn){
                    printf("Do Morphology_Mode again.\n");
                    mode = Morphology_Mode;
                    src = dst_morphology.clone();
                    break;
                }
                else if(next_mode == keyIn){
                    printf("Jump to the Canny_Mode.\n");
                    mode = Canny_Mode;
                    moveWindow(morphology_name, 500, 0);
                    moveWindow(canny_name, 0, 0);
                    break;
                }
                else if (equalization_mode == keyIn){
                    printf("Equalization image.\n");
                    His_Equ_Flag = 1;
                    
                    src = dst_morphology.clone();
                    // Back Up
                    src_before_equ = src.clone();

                    /// Apply Histogram Equalization
                    equalizeHist( src, dst_equalization );

                    /// Display results
                    namedWindow( Equ_source_name, WINDOW_AUTOSIZE );
                    moveWindow(Equ_source_name, 500, 0);
                    imshow( Equ_source_name, src_before_equ );
                    
                    src = dst_equalization.clone();
                    break;
                }
                else
                    break;
                
                
            case Canny_Mode:
//=========================================
//  Canny_Operations
//=========================================
                /// Create window
                namedWindow(canny_name, WINDOW_AUTOSIZE);
                
                /// Create Trackbar to select threshold1
                createTrackbar("brightness: ", canny_name, &threshold1, max_threshold1, Canny_Operations );
                
                /// Create Trackbar to select threshold2
                createTrackbar( "contrast: ", canny_name, &threshold2, max_threshold2, Canny_Operations );
                
                /// Default start
                Canny_Operations(0, 0);
                
                keyIn = waitKey(0);
                if(back_mode == keyIn){
                    printf("Go back to Morphology_Mode.\n");
                    mode = Morphology_Mode;
                    src = src_row.clone();
                    destroyWindow(standard_name);
                    moveWindow(morphology_name, 0, 0);
                    moveWindow(canny_name, 500, 0);
                    break;
                }
                else if(loop_mode == keyIn){
                    printf("You are in the Canny_Mode.\nIn this mode no loop mode.\n");
                    mode = Canny_Mode;
                    break;
                }
                else if(next_mode == keyIn){
                    printf("Jump to the Standard_Hough_Mode.\n");
                    mode = Standard_Hough_Mode;
                    destroyWindow(morphology_name);
                    moveWindow(canny_name, 500, 0);
                    moveWindow(standard_name, 0, 0);
                    break;
                }
                else
                    break;
                
                
            case Standard_Hough_Mode:
//=========================================
//  Standard_Hough
//=========================================
                /// Create window
                namedWindow( standard_name, WINDOW_AUTOSIZE );
                
                /// Create Trackbar to select s_trackbar
                createTrackbar( "Thres:", standard_name, &s_trackbar, max_trackbar, Standard_Hough);
                
                /// Default start
                Standard_Hough(0, 0);

                keyIn = waitKey(0);
                if(back_mode == keyIn){
                    printf("Go back to Canny_Mode.\n");
                    mode = Canny_Mode;
                    src = src_row.clone();
                    moveWindow(canny_name, 0, 0);
                    moveWindow(standard_name, 500, 0);
                    break;
                }
                else if(loop_mode == keyIn){
                    printf("You are in the Standard_Hough_Mode.\nIn this mode no loop mode.\n");
                    mode = Standard_Hough_Mode;
                    break;
                }
                else if(next_mode == keyIn){
                    printf("!!!! this is the Last mode. !!!!\n");
                    mode = Standard_Hough_Mode;
                    
                    vector<Mat> function_xy;
                    function_xy.clear();
                    for( size_t i = 0; i < function_ab.size(); i++ )
                        for( size_t j = i + 1; j < function_ab.size(); j++ )
                            if(i != j){
                                Mat A = (Mat_<float>(2, 2) <<
                                         function_ab[i].at<float>(0, 0), -1,
                                         function_ab[j].at<float>(0, 0), -1
                                         );
                                Mat B = (Mat_<float>(2, 1) <<
                                         -function_ab[i].at<float>(1, 0),
                                         -function_ab[j].at<float>(1, 0)
                                         );
                                Mat x_out;

                                solve(A, B, x_out);
                                
                                
                                if (
                                    x_out.at<float>(0, 0) < 0 ||
                                    x_out.at<float>(1, 0) < 0 ||
                                    x_out.at<float>(0, 0) > src.cols ||
                                    x_out.at<float>(1, 0) > src.rows    ) {
                                    printf("......");
                                    cout << x_out << endl;
                                }else
                                    function_xy.push_back(x_out);
                            }

                    /// Print out
                    cout << "Print out dot" << endl;
                    for( size_t i = 0; i < function_xy.size(); i++ )
                        cout << function_xy[i] << endl;
                    cout << endl << endl;

                    break;
                }
                else
                    break;
                
            default:
                break;
        }
        
        if (reseat_mode == keyIn){
            printf("restart...\n...\n...\n...\n");
            mode = Morphology_Mode;
            src = src_row.clone();
            morph_elem = 0;
            morph_size = 0;
            morph_operator = 0;
            destroyWindow(canny_name);
            destroyWindow(standard_name);
            destroyWindow(Equ_source_name);
        }
        else if (esc == keyIn){
            break;
        }
        else if (save_image == keyIn){
            printf("\n\n\n\nSave Image.\n");
            // Define save file name
            char str_save_name[100];
            char str_do_number[30];
            strcpy(str_save_name, save_name);
            /// Morphology_Operations
            sprintf(str_do_number, "_Morph_%d", morph_operator);
            strcat(str_save_name, str_do_number);
            sprintf(str_do_number, "_%d", morph_elem);
            strcat(str_save_name, str_do_number);
            if (morph_size > 10) {
                sprintf(str_do_number, "_%d", morph_size);
                strcat(str_save_name, str_do_number);
            }else{
                sprintf(str_do_number, "_0%d", morph_size);
                strcat(str_save_name, str_do_number);
            }
            
            /// Canny_Operations
            sprintf(str_do_number, "_Canny");
            strcat(str_save_name, str_do_number);
            if (threshold1 > 100) {
                sprintf(str_do_number, "_%d", threshold1);
                strcat(str_save_name, str_do_number);
            }else if(threshold1 > 10){
                sprintf(str_do_number, "_0%d", threshold1);
                strcat(str_save_name, str_do_number);
            }
            else{
                sprintf(str_do_number, "_00%d", threshold1);
                strcat(str_save_name, str_do_number);
            }
            if (threshold2 > 100) {
                sprintf(str_do_number, "_%d", threshold2);
                strcat(str_save_name, str_do_number);
            }else if(threshold2 > 10){
                sprintf(str_do_number, "_0%d", threshold2);
                strcat(str_save_name, str_do_number);
            }
            else{
                sprintf(str_do_number, "_00%d", threshold2);
                strcat(str_save_name, str_do_number);
            }
            
            /// Standard_Hough
            sprintf(str_do_number, "_Hough");
            strcat(str_save_name, str_do_number);
            sprintf(str_do_number, "_%d", s_trackbar);
            strcat(str_save_name, str_do_number);
            
            /// Histogram equalization
            
            if(His_Equ_Flag){
                sprintf(str_do_number, "_Equ");
                strcat(str_save_name, str_do_number);
            }
            
            strcat(str_save_name, ".jpg");
            puts(str_save_name);
            
            // Save image
            imwrite(str_save_name, dst_standard_hough);
            break;
        }

    }

    printf("END\n");
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
    cvtColor( dst_canny, dst_standard_hough, COLOR_GRAY2BGR );
    
    /// 1. Use Standard Hough Transform
    HoughLines( dst_canny, s_lines, 1, CV_PI/180, min_threshold + s_trackbar, 0, 0 );
    
    /// Clear all contant
    function_ab.clear();
    
    /// Show the result
    for( size_t i = 0; i < s_lines.size(); i++ )
    {
        float r = s_lines[i][0], t = s_lines[i][1];
        double cos_t = cos(t), sin_t = sin(t);
        double x0 = r*cos_t, y0 = r*sin_t;
        double alpha = 1000;
        
        Point pt1( cvRound(x0 + alpha*(-sin_t)), cvRound(y0 + alpha*cos_t) );
        Point pt2( cvRound(x0 - alpha*(-sin_t)), cvRound(y0 - alpha*cos_t) );
        line( dst_standard_hough, pt1, pt2, Scalar(255,0,0), 3, LINE_AA);
        
        cout << "pt1: " << pt1 << "\tpt2: " << pt2 << endl;
        
//        /// Calculate a and b to generate line function
//        Mat A = (Mat_<float>(2, 2) <<
//                 pt1.x, 1,
//                 pt2.x, 1);
//        Mat B = (Mat_<float>(2, 1) <<
//                 pt1.y,
//                 pt2.y);
//        Mat x_out;
//        
//        solve(A, B, x_out);
//        
//        function_ab.push_back(x_out);
        
        float slope, intercept;
        float dx, dy;
        
        dx = pt2.x - pt1.x;
        dy = pt2.y - pt1.y;
        
        if (dx == 0) {
            slope = 1;
            intercept = -(slope * pt1.x);
        }else{
            slope = dy / dx;
            intercept = pt1.y - slope * pt1.x;
        }
        Mat perameter = (Mat_<float>(2, 1) <<
                           slope,
                           intercept);;
        
        function_ab.push_back(perameter);
    }
    
    /// Print out x
    for( size_t i = 0; i < s_lines.size(); i++ )
        cout << function_ab[i] << endl;
    cout << endl << endl;
    
    imshow( standard_name, dst_standard_hough );
}
