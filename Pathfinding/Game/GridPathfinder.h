#pragma once

#include "Pathfinder.h"
#include <allegro5/allegro.h>
class GridGraph;
class GridVisualizer;
class GraphicsBuffer;
class PathfindingDebugContent;

class GridPathfinder:public Pathfinder
{
public:
	friend class PathfindingDebugContent;

	GridPathfinder( GridGraph* pGraph );
	virtual ~GridPathfinder();

	virtual const Path& findPath( Node* pFrom, Node* pTo ) = 0;

	void setCurrentColor(int r, int g, int b)
	{
		currentColor.r = r;
		currentColor.g = g;
		currentColor.b = b;
	}
	
#ifdef VISUALIZE_PATH
	//just for visualization
public:
	void drawVisualization( Grid* pGrid, GraphicsBuffer* pDest );
protected:
	std::vector<Node*> mVisitedNodes;
	GridVisualizer* mpVisualizer;
#endif

	double mTimeElapsed;

private:
	ALLEGRO_COLOR currentColor;
};

struct NodeRecord
{
	Node* mNode;
	NodeRecord* mConnection;
	float mCostSoFar;

	bool operator<(const NodeRecord node)
	{
		return this->mCostSoFar < node.mCostSoFar;
	}
};