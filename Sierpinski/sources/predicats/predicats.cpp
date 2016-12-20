#include <cmath>
#include "predicats.hpp"

namespace
{
	typedef Pvertex3D   Point_t;
	typedef Ptriangle3D Triangle_t;
	
	double length2(const Pvertex3D& p)
	{
		return p.x*p.x + p.y*p.y + p.z*p.z;
	}
	Pvertex3D cross(const Pvertex3D& p1, const Pvertex3D& p2)
	{
		Pvertex3D tmp(0.0, 0.0, 0.0);
		tmp.x = p1.y*p2.z - p1.z*p2.y;
		tmp.y = p1.z*p2.x - p1.x*p2.z;
		tmp.z = p1.x*p2.y - p1.y*p2.x;
		return tmp;
	}
	Pvertex3D operator-(const Pvertex3D& p1, const Pvertex3D& p2)
	{
		return Pvertex3D(p1.x-p2.x, p1.y-p2.y, p1.z-p2.z);
	}
	Pvertex3D operator+(const Pvertex3D& p1, const Pvertex3D& p2)
	{
		return Pvertex3D(p1.x+p2.x, p1.y+p2.y, p1.z+p2.z);
	}
	Pvertex3D operator*(const Pvertex3D& p, double scalar)
	{
		return Pvertex3D(p.x*scalar, p.y*scalar, p.z*scalar);
	}
	double dot(const Pvertex3D& p1, const Pvertex3D& p2)
	{
		return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
	}
	Pvertex3D operator/(const Pvertex3D& p, double div)
	{
		return Pvertex3D(p.x/div, p.y/div, p.z/div);
	}
	double length(const Pvertex3D& p)
	{
		return std::sqrt(length2(p));
	}
	double radian2deg(double radian)
	{
		return radian*(180.0/M_PI);
	}
	double computeAngle(const Pvertex3D& a, const Pvertex3D& b, const Pvertex3D& c)
	{
		Pvertex3D ab  = b - a;
		Pvertex3D ac  = c - a;
		double d_ab   = length(ab);
		double d_ac   = length(ac);
		double scalar = dot(ab , ac);
		return radian2deg(scalar/(d_ab*d_ac));
	}
}

bool isWellOriented(const Pvertex3D& v1, const Pvertex3D& v2, const Pvertex3D& v3)
{
	return cross(v1-v2, v1-v3).z > 0.0;
}

bool isInThisTriangle(const Pvertex3D& v, const Ptriangle3D& t)
{
	double denominator = ((t.b.y - t.c.y)*(t.a.x - t.c.x) + (t.c.x - t.b.x)*(t.a.y - t.c.y));
	double a           = ((t.b.y - t.c.y)*(v.x - t.c.x) + (t.c.x - t.b.x)*(v.y - t.c.y))/denominator;
	double b           = ((t.c.y - t.a.y)*(v.x - t.c.x) + (t.a.x - t.c.x)*(v.y - t.c.y))/denominator;
	double c           = 1.0 - a - b;
	
	return 0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0 && 0.0 <= c && c <= 1.0;
}

namespace
{
	struct Matrix3x3 final
	{
		double m[9];
	};
	
	double det(const Matrix3x3& mat)
	{
		double term_1 = mat.m[0]*(mat.m[4]*mat.m[8] - mat.m[7]*mat.m[5]);
		double term_2 = mat.m[1]*(mat.m[3]*mat.m[8] - mat.m[6]*mat.m[5]);
		double term_3 = mat.m[2]*(mat.m[3]*mat.m[7] - mat.m[6]*mat.m[4]);
		return term_1 - term_2 + term_3;
	}
	
	// true = (+), false = (-)
	bool signMatrix(const Matrix3x3& mat)
	{
		return det(mat) <= 0.0;
	}
}

bool isInSurroundingCircle(const Pvertex3D& p, const Pvertex3D& q, const Pvertex3D& r, const Pvertex3D& s)
{
	double qxpx = q.x-p.x;
	double rxpx = r.x-p.x;
	double sxpx = s.x-p.x;
	double qypy = q.y-p.y;
	double rypy = r.y-p.y;
	double sypy = s.y-p.y;
	Matrix3x3 matrix = {
		qxpx, rxpx, sxpx,
		qypy, rypy, sypy,
		qxpx*qxpx + qypy*qypy, rxpx*rxpx + rypy*rypy, sxpx*sxpx + sypy*sypy
	};
	return signMatrix(matrix);
}

Point_t centerSurroundingCircle2D(const Triangle_t& t)
{
	Pvertex3D ab    = t.b - t.a;
	Pvertex3D ac    = t.c - t.a;
	Pvertex3D abXac = cross(ab, ac);
	double len_ab    = length2(ab);
	double len_ac    = length2(ac);
	double len_abXac = length2(abXac);
	Pvertex3D i      = ac*len_ab;
	Pvertex3D j      = ab*len_ac;
	Pvertex3D k      = cross(i-j, abXac)*(0.5/len_abXac);
	return t.a + k;
}

bool isPoorQuality(const Ptriangle3D& tr, double angleThreshold)
{
	return (computeAngle(tr.a, tr.b, tr.c) < angleThreshold || 
			computeAngle(tr.b, tr.a, tr.c) < angleThreshold ||
			computeAngle(tr.c, tr.a, tr.b) < angleThreshold);
}

bool isInCircleOfDiametral(const Pvertex3D& a, const Pvertex3D& b, const Pvertex3D& t)
{
	Pvertex3D center = (b-a)/2.0;
	double    radius = length2(center);
	return (t.x-center.x)*(t.x-center.x) + (t.y-center.y)*(t.y-center.y) + (t.z-center.z)*(t.z-center.z) <= radius*radius;
}

Pvertex3D barycentre(const Ptriangle3D& triangle)
{
	Pvertex3D a_bary = triangle.a*0.333;
	Pvertex3D b_bary = triangle.b*0.333;
	Pvertex3D c_bary = triangle.c*0.333;
	return a_bary + b_bary + c_bary;
}
