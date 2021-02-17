#pragma once
#include "Math.h"

typedef struct Color
{
private:
	static void RGBToHSV(float offset, float dominantcolor, float colorone, float colortwo, float& h, float& s, float& v)
	{
		v = dominantcolor;
		if (!IS_FLOAT_EQUAL(v, 0.0f))
		{
			float num;
			if (colorone > colortwo)
			{
				num = colortwo;
			}
			else
			{
				num = colorone;
			}
			float num2 = v - num;
			if (!IS_FLOAT_EQUAL(num2, 0.0f))
			{
				s = num2 / v;
				h = offset + (colorone - colortwo) / num2;
			}
			else
			{
				s = 0.0f;
				h = offset + (colorone - colortwo);
			}
			h /= 6.0f;
			if (h < 0.0f)
			{
				h += 1.0f;
			}
		}
		else
		{
			s = 0.0f;
			h = 0.0f;
		}
	}

public:
	Color(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color(float r, float g, float b) : Color(r, g, b, 1.0f)
	{}

	Color() : Color(1.0f, 1.0f, 1.0f, 1.0f)
	{}

	Color(const Color& color) : Color(color.r, color.g, color.b, color.a)
	{}

	float GetGrayScale() 
	{
		return 0.299f * r + 0.587f * g + 0.114f * b;
	}

	void Gamma(float gamma)
	{
		r = pow(r, gamma);
		g = pow(g, gamma);
		b = pow(b, gamma);
	}

	static void RGBToHSV(const Color& color, float& h, float& s, float& v)
	{
		if (color.b > color.g && color.b > color.r)
		{
			RGBToHSV(4.0f, color.b, color.r, color.g, h, s, v);
		}
		else if (color.g > color.r)
		{
			RGBToHSV(2.0f, color.g, color.b, color.r, h, s, v);
		}
		else
		{
			RGBToHSV(0.0f, color.r, color.g, color.b, h, s, v);
		}
	}

	void ToHSV(float& h, float& s, float& v)
	{
		RGBToHSV(*this, h, s, v);
	}

	Color operator+(const Color& color) const
	{
		return Color(color.r + r, color.g + g, color.b + b, color.a + a);
	}

	Color operator-(const Color& color) const
	{
		return Color(r - color.r, g - color.g, b - color.b, a - color.a);
	}

	Color operator*(const Color& color) const
	{
		return Color(r * color.r, g * color.g, b * color.b, a * color.a);
	}

	Color operator*(float scale) const
	{
		return Color(r * scale, g * scale, b * scale, a * scale);
	}

	Color operator/(const Color& color) const
	{
		return Color(r / color.r, g / color.g, b / color.b, a / color.a);
	}

	Color operator/(float scale) const
	{
		float is = 1.0f / scale;
		return Color(r * is, g * is, b * is, a * is);
	}

	Color& operator += (const Color& color)
	{
		r = r + color.r;
		g = g + color.g;
		b = b + color.b;
		a = a + color.a;
		return *this;
	}

	Color& operator -= (const Color& color)
	{
		r = r - color.r;
		g = g - color.g;
		b = b - color.b;
		a = a - color.a;
		return *this;
	}

	Color& operator *= (const Color& color)
	{
		r = r * color.r;
		g = g * color.g;
		b = b * color.b;
		a = a * color.a;
		return *this;
	}

	Color& operator *= (float scale)
	{
		r = r * scale;
		g = g * scale;
		b = b * scale;
		a = a * scale;
		return *this;
	}

	Color& operator /= (const Color& color)
	{
		r = r / color.r;
		g = g / color.g;
		b = b / color.b;
		a = a / color.a;
		return *this;
	}

	Color& operator /= (float scale)
	{
		float is = 1.0f / scale;
		r *= is;
		g *= is;
		b *= is;
		a *= is;
		return *this;
	}

	bool operator==(const Color& color) const
	{
		return IS_FLOAT_EQUAL(r, color.r) && IS_FLOAT_EQUAL(g, color.g) && IS_FLOAT_EQUAL(b, color.b) && IS_FLOAT_EQUAL(a, color.a);
	}

	bool operator!=(const Color& color) const
	{
		return !(IS_FLOAT_EQUAL(r, color.r)) || !(IS_FLOAT_EQUAL(g, color.g)) || !(IS_FLOAT_EQUAL(b, color.b)) || !(IS_FLOAT_EQUAL(a, color.a));
	}

	float& operator[](int index)
	{
		if (index < 0 || index >= 4)
		{
			throw "invalid index!";
		}
		switch (index)
		{
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3:
			return a;
		default:
			return r;
		}
	}

	float operator[](int index) const
	{
		{
			if (index < 0 || index >= 4)
			{
				throw "invalid index!";
			}
			switch (index)
			{
			case 0:
				return r;
			case 1:
				return g;
			case 2:
				return b;
			case 3:
				return a;
			default:
				return 0.0f;
			}
		}
	}

	friend Color operator * (float scale, const Color& color)
	{
		return Color(color.r * scale, color.g * scale, color.b * scale, color.a * scale);
	}

	static Color Lerp(const Color& a, const Color& b, float t)
	{
		return Color(a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t, a.a + (b.a - a.a) * t);
	}

	static void Lerp(const Color& a, const Color& b, Color& out, float t)
	{
		out = Color(a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t, a.a + (b.a - a.a) * t);
	}

	static Color Lerp01(const Color& a, const Color& b, float t)
	{
		if (t < 0.0f)
			t = 0.0f;
		else if (t > 1.0f)
			t = 1.0f;
		return Lerp(a, b, t);
	}

	static void Lerp01(const Color& a, const Color& b, Color& out, float t)
	{
		if (t < 0.0f)
			t = 0.0f;
		else if (t > 1.0f)
			t = 1.0f;
		Lerp(a, b, out, t);
	}

	static Color HSVToRGB(float h, float s, float v)
	{
		Color col = Color(1, 1, 1, 1);
		if (IS_FLOAT_EQUAL(s, 0.0f))
		{
			col.r = v;
			col.g = v;
			col.b = v;
		}
		else if (IS_FLOAT_EQUAL(v, 0.0f))
		{
			col.r = 0.0f;
			col.g = 0.0f;
			col.b = 0.0f;
		}
		else
		{
			col.r = 0.0f;
			col.g = 0.0f;
			col.b = 0.0f;
			float num = h * 6.0f;
			int num2 = (int)floor(num);
			float num3 = num - (float)num2;
			float num4 = v * (1.0f - s);
			float num5 = v * (1.0f - s * num3);
			float num6 = v * (1.0f - s * (1.0f - num3));
			switch (num2 + 1)
			{
			case 0:
				col.r = v;
				col.g = num4;
				col.b = num5;
				break;
			case 1:
				col.r = v;
				col.g = num6;
				col.b = num4;
				break;
			case 2:
				col.r = num5;
				col.g = v;
				col.b = num4;
				break;
			case 3:
				col.r = num4;
				col.g = v;
				col.b = num6;
				break;
			case 4:
				col.r = num4;
				col.g = num5;
				col.b = v;
				break;
			case 5:
				col.r = num6;
				col.g = num4;
				col.b = v;
				break;
			case 6:
				col.r = v;
				col.g = num4;
				col.b = num5;
				break;
			case 7:
				col.r = v;
				col.g = num6;
				col.b = num4;
				break;
			}
		}
		return col;
	}

	static void HSVToRGB(float h, float s, float v, Color& out)
	{
		out = HSVToRGB(h, s, v);
	}

	static Color Color32(unsigned short r, unsigned short g, unsigned short b, unsigned short a)
	{
		float inv255 = 1.0f / 255.0f;
		float red = ((float)r) * inv255;
		float green = ((float)g) * inv255;
		float blue = ((float)b) * inv255;
		float alpha = ((float)a) * inv255;
		return Color(red, green, blue, alpha);
	}

	static Color Color32(unsigned short r, unsigned short g, unsigned short b)
	{
		float inv255 = 1.0f / 255.0f;
		float red = ((float)r) * inv255;
		float green = ((float)g) * inv255;
		float blue = ((float)b) * inv255;
		return Color(red, green, blue, 1.0f);
	}
	
public:
	float r, g, b, a;
} Color;

#define COLOR_RED			Color(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GREEN			Color(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_BLUE			Color(0.0f, 0.0f, 1.0f, 1.0f)
#define COLOR_WHITE			Color(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_BLACK			Color(0.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_YELLOW		Color(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_CYAN			Color(0.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_MAGENTA		Color(1.0f, 0.0f, 1.0f, 1.0f)
#define COLOR_GRAY			Color(0.5f, 0.5f, 0.5f, 1.0f)
#define COLOR_CLEAR			Color(0.0f, 0.0f, 0.0f, 0.0f)
#define COLOR_DEFAULT_SKY	Color(0.0265f, 0.0718f, 0.19397f, 1.0f)