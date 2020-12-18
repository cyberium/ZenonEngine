#pragma once


struct ColorRGB
{
	float r;
	float g;
	float b;

	ColorRGB()
		: r(0.0f)
		, g(0.0f)
		, b(0.0f)
	{}

	explicit ColorRGB(float all)
		: r(all)
		, g(all)
		, b(all)
	{}

	explicit ColorRGB(float r_, float g_, float b_)
		: r(r_)
		, g(g_)
		, b(b_)
	{}

	inline bool operator==(const ColorRGB& Other)
	{
		return r == Other.r && g == Other.g && b == Other.b;
	}
	inline bool operator!=(const ColorRGB& Other)
	{
		return false == this->operator==(Other);
	}
};

inline ColorRGB operator+(const ColorRGB& v1, const ColorRGB& v2)
{
	return ColorRGB(v1.r + v2.r, v1.g + v2.g, v1.b + v2.b);
}

inline ColorRGB operator*(const ColorRGB& v1, float Value)
{
	return ColorRGB(Value * v1.r, Value * v1.g, Value * v1.b);
}

inline ColorRGB operator*(const ColorRGB& v1, const ColorRGB& v2)
{
	return ColorRGB(v1.r * v2.r, v1.g * v2.g, v1.b * v2.b);
}



struct ColorRGBA
{
	float r;
	float g;
	float b;
	float a;

	ColorRGBA()
		: r(0.0f)
		, g(0.0f)
		, b(0.0f)
		, a(0.0f)
	{}

	explicit ColorRGBA(float all)
		: r(all)
		, g(all)
		, b(all)
		, a(all)
	{}

	explicit ColorRGBA(ColorRGB ColorRGB, float a_)
		: r(ColorRGB.r)
		, g(ColorRGB.g)
		, b(ColorRGB.b)
		, a(a_)
	{}

	explicit ColorRGBA(float r_, float g_, float b_, float a_)
		: r(r_)
		, g(g_)
		, b(b_)
		, a(a_)
	{}

	inline bool operator==(const ColorRGBA& Other)
	{
		return r == Other.r && g == Other.g && b == Other.b && a == Other.a;
	}
	inline bool operator!=(const ColorRGBA& Other)
	{
		return false == this->operator==(Other);
	}
};

inline ColorRGBA operator+(const ColorRGBA& v1, const ColorRGBA& v2)
{
	return ColorRGBA(v1.r + v2.r, v1.g + v2.g, v1.b + v2.b, v1.a + v2.a);
}

inline ColorRGBA operator*(const ColorRGBA& v1, float Value)
{
	return ColorRGBA(Value * v1.r, Value * v1.g, Value * v1.b, Value * v1.a);
}

inline ColorRGBA operator*(const ColorRGBA& v1, const ColorRGBA& v2)
{
	return ColorRGBA(v1.r * v2.r, v1.g * v2.g, v1.b * v2.b, v1.a * v2.a);
}
