#include <cstdint>
#include <iostream>
#include <exception>
#include <list>
#include <vector>

#include "neighbors.hpp"
#include "file_io.hpp"
#include "OffLoader.hpp"
#include "logs.hpp"


namespace
{
	#define DIMENSION_REQUIRED  3
	#define VERTEX_NUMBER_INDEX 0
	#define FACE_NUMBER_INDEX   1
	typedef std::vector<uint32_t> Header_vec; //!< Simplify modifications.
	
	/**
	 * @brief Read the header of the OFF \b file to pre-allocate containers \b v and \b t.
	 * @param file The pre-opened OFF file to read.
	 * @param v    The vertex   container to pre-allocate.
	 * @param t    The triangle container to pre-allocate.
	 * @return The previously read header.
	 * @throw std::string if the header is absent.
	 */
	Header_vec readOffHeader(InputFile& file, std::vector<Vertex>& v, std::vector<TopoTriangle>& t)
	{
		Header_vec header = file.readFromLine<uint32_t>(DIMENSION_REQUIRED);
		v.reserve(header.at(VERTEX_NUMBER_INDEX));
		t.reserve(header.at(FACE_NUMBER_INDEX));
		return header;
	}
	
	/**
	 * @brief Read \b nb vertices from the OFF \b file and store them into \b v.
	 * @param file The pre-opened OFF file to read.
	 * @param v    The vertex container to fill.
	 * @param nb   The number of vertex the function would read.
	 * @throw std::string If there is not enough vertex lines on the file.
	 */
	void readOffVertices(InputFile& file, std::vector<Vertex>& v, uint32_t nb)
	{
		uint32_t vertexIndex = 0;
		while(vertexIndex < nb)
		{
			v.push_back(file.readFromLine<VertexType, std::vector>(DIMENSION_REQUIRED));
			++vertexIndex;
		}
	}
	
	/**
	 * @brief Read from the OFF \b file \b nb triangle and add them to \b t.
	 * It also manages topology of \b v.
	 * @param file The pre-opened OFF file to read.
	 * @param v    The vertex container to modify for topology.
	 * @param t    The triangle container to fill.
	 * @param nb   The number of triangle the function would read.
	 * @throw std::string If there is not enough triangle lines on the file.
	 */
	void readOffTriangles(InputFile& file, std::vector<Vertex>& v, std::vector<TopoTriangle>& t, uint32_t nb)
	{
		neighbor::MapEdges         map;
		uint32_t                   faceIndex = 0;
		std::vector<IndexVertex_t> indexes(3, 0);
		while(faceIndex < nb)
		{
			std::vector<IndexVertex_t> tmp = file.readFromLine<IndexVertex_t>(4);
			std::copy(std::next(tmp.begin()), tmp.end(), indexes.begin());
			t.push_back(indexes);
			for(uint32_t i=0;i<indexes.size();++i)
			{
				if (v.at(i).face() == -1)
				{
					v.at(i).face(faceIndex);
				}
				TopoTriangle::Edge key = {indexes.at(i), indexes.at((i+1 == indexes.size()) ? 0 : i+1)};
				neighbor::insert(map, key, faceIndex, t);
			}
			++faceIndex;
		}
	}
}


void OffLoader::load(VertexContainer& vertices, TriangleContainer& triangles, const std::string& fname)
{
	InputFile file(fname);
	try
	{
		Header_vec header = readOffHeader(file, vertices, triangles);
		readOffVertices (file, vertices, header.at(VERTEX_NUMBER_INDEX));
		readOffTriangles(file, vertices, triangles, header.at(FACE_NUMBER_INDEX));
	}
	catch(std::string& eof)
	{
		std::cerr << eof << std::endl;
		throw std::runtime_error("Error while parsing the OFF file");
	}
	mtl::log::info("Succesfully load", fname);
}

void OffLoader::dump(const VertexContainer& vertices, const TriangleContainer& triangles, const std::string& fname)
{
	OutputFile file(fname);
	file.write(std::vector<std::string>(1, "OFF"), "");
	std::vector<uint32_t> header(DIMENSION_REQUIRED, 0);
	header.at(VERTEX_NUMBER_INDEX) = vertices.size();
	header.at(FACE_NUMBER_INDEX)   = triangles.size();
	file.write<uint32_t>(header, " ");
	
	for(auto v : vertices)
	{
		file.write<VertexType>(std::vector<VertexType>(v.begin(), v.end()), " ");
	}
	for(const auto f : triangles)
	{
		std::list<IndexVertex_t> indexes(f.beginVertice(), f.endVertice());
		indexes.push_front(3);
		file.write<IndexVertex_t, std::list>(indexes, " ");
	}
	mtl::log::info("Succesfully write", fname);
}
