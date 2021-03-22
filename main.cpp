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
    image.save("source.png");

    InvertFilter invert;
    invert.process(image).save("invert.png");
}
