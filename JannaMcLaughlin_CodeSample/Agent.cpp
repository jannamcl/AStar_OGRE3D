#include "Agent.h"

Agent::Agent(std::string name, std::string filename, float height, float angleOffset) 
{
	using namespace Ogre;

	if (sceneMgr == NULL)
		return;

	this->height = height;
	this->angleOffset = angleOffset;
	mBodyNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * height);
	mBodyEntity = sceneMgr->createEntity(name, filename);
	mBodyNode->attachObject(mBodyEntity);

}
Ogre::Vector3
Agent::getPosition()
{
	Ogre::Vector3 position = Ogre::Vector3(this->x, this->y, this->z);
	return position;
}

void 
Agent::setPosition(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->mBodyNode->setPosition(this->x, this->y + height, this->z);
}

void 
Agent::setBaseAnimation(AnimID id, bool reset)
{
}
	
void Agent::setTopAnimation(AnimID id, bool reset)
{
}

bool 
Agent::nextLocation()
{
	if (mWalkList.empty())
		return false;

	mDestination = mWalkList.front();	// get the first element
	mWalkList.pop_front();				// remove the first element
	mDirection = mDestination - this->mBodyNode->getPosition();
	mDistance = mDirection.normalise();
	
	return true;
}

void Agent::discoverAnims()
{
	using namespace Ogre;
	std::map<std::string, Ogre::AnimationState*> mAnims;
	AnimationStateSet* aSet = mBodyEntity->getAllAnimationStates();
	AnimationStateIterator iter = mBodyEntity->getAllAnimationStates()->getAnimationStateIterator();
	while (iter.hasMoreElements()) // setup each animation
	{
		AnimationState *a = iter.getNext();
		mAnims[a->getAnimationName()] = a;
		a->setLoop(true);
		//mFadingIn[a->getAnimationName()] = false;
		//mFadingOut[a->getAnimationName()] = false;
	}
}