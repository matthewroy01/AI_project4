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
	while (open.size() > 0)
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
		for (unsigned int i = 0; i < pConnections.size() - 1; i++)
		{
			// get the cost estimate for the end node
			endNode = pConnections[i]->getToNode();
			endNodeCost = pCurrent->mCostSoFar + pConnections[i]->getCost();

			// skip if the node is closed
			/* if the end node is in the closed list */

			// ...or if it is open and we've found a worse route
			/* if the end node is in the open list */

				// here we foind the record in the open list corresponding to the end node

			// otherwise we know we've got an unvisited node, so make a record for it
			//else
			{
				NodeRecord* endNodeRecord;
				endNodeRecord->mNode = endNode;
				// we're here if we need to update the node - update the cost and connection
				endNodeRecord->mCostSoFar = endNodeCost;
				endNodeRecord->mConnection = pConnections[i]->getFromNode();

				/* if the open list doesn't already contain endNode */
				{
					// add it to the open list
					open.push(endNodeRecord);
				}
			}
		}

		// we've finished looking at the connections for the current node, so add it to the closed list and remove it from the open list
		close.push_back(open.top()->mNode);
		open.pop();
	}

	// we're here if we've either found the goal. or if we've no more nodes to search, find which
	if (pCurrent->mNode != pTo)
	{
		// we've run out of nodes without finding the goal, so there's no solution
		return mPath; // return nothing?
	}
	else
	{
		// compile the list of connections in the path
		// mPath = ???

		// work back long the path, accumlating connections
		while (pCurrent->mNode != pStartRecord->mNode)
		{
			mPath.addNode(pCurrent->mConnection);
			pCurrent->mNode = pCurrent->mConnection;
		}

		// reverse the path and return it

	}

	return mPath;

}