#include <algorithm>
#include <iostream>

#include "Triangle.hpp"


Triangle::Triangle(IndexVertex_t a, IndexVertex_t b, IndexVertex_t c)
{
	this->vertices[0] = a;
	this->vertices[1] = b;
	this->vertices[2] = c;
}

Triangle::Triangle(void) : Triangle(0, 0, 0)
{
	
}

Triangle::Triangle(const Triangle& other) : Triangle(other.vertices[0], other.vertices[1], other.vertices[2])
{
	
}

Triangle::Triangle(Triangle&& other) : Triangle(other.vertices[0], other.vertices[1], other.vertices[2])
{
	other.reset();
}

Triangle::~Triangle(void)
{
	this->reset();
}

Triangle& Triangle::reset(void)
{
	this->vertices[0] = 0;
	this->vertices[1] = 0;
	this->vertices[2] = 0;
	return *this;
}

Triangle::const_iterator_v Triangle::beginVertice(void) const
{
	return this->vertices;
}

Triangle::const_iterator_v Triangle::endVertice(void) const
{
	return this->vertices+3;
}

Triangle& Triangle::operator=(const Triangle& other)
{
	std::copy(other.vertices, other.vertices+3, this->vertices);
	return *this;
}

Triangle& Triangle::operator=(Triangle&& other)
{
	std::copy(other.vertices, other.vertices+3, this->vertices);
	other.reset();
	return *this;
}

bool Triangle::operator!=(const Triangle& other) const
{
	return !(*this == other);
}

bool Triangle::operator==(const Triangle& other) const
{
	for(int32_t i=0;i<3;++i)
	{
		if (this->vertices[i] != other.vertices[i])
		{
			return false;
		}
	}
	return true;
}

IndexVertex_t Triangle::findVertexIndex(IndexVertex_t v) const
{
	for(int32_t i=0;i<3;++i)
	{
		if (this->vertices[i] == v)
		{
			return i;
		}
	}
	return -1;
}

#define CHECK_I(i, what) if (i == -1) return what

IndexVertex_t Triangle::getAdjVertexTrigo(IndexVertex_t index) const
{
	int32_t i = this->findVertexIndex(index);
	CHECK_I(i, 0);
	return this->vertices[(i == 2) ? 0 : i+1];
}

IndexVertex_t Triangle::getAdjVertexClock(IndexVertex_t index) const
{
	int32_t i = this->findVertexIndex(index);
	CHECK_I(i, 0);
	return this->vertices[(i == 0) ? 2 : i-1];
}

std::ostream& operator<<(std::ostream& out, Triangle& t)
{
	std::copy(t.beginVertice(), t.endVertice(), std::ostream_iterator<IndexVertex_t>(out, " "));
	return out;
}
