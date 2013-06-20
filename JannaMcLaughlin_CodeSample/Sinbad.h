#ifndef SINBAD_H
#define SINBAD_H

#include "BaseApplication.h"
#include <deque>
#include "Grid.h"
#include "aStar.h"
#include "Agent.h"


extern Ogre::SceneManager* sceneMgr; // Defined in main.cpp
class GridNode; // forward declarations
class Grid;
class Agent;


#define NUM_ANIMS 13  // number of animations the character has. Should be made character specific

class Sinbad : public Agent 
{
protected:

	// all the animations our character has, and a null ID
	// some of these affect separate body parts and will be blended together
	enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,
		ANIM_RUN_BASE,
		ANIM_RUN_TOP,
		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,
		ANIM_DRAW_SWORDS,
		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,
		ANIM_DANCE,
		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
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
	Sinbad(std::string name, std::string filename, float height = 5, float angleOffset = 90); // takes a mesh file

	void update(Ogre::Real deltaTime);		// update the agent

	void moveTo(GridNode* node);							// Use A* to move to this node from your current spot
	
};

#endif