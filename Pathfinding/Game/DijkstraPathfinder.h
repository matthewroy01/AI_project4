#ifndef DIJKSTRAPATHFINDER_H
#define DIJKSTRAPATHFINDER_H

#include "GridPathfinder.h"
#include <list>
#include <vector>
#include <queue>
#include <algorithm>

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

	NodeRecord* checkOpenList(std::priority_queue<NodeRecord*> queue, Node* node);
	NodeRecord* checkOpenList(std::vector<NodeRecord*> vect, Node* node);
	NodeRecord* checkClosedList(std::vector<NodeRecord*> vect, Node* node);
	NodeRecord* getSmallestCost(std::vector<NodeRecord*> vect);
	int getIDFromOpen(std::vector<NodeRecord*> vect, Node* node);

private:
	
};

#endif