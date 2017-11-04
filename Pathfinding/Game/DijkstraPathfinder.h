#ifndef DIJKSTRAPATHFINDER_H
#define DIJKSTRAPATHFINDER_H

#include "GridPathfinder.h"
#include <vector>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;

class DijkstraPathfinder :public GridPathfinder
{
public:
	DijkstraPathfinder(Graph* pGraph);
	~DijkstraPathfinder();

	const Path& findPath(Node* pFrom, Node* pTo);//make sure to delete the path when you are done!

private:
};

#endif