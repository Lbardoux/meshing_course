/**
 * @file TopoTriangle.hpp
 * @brief This kind of triangle got some information about his neighborhood.
 * 
 * In order to compute, you'll need :
 *    - -std=c++11
 *    - Triangle.o
 *    - Triangle.hpp
 *    - TopoTriangle.o
 * 
 * @author MTLCRBN
 * @version 1.0
 */
#ifndef TOPOTRIANGLE_HPP_INCLUDED
#define TOPOTRIANGLE_HPP_INCLUDED

#include <vector>
#include <cstdint>
#include <utility>
#include "Triangle.hpp"


typedef int32_t IndexFace_t; //!< A way to avoid multiple rewrite when talking about face index.

/**
 * @class TopoTriangle
 * @brief This kind of Triangle got some information about neighborhood
 */
class TopoTriangle final : public Triangle
{
	public:
		//! @brief A wrapper for an edge.
		struct Edge final
		{
			IndexVertex_t a; //!< The first  point of the Edge.
			IndexVertex_t b; //!< The second point of the Edge.
			//! @brief Allow comparison between 2 edges by ==.
			bool operator==(const Edge& other);
			//! @brief Allow comparison between 2 edges by !=.
			bool operator!=(const Edge& other);
			//! @brief Convert this edge into a pair of indexes, first <= second.
			operator std::pair<IndexVertex_t, IndexVertex_t>(void);
		};
		// ## Constructors ###################################################################
		/**
		 * @snippet TopoTriangle.hpp 3Constructor
		 * @brief Construct a triangle \b abc.
		 * @param[in] a The index of the first  point.
		 * @param[in] b The index of the second point.
		 * @param[in] c The index of the third  point.
		 */
		TopoTriangle(IndexVertex_t a, IndexVertex_t b, IndexVertex_t c);
		/**
		 * @snippet TopoTriangle.hpp copyConstructor
		 * @brief Copy \p other into \b this.
		 * @param[in] other The topological triangle to copy.
		 */
		TopoTriangle(const TopoTriangle& other);
		/**
		 * @snippet TopoTriangle.hpp moveConstructor
		 * @brief Move \p other into \b this.
		 * @param[inout] other The topological triangle to move.
		 */
		TopoTriangle(TopoTriangle&& other);
		/**
		 * @snippet TopoTriangle.hpp containerConstructor
		 *
		 */
		template<template<typename...> class Container>
		TopoTriangle(const Container<IndexVertex_t>& container) : Triangle(container)
		{
			this->neighbors[0] = this->neighbors[1] = this->neighbors[2] = -1;
		}
		// ###################################################################################
		
		// ## Operators ######################################################################
		/**
		 * @snippet TopoTriangle.hpp operator=copy
		 * @brief Affect \p other to \b this by copy.
		 * @param[in] other The triangle to copy
		 * @return A reference to \b this.
		 */
		TopoTriangle& operator=(const TopoTriangle& other);
		/**
		 * @snippet TopoTriangle.hpp operator=move
		 * @brief Affect \p other to \b this by move.
		 * @param[in] other The triangle to move
		 * @return A reference to \b this.
		 */
		TopoTriangle& operator=(TopoTriangle&& other);
		/**
		 * @snippet TopoTriangle.hpp operator==
		 * @brief Check for equality between \b this and \p other.
		 * @param[in] other The other Triangle you're working with.
		 * @return true if they're equal, false otherwise.
		 */
		bool operator==(const TopoTriangle& other) const;
		/**
		 * @snippet TopoTriangle.hpp operator!=
		 * @brief Check for inequality between \b this and \p other.
		 * @param[in] other The other Triangle you're working with.
		 * @return false if they're equal, true otherwise.
		 */
		bool operator!=(const TopoTriangle& other) const;
		// ###################################################################################
		//! @brief It will erase every neighbors information.
		~TopoTriangle(void);
		
		// ## Utilitaries ####################################################################
		/**
		 * @snippet TopoTriangle.hpp addNeighbor
		 * @brief Add \p neighbor to this triangle, which is a neighbor with the commond edge \p edge.
		 * @param[in] neighbor The neighbor index.
		 * @param[in] edge     The shared edge between the 2 faces.
		 * @return A reference to \n this.
		 */
		TopoTriangle& addNeighbor(IndexFace_t neighbor, const Edge& edge);
		/**
		 * @snippet TopoTriangle.hpp getAdjTriangleTrigo
		 * @brief Get the next Triangle index of \p current in this triangle.
		 * @param[in] current The index of rotation point.
		 * @return this index.
		 */
		IndexFace_t getAdjTriangleTrigo(IndexVertex_t current) const;
		/**
		 * @snippet TopoTriangle.hpp removeNeighbor
		 * @brief Remove the neighbor of index \p i.
		 * @param[in] i The neighbor index.
		 * @return A reference to \b this.
		 */
		TopoTriangle& removeNeighbor(IndexFace_t i);
		/**
		 * @snippet TopoTriangle.hpp getNeighbors
		 * @brief Grant access to the container of neighbors.
		 * @return a const reference to this container.
		 */
		const IndexFace_t* getNeighbors(void) const;
		/**
		 * @snippet TopoTriangle.hpp getOppositeVertexOf
		 * @brief Get the index of the vertex which is at the opposite of
		 * the neighbor \p neighbor.
		 * @param[in] neighbor The index of the neighbor you want.
		 * @return -1 if there is no \p neighbor known, the index of the vertex otherwise.
		 */
		IndexVertex_t getOppositeVertexOf(IndexFace_t neighbor) const;
		/**
		 * @snippet TopoTriangle.hpp getOppositeNeighborOf
		 * @brief Get the neighbor which is the opposite of the vertex of index \p index.
		 * @param[in] index The index of the vertex you want the opposite.
		 * @return -1 if there is no neighbor, an index for triangle otherwise.
		 */
		IndexFace_t getOppositeNeighborOf(IndexVertex_t index) const;
		
		Edge getEdgeWithout(IndexVertex_t index) const;
		IndexVertex_t getVertexOutsideOf(const Edge& edge) const;
		
		void copyNeighbors(const TopoTriangle& other);
		
		Edge getCommonEdge(IndexFace_t other) const;
		// ###################################################################################
	
	private:
		IndexFace_t neighbors[3]; //!< The indexes of the neighbors, \b max_size = 3;
		
		TopoTriangle(void) = delete;
	
};


std::ostream& operator<<(std::ostream& out, const TopoTriangle& t);

#endif
