/**
 * @file neighbors.hpp
 * @brief Offers some functions to manage the adjacency of neighbors.
 * @author MTLCRBN
 */
#ifndef NEIGHBORS_HPP_INCLUDED
#define NEIGHBORS_HPP_INCLUDED

#include <map>
#include "Vertex3D.hpp"
#include "TopoTriangle.hpp"

namespace neighbor
{
	typedef std::pair<IndexVertex_t, IndexVertex_t> Key_t;
	typedef std::map<Key_t, IndexFace_t>            MapEdges;
	
	/**
	 * @brief Insert \p key into \p map, and manages neighborhoods with \p faces and \p index.
	 * @param[inout] map   The map which contains the adjacencies.
	 * @param[in]    key   The current edge you wanna insert in addition of \p index.
	 * @param[in]    index The triangle index you're managing.
	 * @param[inout] faces The triangles container.
	 */
	void insert(MapEdges& map, const TopoTriangle::Edge& key, IndexFace_t index, std::vector<TopoTriangle>& faces);
}

#endif
