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