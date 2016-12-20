/**
 * @file MeshIterator.hpp
 * @brief Defines a template for a Mesh iterator (not circulator)
 * @author MLTCRBN
 * @version 0.2
 */
#ifndef MESHITERATOR_HPP_INCLUDED
#define MESHITERATOR_HPP_INCLUDED

#include <vector>
#include <exception>
#include <cstdint>
#include <type_traits>

#include "common.hpp"
#include "Vertex3D.hpp"

//! Previous declaration for friendship.
class Mesh;
class TopoTriangle;

/**
 * @class MeshIterator
 * @brief This is a template to avoid code copy
 * It's all fine, I'll just have to typedef some of them :D
 */
template<typename Content>
class MeshIterator final
{
	private:
		static_assert(std::is_same<Content, Vertex>::value || std::is_same<Content, TopoTriangle>::value, "Content must be Vertex3Df or TopoTriangle");
		friend class Mesh;
		typedef std::vector<Content>* Data_ptr; //!< To simplify rewrite of cod eif this type change.
		
		Data_ptr data;    //!< A pointer to the vector of data.
		int32_t  counter; //!< A counter to know where you are on the vector.
		
		/**
		 * @brief A private constructor for the class Mesh
		 * @param[in] container An address of the data vector.
		 * @param[in] where     The point where you wanna start.
		 */
		MeshIterator(Data_ptr container, int32_t where) : data(container), counter(where){}
	
	public:
		//! @brief Create an invalid iterator.
		MeshIterator(void) : MeshIterator(nullptr, 0){}
		/**
		 * @brief Create an iterator by copying \p other.
		 * @param[in] other the iterator to copy.
		 */
		MeshIterator(const MeshIterator<Content>& other) : MeshIterator(other.data, other.counter){}
		/**
		 * @brief Create an iterator by moving \p other.
		 * @param[in] other the iterator to move.
		 */
		MeshIterator(MeshIterator&& other) : MeshIterator(other.data, other.counter)
		{
			other.data    = nullptr;
			other.counter = -1;
		}
		
		//! @brief Destroy this iterator.
		~MeshIterator(void)
		{
			this->data    = nullptr;
			this->counter = 0;
		}
		
		/**
		 * @brief Affect \p other to \b this by copy.
		 * @param[in] other the iterator to copy.
		 * @return A reference to \b this.
		 */
		MeshIterator& operator=(const MeshIterator<Content>& other)
		{
			this->data    = other.data;
			this->counter = other.counter;
			return *this;
		}
		/**
		 * @brief Affect \p other to \b this by move.
		 * @param[in] other the iterator to move.
		 * @return A reference to \b this.
		 */
		MeshIterator& operator=(MeshIterator<Content>&& other)
		{
			*this = other;
			other.data    = nullptr;
			other.counter = -1;
			return *this;
		}
		/**
		 * @brief Compare equality between \b this and \p other.
		 * @param[in] other the iterator to compare with.
		 * @return true if they're equal, false otherwise.
		 */
		bool operator==(const MeshIterator<Content>& other)
		{
			return this->data == other.data && this->counter == other.counter;
		}
		/**
		 * @brief Compare inequality between \b this and \p other.
		 * @param[in] other the iterator to compare with.
		 * @return false if they're equal, true otherwise.
		 */
		bool operator!=(const MeshIterator<Content>& other)
		{
			return !(*this == other);
		}
		/**
		 * @brief PreIncrement this iterator
		 * @return The current iterator.
		 */
		MeshIterator& operator++(void)
		{
			++this->counter;
			return *this;
		}
		/**
		 * @brief PostIncrement this iterator
		 * @return A copy before increment of this iterator.
		 */
		MeshIterator operator++(int i)
		{
			MeshIterator<Content> copy = *this;
			++this->counter;
			return copy;
		}
		/**
		 * @brief Dereference this iterator
		 * @return The value this iterator is working with
		 * @throw std::range_error if you're using a bad iterator.
		 */
		Content& operator*(void)
		{
			if (this->data == nullptr || this->counter < 0 || this->counter >= (int32_t)this->data->size())
			{
				throw std::range_error("Bad iterator");
			}
			return this->data->at(this->counter);
		}
};

#endif
