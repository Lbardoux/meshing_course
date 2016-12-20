/**
 * @file common.hpp
 * @brief Defines some commons things for Mesh and cie.
 * It also defines some debug messages.
 */
#ifndef COMMON_HPP_INCLUDED
#define COMMON_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <list>
#include "Vertex3D.hpp"
#include "TopoTriangle.hpp"

typedef double                        VertexType;
typedef Vertex3D<VertexType>          Vertex;
typedef std::vector<Vertex>           VertexContainer;
typedef std::vector<TopoTriangle>     TriangleContainer;
typedef std::list<IndexVertex_t>      Border_c;
typedef std::list<TopoTriangle::Edge> Curve_c;


#endif

