/**
 * @file Vertex3D.hpp
 * @brief Describe a polymorph 3D vertex and some utilitaries functions.
 *
 * In order to compute this file, you'll need at least :
 *   - \b -std=c++11
 *
 *
 * @author  MTLCRBN
 * @version 2.03
 * @date    The 10th November 2016
 */
#ifndef VERTEX3D_HPP_INCLUDED
#define VERTEX3D_HPP_INCLUDED

#include <type_traits> // For every used traits.
#include <iostream>    // For std::ostream.
#include <iterator>    // For std::ostream_iterator.
#include <algorithm>   // For std::copy.
#include <cmath>       // For std::fabs, std::sqrt, std::pow.
#include <limits>      // For std::numeric_limits.
#include <utility>     // For std::pair
#include <cstdint>     // For int32_t
#include <sstream>     // For std::stringstream


/**
 * @class Vertex3D
 * @brief Declaration of a class to manipulate a 3D vertex on cartesian system.
 *
 * Type : It should be an arithmetic type only, otherwise you won't be able
 * to compile.<br />
 * Do not hesitate to add more constructors/operator=, or conversion operator :D
 */
template<typename Type>
class Vertex3D
{
	public:
		// ## Constructors ###############################################################
		/**
		 * @brief Build up a vertex with the default values of \b Type if any.
		 * In theory, every arithmetical type got a default construction routine.
		 */
		Vertex3D(void) : Vertex3D(Type(), Type(), Type())
		{
			this->face(-1);
		}
		/**
		 * @brief Copy \p other while creating a new Vertex.
		 * @param[in] other The Vertex3D of the same \b Type to copy.
		 */
		Vertex3D(const Vertex3D<Type>& other) : Vertex3D(other._x, other._y, other._z)
		{
			this->face(other.face());
		}
		/**
		 * Copy by move \p other into a fresh new Vertex3D.
		 * @param[in] other The Vertex3D of the same \b Type to move.
		 * @post \p other is now "empty" (back to default values).
		 */
		Vertex3D(Vertex3D<Type>&& other) : Vertex3D(other)
		{
			other.reset();
		}
		/**
		 * @brief Build up a Vertex3D with coordinates on the cartesian system.
		 * @param[in] x_cart The x coordinate on the cartesian system.
		 * @param[in] y_cart The y coordinate on the cartesian system.
		 * @param[in] z_cart The z coordinate on the cartesian system.
		 */
		Vertex3D(Type x_cart, Type y_cart, Type z_cart)
		{
			this->x(x_cart).y(y_cart).z(z_cart).face(-1);
		}
		/**
		 * @brief Allow you to create a vertex with the third first elements of \p container.
		 * @param[in] container The container to copy into this vertex.
		 * @pre \p container must contains at least 3 values.
		 * @pre \p container has to support iterators.
		 * @post The created vertex gots a copy of the 3 first elements of \p container.
		 */
		template<template<typename...> class Container>
		Vertex3D(const Container<Type>& container)
		{
			this->copyContainer(container);
			this->face(-1);
		}
		/**
		 * @brief Build a vertex with a static array (with the basic syntax).
		 * @pre  You'll have to ensure than the \p array contains at least 3 elements.
		 * @pre  \p array must not be \b nullptr !
		 * @post The new vertex contains a copy of the 3 first elements of \p array.
		 * @param[in] array The adress of the first element.
		 */
		Vertex3D(const Type *const array) : Vertex3D<Type>(array[0], array[1], array[2]){}
		/**
		 * @brief Create a vertex3D with a class \b S instance \p s.
		 * @param[in] s The struct or class to copy.
		 * @pre \p s must be a class or struct, no union.
		 * @pre \p s must define a public \b value_type of the same type of this vertex.
		 * @pre \p s must at least provides \b getX(), \b getY(), \b getZ() routines.
		 */
		template<class S>
		Vertex3D(const S& s)
		{
			*this = s;
			this->face(-1);
		}
		// ###############################################################################

		//! @brief Destroy the current Vertex.
		virtual ~Vertex3D(void)
		{
			this->reset();
		}

		// ## Getters ####################################################################
		/**
		 * @brief Grant access to the \b x value of this vertex.
		 * @return The x value on cartesian system.
		 */
		inline Type x(void) const {return this->_x;}
		/**
		 * @brief Grant access to the \b y value of this vertex.
		 * @return The y value on cartesian system.
		 */
		inline Type y(void) const {return this->_y;}
		/**
		 * @brief Grant access to the \b z value of this vertex.
		 * @return The z value on cartesian system.
		 */
		inline Type z(void) const {return this->_z;}
		/**
		 * @brief Grant access to the \b face value of this vertex.
		 * @return The face index, or -1 if none.
		 */
		inline int32_t face(void) const {return this->_i;}
		/**
		 * @brief Get the lowest and the highest value possible for this vertex.
		 * @return A pair which contains <lower, higher>.
		 */
		std::pair<Type,Type> limits(void) const
		{
			return std::make_pair(std::numeric_limits<Type>::min(),
			                      std::numeric_limits<Type>::max());
		}
		// ###############################################################################

		// ## Setters ####################################################################
		/**
		 * @brief Affect \p value to the x coordinate of this vertex.
		 * @param[in] value The value on the cartesian system.
		 * @post \b this got the new \p value as \b x coordinate.
		 * @return A reference to the current vertex you're modifying.
		 */
		Vertex3D<Type>& x(Type value)
		{
			this->_x = value;
			return *this;
		}
		/**
		 * @brief Affect \p value to the y coordinate of this vertex.
		 * @param[in] value The value on the cartesian system.
		 * @post \b this got the new \p value as \b y coordinate.
		 * @return A reference to the current vertex you're modifying.
		 */
		Vertex3D<Type>& y(Type value)
		{
			this->_y = value;
			return *this;
		}
		/**
		 * @brief Affect \p value to the z coordinate of this vertex.
		 * @param[in] value The value on the cartesian system.
		 * @post \b this got the new \p value as \b z coordinate.
		 * @return A reference to the current vertex you're modifying.
		 */
		Vertex3D<Type>& z(Type value)
		{
			this->_z = value;
			return *this;
		}
		/**
		 * @brief reset every values of this vertex.
		 * @return A reference  to this vertex.
		 */
		Vertex3D<Type>& reset(void)
		{
			this->_x = Type();
			this->_y = Type();
			this->_z = Type();
			this->_i = -1;
			return *this;
		}
		/**
		 * @brief Affect \p index_face to this vertex.
		 * @param[in] index_face The index of the face you want to affect.
		 * @return A reference  to this vertex.
		 */
		Vertex3D<Type>& face(int32_t index_face)
		{
			this->_i = index_face;
			return *this;
		}
		// ###############################################################################

		// ## Operators ##################################################################
		/**
		 * @brief Copy \p other by affection.
		 * @param[in] other The vertex3D to copy.
		 * @post \b this got a copy of \p other's values.
		 * @return A reference onto the affected vertex.
		 */
		Vertex3D<Type>& operator=(const Vertex3D<Type>& other)
		{
			this->x(other._x).y(other._y).z(other._z).face(other.face());
			return *this;
		}
		/**
		 * @brief Move \p other by affection.
		 * @param[in] other The vertex3D to move.
		 * @post \b this moves \p other's values.
		 * @return A reference onto the affected vertex.
		 */
		Vertex3D<Type>& operator=(Vertex3D<Type>&& other)
		{
			this->x(other._x).y(other._y).z(other._z).face(other.face());
			other.reset();
			return *this;
		}
		/**
		 * @brief Affect a static array \p arr to \b this..
		 * @param[in] arr A static array of \b Type.
		 * @pre \p arr must contains at least 3 elements.
		 * @pre \p array must no be nullptr !
		 * @post \b this got a copy of the 3 first elements of \p arr.
		 * @return A reference onto the affected vertex.
		 */
		Vertex3D<Type>& operator=(const Type *const arr)
		{
			std::copy(arr, arr+Vertex3D<Type>::NB_TO_END, &this->_x);
			return *this;
		}
		/**
		 * @brief Allow you to affect a vertex  coordinates with the third first elements
		 * of \p container.
		 * @param[in] container The container to copy into this vertex.
		 * @post \b this got a copy of the 3 first elements of \p container.
		 * @pre \p container must contains at least 3 values and has to support iterators.
		 */
		template<template<typename...> class Container>
		Vertex3D<Type>& operator=(const Container<Type>& container)
		{
			return this->copyContainer(container);
		}
		/**
		 * @brief If possible, affect \p s to your Vertex3D.
		 * @param[in] s A class or a struct (not union) you wanna affect to this vertex.
		 * @pre \p s must be a class or struct, no union.
		 * @pre \p s must define a public \b value_type of the same type of this vertex.
		 * @pre \p s must at least provides \b getX(), \b getY(), \b getZ() routines.
		 */
		template<class S>
		Vertex3D<Type>& operator=(const S& s)
		{
			this->checkCompatibility<S>();
			S& ss = const_cast<S&>(s);
			this->x(ss.getX()).y(ss.getY()).z(ss.getZ());
			return *this;
		}
		/**
		 * @brief Sum the coordinates of \b this and \p other.
		 * @param[in] other The other vertex to work with.
		 * @return A new vertice which contains the result.
		 */
		Vertex3D<Type> operator+(const Vertex3D<Type>& other) const
		{
			return Vertex3D<Type>(this->_x+other._x, this->_y+other._y, this->_z+other._z);
		}
		/**
		 * @brief Sub the coordinates of \b this and \p other.
		 * @param[in] other The other vertex to work with.
		 * @return A new vertice which contains the result.
		 */
		Vertex3D<Type> operator-(const Vertex3D<Type>& other) const
		{
			return Vertex3D<Type>(this->_x-other._x, this->_y-other._y, this->_z-other._z);
		}
		/**
		 * @brief Compare for equality between \b this and \p other.
		 * @param[in] other The vertex to compare with.
		 * @return true if these vertices are the same, false otherwise.
		 */
		bool operator==(const Vertex3D<Type>& other) const
		{
			return this->compare(other, std::is_floating_point<Type>());
		}
		/**
		 * @brief Compare for inequality between \b this and \p other.
		 * @param[in] other The vertex to compare with.
		 * @return false if these vertices are the same, true otherwise.
		 */
		bool operator!=(const Vertex3D<Type>& other) const
		{
			return !(*this == other);
		}
		/**
		 * @brief Multiply the coordinates of \b this by \p value.
		 * @param[in] value The value to multiply each coordinate.
		 * @return A new vertice which contains the result.
		 */
		template<typename TypeMult>
		Vertex3D<Type> operator*(TypeMult value) const
		{
			static_assert(std::is_arithmetic<TypeMult>::value, "Right operand of * must be numeric !");
			Type v = static_cast<Type>(value);
			return Vertex3D<Type>(this->_x*v, this->_y*v, this->_z*v);
		}
		/**
		 * @brief Divide the coordinates of \b this by \p value.
		 * @param[in] value The value to divide each coordinate.
		 * @pre value must not be 0, to avoid division by zero.
		 * @return A new vertice which contains the result.
		 */
		template<typename TypeMult>
		Vertex3D<Type> operator/(TypeMult value) const
		{
			static_assert(std::is_arithmetic<TypeMult>::value, "Right operand of / must be numeric !");
			Type v = static_cast<Type>(value);
			return Vertex3D<Type>(this->_x/v, this->_y/v, this->_z/v);
		}
		/**
		 * @brief Allow direct and implicit conversion of this Vertex3D into a pointer array
		 * of 3 elements.
		 */
		operator Type* (void)
		{
			return this->begin();
		}
		/**
		 * @brief Create a copy of the coordinates into the fresh new container.
		 * The container must accept iterators and begin()/end() routines.
		 * This container is read only :D.
		 */
		template<template<typename...> class Container>
		operator Container<Type> (void) const
		{
			return Container<Type>(this->begin(), this->end());
		}
		/**
		 * @brief Convert if possible \b this to an instance of \b S.
		 * @pre \b S must defines a public typedef value_type of the same type than this vertex.
		 * @pre \b S must be a class or struct, no union.
		 * @pre \b S must defines a public constructor with 3 \b value_type arguments.
		 */
		template<class S>
		operator S(void) const
		{
			this->checkCompatibility<S>();
			return S(this->_x, this->_y, this->_z);
		}

		// ###############################################################################

		// ## Iterators ##################################################################
		typedef const Type* const_iterator;  //!< To iterate over const elements.
		typedef Type*       iterator;        //!< To iterate over       elements.
		typedef std::size_t difference_type; //!< Some definition for iterators compatibility.
		typedef std::size_t size_type;       //!< Some definition for iterators compatibility.
		typedef Type        value_type;      //!< The basic type you manipulate.
		typedef Type*       pointer;         //!< A pointer   to the basic type.
		typedef Type&       reference;       //!< A reference to the basic type.

		/**
		 * @brief Return the beginning of an array of 3 elements, in order to use it with openGL for example.
		 * @return a read/write array of { \b x, \b y, \b z }.
		 */
		iterator begin(void)
		{
			return &this->_x;
		}
		/**
		 * @brief Return the ending of an array of 3 elements, in order to stop iteration.
		 * @warning You don't have to read at this position, it may product undefined behavior !
		 * @return The end of the array.
		 */
		iterator end(void)
		{
			return &this->_x+Vertex3D<Type>::NB_TO_END;
		}
		/**
		 * @brief Return the beginning of an array of 3 elements, in order to use it with openGL for example.
		 * @return a read only array of { \b x, \b y, \b z }.
		 */
		const_iterator cbegin(void) const
		{
			return &this->_x;
		}
		/**
		 * @brief Return the ending of an array of 3 elements, in order to stop iteration.
		 * @warning You don't have to read at this position, it may product undefined behavior !
		 * @return The end of the array.
		 */
		const_iterator cend(void) const
		{
			return &this->_x+Vertex3D<Type>::NB_TO_END;
		}
		// ###############################################################################

		// ## Utilitaries ################################################################
		/**
		 * @brief Compute the vectorial product between \b this and \p other.
		 * @param[in] other The vertex to work with.
		 * @return A new vertex which contains the result.
		 */
		Vertex3D<Type> cross(const Vertex3D<Type>& other) const
		{
			return Vertex3D<Type>(this->_y*other._z - this->_z*other._y,
			                      this->_z*other._x - this->_x*other._z,
			                      this->_x*other._y - this->_y*other._x);
		}
		/**
		 * @brief Compute the scalar product of \b this and \p other.
		 * @param[in] other The other vertex to work with.
		 * @return The result of this computing.
		 */
		double dot(const Vertex3D<Type>& other) const
		{
			return static_cast<double>(this->_x*other._x + this->_y*other._y + this->_z*other._z);
		}
		/**
		 * @brief Compute the length of \b this.
		 * @pre Your vertex must not be null (0, 0, 0).
		 * @return the computed length.
		 */
		double length(void) const
		{
			return std::sqrt(std::pow(this->_x, 2.0) +
			                 std::pow(this->_y, 2.0) +
			                 std::pow(this->_z, 2.0));
		}
		/**
		 * @brief Normalize a copy of \b this.
		 * @pre The length of \b this must not be 0 !
		 * @return A copy of \b this, normalized.
		 */
		Vertex3D<Type> normalize(void) const
		{
			return (*this)/this->length();
		}
		/**
		 * @brief Compute the "vector" between \b this and \p other.
		 * @param[in] other The other vertex you're working with.
		 * @return A new vertex which contains the result.
		 */
		Vertex3D<Type> vectorTo(const Vertex3D<Type>& other) const
		{
			return other - (*this);
		}
		/**
		 * @brief Compute the distance between \b this and \p other.
		 * @param[in] other The other vertex you're working with.
		 * @return The distance which separate \p other and \b this.
		 */
		double distance(const Vertex3D<Type>& other) const
		{
			return Vertex3D<Type>(*this-other).length();
		}
		// ###############################################################################

	private: // You shall not pass !
		//! @brief Ensure that you give a valid type for the coordinate system.
		static_assert(std::is_arithmetic<Type>::value, "For Vertex3D<Type>, Type must be arithmetic !");
		static const std::size_t NB_TO_END = 3; //!< The number to read inside a random container.

		/**
		 * @brief Compare using floating point way.
		 * @param[in] other The vertex to compare with.
		 * @return true if these vertices are the same, false otherwise.
		 */
		bool compare(const Vertex3D<Type>& other, std::true_type) const
		{
			static const Type EPSILON = 0.00001;
			return std::fabs(this->_x - other._x) < EPSILON &&
			       std::fabs(this->_y - other._y) < EPSILON &&
			       std::fabs(this->_z - other._z) < EPSILON;
		}
		/**
		 * @brief Compare using integral way.
		 * @param[in] other The vertex to compare with.
		 * @return true if these vertices are the same, false otherwise.
		 */
		bool compare(const Vertex3D<Type>& other, std::false_type) const
		{
			return this->_x == other._x && this->_y == other._y && this->_z == other._z;
		}
		/**
		 * @brief Check some aspects of \b S.
		 */
		template<class S>
		void checkCompatibility(void) const
		{
			static_assert(std::is_class<S>::value, "Vertex3D operator=(), right operand must be class or structure !");
			static_assert(std::is_same<Vertex3D<Type>::value_type, typename S::value_type>::value, "Vertex3D operator=(), right operand must defines the same public value_type than the vertex !");
		}
		/**
		 * @brief Copy \p container 3 first elements into \b this.
		 * @param[in] container The container with value desirated.
		 * @return A reference to \b this.
		 */
		template<template<typename...> class Container>
		Vertex3D<Type>& copyContainer(const Container<Type>& container)
		{
			typename Container<Type>::const_iterator it = container.cbegin();
			std::copy(it, std::next(it, Vertex3D<Type>::NB_TO_END), &this->_x);
			return *this;
		}
	
	protected:
		Type    _x; //!< The \b x coordinate on 3D system.
		Type    _y; //!< The \b y coordinate on 3D system.
		Type    _z; //!< The \b z coordinate on 3D system.
		int32_t _i; //!< The index of a face this vertex is linked to.

};

/**
 * @brief Print the content of \p that on \p out.
 * @param[inout] out  The output stream to print on.
 * @param[in]    that The vertex to print on \p out.
 * @return A reference to \p out, in order to chain calls.
 */
template<typename Type>
std::ostream& operator<<(std::ostream& out, const Vertex3D<Type>& that)
{
	std::copy(that.cbegin(), that.cend(), std::ostream_iterator<Type>(out, " "));
	return out;
}

/**
 * @brief Multiply the coordinates of \b this by \p value.
 * @param[in] value The value to multiply each coordinate.
 * @param[in] that  The vertex you want to multiply.
 * @return A new vertice which contains the result.
 */
template<typename TypeMult, typename Type>
Vertex3D<Type> operator*(TypeMult value, const Vertex3D<Type>& that)
{
	static_assert(std::is_arithmetic<TypeMult>::value, "Left operand of * must be numeric !");
	return that*value;
}

typedef Vertex3D<float>        Vertex3Df; //!< Defines a float  vertex3D.
typedef Vertex3D<double>       Vertex3Dd; //!< Defines a double vertex3D.
typedef Vertex3D<int>          Vertex3Di; //!< Defines an int   vertex3D.
typedef Vertex3D<unsigned int> Vertex3Du; //!< Defines an uint  vertex3D.


#endif
