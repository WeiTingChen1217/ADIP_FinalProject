## 題目1
正確標示出文件/書本封面

* [IMG_2334.JPG](#IMG_2334.JPG)
`/doc/ADIP_Final/nolight_lv1/IMG_2335.JPG`

<br />

<a name="IMG_2334.JPG"></a> 
### 圖片 IMG_2334.JPG

[圖片1](/doc/ADIP_Final/nolight_lv1/IMG_2334.JPG)

主程式位置:[/ADIP_FinalProject/main.cpp](/ADIP_FinalProject/main.cpp)

處理模式一共分為三種 **Morphology_Mode**, **Canny_Mode**, **Hough_Mode**, 其中 **Morphology_Mode** 內有附加 Histogram equalization 功能

- key board "1" : 切換至上一個模式
- key board "2" : 將目前結果再做一次處理
- key board "3" : 切換至下一個模式
- key board "E" : 執行 Histogram equalization (僅再 Morphology_Mode 才有此項功能)
- key board "R" : 重頭來過
- key board "S" : 儲存結果

>>**Parameters**
>>- **str_input_image_1** - 輸入圖片位置

儲存格式:
>>Ex:[IMG_2334_Morph_1_0_07_Canny_050_150_Hough_108.jpg](/output/IMG_2334_Morph_1_0_07_Canny_050_150_Hough_108.jpg)
>>於 Morphology_Mode 參數 Operator=1, Element=0, Kernel size=07
>>於 Canny_Mode 參數 threshold1=50, threshold2=150
>>於 Hough_line_Mode 參數 threshold=108


<br />

