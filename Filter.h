#pragma once
#include<qimage.h>
//#include<memory>
#include<iostream>
#include<cstdlib>
class Filter
{
	protected:
		virtual QColor calcNewPixelColor(const QImage& img, int x, int y) const = 0;
	public:
		virtual ~Filter() = default;
		virtual QImage process(const QImage & img) const;
};

class InvertFilter : public Filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class Kernel
{
protected:
	std::unique_ptr<float[]> data;
	std::size_t radius;
	std::size_t getLen() const { return getSize() * getSize();  }

public:
	Kernel(std::size_t radius) : radius(radius)
	{
		data = std::make_unique<float[]>(getLen()); //выделение памяти под массив
	}
	Kernel(const Kernel& other) : Kernel(other.radius)
	{
		std::copy(other.data.get(), other.data.get() + getLen(), data.get()); //указатель куда копировать, до куда, откуда
	}
	//аксессоры
	std::size_t getRadius() const { return radius; }
	std::size_t getSize() const { return 2 * radius + 1; }
	float operator [] (std::size_t id) const { return data[id]; } //почему const?
	float& operator [] (std::size_t id) { return data[id]; }

};

class MatrixFilter : public Filter
{
protected:
	Kernel mKernel;
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
public:
	MatrixFilter(const Kernel& kernel) : mKernel(kernel) {};
	virtual ~MatrixFilter() = default;
};

class BlurKernel : public Kernel
{
public:
	using Kernel::Kernel;
	BlurKernel(std::size_t radius = 2) : Kernel(radius)
	{
		for (std::size_t i = 0; i < getLen(); i++)
			data[i] = 1.f / getLen();
	}
};

class BlurFilter : public MatrixFilter
{
public:
	BlurFilter(std::size_t radius = 1) : MatrixFilter(BlurKernel(radius)) {}
};

class GaussinKernel : public Kernel
{
public:
	using Kernel::Kernel;
	GaussinKernel(std::size_t radius = 2, float sigma = 3.f) : Kernel(radius)
	{
		float norm = 0;
		int signed_radius = static_cast<int>(radius);

		for(int i = -signed_radius; i <= signed_radius; i++)
			for (int j = -signed_radius; j <= signed_radius; j++)
			{
				std::size_t idx = (i + radius) * getSize() + (j + radius);
				data[idx] = std::exp( -(i * i + j * j) / (sigma * sigma) );
				norm += data[idx];
			}

		for (std::size_t idx = 0; idx < getLen(); idx++)
			data[idx] /= norm;
	}
};

class GaussinFilter : public MatrixFilter
{
public:
	GaussinFilter(std::size_t radius = 1) : MatrixFilter(GaussinKernel(radius)) {}
};

class GrayScaleFilter : public Filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class SepiaFilter : public Filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class IncreaseBrightnessFilter : public Filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class ySobelKernel : public Kernel
{
public:
	using Kernel::Kernel;
	ySobelKernel(std::size_t radius = 1) : Kernel(radius)
	{
		data[0] = -1, data[1] = -2, data[2] = -1, 
			data[3] = 0, data[4] = 0, data[5] = 0,
			data[6] = 1, data[7] = 2, data[8] = 1;
	}
};

class ySobelFilter : public MatrixFilter
{
public:
	ySobelFilter(std::size_t radius = 1) : MatrixFilter(ySobelKernel(radius)) {}
};

class xSobelKernel : public Kernel
{
public:
	using Kernel::Kernel;
	xSobelKernel(std::size_t radius = 1) : Kernel(radius)
	{
		data[0] = -1, data[1] = 0, data[2] = 1,
			data[3] = -2, data[4] = 0, data[5] = 2,
			data[6] = -1, data[7] = 0, data[8] = 1;
	}
};

class xSobelFilter : public MatrixFilter
{
public:
	xSobelFilter(std::size_t radius = 1) : MatrixFilter(xSobelKernel(radius)) {}
};

class SharpnessKernel : public Kernel
{
public:
	using Kernel::Kernel;
	SharpnessKernel(std::size_t radius = 1) : Kernel(radius)
	{
		data[0] = 0, data[1] = -1, data[2] = 0,
			data[3] = -1, data[4] = 5, data[5] = -1,
			data[6] = 0, data[7] = -1, data[8] = 0;
	}
};

class SharpnessFilter : public MatrixFilter
{
public:
	SharpnessFilter(std::size_t radius = 1) : MatrixFilter(SharpnessKernel(radius)) {}
};

class PerfectReflector : public Filter
{
public:
	QColor calcNewPixelColor(const QImage& img, int x, int y, QColor& maxColor) const;
	QImage process(const QImage& img) const;
};

class HistogramStretch : public Filter
{
public:
	QColor calcNewPixelColor(const QImage& img, int x, int y, QColor& minColor, QColor& maxColor) const;
	QImage process(const QImage& img) const;
};

class MotionBlurKernel : public Kernel
{
public:
	using Kernel::Kernel;
	MotionBlurKernel(std::size_t radius = 2) : Kernel(radius)
	{
		for (std::size_t idx = 0; idx < getLen(); idx++)
		{
			if (idx % getSize() == 0)
				data[idx] = 1.f / getSize();
			else
				data[idx] = 0;
		}
	}
};

class MotionBlurFilter : public MatrixFilter
{
public:
	MotionBlurFilter(std::size_t radius = 2) : MatrixFilter(MotionBlurKernel(radius)) {}
};

class EmbossingKernel : public Kernel
{
public:
	using Kernel::Kernel;
	EmbossingKernel(std::size_t radius = 1) : Kernel(radius)
	{
		data[0] = 0, data[1] = 1, data[2] = 0,
			data[3] = 1, data[4] = 0, data[5] = -1,
			data[6] = 0, data[7] = -1, data[8] = 0;
	}
};

class EmbossingFilter : public MatrixFilter
{
public:
	EmbossingFilter(std::size_t radius = 1) : MatrixFilter(EmbossingKernel(radius)) {}
	QColor calcNewPixelColor(const QImage& img, int x, int y) const;
};

class MedianFilter //: public MatrixFilter
{
private:
	std::size_t radius;
public:
	MedianFilter(std::size_t radius = 1) : radius(radius) {}
	QImage process(const QImage& img) const;
};

class MathMorphKernel : public Kernel
{
public:
	using Kernel::Kernel;
	MathMorphKernel(std::size_t radius = 1) : Kernel(radius)
	{
		std::cout << "Сhange a structural element (y/n)?";
		char a;
		std::cin >> a;
		if (a == 'y')
		{
			std::cout << "Radius: ";
			std::cin >> radius;
			for (std::size_t idx = 0; idx < getLen(); idx++)
			{
				std::cout << "[" << idx / getSize() << "]" <<
					"[" << idx % getSize() << "] : ";
				std::cin >> data[idx];
			}
		}
		if (a == 'n')
		{
			for (std::size_t idx = 0; idx < getLen(); idx++)
				data[idx] = 1;
		}
	}
};

class DilationFilter : public MatrixFilter
{
public:
	DilationFilter(std::size_t radius = 1) : MatrixFilter(MathMorphKernel(radius)) {}
	DilationFilter(Kernel& kernel) : MatrixFilter(kernel) {}
	QColor calcNewPixelColor(const QImage& img, int x, int y) const;
};

class ErosionFilter : public MatrixFilter
{
public:
	ErosionFilter(std::size_t radius = 1) : MatrixFilter(MathMorphKernel(radius)) {}
	ErosionFilter(Kernel& kernel) : MatrixFilter(kernel) {}
	QColor calcNewPixelColor(const QImage& img, int x, int y) const;
};

class MathGradFilter
{
private:
	std::size_t radius;
public:
	MathGradFilter(std::size_t radius = 1) : radius(radius) {}
	QImage process(const QImage& img) const;
};

class TransferFilter : public Filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

class RotationFilter : public Filter
{
private:
	int x0, y0, a;
public:
	RotationFilter(int x0 = 0, int y0 = 0, int a = 0) : x0(x0), y0(y0), a(a) {}
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};