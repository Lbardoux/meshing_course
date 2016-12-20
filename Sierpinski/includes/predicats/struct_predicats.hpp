/**
 * @file struct_predicats.hpp
 * @brief Defines some usefull structures for every predicats.
 * @author MTLCRBN
 */
#ifndef STRUCT_PREDICATS_HPP_INCLUDED
#define STRUCT_PREDICATS_HPP_INCLUDED

struct Pvertex3D final
{
    typedef double value_type;
	value_type x;
	value_type y;
	value_type z;
	Pvertex3D(value_type _x, value_type _y, value_type _z) : x(_x), y(_y), z(_z){}
	
	value_type getX(void){return this->x;}
	value_type getY(void){return this->y;}
	value_type getZ(void){return this->z;}
};

struct Ptriangle3D final
{
	Pvertex3D a;
	Pvertex3D b;
	Pvertex3D c;
	Ptriangle3D(const Pvertex3D& _a, const Pvertex3D& _b, const Pvertex3D& _c) : a(_a), b(_b), c(_c){}
};

#endif
