#include <allegro5/allegro.h>
#include "Game.h"
#include "GameApp.h"
#include "GameMessageManager.h"
#include "PathToMessage.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Vector2D.h"
#include "Grid.h"
#include "GridGraph.h"
#include "Connection.h"
#include "Path.h"
#include "DepthFirstPathfinder.h"
#include "Pathfinder.h"
#include "GridPathfinder.h"
#include "GridVisualizer.h"
#include "DebugDisplay.h"
#include "PathfindingDebugContent.h"

#include "DijkstraPathfinder.h"
#include "AStarPathfinder.h"
#include "InputManager.h"

#include <fstream>
#include <vector>

const IDType BACKGROUND_ID = ENDING_SEQUENTIAL_ID + 1;
const int GRID_SQUARE_SIZE = 32;
const std::string gFileName = "pathgrid.txt";

GameApp::GameApp()
:mpMessageManager(NULL)
,mpGrid(NULL)
,mpGridGraph(NULL)
,mpPathfinder(NULL)
,mpDebugDisplay(NULL)
{
}

GameApp::~GameApp()
{
	cleanup();
}

bool GameApp::init()
{
	bool retVal = Game::init();
	if( retVal == false )
	{

		return false;
	}

	mpMessageManager = new GameMessageManager();

	//create and load the Grid, GridBuffer, and GridRenderer
	mpGrid = new Grid(mpGraphicsSystem->getWidth(), mpGraphicsSystem->getHeight(), GRID_SQUARE_SIZE);
	mpGridVisualizer = new GridVisualizer( mpGrid );
	std::ifstream theStream( gFileName );
	mpGrid->load( theStream );

	//create the GridGraph for pathfinding
	mpGridGraph = new GridGraph(mpGrid);
	//init the nodes and connections
	mpGridGraph->init();

	/*mpPathfinder = new DepthFirstPathfinder(mpGridGraph);*/
	mpPathfinder = new DijkstraPathfinder(mpGridGraph);


	//load buffers
	mpGraphicsBufferManager->loadBuffer( BACKGROUND_ID, "wallpaper.bmp");

	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( BACKGROUND_ID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, pBackGroundBuffer->getWidth(), pBackGroundBuffer->getHeight() );
	}

	//debug display
	PathfindingDebugContent* pContent = new PathfindingDebugContent( mpPathfinder );
	mpDebugDisplay = new DebugDisplay( Vector2D(0,12), pContent );

	mpInputManager = new InputManager();

	mLastPos = Vector2D(0, 0);
	mPos = Vector2D(0, 0);

	mpMasterTimer->start();
	return true;
}

void GameApp::cleanup()
{
	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpGrid;
	mpGrid = NULL;

	delete mpGridVisualizer;
	mpGridVisualizer = NULL;

	delete mpGridGraph;
	mpGridGraph = NULL;

	delete mpPathfinder;
	mpPathfinder = NULL;

	delete mpDebugDisplay;
	mpDebugDisplay = NULL;

	delete mpInputManager;
	mpInputManager = NULL;
}

void GameApp::beginLoop()
{
	//should be the first thing done
	Game::beginLoop();
}

void GameApp::processLoop()
{
	//get back buffer
	GraphicsBuffer* pBackBuffer = mpGraphicsSystem->getBackBuffer();
	//copy to back buffer
	mpGridVisualizer->draw( *pBackBuffer );
#ifdef VISUALIZE_PATH
	//show pathfinder visualizer
	mpPathfinder->drawVisualization(mpGrid, pBackBuffer);
#endif

	mpDebugDisplay->draw( pBackBuffer );

	mpInputManager->Update();

	mpMessageManager->processMessagesForThisframe();

	//should be last thing in processLoop
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}

void GameApp::changeAlgorithm(int type)
{
	// delete the pathfinder and debug info
	if (mpPathfinder != NULL)
	{
		delete mpPathfinder;
		mpPathfinder = NULL;
	}

	if (mpDebugDisplay != NULL)
	{
		delete mpDebugDisplay;
		mpDebugDisplay = NULL;
	}

	// and then recreate it based on type
	switch (type)
	{
		case 0:
		{
			mpPathfinder = new DijkstraPathfinder(mpGridGraph);
			break;
		}
		case 1:
		{
			mpPathfinder = new AStarPathfinder(mpGridGraph);
			break;
		}
		default:
		{
			std::cout << "GameApp: " << type << " is not a valid type" << std::endl;
		}
	}
	//GameMessage* pMessage = new PathToMessage(mLastPos, mPos);
	//mpMessageManager->addMessage(pMessage, 0);

	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}

void GameApp::UpdatePaths()
{
	// send a message to keep drawing whenever we click
	GameMessage* pMessage = new PathToMessage(mLastPos, mPos);
	mpMessageManager->addMessage(pMessage, 0);
}