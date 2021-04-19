#include <QtCore/QCoreApplication>
#include<iostream>
#include<string.h>
#include<qpixmap.h>
#include"Filter.h"

using namespace std;

int main(int argc, char *argv[])
{
    string s = "D:/Projects/QtConsoleApplication1/image.png";
    //string s;
    QImage image;

    /*for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "-p") && (i + 1 < argc))
            s = argv[i + 1];
    }*/

    image.load(QString(s.c_str()));
    image.save("image/source.png");

    /*InvertFilter invert;
    invert.process(image).save("image/invert.png");

    BlurFilter blur(5);
    blur.process(image).save("image/blur5.png");

    GaussinFilter Gauss(7);
    Gauss.process(image).save("image/Gauss7.png");

    GrayScaleFilter gray_scale;
    gray_scale.process(image).save("image/gray_scale.png");

    SepiaFilter sepia;
    sepia.process(image).save("image/sepia.png");

    IncreaseBrightnessFilter brightness;
    brightness.process(image).save("image/brightness.png");

    ySobelFilter y_sobel;
    y_sobel.process(image).save("image/y_sobel.png");

    xSobelFilter x_sobel;
    x_sobel.process(image).save("image/x_sobel.png");

    SharpnessFilter sharpness;
    sharpness.process(image).save("image/sharpness.png");

    PerfectReflector reflector;
    reflector.process(image).save("image/reflector.png");

    HistogramStretch stretch;
    stretch.process(image).save("image/stretch.png");

    MotionBlurFilter motion(8);
    motion.process(image).save("image/motion8.png");

    EmbossingFilter embossing;
    embossing.process(image).save("image/embossing.png");

    MedianFilter median(2);
    median.process(image).save("image/median2.png"); */


}
