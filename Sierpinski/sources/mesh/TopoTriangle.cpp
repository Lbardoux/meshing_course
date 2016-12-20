#include <algorithm>
#include <string>
#include <iostream>

#include "logs.hpp"
#include "TopoTriangle.hpp"



static void allocateNeighbors(IndexFace_t* n)
{
	n[0] = n[1] = n[2] = -1;
}

TopoTriangle::TopoTriangle(IndexVertex_t a, IndexVertex_t b, IndexVertex_t c) : Triangle(a, b, c)
{
	allocateNeighbors(this->neighbors);
	
}

TopoTriangle::~TopoTriangle(void)
{
	
}

TopoTriangle::TopoTriangle(const TopoTriangle& other) : Triangle(other)
{
	allocateNeighbors(this->neighbors);
	std::copy(other.neighbors, other.neighbors+3, this->neighbors);
}

TopoTriangle::TopoTriangle(TopoTriangle&& other) : Triangle(std::move(other))
{
	allocateNeighbors(this->neighbors);
	std::copy(other.neighbors, other.neighbors+3, this->neighbors);
	other.neighbors[0] = other.neighbors[1] = other.neighbors[2] = -1;
}

TopoTriangle& TopoTriangle::operator=(const TopoTriangle& other)
{
	Triangle::operator=(other);
	std::copy(other.neighbors, other.neighbors+3, this->neighbors);
	return *this;
}

TopoTriangle& TopoTriangle::operator=(TopoTriangle&& other)
{
	Triangle::operator=(std::move(other));
	std::copy(other.neighbors, other.neighbors+3, this->neighbors);
	other.neighbors[0] = other.neighbors[1] = other.neighbors[2] = -1;
	return *this;
}

bool TopoTriangle::operator==(const TopoTriangle& other) const
{
	bool legacy = Triangle::operator==(other);
	if (legacy == false)
	{
		return false;
	}
	for(int32_t i=0;i<3;++i)
	{
		if (this->neighbors[i] != other.neighbors[i])
		{
			return false;
		}
	}
	return true;
}

bool TopoTriangle::operator!=(const TopoTriangle& other) const
{
	return !(*this == other);
}

bool TopoTriangle::Edge::operator==(const Edge& other)
{
	return (this->a == other.a && this->b == other.b) || (this->b == other.a && this->a == other.b);
}

bool TopoTriangle::Edge::operator!=(const Edge& other)
{
	return !(*this == other);
}

typedef std::pair<IndexVertex_t, IndexVertex_t> Pair_t;
TopoTriangle::Edge::operator std::pair<IndexVertex_t, IndexVertex_t>(void)
{
	return (this->a < this->b) ? Pair_t(this->a, this->b) : Pair_t(this->b, this->a);
}


static const IndexFace_t indexes[3] = {0, 2, 1};
static int32_t computeFaceIndex(const TopoTriangle& that, const TopoTriangle::Edge& edge)
{
	IndexVertex_t ea = that.findVertexIndex(edge.a);
	IndexVertex_t eb = that.findVertexIndex(edge.b);
	if (ea == -1 || eb == -1)
	{
		std::cout << that << std::endl;
		std::cout << "{" << edge.a << ", " << edge.b << "}" << std::endl;
		throw std::string("Error, cannot find this vertex index !");
	}
	return indexes[(ea+eb)%3];
}

TopoTriangle& TopoTriangle::addNeighbor(IndexFace_t neighbor, const TopoTriangle::Edge& edge)
{
	this->neighbors[computeFaceIndex(*this, edge)] = neighbor;
	return *this;
}

IndexFace_t TopoTriangle::getAdjTriangleTrigo(IndexVertex_t current) const
{
	return this->neighbors[computeFaceIndex(*this, {current, this->getAdjVertexClock(current)})];
}

TopoTriangle& TopoTriangle::removeNeighbor(IndexFace_t i)
{
	int32_t index = this->findVertexIndex(i);
	if (index != -1)
	{
		this->neighbors[index] = -1;
	}
	return *this;
}

const IndexFace_t* TopoTriangle::getNeighbors(void) const
{
	return this->neighbors;
}

IndexVertex_t TopoTriangle::getOppositeVertexOf(IndexFace_t neighbor) const
{
	for(uint32_t i=0;i<3;++i)
	{
		if (this->neighbors[i] == neighbor)
		{
			return this->vertices[i];
		}
	}
	return -1;
}

IndexFace_t TopoTriangle::getOppositeNeighborOf(IndexVertex_t index) const
{
	int32_t i = this->findVertexIndex(index);
	if (i == -1)
	{
		return -1;
	}
	return this->neighbors[i];
}

void TopoTriangle::copyNeighbors(const TopoTriangle& other)
{
	for(uint32_t i=0;i<3;++i)
	{
		this->neighbors[i] = other.neighbors[i];
	}
}

TopoTriangle::Edge TopoTriangle::getEdgeWithout(IndexVertex_t index) const
{
	IndexVertex_t pts[2] = {0, 0};
	uint32_t j = 0;
	for(uint32_t i=0;i<3;++i)
	{
		if (this->vertices[i] != index)
		{
			pts[j] = this->vertices[i];
			j += (j != 1) ? 1 : 0;
		}
	}
	TopoTriangle::Edge edge = {pts[0], pts[1]};
	return edge;
}

IndexVertex_t TopoTriangle::getVertexOutsideOf(const TopoTriangle::Edge& edge) const
{
	for(uint32_t i=0;i<3;++i)
	{
		if (this->vertices[i] != edge.a && this->vertices[i] != edge.b)
		{
			return this->vertices[i];
		}
	}
	return 0;
}

TopoTriangle::Edge TopoTriangle::getCommonEdge(IndexFace_t other) const
{
	for(uint32_t i=0;i<3;++i)
	{
		if (this->getOppositeNeighborOf(this->vertices[i]) == other)
		{
			TopoTriangle::Edge result;
			result.a = this->vertices[(i+1)%3];
			result.b = this->vertices[(i+2)%3];
			return result;
		}
	}
	TopoTriangle::Edge result = {-1, -1};
	mtl::log::error("TopoTriangle::getCommonEdge(), other isn't a valid neighbor --> -1 -1");
	return result;
}

std::ostream& operator<<(std::ostream& out, const TopoTriangle& t)
{
	out << "Triangle : {" << std::endl << "\tvertices = [ ";
	std::copy(t.beginVertice(), t.endVertice(), std::ostream_iterator<int32_t>(out, " "));
	out << "]," << std::endl << "\tneighbors = [ ";
	std::copy(t.getNeighbors(), t.getNeighbors()+3, std::ostream_iterator<int32_t>(out, " "));
	std::cout << "]" << std::endl << "}";
	return out;
}
