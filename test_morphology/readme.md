將圖片`coins_512x512.raw`的雜訊去除`Morphology_2.cpp`及黑色圓點去除 `Morphology_2-2.cpp`

### Morphology_2.cpp

**Parameters**
- **Operator** - 0 (Closing)
- **Element** - 0
- **Kernel size** - 4

結果
![圖片](/output/test_morphology/coins_512x512_1.jpg)

### Morphology_2-2.cpp

**Parameters**
- **Operator** - 1 (Opening)
- **Element** - 0
- **Kernel size** - 11

結果
![圖片](/output/test_morphology/coins_512x512_2.jpg)
