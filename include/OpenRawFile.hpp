//
//  OpenRawFile.hpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 10/31/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#ifndef OpenRawFile_hpp
#define OpenRawFile_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/** @brief Open .row image and convert it to CvMat format
 @param FileNameOri Name of file to be loaded.
 @param lenamat output array.
 */
void OpenRAW(char FileNameOri[], CvMat *lenamat);

#endif /* OpenRawFile_hpp */
