/**
 * @file TriangleIterator.hpp
 * @brief Offers an iterator to iterate through the triangles of a Mesh
 * @author MTLCRBN
 * @version 2.0
 */
#ifndef TRIANGLEITERATOR_HPP_INCLUDED
#define TRIANGLEITERATOR_HPP_INCLUDED

#include "TopoTriangle.hpp"
#include "MeshIterator.hpp"

//! Defines an iterator on triangles :D
typedef MeshIterator<TopoTriangle> _TriangleIterator;

#endif
