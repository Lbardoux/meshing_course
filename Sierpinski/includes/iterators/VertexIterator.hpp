/**
 * @file VertexIterator.hpp
 * @brief Offers an iterator to iterate through the vertices of a Mesh
 * @author MTLCRBN
 * @version 2.0
 */
#ifndef VERTEXITERATOR_HPP_INCLUDED
#define VERTEXITERATOR_HPP_INCLUDED

#include "Vertex3D.hpp"
#include "MeshIterator.hpp"

//! Defines an iterator on vertices :D
typedef MeshIterator<Vertex> _VertexIterator;

#endif
