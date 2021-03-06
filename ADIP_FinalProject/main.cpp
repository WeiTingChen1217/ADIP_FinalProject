//
//  main.cpp
//  ADIP_FinalProject
//
//  Created by WeiTingChen on 12/8/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//
#define _CRT_SECURE_NO_WARNINGS
//#define AUTO

#include <iostream>
//#include <direct.h>
#include <opencv2/opencv.hpp>

#define Morphology_Mode         1
#define Canny_Mode              2
#define Standard_Hough_Mode     3
#define ROI_four_point          4


/// key board define
#define esc                     27  // the key esc
#define back_mode               49  // the key 1
#define loop_mode               50  // the key 2
#define next_mode               51  // the key 3
#define reseat_mode             114 // the key R
#define equalization_mode       101 // the key E
#define save_image              115 // the key S
#define Gamma_Correction        103 // the key G



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
int s_trackbar = max_trackbar/2;

const char* morphology_name = "Morphology Transformations Demo";
const char* canny_name = "Canny Transformations Demo";
const char* standard_name = "Standard Hough Lines Demo";
const char* Equ_source_name = "Equ_source image";

const char* str_input_image_1 = "../../doc/test/0.JPG";
const char* save_name = "../../output/IMG0";

vector<Mat> function_ab;


/** Function Headers */
void Morphology_Operations( int, void* );
void Canny_Operations(int, void*);
void Standard_Hough( int, void* );
void calibrateTransform(Mat* _src, Mat* out, vector<Point2f> point);
void rotateImage(IplImage* img, IplImage *img_rotate,int degree);
void findPoint(vector<Point2f> &pt);
void findCornerPoint(vector<Point2f> &pt, vector<Point2f> &corners);
void GammaCorrection(Mat& _src, Mat& _dst, float fGamma);


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "ADIP Final Project.\n";
    std::cout << "Enter \"esc\" to end the project.\n";

    
//    char str_input_image_1[] = "/Users/WeiTing_Chen/Desktop/ADIP_FinalProject/doc/ADIP_Final/nolight_lv1/IMG_2334.JPG";
    
    
    // Read File
    Mat src_color;
    
    if (argc == 2) {
        // Use command parameter
        src = imread(argv[1], IMREAD_GRAYSCALE);
        src_color = imread(argv[1], IMREAD_COLOR);
    }
    else {
        // Use default parameter
        cout << "Use default parameter" << endl;
        src = imread(str_input_image_1, IMREAD_GRAYSCALE);
        src_color = imread(str_input_image_1, IMREAD_COLOR);
    }
    
    // Error Information
    if( src.empty() || src_color.empty())
    {
        if (argc == 2)
            fprintf(stderr, "=== Cannot Read Command File : %s ===\n", argv[1]);
        else
            fprintf(stderr, "=== Cannot Read Default File : %s ===\n", str_input_image_1);
        
        system("PAUSE");
        exit(1);	
    }
    
    
    
    Size size(src.cols/4,src.rows/4);
    resize(src, src, size);
    Size size2(src_color.cols/4, src_color.rows/4);
    resize(src_color, src_color, size2);
    
    src_row = src.clone();
    
    // Local varibale for while loop
    int mode = Morphology_Mode, keyIn = 0;
    int His_Equ_Flag = 0, Gamma_Flag = 0;
    vector<Point2f> pt;
    // Calibrate
    Mat result;
    
    while (1) {
        switch (mode) {
            case Morphology_Mode:
//=========================================
//  Morphology_Operations
//=========================================
            {
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
                
#ifdef AUTO
                keyIn = next_mode;
#else
                keyIn = waitKey(0);
#endif
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
                else if (Gamma_Correction == keyIn){
                    printf("Do Gamma_Correction.\n");
                    Gamma_Flag = 1;

                    
                    Mat dst_gama;
                    GammaCorrection(src, dst_gama, 0.3);
                    
                    /// Display results
                    namedWindow( "Gama", WINDOW_AUTOSIZE );
                    moveWindow("Gama", 500, 0);
                    imshow( "Gama", dst_gama );
                    
                    src = dst_gama.clone();
                    break;
                }
                else
                    break;
            }
                
            case Canny_Mode:
//=========================================
//  Canny_Operations
//=========================================
            {
                /// Create window
                namedWindow(canny_name, WINDOW_AUTOSIZE);
                
                /// Create Trackbar to select threshold1
                createTrackbar("brightness: ", canny_name, &threshold1, max_threshold1, Canny_Operations );
                
                /// Create Trackbar to select threshold2
                createTrackbar( "contrast: ", canny_name, &threshold2, max_threshold2, Canny_Operations );
                
                /// Default start
                Canny_Operations(0, 0);
                
#ifdef AUTO
                keyIn = next_mode;
                
#else
                keyIn = waitKey(0);
#endif

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
            }
                
            case Standard_Hough_Mode:
//=========================================
//  Standard_Hough
//=========================================
            {
                /// Create window
                namedWindow( standard_name, WINDOW_AUTOSIZE );
                
                /// Create Trackbar to select s_trackbar
                createTrackbar( "Thres:", standard_name, &s_trackbar, max_trackbar, Standard_Hough);
                
                /// Default start
                Standard_Hough(0, 0);

#ifdef AUTO
                keyIn = next_mode;
#else
                keyIn = waitKey(0);
#endif

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
                    printf("Jump to the ROI_four_point.\n");
                    mode = ROI_four_point;
                    
                    destroyWindow(canny_name);
                    destroyWindow(standard_name);
                    
                    findPoint(pt);
                    

                    /// Print out
//                    cout << "Print out dot" << endl;
//                    for( size_t i = 0; i < pt.size(); i++ )
//                        cout << pt[i] << endl;
//                    cout << endl << endl;

                    break;
                }
                else
                    break;
            }
            case ROI_four_point:
//=========================================
//  Find book's or paper's corner point
//=========================================
            {
                //  Find book's or paper's corner point
                vector<Point2f> corners;
                findCornerPoint(pt, corners);
                
                calibrateTransform(&src_color, &result, corners);

                namedWindow("Test1", CV_WINDOW_AUTOSIZE);
                moveWindow("Test1", 0, 0);
                imshow("Test1", src_color);
                namedWindow("Test2", CV_WINDOW_AUTOSIZE);
                moveWindow("Test2", src_color.cols, 0);
                imshow("Test2", result);
                
                keyIn = waitKey(0);
                
                break;
            }
//            default:
//                break;
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
            if (s_trackbar > 100) {
                sprintf(str_do_number, "_%d", s_trackbar);
                strcat(str_save_name, str_do_number);
            }else if(s_trackbar > 10){
                sprintf(str_do_number, "_0%d", s_trackbar);
                strcat(str_save_name, str_do_number);
            }
            else{
                sprintf(str_do_number, "_00%d", s_trackbar);
                strcat(str_save_name, str_do_number);
            }
            
        /// Histogram equalization
            
            if(His_Equ_Flag){
                sprintf(str_do_number, "_Equ");
                strcat(str_save_name, str_do_number);
            }
            
        /// Gamma

            if(Gamma_Flag){
                sprintf(str_do_number, "_Gamma");
                strcat(str_save_name, str_do_number);
            }
            
            strcat(str_save_name, ".jpg");
            puts(str_save_name);
            
            // Save image
            imwrite(str_save_name, result);
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
    Canny(dst_morphology, dst_canny, threshold1, threshold2, 3);
    
    //反轉影像，讓邊緣呈現黑線
    Mat dst_2;
    threshold(dst_canny, dst_2, 128, 255, THRESH_BINARY_INV);
    
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
        
        int delta_x = pt1.x - pt2.x;
        int delta_y = pt1.y - pt2.y;
        
        Mat perameter;
        if (delta_x == 0)
            perameter = (Mat_<float>(1, 3) << 1, 0, -(pt1.x));
        else if (delta_y == 0)
            perameter = (Mat_<float>(1, 3) << 0, 1, -(pt1.y));
        else {
//            cout << "delta_x, y:" << delta_x << ", " << delta_y << endl;
            float slope = (float)delta_y / delta_x;
            perameter = (Mat_<float>(1, 3) << slope, -1, pt1.y-slope*pt1.x);
        }
        
        function_ab.push_back(perameter);
    }
    
    /// Print out x
//    for( size_t i = 0; i < s_lines.size(); i++ )
//        cout << function_ab[i] << endl;
//    cout << endl << endl;
    
    imshow( standard_name, dst_standard_hough );
}

/**
 * @calibrate img
 */
void calibrateTransform(Mat* src, Mat* out, vector<Point2f> point){
    
    int width = 0, height = 0;
    int rotate = 0;
    // Get min rectangle
    RotatedRect box = minAreaRect(cv::Mat(point));
    
    // Calculate Size
    if (box.boundingRect().width > box.boundingRect().height) {
        height = box.boundingRect().width;
        width = height / sqrt(2);
        rotate = 1;
    }
    else {
        height = box.boundingRect().height;
        width = height / sqrt(2);
        rotate = 0;
    }
    
    vector<Point2f> pointM(4);
    if (rotate == 1) {
        // Rotate Left 90 degree
        pointM[0] = point[1];
        pointM[1] = point[3];
        pointM[2] = point[0];
        pointM[3] = point[2];
    }else
        pointM = point;
    
    
//    cout << "rotate:" << pointM << endl;
    // Set Transform coords
    vector<Point2f> point_trans(4);
    point_trans[0] = Point2f(0, 0);
    point_trans[1] = Point2f(width, 0);
    point_trans[2] = Point2f(0, height);
    point_trans[3] = Point2f(width, height);
    
    
    // Get Transform Matrix & Output Image
    Mat transform = getPerspectiveTransform(pointM, point_trans);
    
    // Transform
    Size size(width, height); // Set Output size
    warpPerspective(*src, *out, transform, size, INTER_LINEAR, BORDER_CONSTANT);
    
}

void findPoint(vector<Point2f> &pt) {
    pt.clear();
    
    for (size_t i = 0; i < function_ab.size(); i++)
        for (size_t j = i + 1; j < function_ab.size(); j++) {
            
            float a1 = function_ab[i].at<float>(0, 0);
            float b1 = function_ab[i].at<float>(0, 1);
            float c1 = function_ab[i].at<float>(0, 2);
            float a2 = function_ab[j].at<float>(0, 0);
            float b2 = function_ab[j].at<float>(0, 1);
            float c2 = function_ab[j].at<float>(0, 2);
            
            float delta = a1*b2 - a2*b1;
            float delta_x = (-c1)*b2 - (-c2)*b1;
            float delta_y = (-c2)*a1 - (-c1)*a2;
            
            Point2f _pt(delta_x / delta, delta_y / delta);
            
            if (_pt.x < 0 || _pt.x > src.cols ||
                _pt.y < 0 || _pt.y > src.rows) {
                //                                printf("......");
                //                                cout << _pt << endl;
            }
            else
                pt.push_back(_pt);
        }
}


void findCornerPoint(vector<Point2f> &pt, vector<Point2f> &corners) {
    
    Point2f pt0, pt1, pt2, pt3;
    float min_dist_0 = 0, min_dist_1 = 0, min_dist_2 = 0, min_dist_3 = 0;
    
    for (size_t i = 0; i < pt.size(); i++) {
        
        Point2f _pt1 = pt[i];
        
        /// find pt0
        float delta_0_x = abs(0 - _pt1.x);
        float delta_0_y = abs(0 - _pt1.y);
        float dist_0 = sqrt(pow(delta_0_x, 2) + pow(delta_0_y, 2));
        
        if (i == 0 || dist_0 < min_dist_0) {
            pt0 = _pt1;
            min_dist_0 = dist_0;
        }
        
        float delta_1_x = abs(src.cols - _pt1.x);
        float delta_1_y = abs(0 - _pt1.y);
        float dist_1 = sqrt(pow(delta_1_x, 2) + pow(delta_1_y, 2));
        
        if (i == 0 || dist_1 < min_dist_1) {
            pt1 = _pt1;
            min_dist_1 = dist_1;
        }
        
        float delta_2_x = abs(0 - _pt1.x);
        float delta_2_y = abs(src.rows - _pt1.y);
        float dist_2 = sqrt(pow(delta_2_x, 2) + pow(delta_2_y, 2));
        
        if (i == 0 || dist_2 < min_dist_2) {
            pt2 = _pt1;
            min_dist_2 = dist_2;
        }
        
        float delta_3_x = abs(src.cols - _pt1.x);
        float delta_3_y = abs(src.rows - _pt1.y);
        float dist_3 = sqrt(pow(delta_3_x, 2) + pow(delta_3_y, 2));
        
        if (i == 0 || dist_3 < min_dist_3) {
            pt3 = _pt1;
            min_dist_3 = dist_3;
        }
    }
    
    corners.push_back(pt0);
    corners.push_back(pt1);
    corners.push_back(pt2);
    corners.push_back(pt3);
}

/**
 * @ Gamma_Correction
 * Reference: http://www.codepool.biz/image-processing-opencv-gamma-correction.html
 */
void GammaCorrection(Mat& _src, Mat& _dst, float fGamma)
{
    
    unsigned char lut[256];
    
    for (int i = 0; i < 256; i++)
        
    {
        
        lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0), fGamma) * 255.0f);
        
    }
    
    _dst = _src.clone();
    
    const int channels = _dst.channels();
    
    switch (channels)
    
    {
            
        case 1:
            
        {
            
            MatIterator_<uchar> it, end;
            
            for (it = _dst.begin<uchar>(), end = _dst.end<uchar>(); it != end; it++)
                
                *it = lut[(*it)];
            
            break;
            
        }
            
        case 3:
            
        {
            
            MatIterator_<Vec3b> it, end;
            
            for (it = _dst.begin<Vec3b>(), end = _dst.end<Vec3b>(); it != end; it++)
                
            {
                
                (*it)[0] = lut[((*it)[0])];
                
                (*it)[1] = lut[((*it)[1])];
                
                (*it)[2] = lut[((*it)[2])];
                
            }
            
            break;
            
        }
            
    }
    
}
