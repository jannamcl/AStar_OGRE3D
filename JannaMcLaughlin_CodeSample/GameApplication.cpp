#include "GameApplication.h"
#include "grid.h"
#include "aStar.h"


//-------------------------------------------------------------------------------------
GameApplication::GameApplication(void)
{
}
//-------------------------------------------------------------------------------------
GameApplication::~GameApplication(void)
{
}

//-------------------------------------------------------------------------------------
void GameApplication::createScene(void)
{
	loadEnv();
	setupEnv();
	loadObjects();
	loadCharacters();
}

void // Load the buildings or ground plane, etc
GameApplication::loadEnv()
{
	using namespace Ogre;

	// create a floor mesh resource
	MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	Plane(Vector3::UNIT_Y, 0), 100, 100, 10, 10, true, 1, 10, 10, Vector3::UNIT_Z);

	// create a floor entity, give it a material, and place it at the origin
    Entity* floor = mSceneMgr->createEntity("Floor", "floor");
    floor->setMaterialName("Examples/Rockwall");
	floor->setCastShadows(false);
    mSceneMgr->getRootSceneNode()->attachObject(floor);
}


void // Set up lights, shadows, etc
GameApplication::setupEnv()
{
	using namespace Ogre;

	// set background and some fog
	//mSceneMgr->setFog(Ogre::FOG_LINEAR, ColourValue(1.0f, 1.0f, 0.8f), 0, 15, 100);

	// create a skyplane 5000 units away, facing down, 10000 square units large, with 3x texture tiling
    mSceneMgr->setSkyPlane(true, Plane(0, -1, 1, 5000), "Examples/SpaceSkyPlane", 10000, 3);

	// create a skyplane 5000 units away, facing down, 10000 square units large, with 3x texture tiling
    mSceneMgr->setSkyPlane(true, Plane(0, 1, 1, 5000), "Examples/SpaceSkyPlane", 10000, 3);


	// set shadow properties
	mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
	mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTextureSize(1024);
	mSceneMgr->setShadowTextureCount(1);

	// use a small amount of ambient lighting
	mSceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));

	// add a bright light above the scene
	Light* light = mSceneMgr->createLight();
	light->setType(Light::LT_POINT);
	light->setPosition(-10, 40, 20);
	light->setSpecularColour(ColourValue::White);
}

void // Load other propers or objects (e.g. furniture)
GameApplication::loadObjects()
{
	using namespace Ogre;

	Ogre::Entity *ent;
	Ogre::SceneNode *node;

	ent = mSceneMgr->createEntity("Knot1", "knot.mesh");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot1Node", Ogre::Vector3(5.0f, 1.4f,  5.0f));
	node->attachObject(ent);
	node->setScale(0.04f, 0.04f, 0.04f);

	ent = mSceneMgr->createEntity("Knot2", "knot.mesh");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot2Node", Ogre::Vector3(5.0f, 1.4f,  15.0f));
	node->attachObject(ent);
	node->setScale(0.04f, 0.04f, 0.04f);

	ent = mSceneMgr->createEntity("Knot3", "knot.mesh");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot3Node", Ogre::Vector3(-35, 1.4f,  0));
	node->attachObject(ent);
	node->setScale(0.04f, 0.04f, 0.04f);

	ent = mSceneMgr->createEntity("Knot4", "knot.mesh");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot4Node", Ogre::Vector3(0, 1.4f,  -25));
	node->attachObject(ent);
	node->setScale(0.04f, 0.04f, 0.04f);
}

void // Load actors, agents or characters
GameApplication::loadCharacters()
{

	// set up grid
	int numRows = 10;
	int numCols = 10;

	Grid* grid = new Grid(numRows, numCols, 10, 14, -45, -45);
	
	//set obstacles
	GridNode* g = grid->getNode(5,5); 
	g->setOccupied();
	g = grid->getNode(6,5);
	g->setOccupied();
	g = grid->getNode(5,1); 
	g->setOccupied();
	g = grid->getNode(2,5); 
	g->setOccupied();

	//set agents

	Sinbad* agent = new Sinbad("Sinbad", "Sinbad.mesh");
	agentList.push_back(agent);
	agent->setGrid(grid); // need a pointer to the global grid
	agent->setCurrentNode(grid->getNode(25)); // position where the character begins
	agent->setGoalNode(grid->getNode(89));
	agent->setPosition(agent->getCurrentNode()->getX(), 0, agent->getCurrentNode()->getY());
	
	Penguin* agent2 = new Penguin("Penguin", "Penguin.mesh");
	agentList.push_back(agent2);
	agent2->setCurrentNode(grid->getNode(10));//start location for the agent
	agent2->setGoalNode(grid->getNode(91));//goal location for the agent
	agent2->setGrid(grid); // need a pointer to the global grid
	agent2->setCurrentNode(agent2->getCurrentNode()); // position where the character begins
	agent2->setPosition(agent2->getCurrentNode()->getX(), 0, agent2->getCurrentNode()->getY());

	Robot* agent3 = new Robot("Robot", "Robot.mesh");
	agentList.push_back(agent3);
	agent3->setCurrentNode(grid->getNode(0));//start location for the agent
	agent3->setGoalNode(grid->getNode(99));//goal location for the agent
	agent3->setGrid(grid); // need a pointer to the global grid
	agent3->setCurrentNode(agent3->getCurrentNode()); // position where the character begins
	agent3->setPosition(agent3->getCurrentNode()->getX(), 0, agent3->getCurrentNode()->getY());

	Robot* agent4 = new Robot("Robot2", "Robot.mesh");
	agentList.push_back(agent4);
	agent4->setCurrentNode(grid->getNode(81));//start location for the agent
	agent4->setGoalNode(grid->getNode(97));//goal location for the agent
	agent4->setGrid(grid); // need a pointer to the global grid
	agent4->setCurrentNode(agent4->getCurrentNode()); // position where the character begins
	agent4->setPosition(agent4->getCurrentNode()->getX(), 0, agent4->getCurrentNode()->getY());

	Penguin* agent5 = new Penguin("Penguin2", "Penguin.mesh");
	agentList.push_back(agent5);
	agent5->setCurrentNode(grid->getNode(8));//start location for the agent
	agent5->setGoalNode(grid->getNode(21));//goal location for the agent
	agent5->setGrid(grid); // need a pointer to the global grid
	agent5->setCurrentNode(agent5->getCurrentNode()); // position where the character begins
	agent5->setPosition(agent5->getCurrentNode()->getX(), 0, agent5->getCurrentNode()->getY());

	Sinbad* agent6 = new Sinbad("Sinbad2", "Sinbad.mesh");
	agentList.push_back(agent6);
	agent6->setGrid(grid); // need a pointer to the global grid
	agent6->setCurrentNode(grid->getNode(43)); // position where the character begins
	agent6->setGoalNode(grid->getNode(96));
	agent6->setPosition(agent6->getCurrentNode()->getX(), 0, agent6->getCurrentNode()->getY());

	// A*
	int count = 0;
	while(count<100)
	{
	
		for(iter = agentList.begin(); iter != agentList.end(); iter++)
		{
			//(*iter)->discoverAnims();
			Astar a = Astar(grid, (*iter)->getCurrentNode(), (*iter)->getGoalNode());
			bool isPath = a.findPath();
			std::list<GridNode*> temp;

			if(isPath)
			{
				temp = a.getPath(0);
			
				std::list<GridNode*>::iterator it;
				for(it = temp.begin(); it != temp.end(); it++)
				{
					if(*it != NULL)
						(*iter)->moveTo(*it);
				}
				
				for(it = temp.end(); it != temp.begin(); it--)
				{
					if(it == temp.end())
						continue;
					if((*it) != NULL)
						(*iter)->moveTo(*it);
				}
			}
		}
		count++;
	}

	/*Testing the grid*/
	//agent->testGrid(grid);
}

void 
GameApplication::addTime(Ogre::Real deltaTime)
{
	// update all agents
	std::list<Agent*>::iterator iter;
	for (iter = agentList.begin(); iter != agentList.end(); iter++)
		(*iter)->update(deltaTime);

	//updateCamera(deltaTime);
}


