#include "neighbors.hpp"

void neighbor::insert(MapEdges& map, const TopoTriangle::Edge& key, IndexFace_t index, std::vector<TopoTriangle>& faces)
{
	TopoTriangle::Edge k = key;
	MapEdges::iterator it = map.find(k);
	if (it == map.end())
	{
		map.insert(std::pair<neighbor::Key_t, IndexFace_t>(k, index));
	}
	else
	{
		faces.at(index).addNeighbor(it->second, k);
		faces.at(it->second).addNeighbor(index, k);
	}
}
