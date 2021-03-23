#pragma once
#include<qimage.h>
#include<memory>
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