#ifndef Robot_H
#define Robot_H

#include "BaseApplication.h"
#include <deque>
#include "Grid.h"
#include "aStar.h"
#include "Agent.h"


extern Ogre::SceneManager* sceneMgr; // Defined in main.cpp
class GridNode; // forward declarations
class Grid;
class Agent;


#define NUM_ANIMS 5  // number of animations the character has. Should be made character specific

class Robot : public Agent 
{
protected:

	// all the animations our character has, and a null ID
	// some of these affect separate body parts and will be blended together
	enum AnimID
	{
		ANIM_DIE,
		ANIM_IDLE,
		ANIM_SHOOT,
		ANIM_SLUMP,
		ANIM_WALK,
		ANIM_NONE
	};

	Ogre::AnimationState* mAnims[NUM_ANIMS];    // master animation list
	AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
	AnimID mTopAnimID;                    // current top (upper-body) animation
	bool mFadingIn[NUM_ANIMS];            // which animations are fading in
	bool mFadingOut[NUM_ANIMS];           // which animations are fading out

	void updateLocomote(Ogre::Real deltaTime);			// update the character's walking

	void setupAnimations();
	void setBaseAnimation(AnimID id, bool reset = false);
	void setTopAnimation(AnimID id, bool reset = false);
	void fadeAnimations(Ogre::Real deltaTime);
	void updateAnimations(Ogre::Real deltaTime);

public:
	Robot(std::string name, std::string filename, float height = 5, float angleOffset = 90); // takes a mesh file

	void update(Ogre::Real deltaTime);		// update the agent

	void moveTo(GridNode* node);							// Use A* to move to this node from your current spot
	
};

#endif