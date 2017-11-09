#include "DijkstraPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>

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
	// we need this to print the elapsed time
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");

	mPath.clear();

	// the node we're keeping track of as we go
	NodeRecord* pCurrent;

	// initialize the record for the start node
	NodeRecord* pStartRecord = new NodeRecord();
	pStartRecord->mNode = pFrom;
	pStartRecord->mConnection = NULL;
	pStartRecord->mCostSoFar = 0;

	// initialize the open and closed lists
	std::vector<NodeRecord*> open;
	open.push_back(pStartRecord);
	std::vector<NodeRecord*> close;

	// iterate through processing each node
	while (open.size() > 0)
	{
		std::vector<Connection*> pConnections;
		NodeRecord* pEndRecord;
		NodeRecord* pAddRecord = new NodeRecord();

		// find the smallest element in the open list
		pCurrent = getSmallestCost(open);

		// if it is the goal node, then terminate
		if (pCurrent->mNode == pTo)
		{
			break;
		}
		else
		{
			// otherwise get its outgoing connections
			pConnections = mpGraph->getConnections(pCurrent->mNode->getId());
		}

		// loop through each connection in turn
		for (unsigned int i = 0; i < pConnections.size(); i++)
		{
			Connection* con = pConnections[i];
			pEndRecord = new NodeRecord();

			// get the cost estimate for the end node
			pEndRecord->mNode = pConnections[i]->getToNode();
			pEndRecord->mCostSoFar = pCurrent->mCostSoFar + pConnections[i]->getCost();

			// skip if the node is closed
			if (checkClosedList(close, pEndRecord->mNode))
			{
				continue;
			}
			// ...or if it is open and we've found a worse route
			else if (checkOpenList(open, pEndRecord->mNode))
			{
				pAddRecord = checkOpenList(open, pEndRecord->mNode);

				if (pAddRecord->mCostSoFar <= pEndRecord->mCostSoFar)
				{
					continue;
				}
			}
			// otherwise we know we've got an unvisited node, so make a record for it
			else
			{
				// we're here if we need to update the node - update the cost and connection
				pAddRecord = pEndRecord;
			}
			pAddRecord->mConnection = pCurrent;

			/* if the open list doesn't already contain endNode */
			if (checkOpenList(open, pEndRecord->mNode) == NULL)
			{
				// add it to the open list
				open.push_back(pAddRecord);
			}
		}

		// we've finished looking at the connections for the current node, so add it to the closed list and remove it from the open list
		close.push_back(pCurrent);
		open.erase(open.begin() + getIDFromOpen(open, pCurrent->mNode));

		//std::cout << "opened: " << open.size() << ", closed: " << close.size() << std::endl;
	}

	// we're here if we've either found the goal. or if we've no more nodes to search, find which
	if (pCurrent->mNode != pTo)
	{
		// we've run out of nodes without finding the goal, so there's no solution
	}
	else
	{
		// compile the list of connections in the path
		// work back long the path, accumlating connections
		while (pCurrent->mNode != pFrom)
		{
			mPath.addNode(pCurrent->mNode);
#ifdef VISUALIZE_PATH
			mVisitedNodes.push_back(pCurrent->mNode);
#endif
			pCurrent = pCurrent->mConnection;
		}
	}

	// we need this to print the elapsed time
	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

	return mPath;

}

NodeRecord* DijkstraPathfinder::checkOpenList(priority_queue<NodeRecord*> queue, Node* node)
{
	// search through the open list for the node
	while (queue.size() > 0)
	{
		// check the only node we can access (the top) and then pop it out to look at the next one
		if (queue.top()->mNode == node)
		{
			return queue.top();
		}
		queue.pop();
	}
	return NULL;
}

NodeRecord* DijkstraPathfinder::checkOpenList(vector<NodeRecord*> vect, Node* node)
{
	// search through the open list for the node
	for (unsigned int i = 0; i < vect.size(); i++)
	{
		if (vect[i]->mNode == node)
		{
			return vect[i];
		}
	}
	return NULL;
}

int DijkstraPathfinder::getIDFromOpen(vector<NodeRecord*> vect, Node* node)
{
	// search through the open list for the node
	for (unsigned int i = 0; i < vect.size(); i++)
	{
		if (vect[i]->mNode == node)
		{
			return i;
		}
	}
	return NULL;
}

NodeRecord* DijkstraPathfinder::checkClosedList(vector<NodeRecord*> vect, Node* node)
{
	// search through the closed list for the node
	for (unsigned int i = 0; i < vect.size(); i++)
	{
		if (vect[i]->mNode == node)
		{
			return vect[i];
		}
	}
	return NULL;
}

NodeRecord* DijkstraPathfinder::getSmallestCost(std::vector<NodeRecord*> vect)
{
	NodeRecord* tmp = vect[0];
	for (unsigned int i = 0; i < vect.size(); i++)
	{
		if (vect[i] < tmp)
		{
			tmp = vect[i];
		}
	}
	return tmp;
}