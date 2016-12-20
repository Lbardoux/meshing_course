/**
 * @file MeshCirculator.hpp
 * @brief This is a template for the Mesh circulators.
 * @author MTLCRBN
 * @version 0.1
 */
#ifndef MESHCIRCULATOR_HPP_INCLUDED
#define MESHCIRCULATOR_HPP_INCLUDED

#include <vector>
#include <cstdint>
#include <exception>
#include <type_traits>
#include <iterator>

#include "common.hpp"
#include "Vertex3D.hpp"
#include "TopoTriangle.hpp"

//! Previous declaration for friendship.
class Mesh;


/**
 * @class MeshCirculator
 * @brief A circulator which you could specialize with the template parameter.
 */
template<typename What>
class MeshCirculator final
{
	private:
		static_assert(std::is_same<What, Vertex>::value || std::is_same<What, TopoTriangle>::value, "What must be Vertex3Df or TopoTriangle");
		friend class Mesh;
		
		typedef std::vector<Vertex>       Vertices_t;  //!< Simplier to write and modify.
		typedef std::vector<TopoTriangle> Triangles_t; //!< Simplier to write and modify.

		TopoTriangle* current;   //!< The current triangle you're working with.
		IndexVertex_t center;    //!< The point you're using as an anchor.
		Vertices_t*   vertices;  //!< A raw pointer on the vertices.
		Triangles_t*  triangles; //!< A raw pointer on the triangles.
		
		/**
		 * @brief A special constructor for Mesh.
		 * @param[in] v The pointer to vertices.
		 * @param[in] t The pointer to triangles.
		 * @param[in] c The index of the point you're using as an anchor for rotation.
		 */
		MeshCirculator(Vertices_t* v, Triangles_t* t, IndexVertex_t c) : center(c), vertices(v), triangles(t)
		{
			this->current = (v != nullptr) ? &(t->at(v->at(c).face())) : nullptr;
		}
		
		//! @brief Case with Vertex3Df
		What& get(std::true_type)
		{
			return this->vertices->at(this->current->getAdjVertexClock(this->center));
		}
		//! @brief Case with TopoTriangle
		What& get(std::false_type)
		{
			return *this->current;
		}
		//! @brief reset this iterator, and make it invalid.
		void reset(void)
		{
			this->triangles = nullptr;
			this->vertices  = nullptr;
			this->center    = -1;
			this->current   = nullptr; 
		}
	public:
		//! @brief Create an invalid iterator.
		MeshCirculator(void) : MeshCirculator(nullptr, nullptr, -1){}
		/**
		 * @brief Create a new circulator by copying \p other.
		 * @param[in] other The circulator to copy.
		 */
		MeshCirculator(const MeshCirculator& other) : MeshCirculator(other.vertices, other.triangles, other.center){}
		/**
		 * @brief Create a new circulator by moving \p other.
		 * @param[inoit] other The circulator to move.
		 */
		MeshCirculator(MeshCirculator&& other) : MeshCirculator(other)
		{
			other.reset();
		}
		/**
		 * @brief Affect \p other to \b this by copy.
		 * @param[in] other The circulator to copy.
		 * @return A reference to \b this.
		 */
		MeshCirculator& operator=(const MeshCirculator& other)
		{
			this->triangles = other.triangles;
			this->vertices  = other.vertices;
			this->center    = other.center;
			this->current   = other.current;
			return *this;
		}
		/**
		 * @brief Affect \p other to \b this by move.
		 * @param[in] other The circulator to move.
		 * @return A reference to \b this.
		 */
		MeshCirculator& operator=(MeshCirculator&& other)
		{
			*this = other;
			other.reset();
			return *this;
		}
		/**
		 * @brief Check for equality between \b this and \p other.
		 * @param[in] other The other circulator to compare with.
		 * @return true if they're equal, false otherwise.
		 */
		bool operator==(const MeshCirculator& other)
		{
			return this->triangles == other.triangles && this->center  == other.center &&
			       this->vertices  == other.vertices  && this->current == other.current;
		}
		/**
		 * @brief Check for inequality between \b this and \p other.
		 * @param[in] other The other circulator to compare with.
		 * @return false if they're equal, true otherwise.
		 */
		bool operator!=(const MeshCirculator& other)
		{
			return !(*this == other);
		}
		/**
		 * @brief PreIncrement operation for this circulator.
		 * @return A reference into the current iterator.
		 */
		MeshCirculator& operator++(void)
		{
			this->current = &(this->triangles->at(this->current->getAdjTriangleTrigo(this->center)));
			return *this;
		}
		/**
		 * @brief PostIncrement operation for this circulator.
		 * @return A copy of the iterator before increment.
		 */
		MeshCirculator operator++(int i)
		{
			MeshCirculator copy = *this;
			i;
			++(*this);
			return copy;
		}
		/**
		 * @brief Dereference this iterator.
		 * @throw std::range_error if this iterator is invalid.
		 * @return A reference on the object you're manipulating.
		 */
		What& operator*(void)
		{
			if (this->vertices == nullptr || this->triangles == nullptr || this->current == nullptr)
			{
				throw std::range_error("Bad iterator");
			}
			return this->get(std::is_same<What, Vertex3Df>());
		}
};


#endif
