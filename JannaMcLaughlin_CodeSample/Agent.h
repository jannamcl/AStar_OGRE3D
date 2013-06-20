#include "BaseApplication.h"
#include <deque>
#include "Grid.h"
#include "aStar.h"

#ifndef AGENT_H
#define AGENT_H




extern Ogre::SceneManager* sceneMgr; // Defined in main.cpp
class GridNode; // forward declarations
class Grid;


//#define NUM_ANIMS 13  // number of animations the character has. Should be made character specific

class Agent 
{
protected:
	enum AnimID{};

	// all the animations our character has, and a null ID
	// some of these affect separate body parts and will be blended together
	Ogre::SceneNode* mBodyNode; 
	Ogre::Entity* mBodyEntity; 
	float height;						// Characters are different heights
	float angleOffset;					// Some times they are created facing different directions
	float x;
	float y;
	float z;


	Ogre::Real mTimer;					  // general timer to see how long animations have been playing
	Ogre::Real mVerticalVelocity;		  // for jumping

	Ogre::Real mDistance;					// The distance the agent has left to travel
	Ogre::Vector3 mDirection;				// The direction the object is moving
	Ogre::Vector3 mDestination;				// The destination the object is moving towards
	std::deque<Ogre::Vector3> mWalkList;	// The list of points we are walking to
	Ogre::Real mWalkSpeed;					// The speed at which the object is moving
	bool nextLocation();					// Is there another destination?
	virtual void updateLocomote(Ogre::Real deltaTime) = 0;			// update the character's walking

	virtual void setupAnimations() = 0;
	void setBaseAnimation(AnimID id, bool reset = false);
	void setTopAnimation(AnimID id, bool reset = false);
	virtual void fadeAnimations(Ogre::Real deltaTime) = 0;
	virtual void updateAnimations(Ogre::Real deltaTime) = 0;

	std::list<GridNode*> path;		// store path to follow
	GridNode* currentNode;			// where is this agent?
	GridNode* goalNode;				// this can be set if the agent has a goal location they are trying to get to
	Grid* grid;						// pointer to the global grid
public:
	Agent(){};
	Agent(std::string name, std::string filename, float height = 5, float angleOffset = 90); // takes a mesh file
	void setPosition(float x, float y, float z); 
	Ogre::Vector3 getPosition();
	virtual void update(Ogre::Real deltaTime) = 0;		// update the agent
	
	GridNode* getCurrentNode(){return currentNode;};		// get the currentNode
	void setCurrentNode(GridNode* g){currentNode = g;};		// set the currentNode
	void setGoalNode(GridNode* g){goalNode = g;};			// set a goalNode
	GridNode* getGoalNode(){return goalNode;};		// get the goalNode, if any
	void setGrid(Grid* g) {grid = g;};						// keep a pointers to the universal grid
	virtual void moveTo(GridNode* node) = 0;							// Use A* to move to this node from your current spot
	void testGrid(Grid* grid);
	void Agent::discoverAnims();
};

#endif