/*
    This file is a part of the RepSnapper project.
    Copyright (C) 2010  Kulitorum

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "config.h"
#pragma once
#include <vector>
#include <list>
#include "platform.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>

#include <vmmlib/vmmlib.h>
#include <polylib/Polygon2d.h>

#include "stdafx.h"
#include "string.h"

//#include "gcode.h"
#include "math.h"
#include "settings.h"

#define ABS(a)	   (((a) < 0) ? -(a) : (a))

/* A number that speaks for itself, every kissable digit.                    */

#define PI 3.141592653589793238462643383279502884197169399375105820974944592308

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define RESOLUTION 4
#define FREE(p)            {if (p) {free(p); (p)= NULL;}}

using namespace std;
using namespace vmml;
using namespace PolyLib;

class RFO;


enum AXIS {NEGX, POSX, NEGY, POSY, NEGZ, POSZ, NOT_ALIGNED};

enum filetype_t{
    ASCII_STL,
    BINARY_STL,
    NONE_STL
};


void renderBitmapString(Vector3d pos, void* font, string text);
void checkGlutInit();






class Triangle
{
public:
	Triangle(const Vector3d &Norml, const Vector3d &Point1,
		 const Vector3d &Point2, const Vector3d &Point3) 
		{ Normal = Norml ; A=Point1;B=Point2;C=Point3;}
	Triangle() {};

	/* Represent the triangle as an array of length 3 {A, B, C} */
	Vector3d &operator[](const int index);

	void SetPoints(const Vector3d &P1, const Vector3d &P2, const Vector3d &P3) { A=P1;B=P2;C=P3; }
	void SetNormal(const Vector3d &Norml) { Normal=Norml;}
	double area();

	AXIS axis;			// Used for auto-rotation
	Vector3d A,B,C,Normal;	// p1,p2,p3, Normal
	Vector3d GetMax();
	Vector3d GetMin();
	void AccumulateMinMax(Vector3d &min, Vector3d &max);
	void Translate(const Vector3d &vector);
	int CutWithPlane(double z, const Matrix4d &T, 
			 Vector2d &lineStart, Vector2d &lineEnd);
};


#define sqr(x) ((x)*(x))

class Slicer
{
public:
	Slicer();

    int load(std::string filename);

	void clear() { triangles.clear(); }
	void displayInfillOld(const Settings &settings, CuttingPlane &plane, 
			      guint LayerNr, vector<int>& altInfillLayers);
	void draw (RFO &rfo, const Settings &settings);
	void draw_geometry ();
	void CenterAroundXY();
	// Extract a 2D polygonset from a 3D model:
	void CalcCuttingPlane(double where, CuttingPlane &plane, const Matrix4d &T);
	// Auto-Rotate object to have the largest area surface down for printing:
	void OptimizeRotation(); 
	void CalcCenter();
	// Rotation for manual rotate and used by OptimizeRotation:
	void RotateObject(Vector3d axis, double angle);  
    void Scale(double scale_factor);
    double getScaleFactor(){ return scale_factor; };

	vector<Triangle>  triangles;
	Vector3d Min, Max, Center;

private:
    double scale_factor;
    int loadASCIISTL(std::string filename);
    int loadBinarySTL(std::string filename);
    filetype_t getFileType(std::string filename);
};


#include "rfo.h"