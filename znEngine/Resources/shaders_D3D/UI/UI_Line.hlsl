#include "..\\_gamedata\\shaders_D3D\\UI\UI_Common.h"

/* Geometry GLSL shader that demonstrates how to draw basic thick and smooth bezier curves in 3D.
 * This file is a part of shader-3dcurve example (https://github.com/vicrucann/shader-3dcurve).
 *
 * Victoria Rudakova
 * January 2017
 * MIT license
 
 * Modify: Alex Stenfard
 * March 2019
*/

struct GeometryShaderOutput
{
    float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

cbuffer Material : register(b1)
{
    float4  Color;
	float   Thickness;
	float2  Viewport;
	float   MiterLimit;
	uint    Segments;
};

static const int SegmentsMax = 30; // max_vertices = (SegmentsMax+1) * 4;
static const int SegmentsMin = 3;  // min number of segments per curve

float2 toScreenSpace(float4 vertex)
{
    return float2( vertex.xy / vertex.w ) * Viewport;
}

float toZValue(float4 vertex)
{
    return (vertex.z/vertex.w);
}

float4 toBezier(float delta, int i, float4 P0, float4 P1, float4 P2, float4 P3)
{
    float t = delta * float(i);
    float t2 = t * t;
    float one_minus_t = 1.0 - t;
    float one_minus_t2 = one_minus_t * one_minus_t;
    return (P0 * one_minus_t2 * one_minus_t + P1 * 3.0 * t * one_minus_t2 + P2 * 3.0 * t2 * one_minus_t + P3 * t2 * t);
}

void drawSegment(float2 points[4], float zValues[4], inout TriangleStream<GeometryShaderOutput> output)
{
    float2 p0 = points[0];
    float2 p1 = points[1];
    float2 p2 = points[2];
    float2 p3 = points[3];

    // perform naive culling
    float2 area = Viewport * 4;
    if( p1.x < -area.x || p1.x > area.x ) return;
    if( p1.y < -area.y || p1.y > area.y ) return;
    if( p2.x < -area.x || p2.x > area.x ) return;
    if( p2.y < -area.y || p2.y > area.y ) return;

    // determine the direction of each of the 3 segments (previous, current, next)
    float2 v0 = normalize( p1 - p0 );
    float2 v1 = normalize( p2 - p1 );
    float2 v2 = normalize( p3 - p2 );

    // determine the normal of each of the 3 segments (previous, current, next)
    float2 n0 = float2( -v0.y, v0.x );
    float2 n1 = float2( -v1.y, v1.x );
    float2 n2 = float2( -v2.y, v2.x );

    // determine miter lines by averaging the normals of the 2 segments
    float2 miter_a = normalize( n0 + n1 );	// miter at start of current segment
    float2 miter_b = normalize( n1 + n2 );  // miter at end of current segment

    // determine the length of the miter by projecting it onto normal and then inverse it
    float an1 = dot(miter_a, n1);
    float bn1 = dot(miter_b, n2);
    if (an1 == 0) 
		an1 = 1;
		
    if (bn1 == 0) 
		bn1 = 1;
	
    float length_a = Thickness / an1;
    float length_b = Thickness / bn1;

    // prevent excessively long miters at sharp corners
    if( dot( v0, v1 ) < (- MiterLimit) ) 
	{
        miter_a = n1;
        length_a = Thickness;

        // close the gap
        if( dot( v0, n1 ) > 0 ) 
		{
            //VertexOut.mColor = colors[1];
			GeometryShaderOutput o;
            o.position = float4( ( p1 + Thickness * n0 ) / Viewport, zValues[1], 1.0 );
			o.texCoord = float2( 0, 0 );
			output.Append(o);

            //VertexOut.mColor = colors[1];
            o.position = float4( ( p1 + Thickness * n1 ) / Viewport, zValues[1], 1.0 );
			o.texCoord = float2( 0, 0 );
			output.Append(o);
			
            //VertexOut.mColor = colors[1];
            o.position = float4( p1 / Viewport, 0.0, 1.0 );
			o.texCoord = float2( 0, 0.5 );
			output.Append(o);
			
			output.RestartStrip();
        }
        else 
		{
            //colors[1];
			GeometryShaderOutput o;
            o.position = float4( ( p1 - Thickness * n1 ) / Viewport, zValues[1], 1.0 );
			o.texCoord = float2( 0, 1 );
			output.Append(o);

            //colors[1];
            o.position = float4( ( p1 - Thickness * n0 ) / Viewport, zValues[1], 1.0 );
			o.texCoord = float2( 0, 1 );
			output.Append(o);

            //colors[1];
            o.position = float4( p1 / Viewport, zValues[1], 1.0 );
			o.texCoord = float2( 0, 0.5 );
			output.Append(o);
			
			output.RestartStrip();
        }
    }
	
    if( dot( v1, v2 ) < -MiterLimit ) 
	{
        miter_b = n1;
        length_b = Thickness;
    }
	
	GeometryShaderOutput o;
    o.position = float4( ( p1 + length_a * miter_a ) / Viewport, zValues[1], 1.0 );
	o.texCoord = float2( 0, 0 );
	output.Append(o);

    o.position = float4( ( p1 - length_a * miter_a ) / Viewport, zValues[1], 1.0 );
	o.texCoord = float2( 0, 1 );
	output.Append(o);

    o.position = float4( ( p2 + length_b * miter_b ) / Viewport, zValues[2], 1.0 );
	o.texCoord = float2( 0, 0 );
	output.Append(o);

    o.position = float4( ( p2 - length_b * miter_b ) / Viewport, zValues[2], 1.0 );
	o.texCoord = float2( 0, 1 );
	output.Append(o);

	output.RestartStrip();
}

[maxvertexcount(124)]
void GS_main(lineadj VertexShaderOutput points[4], inout TriangleStream<GeometryShaderOutput> output)
{
    // cut segments number if larger or smaller than allowed
    uint nSegments = (Segments > SegmentsMax)? SegmentsMax : Segments;
    nSegments = (nSegments < SegmentsMin)? SegmentsMin: nSegments;

    // 4 control points
    float4 B[4] = { points[0].position, points[1].position, points[2].position, points[3].position };

    // use the points to build a bezier line
    float delta = 1.0 / float(nSegments);
	
    float4 Points[4] = { float4(0.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 0.0f) }; // segments of curve in 3d
    float zValues[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    	
    for (uint i = 0; i <= nSegments; i++)
	{
        // first point
        if (i == 0)
		{
            Points[1] = toBezier(delta, i, B[0], B[1], B[2], B[3]);
            Points[2] = toBezier(delta, i+1, B[0], B[1], B[2], B[3]);
            Points[3] = toBezier(delta, i+2, B[0], B[1], B[2], B[3]);
            float4 dir = normalize(Points[1] - Points[2]);
            Points[0] = Points[1] + dir * 0.01;
        }
        else if (i < nSegments - 1)
		{
            Points[0] = Points[1];
            Points[1] = Points[2];
            Points[2] = Points[3];
            Points[3] = toBezier(delta, i+2, B[0], B[1], B[2], B[3]);
        }
        // last point
        else 
		{
            Points[0] = Points[1];
            Points[1] = Points[2];
            Points[2] = Points[3];
            float4 dir = normalize(Points[2] - Points[1]);
            Points[3] = Points[2] + dir * 0.001f;
        }

        // segments of curve in 2d
        float2 points[4];
        points[0] = toScreenSpace(Points[0]);
        points[1] = toScreenSpace(Points[1]);
        points[2] = toScreenSpace(Points[2]);
        points[3] = toScreenSpace(Points[3]);

        zValues[0] = toZValue(Points[0]);
        zValues[1] = toZValue(Points[1]);
        zValues[2] = toZValue(Points[2]);
        zValues[3] = toZValue(Points[3]);

        drawSegment(points, zValues, output);
    }
}

float4 PS_LineV2(GeometryShaderOutput input) : SV_TARGET
{
    return Color;
}
