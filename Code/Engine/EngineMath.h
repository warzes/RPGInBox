#pragma once

class Point2 final
{
public:
	int x = 0;
	int y = 0;
};

class Size2 final
{
public:
	int width = 0;
	int height = 0;
};

//// Rectangle, 4 components
//class Rectangle final
//{
//public:
//	float x = 0.0f;                // Rectangle top-left corner position x
//	float y = 0.0f;                // Rectangle top-left corner position y
//	float width = 0.0f;            // Rectangle width
//	float height = 0.0f;           // Rectangle height
//};
//
//// Color, 4 components, R8G8B8A8 (32bit)
//class Color final
//{
//public:
//	unsigned char r = 0;	// Color red value
//	unsigned char g = 0;	// Color green value
//	unsigned char b = 0;	// Color blue value
//	unsigned char a = 0;	// Color alpha value
//};
//
//class Vector2 final
//{
//public:
//	float x = 0.0f;
//	float y = 0.0f;
//};
//
//class Vector3 final
//{
//public:
//	float x = 0.0f;
//	float y = 0.0f;
//	float z = 0.0f;
//};
//
//class Vector4 final
//{
//public:
//	float x = 0.0f;
//	float y = 0.0f;
//	float z = 0.0f;
//	float w = 0.0f;
//};
//
//class Quaternion final
//{
//public:
//	float x = 0.0f;
//	float y = 0.0f;
//	float z = 0.0f;
//	float w = 0.0f;
//};
//
//// Matrix, 4x4 components, column major, OpenGL style, right handed
//class Matrix4x4 final
//{
//public:
//	float m0, m4, m8, m12;  // Matrix first row (4 components)
//	float m1, m5, m9, m13;  // Matrix second row (4 components)
//	float m2, m6, m10, m14; // Matrix third row (4 components)
//	float m3, m7, m11, m15; // Matrix fourth row (4 components)
//};