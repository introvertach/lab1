#include "Filter.h"

template <class T>
T clamp(T value, T max, T min)
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}

QImage Filter::process(const QImage& img) const
{
	QImage result(img);

	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = calcNewPixelColor(img, x, y);
			result.setPixelColor(x, y, color);
		}

	return result;
}

QColor InvertFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	color.setRgb(255 - color.red(), 255 - color.green(), 255 - color.blue());
	return color;
}

QColor MatrixFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	float ReturnR = 0;
	float ReturnG = 0;
	float ReturnB = 0;
	int radius = mKernel.getRadius();
	int size = mKernel.getSize();

	for(int i = -radius; i <= radius; i++)
		for (int j = -radius; j <= radius; j++)
		{
			int idx = (i + radius) * size + (j + radius);
			QColor color = img.pixelColor(clamp(x + j, img.width() - 1, 0), 
				clamp(y + i, img.height() - 1, 0));

			ReturnR += color.red() * mKernel[idx];
			ReturnG += color.green() * mKernel[idx];
			ReturnB += color.blue() * mKernel[idx];
		}
	return QColor(clamp(ReturnR, 255.f, 0.f), clamp(ReturnG, 255.f, 0.f), 
		clamp(ReturnB, 255.f, 0.f));
}

QColor GrayScaleFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	int Intensity = 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue();
	color.setRgb(Intensity, Intensity, Intensity);
	return color;
}

QColor SepiaFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	float Intensity = 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue();
	int k = 40;
	color.setRgb(clamp(Intensity + 2 * k, 255.f, 0.f), clamp(Intensity + 0.5f * k, 255.f, 0.f),
		clamp(Intensity - k, 255.f, 0.f));
	return color;
}

QColor IncreaseBrightnessFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	float k = 50;
	color.setRgb(clamp(color.red() + k, 255.f, 0.f), clamp(color.green() + k, 255.f, 0.f),
		clamp(color.blue() + k, 255.f, 0.f));
	return color;
}

QImage PerfectReflector::process(const QImage& img) const
{
	QImage result(img);

	QColor maxColor;
	maxColor.setRgb(0, 0, 0);
	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			if (maxColor.red() < img.pixelColor(x, y).red())
				maxColor.setRed(img.pixelColor(x, y).red());

			if (maxColor.green() < img.pixelColor(x, y).green())
				maxColor.setGreen(img.pixelColor(x, y).green());

			if (maxColor.blue() < img.pixelColor(x, y).blue())
				maxColor.setBlue(img.pixelColor(x, y).blue());
		}

	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = calcNewPixelColor(img, x, y, maxColor);
			result.setPixelColor(x, y, color);
		}

	return result;
}

QColor PerfectReflector::calcNewPixelColor(const QImage& img, int x, int y, QColor& maxColor) const
{
	QColor color = img.pixelColor(x, y);
	color.setRgb(color.red() * 255 / maxColor.red(), color.green() * 255 / maxColor.green(), color.blue() * 255 / maxColor.blue());
	return color;
}

QImage HistogramStretch::process(const QImage& img) const
{
	QImage result(img);

	QColor minColor;
	QColor maxColor;
	minColor.setRgb(255, 255, 255);
	maxColor.setRgb(0, 0, 0);
	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			if (maxColor.red() > img.pixelColor(x, y).red())
				minColor.setRed(img.pixelColor(x, y).red());
			if (maxColor.red() < img.pixelColor(x, y).red())
				maxColor.setRed(img.pixelColor(x, y).red());

			if (maxColor.green() > img.pixelColor(x, y).green())
				minColor.setGreen(img.pixelColor(x, y).green());
			if (maxColor.green() < img.pixelColor(x, y).green())
				maxColor.setGreen(img.pixelColor(x, y).green());

			if (maxColor.blue() > img.pixelColor(x, y).blue())
				minColor.setBlue(img.pixelColor(x, y).blue());
			if (maxColor.blue() < img.pixelColor(x, y).blue())
				maxColor.setBlue(img.pixelColor(x, y).blue());
		}

	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = calcNewPixelColor(img, x, y, minColor, maxColor);
			result.setPixelColor(x, y, color);
		}

	return result;
}

QColor HistogramStretch::calcNewPixelColor(const QImage& img, int x, int y, QColor& minColor, QColor& maxColor) const
{
	QColor color = img.pixelColor(x, y);
	color.setRgb(clamp(255 * (color.red() - minColor.red()) / (maxColor.red() - minColor.red()), 255, 0),
		clamp(255 * (color.green() - minColor.green()) / (maxColor.green() - minColor.green()), 255, 0),
		clamp(255 * (color.blue() - minColor.blue()) / (maxColor.blue() - minColor.blue()), 255, 0));
	return color;
}

QImage MedianFilter::process(const QImage& img) const
{
	QImage result(img);

	int size = 2 * radius + 1;
	Kernel Kernel1(radius), Kernel2(radius), Kernel3(radius);
	QColor color;
	for (int x = radius; x < img.width() - radius; x++)
		for (int y = radius; y < img.height() - radius; y++)
		{
			for(int a = x - radius; a < x + radius; a++)
				for (int b = y - radius; b < y + radius; b++)
				{
					int idx = (a - x + radius) * size + b - y + radius;
					Kernel1[idx] = static_cast<float>(img.pixelColor(a, b).red());
					Kernel2[idx] = static_cast<float>(img.pixelColor(a, b).green());
					Kernel3[idx] = static_cast<float>(img.pixelColor(a, b).blue());
				}

			for (int i = 0; i < size * size - 1; i++)
			{
				for (int j = i+1; j < size * size; j++)
				{
					if (Kernel1[i] > Kernel1[j])
					{
						float temp = Kernel1[i];
						Kernel1[i] = Kernel1[j];
						Kernel1[j] = temp;
					}

					if (Kernel2[i] > Kernel2[j])
					{
						float temp = Kernel2[i];
						Kernel2[i] = Kernel2[j];
						Kernel2[j] = temp;
					}

					if (Kernel3[i] > Kernel3[j])
					{
						float temp = Kernel3[i];
						Kernel3[i] = Kernel3[j];
						Kernel3[j] = temp;
					}
				}
			}
			color.setRgb(Kernel1[(size * size) / 2], Kernel2[(size * size) / 2], 
				Kernel3[(size * size) / 2]);
			result.setPixelColor(x, y, color);
		}
	return result;
}

