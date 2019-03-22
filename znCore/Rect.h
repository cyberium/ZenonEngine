#pragma once

class Rect
{
public:
    Rect(vec2 Point, vec2 Size)
        : X(Point.x)
        , Y(Point.y)
        , Width(Size.x)
        , Height(Size.y)
    {}

	Rect(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f)
		: X(x)
		, Y(y)
		, Width(width)
		, Height(height)
	{}

	float X;
	float Y;
	float Width;
	float Height;

    bool isPointInRect(vec2 Point) const
    {
        return (Point.x >= X) && (Point.x < (X + Width)) && (Point.y >= Y) && (Point.y < (Y + Height));
    }
};