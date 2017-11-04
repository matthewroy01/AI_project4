#include "DijkstraPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

DijkstraPathfinder::DijkstraPathfinder(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
}

DijkstraPathfinder::~DijkstraPathfinder()
{
}

const Path& DijkstraPathfinder::findPath(Node* pFrom, Node* pTo)
{
	NodeRecord* pCurrent;
	std::vector<Connection*> pConnections;
	Node* endNode;
	float endNodeCost;

	// initialize the record for the start node
	NodeRecord* pStartRecord;
	pStartRecord->mNode = pFrom;
	pStartRecord->mConnection = NULL;
	pStartRecord->mCostSoFar = 0;

	// initialize the open and closed lists
	std::priority_queue<NodeRecord*> open;
	open.push(pStartRecord);
	std::vector<Node*> close;

	// iterate through processing each node
	while (open.size())
	{
		// find the smallest element in the open list
		pCurrent = open.top();

		// if it is the goal node, then terminate
		if (pCurrent->mNode == pTo)
		{
			break;
		}

		// otherwise get its outgoing connections
		pConnections = mpGraph->getConnections(pCurrent->mNode->getId());

		// loop through each connection in turn
		for (int i = 0; i < pConnections.size() - 1; i++)
		{
			// get the cost estimate for the end node
			endNode = pConnections[i]->getToNode();
			endNodeCost = pCurrent->mCostSoFar + pConnections[i]->getCost();

			// skip if the node is closed
			bool skip = false;
			for (int i = 0; i < close.size(); i++)
			{
				if (close[i] == endNode)
				{
					skip = true;
				}
			}

			if (skip == true)
			{
				for (int i = 0; i < open.size(); i++)
				{
					if (open[i])
					{
						skip = true;
					}
				}
			}

			// ...or if it is open and we've found a worse route

				// here we foind the record in the open list corresponding to the end node

			// otherwise we know we've got an unvisited node, so make a record for it

			// we're here if we need to update the node - update the cost and connection

			// add it to the open list

		// we've finished looking at the connections for the current node, so add it to the closed list and remove it from the open list

	// we're here if we've either found the goal. or if we've no more nodes to search, find which

		// we've run out of nodes without finding the goal, so there's no solution

		// compile the list of connections in the path

		// work back long the path, accumlating connections

		// reverse the path and return it

	return mPath;

}

