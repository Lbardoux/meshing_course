/**
 * @file OffLoader.hpp
 * @brief Wrap the loading/dump parts about off files for Mesh.
 * @author MTLCRBN
 */
#ifndef OFFLOADER_HPP_INCLUDED
#define OFFLOADER_HPP_INCLUDED

#include <string>
#include "common.hpp"

/**
 * @brief Wrap every actions required to read/dump OFF files.
 * It requires Mesh for the commons aspects (like vertice type for example).
 */
class OffLoader final
{
	private:
		friend class Mesh;
		OffLoader(void) = delete;
		/**
		 * @brief Load from an OFF file \p fname, and fill \p vertices and \p triangles with it content.
		 * @param[inout] vertices  The vertex   container to fill with the content.
		 * @param[inout] triangles The triangle container to fill with the content.
		 * @param[in]    fname     The name of the file to load.
		 * @throw std::runtime_error If there is any problem while loading.
		 */
		static void load(VertexContainer& vertices, TriangleContainer& triangles, const std::string& fname);
		
		/**
		 * @brief Dump the content of \p vertices and \p triangles into a new file named \p fname.
		 * @param[in] vertices  The vertex   container to dump.
		 * @param[in] triangles The triangle container to dump.
		 * @param[in] fname     The name of the file.
		 */
		static void dump(const VertexContainer& vertices, const TriangleContainer& triangles, const std::string& fname);
	
};

#endif
