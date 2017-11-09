#include "AStarPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include "GameApp.h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>
#include "Grid.h"
#include "Vector2D.h"

using namespace std;

AStarPathfinder::AStarPathfinder(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
	// we need the grid for calculating the distance
	GameApp* gameApp = dynamic_cast<GameApp*>(gpGame);

	mpGrid = gameApp->getGrid();
}

AStarPathfinder::~AStarPathfinder()
{
}

const Path& AStarPathfinder::findPath(Node* pFrom, Node* pTo)
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
	// also stop if the end node is in the open list
	while (open.size() > 0)
	{
		std::vector<Connection*> pConnections;
		NodeRecord* pEndRecord;
		NodeRecord* pAddRecord = new NodeRecord();

		// find the smallest element in the open list
		pCurrent = getSmallestCost(open, pTo);

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
			else
			{
				// or if it is, time to break
				break;
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

NodeRecord* AStarPathfinder::checkOpenList(priority_queue<NodeRecord*> queue, Node* node)
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

NodeRecord* AStarPathfinder::checkOpenList(vector<NodeRecord*> vect, Node* node)
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

int AStarPathfinder::getIDFromOpen(vector<NodeRecord*> vect, Node* node)
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

NodeRecord* AStarPathfinder::checkClosedList(vector<NodeRecord*> vect, Node* node)
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

NodeRecord* AStarPathfinder::getSmallestCost(std::vector<NodeRecord*> vect, Node* to)
{
	// in A*, we also need to check a distance between the current node and the goal
	Vector2D toPos, fromPos;

	toPos = mpGrid->getULCornerOfSquare(to->getId());

	NodeRecord* tmp = vect[0];
	float tmpDistance = 9999999999;
	for (unsigned int i = 0; i < vect.size(); i++)
	{
		fromPos = mpGrid->getULCornerOfSquare(vect[i]->mNode->getId());
		if (getDistance(toPos, fromPos) < tmpDistance)
		{
			tmpDistance = getDistance(toPos, fromPos);
			tmp = vect[i];
		}
	}
	return tmp;
}

float AStarPathfinder::getDistance(Vector2D to, Vector2D from)
{
	// return the distance between the mover and the target
	return sqrt(pow(from.getX() - to.getX(), 2.0f)
		+ pow(from.getY() - to.getY(), 2.0f));
}