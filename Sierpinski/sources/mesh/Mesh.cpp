#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <map>
#include <list>
#include <queue>
#include <stack>
#include <exception>

#include "Mesh.hpp"
#include "logs.hpp"

// plugins
#include "OffLoader.hpp"
#include "neighbors.hpp"


// ## PARTIE TP1 ##############################################################################################
// La majorité des choses ont été bougé dans le plugin OffLoader
void Mesh::empty(void)
{
	// Puisque clear() refuse de dé-réserver, systeme D
	std::vector<Vertex> foo;
	this->vertices = std::move(foo);
	std::vector<TopoTriangle> bar;
	this->triangles = std::move(bar);
	this->borders.clear();
	this->constraints.clear();
	this->indexBeforeVoronoi = 0;
	mtl::log::info("Remove everything from the mesh");
}
void Mesh::loadMeshFromOff(const std::string& fname)
{
	this->empty();
	try
	{
		OffLoader::load(this->vertices, this->triangles, fname);
	}
	catch(std::runtime_error& exception)
	{
		// Do something usefull here, like displaying an error message.
		this->empty();
	}
}
void Mesh::dumpToOff(const std::string& fname) const
{
	OffLoader::dump(this->vertices, this->triangles, fname);
}
VertexContainer& Mesh::getVertices(void)
{
	return this->vertices;
}
const VertexContainer& Mesh::getVertices(void) const
{
	return this->vertices;
}
TriangleContainer& Mesh::getTriangles(void)
{
	return this->triangles;
}
const TriangleContainer& Mesh::getTriangles(void) const
{
	return this->triangles;
}
Border_c& Mesh::getBorders(void)
{
	return this->borders;
}
const Border_c& Mesh::getBorders(void) const
{
	return this->borders;
}
Curve_c& Mesh::getCurve(void)
{
	return this->curve;
}
const Curve_c& Mesh::getCurve(void) const
{
	return this->curve;
}
Curve_c& Mesh::getConstraints(void)
{
	return this->constraints;
}
const Curve_c& Mesh::getConstraints(void) const
{
	return this->constraints;
}
// ############################################################################################################

// ## PARTIE TP2 ##############################################################################################
Mesh::triangle_iterator Mesh::beginT(void)
{
	return triangle_iterator(&this->triangles, 0);
}
Mesh::triangle_iterator Mesh::endT(void)
{
	return triangle_iterator(&this->triangles, this->triangles.size());
}
Mesh::vertex_iterator Mesh::beginV(void)
{
	return vertex_iterator(&this->vertices, 0);
}
Mesh::vertex_iterator Mesh::endV(void)
{
	return vertex_iterator(&this->vertices, this->vertices.size());
}
Mesh::vertex_circulator Mesh::beginRV(IndexVertex_t center)
{
	return vertex_circulator(&this->vertices, &this->triangles, center);
}
Mesh::triangle_circulator Mesh::beginRT(IndexVertex_t center)
{
	return triangle_circulator(&this->vertices, &this->triangles, center);
}
// ############################################################################################################

// ## PARTIE TP3 ##############################################################################################
namespace
{
	/**
	 * @brief Pop elements as long as the element is -1.
	 * @param[in,out] queue The queue to pop in.
	 * @throw char To warn about the end of the queue, you must catch it !
	 * @return A valid element to use (not -1).
	 */
	IndexFace_t getNextAvailableIndex(std::queue<IndexFace_t>& queue)
	{
		IndexFace_t result = -1;
		while(result == -1)
		{
			if (queue.empty())
			{
				throw char();// Exception --> queue is empty
			}
			result = queue.front();
			queue.pop();
		}
		return result;
	}
	//! @brief Check if there is enough vertex to make the first triangle.
	inline bool enoughPointsForFirstTriangle(uint32_t nb)
	{
		return nb == 3;
	}
	/**
	 * @brief Get every neighbors indexes from \b t1 and \b t2.
	 * @param[in] t1 The first  triangle to parse.
	 * @param[in] t2 The second triangle to parse.
	 * @pre \b t1 and \b t2 must be neighbor.
	 * @return A container with the neighbors indexes.
	 */
	std::vector<IndexFace_t> collectNeighbors(const TopoTriangle& t1, const TopoTriangle& t2)
	{
		std::vector<IndexFace_t> result;
		for(const TopoTriangle& triangle : {t1, t2})
		{
			for(uint32_t i=0;i<3;++i)
			{
				IndexFace_t current = *(triangle.getNeighbors()+i);
				if (current != -1)
				{
					result.push_back(current);
				}
			}
		}
		return result;
	}
}

void Mesh::createInitialTriangle(void)
{
	const IndexVertex_t t[3] = {0, 1, 2};
	this->triangles.push_back(std::vector<IndexVertex_t>(t, t+3));
	for(IndexVertex_t i=0;i<3;++i)
	{
		this->borders.push_front(t[i]);
	}
}
Ptriangle3D Mesh::buildPtriangle3D(TopoTriangle& t)
{
	Vertex& a = this->vertices.at(*(t.beginVertice()));
	Vertex& b = this->vertices.at(*(t.beginVertice()+1));
	Vertex& c = this->vertices.at(*(t.beginVertice()+2));
	return Ptriangle3D(a, b, c);
}
//#define SEARCH_WITH_DISTANCE
IndexFace_t Mesh::isInOneTriangle(const Vertex& v)
{
#ifndef SEARCH_WITH_DISTANCE
	IndexFace_t i = 0;
	for(auto triangle : this->triangles)
	{
		Ptriangle3D tmp = this->buildPtriangle3D(triangle);
		if (isInThisTriangle(v, tmp))
		{
			return i;
		}
		++i;
	}
	return -1;
#else // It doesn't work, don't even use it
	IndexFace_t index    = rand()%(this->triangles.size());
	IndexFace_t indexMin = -1;
	IndexFace_t next     = index;
	do
	{
		index = next;
		TopoTriangle& triangle  = this->triangles.at(index);
		Ptriangle3D   ptriangle = this->buildPtriangle3D(triangle);
		if (isInThisTriangle(v, ptriangle))
		{
			return index;
		}
		Vertex bary     = barycentre(ptriangle);
		int32_t distMin = v.distance(bary);
		indexMin        = index;
		for(uint32_t i=0;i<3;++i)
		{
			IndexFace_t currentNeighbor = *(triangle.getNeighbors()+i);
			if (currentNeighbor != -1)
			{
				TopoTriangle& neighbor = this->triangles.at(currentNeighbor);
				Vertex bary = barycentre(this->buildPtriangle3D(neighbor));
				int32_t distance = v.distance(bary);
				if (distance < distMin)
				{
					distMin  = distance;
					indexMin = currentNeighbor;
					next     = currentNeighbor;
				}
			}
		}
	}while(index != indexMin);
	return -1;
#endif
}
IndexFace_t Mesh::localDelaunay(IndexFace_t tr_id)
{
	const TopoTriangle& triangle = this->triangles.at(tr_id);
	for(uint32_t i=0;i<3;++i)
	{
		IndexFace_t id = *(triangle.getNeighbors()+i);
		if (id != -1)
		{
			TopoTriangle::Edge  edge  = triangle.getCommonEdge(id);
			const Vertex& p = this->vertices.at(*triangle.beginVertice());
			const Vertex& q = this->vertices.at(*(triangle.beginVertice()+1));
			const Vertex& r = this->vertices.at(*(triangle.beginVertice()+2));
			IndexVertex_t indexOpposite = this->triangles.at(id).getVertexOutsideOf(edge);
			if (indexOpposite != -1)
			{
				const Vertex& s = this->vertices.at(this->triangles.at(id).getVertexOutsideOf(edge));
				if (isInSurroundingCircle(p, q, r, s))
				{
					return id;
				}
			}
		}
	}
	return -1;
}
void Mesh::incrementalDelaunay(const std::vector<IndexFace_t>& newTriangles)
{
	std::queue<IndexFace_t> queue;
	for(auto elt : newTriangles)
	{
		queue.push(elt);
	}
	try
	{
		while(true)
		{
			IndexFace_t current    = getNextAvailableIndex(queue);
			IndexFace_t toFlipWith = this->localDelaunay(current);
			if (toFlipWith != -1)
			{
				const TopoTriangle& TCurrent = this->triangles.at(current);
				const TopoTriangle& TFlip    = this->triangles.at(toFlipWith);
				IndexVertex_t icurrent = TCurrent.getOppositeVertexOf(toFlipWith);
				IndexVertex_t iflip    = TFlip.getOppositeVertexOf(current);
				queue.push(TCurrent.getOppositeNeighborOf(TCurrent.getAdjVertexClock(icurrent)));
				queue.push(TCurrent.getOppositeNeighborOf(TCurrent.getAdjVertexTrigo(icurrent)));
				queue.push(TFlip.getOppositeNeighborOf(TFlip.getAdjVertexClock(iflip)));
				queue.push(TFlip.getOppositeNeighborOf(TFlip.getAdjVertexTrigo(iflip)));
				this->flip(current, toFlipWith);
			}
		}
	}
	catch(char& endOfQueue)
	{
		
	}
}
void Mesh::manageNeighborInside(const std::vector<IndexFace_t> &news, std::vector<IndexFace_t>& concerned)
{
	for(auto tmp : news)
	{
		concerned.push_back(tmp);
	}
	neighbor::MapEdges map;
	for(auto index : concerned)
	{
		if (index == -1)
		{
			continue;
		}
		TopoTriangle& triangle = this->triangles.at(index);
		for(int32_t i=0;i<3;++i)
		{
			TopoTriangle::Edge key = {*(triangle.beginVertice()+i), *(triangle.beginVertice()+(i+1)%3)};
			neighbor::insert(map, key, index, this->triangles);
		}
	}
}
void Mesh::insertPointIntoTriangle(Vertex& v, IndexFace_t indexCurrentFace, IndexVertex_t v_index)
{
	TopoTriangle dying = this->triangles.at(indexCurrentFace);
	auto         it    = dying.beginVertice();
	int32_t      i     = 0;
	int32_t      max_i = 3;
	std::vector<IndexFace_t> news;
	std::vector<IndexFace_t> concerned(dying.getNeighbors(), dying.getNeighbors()+3);
	while(i < max_i)
	{
		IndexVertex_t p3        = dying.getAdjVertexClock(*it);
		IndexFace_t   faceIndex = (i+1 == max_i) ? indexCurrentFace : this->triangles.size();
		TopoTriangle  tmp(v_index, p3, *it);
		
		news.push_back(faceIndex);
		if (i+1 == max_i) // We write over an existing triangle
		{
			v.face(indexCurrentFace);
			for(IndexVertex_t pt=0;pt<3;++pt)
			{
				Vertex& vertex = this->vertices.at(*(dying.beginVertice()+pt));
				if (vertex.face() == indexCurrentFace &&
					!(std::find(tmp.beginVertice(), tmp.endVertice(), vertex.face()) == tmp.endVertice()))
				{
					vertex.face(this->triangles.size()-1);
					break;
				}
			}
			this->triangles.at(indexCurrentFace) = std::move(tmp);
		}
		else // We just create a new triangle
		{
			this->triangles.push_back(std::move(tmp));
		}
		++it;
		++i;
	}
	
	this->manageNeighborInside(news, concerned);
	this->incrementalDelaunay(concerned);
}
IndexFace_t Mesh::findThisFace(IndexVertex_t a, IndexVertex_t b) const
{
	for(uint32_t i=0;i<this->triangles.size();++i)
	{
		const TopoTriangle& f = this->triangles.at(i);
		if (std::find(f.beginVertice(), f.endVertice(), a) != f.endVertice() &&
			std::find(f.beginVertice(), f.endVertice(), b) != f.endVertice())
		{
			return i;
		}
	}
	return -1;
}
void Mesh::updateBorder(IndexVertex_t id, const std::vector<uint32_t>& usages, IndexVertex_t first)
{
	Border_c border2;
	auto it = this->borders.begin();
	for(int32_t i=0;i<(int32_t)usages.size();++i)
	{
		if (usages.at(i) < 2)
		{
			border2.push_back(*it);
		}
		if (i == first)
		{
			border2.push_back(id);
		}
		++it;
	}
	this->borders = std::move(border2);
}
void Mesh::updateNeighborsOutside(std::vector<IndexFace_t>& newTriangles, IndexVertex_t c)
{
	neighbor::MapEdges map;
	for(uint32_t i=0;i<newTriangles.size();++i)
	{
		IndexFace_t   index   = newTriangles.at(i);
		TopoTriangle& current = this->triangles.at(index);
		TopoTriangle::Edge edge1 = {c, *current.beginVertice()};
		TopoTriangle::Edge edge2 = {c, *(current.beginVertice()+1)};
		neighbor::insert(map, edge1, index, this->triangles);
		neighbor::insert(map, edge2, index, this->triangles);
	}
}
void Mesh::insertPointOutside(Vertex& ins, IndexVertex_t index)
{
	int32_t  iFirst = -1;
	uint32_t i      = 0;
	std::vector<uint32_t>    usages(this->borders.size(), 0);
	std::vector<IndexFace_t> newTriangles;
	
	for(auto it=this->borders.begin();it!=this->borders.end();++it)
	{
		Vertex        current = this->vertices.at(*it);
		IndexVertex_t next_id = (std::next(it) == this->borders.end()) ? *this->borders.begin() : *std::next(it);
		Vertex        next    = this->vertices.at(next_id);
		if (isWellOriented(current, next, ins))
		{
			if (iFirst == -1)
				iFirst = (int32_t)i;
			usages.at(i)                   += 1;
			usages.at((i+1)%usages.size()) += 1;
			TopoTriangle newFace(*it, next_id, index);
			IndexFace_t  voisin = this->findThisFace(*it, next_id);
			if (voisin != -1)
			{
				TopoTriangle::Edge edge = {next_id, *it};
				newFace.addNeighbor(voisin, edge);
				this->triangles.at(voisin).addNeighbor(this->triangles.size(), edge);
			}
			ins.face(this->triangles.size());
			newTriangles.push_back(this->triangles.size());
			this->triangles.push_back(std::move(newFace));
		}
		++i;
	}
	this->updateBorder(index, usages, iFirst);
	this->updateNeighborsOutside(newTriangles, index);
	std::vector<IndexFace_t> concerned;
	for(auto ind : newTriangles)
	{
		concerned.push_back(this->triangles.at(ind).getOppositeNeighborOf(index));
	}
	this->incrementalDelaunay(concerned);
}
void Mesh::insertVertexIntoTriangulation(Vertex& v, IndexVertex_t index)
{
	this->vertices.push_back(v);
	if (enoughPointsForFirstTriangle(this->vertices.size()))
	{
		createInitialTriangle();
	}
	else if (this->vertices.size() > 3)
	{
		IndexFace_t indexTriangle = isInOneTriangle(v);
		if (indexTriangle != -1)
		{
			this->insertPointIntoTriangle(v, indexTriangle, index);
		}
		else
		{
			this->insertPointOutside(v, index);
		}
	}
}
void Mesh::readVerticesFromPts(InputFile& file, uint32_t nb)
{
	uint32_t i = 0;
	while(i < nb)
	{
		std::vector<VertexType> vertex = file.readFromLine<VertexType>(2);
		Vertex v(vertex.at(0), vertex.at(1), 0.0f);
		insertVertexIntoTriangulation(v, i++);
	}	
}
void Mesh::loadVertices(InputFile& file)
{
	uint32_t nb = file.readFromLine<uint32_t>(1).at(0);
	this->vertices.reserve(nb);
	this->readVerticesFromPts(file, nb);
}
void Mesh::load2DTriangulationFromPts(const std::string& fname)
{
	this->empty();
	InputFile file(fname);
	try
	{
		mtl::log::info("Loading", fname, "for 2D triangulation -->", mtl::log::hold_on());
		this->loadVertices(file);
		mtl::log::info("Done");
	}
	catch(const std::string &eof)
	{
		mtl::log::error("Error while loading [", fname, "] : ", eof);
		this->empty();
		throw std::invalid_argument("Prematured end of parsing 2D Triangulation");
	}
}
void Mesh::flip(IndexFace_t f1, IndexFace_t f2)
{
	TopoTriangle&            old_f1    = this->triangles.at(f1);
	TopoTriangle&            old_f2    = this->triangles.at(f2);
	IndexVertex_t            unique_f1 = old_f1.getOppositeVertexOf(f2);
	IndexVertex_t            unique_f2 = old_f2.getOppositeVertexOf(f1);
	std::vector<IndexFace_t> concerned = collectNeighbors(old_f1, old_f2);

	Vertex& changed_f2 = this->vertices.at(old_f2.getAdjVertexTrigo(unique_f2));
	if (changed_f2.face() == f1)
	{
		changed_f2.face(f2);
	}
	Vertex& changed_f1 = this->vertices.at(old_f1.getAdjVertexTrigo(unique_f1));
	if (changed_f1.face() == f2)
	{
		changed_f1.face(f1);
	}
	this->triangles.at(f1) = TopoTriangle(unique_f1, old_f1.getAdjVertexTrigo(unique_f1), unique_f2);
	this->triangles.at(f2) = TopoTriangle(unique_f2, old_f2.getAdjVertexTrigo(unique_f2), unique_f1);
	
	neighbor::MapEdges map;
	for(uint32_t i=0;i<concerned.size();++i)
	{
		IndexFace_t   index   = concerned.at(i);
		TopoTriangle& current = this->triangles.at(index);
		for(uint32_t j=0;j<3;++j)
		{
			TopoTriangle::Edge edge = {*(current.beginVertice()+j), *(current.beginVertice()+(j+1)%3)};
			neighbor::insert(map, edge, index, this->triangles);
		}
	}
}
// ############################################################################################################

// ## TP PARTIE IV ############################################################################################
namespace
{
	/**
	 * @brief Detects edges without any voronoi centers as extremity.
	 * @param mesh The mesh where it gonna insert the edges.
	 * @param t    The triangle to parse.
	 */
	void addEdgesOf(Mesh& mesh, TopoTriangle& t)
	{
		const IndexVertex_t* begin = t.beginVertice();
		bool p0 = begin[0] < mesh.getIndexBeforeVoronoi();
		bool p1 = begin[1] < mesh.getIndexBeforeVoronoi();
		bool p2 = begin[2] < mesh.getIndexBeforeVoronoi();
		if (p0 && p1)
		{
			TopoTriangle::Edge edge = {begin[0], begin[1]};
			mesh.getCurve().push_back(edge);
		}
		if (p0 && p2)
		{
			TopoTriangle::Edge edge = {begin[0], begin[2]};
			mesh.getCurve().push_back(edge);
		}
		if (p2 && p1)
		{
			TopoTriangle::Edge edge = {begin[2], begin[1]};
			mesh.getCurve().push_back(edge);
		}
	}
}

void Mesh::Crust(void)
{
	mtl::log::info("Processing Crust algorithm");
	std::vector<Vertex> vertexes;
	vertexes.reserve(this->triangles.size());
	this->indexBeforeVoronoi = this->vertices.size();
	for(auto it=this->beginT();it!=this->endT();++it)
	{
		Vertex tmp = centerSurroundingCircle2D(this->buildPtriangle3D(*it));
		vertexes.push_back(tmp);
	}
	mtl::log::info("---- voronois [OK]");
	for(auto pt : vertexes)
	{
		pt.z(0.0);
		this->insertVertexIntoTriangulation(pt, this->vertices.size());
	}
	mtl::log::info("---- insertions [OK]");
	this->curve.clear();
	for(auto it=this->beginT();it!=this->endT();++it)
	{
		TopoTriangle t = *it;
		addEdgesOf(*this, t);
	}
	mtl::log::info("Crust done");
}
// ############################################################################################################

// ## TP PARTIE V #############################################################################################
Vertex Mesh::centerOfEdge(const TopoTriangle::Edge& edge)
{
	const Vertex& v1 = this->vertices.at(edge.a);
	const Vertex& v2 = this->vertices.at(edge.b);
	return (v2 - v1)/2.0;
}
std::list<IndexFace_t> Mesh::collectPoorQualityTriangles(double threshold)
{
	std::list<IndexFace_t> poor;
	for(int32_t i=0;i<(int32_t)this->triangles.size();++i)
	{
		if (isPoorQuality(this->buildPtriangle3D(this->triangles.at(i)), threshold))
		{
			poor.push_back(i);
		}
	}
	return poor;
}
Curve_c Mesh::collectEncroacheds(Curve_c& notEncroached)
{
	Curve_c result;
	for(auto segment : this->constraints)
	{
		if (this->findThisFace(segment.a, segment.b) == -1)
		{
			result.push_back(segment);
		}
		else
		{
			notEncroached.push_back(segment);
		}
	}
	return result;
}
bool Mesh::encroachSegment(const Vertex& v, Curve_c& segments, TopoTriangle::Edge& edge)
{
	for(auto it=segments.begin();it!=segments.end();++it)
	{
		const Vertex& a = this->vertices.at(it->a);
		const Vertex& b = this->vertices.at(it->b);
		if (isInCircleOfDiametral(a, b, v))
		{
			edge.a = it->a;
			edge.b = it->b;
			segments.erase(it);
			return true;
		}
	}
	return false;
}
/*
function Ruppert(points,segments,threshold):
    T := DelaunayTriangulation(points);
    Q := the set of encroached segments and poor quality triangles;
    while Q is not empty:                 // The main loop
        if Q contains a segment s:
            insert the midpoint of s into T;
        else Q contains poor quality triangle t:
            if the circumcenter of t encroaches a segments s:
                add s to Q;
            else:
                insert the circumcenter of t into T;
            end if;
        end if;
        update Q;
    end while;
    return T;
end Ruppert.
*/
void Mesh::refineDelaunay(double threshold)
{
	mtl::log::info("Starting Ruppert's algorithm");
	mtl::log::info("Collecting poor quality triangles", mtl::log::hold_on());
	std::list<IndexFace_t> Qtriangles = this->collectPoorQualityTriangles(threshold);
	mtl::log::info("(", Qtriangles.size(), "found )");
	mtl::log::info("Collecting encroaching segments", mtl::log::hold_on());
	Curve_c notEncroached;
	Curve_c Qencroach = this->collectEncroacheds(notEncroached);
	mtl::log::info("(", Qencroach.size(), "found )");
	
	mtl::log::info("Starting main loop ...", mtl::log::hold_on());
	while(!Qtriangles.empty() || !Qencroach.empty())
	{
		if (!Qencroach.empty())
		{
			TopoTriangle::Edge edge = Qencroach.front();
			Vertex v                = this->centerOfEdge(edge);
			this->insertVertexIntoTriangulation(v, this->vertices.size());
			Qencroach.pop_front();
		}
		else if (!Qtriangles.empty())
		{
			TopoTriangle& triangle = this->triangles.at(Qtriangles.front());
			Vertex        voronoi  = centerSurroundingCircle2D(this->buildPtriangle3D(triangle));
			TopoTriangle::Edge edge;
			if (this->encroachSegment(voronoi, notEncroached, edge))
			{
				Qencroach.push_front(edge);
			}
			else
			{
				this->insertVertexIntoTriangulation(voronoi, this->vertices.size());
				Qtriangles.pop_front();
			}
		}
	}
	mtl::log::info("Done");
}
void Mesh::loadConstraints(const std::string& fname)
{
	this->empty();
	InputFile file(fname);
	try
	{
		mtl::log::info("Reading", fname, "for a refined Delaunay");
		this->loadVertices(file);
		uint32_t i = 0;
		while(i++ < this->vertices.size())
		{
			std::vector<IndexVertex_t> vector = file.readFromLine<IndexVertex_t>(2);
			TopoTriangle::Edge edge = {vector.at(0), vector.at(1)};
			this->constraints.push_back(edge);
		}
		this->refineDelaunay(22.0);
	}
	catch(const std::string &eof)
	{
		mtl::log::error("Error while loading [", fname, "] : ", eof);
		this->empty();
		throw std::invalid_argument("Error while reading the file");
	}
}
// ############################################################################################################
