/**
 * @file Triangle.hpp
 * @brief Defines a triangle which contains 3 indexes which refer to vertex.
 * 
 * That means \b Triangle isn't aware of the kind of vertex it's manipulating.<br />
 * In order to compute this module, you'll need :
 *    - Triangle.hpp
 *    - Triangle.cpp
 *    - -std=c++11
 * 
 * @author MTLCRBN
 * @version 2.0
 * @date The 10th of November 2016
 */
#ifndef TRIANGLE_HPP_INCLUDED
#define TRIANGLE_HPP_INCLUDED

#include <cstdint>
#include <iterator>
#include <iostream>


typedef int32_t IndexVertex_t;   //!< A specific way to avoid multiple rewrite of the code for the type of vertex index.

/**
 * @class Triangle
 * @brief This kind of triangle doesn't get any topolopgic informations.
 * It just knows the 3 vertices it's compound with by construction.
 */
class Triangle
{
	public:
		// ## Constructors #########################################################
		/**
		 * @snippet Triangle.hpp paramConstructor
		 * @brief Construct a triangle with \p a, \p b, \p c.
		 * @pre \p a \p b \p c must be organized with the trigonometric order, like below
		 * @code
		 *    a       ___
		 *   / \     /   \
		 *  /   \    V    |
		 * b_____c    ___/
		 * @endcode
		 * @param[in] a The first  vertex index for this triangle.
		 * @param[in] b The second vertex index for this triangle.
		 * @param[in] c The third  vertex index for this triangle.
		 */
		Triangle(IndexVertex_t a, IndexVertex_t b, IndexVertex_t c);
		/**
		 * @snippet Triangle.hpp copyConstructor
		 * @brief Construct a triangle which is a copy of \p other.
		 * @param[in] other The triangle to copy.
		 */
		Triangle(const Triangle& other);
		/**
		 * @snippet Triangle.hpp moveConstructor
		 * @brief Construct a triangle by moving the values of \p other.
		 * @param[in] other The Triangle to move.
		 * @post \p other lose every values at the end.
		 */
		Triangle(Triangle&& other);
		/**
		 * @snippet Triangle.hpp containerConstructor
		 * @brief Allow you to initialize a Triangle with a container which contains at least 3 elements.
		 * @pre \p container gots a minimal amount of three elements.
		 * @param[in] container The container to copy. 
		 */
		template<template<typename...> class Container>
		Triangle(const Container<IndexVertex_t>& container)
		{
			typename Container<IndexVertex_t>::const_iterator it = container.cbegin();
			std::copy(it, std::next(it, 3), this->vertices);
		}
		// #########################################################################
		
		//! @brief The destructor
		~Triangle(void);
		
		// ## Setters ##############################################################
		/**
		 * @snippet Triangle.hpp reset
		 * @brief Reset the indexes of this Triangle.
		 */
		virtual Triangle& reset(void);
		// #########################################################################
		
		// ## Getters ##############################################################
		typedef IndexVertex_t* iterator_v; //!< An "iterator" to vertices.
		typedef const IndexVertex_t* const_iterator_v; //!< An "iterator" to vertices.
		
		/**
		 * @snippet Triangle.hpp beginEndVertice
		 * @brief Get the beginning of the vertices indexes.
		 * @return An "iterator" to the beginning of vertices.
		 */
		const_iterator_v beginVertice(void) const;
		/**
		 * @snippet Triangle.hpp beginEndVertice
		 * @brief Get the ending of the vertices indexes.
		 * @warning Don't bother read this value !
		 * @return An "iterator" to the ending of vertices.
		 */
		const_iterator_v endVertice(void) const;
		/**
		 * @snippet Triangle.hpp adjVertex
		 * @brief Allow the user to get the next vertex index by \p index in a trigonometric rotation.
		 * @param[in] index The index which you desire the next one.
		 * @return The next index.
		 */
		IndexVertex_t getAdjVertexTrigo(IndexVertex_t index) const;
		/**
		 * @snippet Triangle.hpp adjVertexClock
		 * @brief Allow the user to get the next vertex index by \p index in a clockwise rotation.
		 * @param[in] index The index which you desire the next one.
		 * @return The next index.
		 */
		IndexVertex_t getAdjVertexClock(IndexVertex_t index) const;
		// #########################################################################
		
		// ## Operators ############################################################
		/**
		 * @snippet Triangle.hpp copyOperator
		 * @brief Copy \p other into \b this.
		 * @param[in] other The Triangle to copy.
		 * @return A reference to \b this.
		 */
		Triangle& operator=(const Triangle& other);
		/**
		 * @snippet Triangle.hpp moveOperator
		 * @brief Move \p other into \b this.
		 * @param[in] other The Triangle to move
		 * @post \p other doesn't contains anything.
		 * @return A reference to \b this.
		 */
		Triangle& operator=(Triangle&& other);
		/**
		 * @snippet Triangle.hpp operator!=
		 * @brief Compare \b this to \p other.
		 * @param[in] other The other Triangle to compare with.
		 * @return true if \b this != þ other, false otherwise
		 */
		bool operator!=(const Triangle& other) const;
		/**
		 * @snippet Triangle.hpp operator==
		 * @brief Compare \b this to \p other.
		 * @param[in] other The other Triangle to compare with.
		 * @return true if \b this == þ other, false otherwise
		 */
		bool operator==(const Triangle& other) const;
		/**
		 * @brief Find the index where \p v is located inside the array
		 * @param[in] v The Index of the vertex you wanna find.
		 * @return -1 if not found, a number [0, 1, 2] otherwise.
		 */
		IndexVertex_t findVertexIndex(IndexVertex_t v) const;
		// #########################################################################
	protected:
		IndexVertex_t vertices[3]; //!< The 3 indexes of the vertices which compound this triangle.
		
		/**
		 * @brief This constructor is the property of the derived classes only.
		 */
		Triangle(void);
	
};

/**
 * @brief Allow the user to display the vertices indexes of the triangle \p t.
 * @param[inout] out The outputstream you wanna write on.
 * @param[in]    t   The triangle you wanna display.
 * @return A reference to \p out.
 */
std::ostream& operator<<(std::ostream& out, Triangle& t);


#if 0
	//! [paramConstructor]
	Triangle t(0, 1, 2); // 0, 1 and 2
	//! [paramConstructor]
	//! [copyConstructor]
	Triangle toCopy(5, 8, 0);
	Triangle copy(toCopy);
	//! [copyConstructor]
	//! [moveConstructor]
	Triangle toMove(8, 4, 2);
	Triangle move(std::move(toMove));
	//! [moveConstructor]
	//! [reset]
	Triangle t(1, 5, 2);
	t.reset(); // 0, 0, 0
	//! [reset]
	//! [beginEndVertice]
	Triangle t(1, 2, 3);
	Triangle t2(5, 4, 8);
	std::copy(t.beginVertice(), t.endVertice(), t2.beginVertice());
	//! [beginEndVertice]
	//! [moveOperator]
	Triangle toMove(8, 4, 2);
	Triangle move(1, 1, 1);
	move = std::move(toMove); // 8, 4, 2
	//! [moveOperator]
	//! [copyOperator]
	Triangle toCopy(5, 8, 0);
	Triangle copy(1, 1, 1);
	copy = toCopy; // 5, 8, 0
	//! [copyOperator]
	//! [operator!=]
	Triangle t(1, 2, 3);
	Triangle t2(5, 4, 8);
	if (t2 != t){...}
	//! [operator!=]
	//! [operator==]
	Triangle t(1, 2, 3);
	Triangle t2(5, 4, 8);
	if (t2 == t){...}
	//! [operator==]
	//! [containerConstructor]
	std::vector<IndexVertex_t> l(3, 2);
	Triangle t(l); // 2, 2, 2
	//! [containerConstructor]
	//! [adjVertex]
	Triangle t(1, 2, 4);
	std::cout << t.getAdjVertexTrigo(2) << std::endl; // 4
	//! [adjVertex]
	//! [adjVertexClock]
	//! [adjVertexClock]
#endif

#endif
