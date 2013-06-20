#include "Robot.h"

Robot::Robot(std::string name, std::string filename, float height, float angleOffset) 
{
	using namespace Ogre;

	if (sceneMgr == NULL)
		return;
	
	this->height = height - 5;
	this->angleOffset = angleOffset - 90;
	mBodyNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * height);
	mBodyEntity = sceneMgr->createEntity(name, filename);
	mBodyNode->attachObject(mBodyEntity);
	mBodyNode->setScale(0.14f, 0.14f, 0.14f);
	setupAnimations();
	



	mWalkSpeed = 15.0f;
	mDirection = Ogre::Vector3::ZERO;
}

void 
Robot::setupAnimations()
{
	this->mTimer = 0;
	this->mVerticalVelocity = 0;

	// this is very important due to the nature of the exported animations
	mBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	Ogre::String animNames[] =
		{"Die","Idle","Shoot","Slump","Walk"};

	// populate our animation list
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		mAnims[i] = mBodyEntity->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	setBaseAnimation(ANIM_IDLE);
	setTopAnimation(ANIM_IDLE);

	// relax the hands since we're not holding anything
	//mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
}

void 
Robot::setBaseAnimation(AnimID id, bool reset)
{
	if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mBaseAnimID] = false;
		mFadingOut[mBaseAnimID] = true;
	}

	mBaseAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}
	
void Robot::setTopAnimation(AnimID id, bool reset)
{
	if (mTopAnimID >= 0 && mTopAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mTopAnimID] = false;
		mFadingOut[mTopAnimID] = true;
	}

	mTopAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}

void 
Robot::updateAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	Real baseAnimSpeed = 1;
	Real topAnimSpeed = 1;

	mTimer += deltaTime;

	// increment the current base and top animation times
	if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);
	if (mTopAnimID != ANIM_NONE) mAnims[mTopAnimID]->addTime(deltaTime * topAnimSpeed);

	// apply smooth transitioning between our animations
	fadeAnimations(deltaTime);
}

void 
Robot::fadeAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	for (int i = 0; i < NUM_ANIMS; i++)
	{
		if (mFadingIn[i])
		{
			// slowly fade this animation in until it has full weight
			Real newWeight = mAnims[i]->getWeight() + deltaTime * 7.5f; //ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight >= 1) mFadingIn[i] = false;
		}
		else if (mFadingOut[i])
		{
			// slowly fade this animation out until it has no weight, and then disable it
			Real newWeight = mAnims[i]->getWeight() - deltaTime * 7.5f; //ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				mAnims[i]->setEnabled(false);
				mFadingOut[i] = false;
			}
		}
	}
}

void 
Robot::updateLocomote(Ogre::Real deltaTime)
{
	
	if (mDirection == Ogre::Vector3::ZERO)
	{
		if (this->nextLocation()) // a check to see if there is another destination
		{
			// Set walking animation
			this->setBaseAnimation(ANIM_WALK, true);
			this->setTopAnimation(ANIM_WALK, true);
		}
	}
	else
	{
		Ogre::Real move = mWalkSpeed * deltaTime;
		mDistance -= move;

		if (mDistance <= 0.0f) // over shooting target
		{
			this->mBodyNode->setPosition(mDestination);
			mDirection = Ogre::Vector3::ZERO;
			 if (!this->nextLocation()) // a check to see if there is another destination
			 {
				this->setBaseAnimation(ANIM_IDLE, true);
				this->setTopAnimation(ANIM_IDLE, true);
			 }
			 else
			 {
				 Ogre::Vector3 src = this->mBodyNode->getOrientation() * Ogre::Vector3::UNIT_X;
				 if ((1.0f + src.dotProduct(mDirection)) < 0.0001f)
				 {
					 this->mBodyNode->yaw(Ogre::Degree(180));
				 }
				 else
				 {
					Ogre::Quaternion quat = src.getRotationTo(mDirection);
					this->mBodyNode->rotate(quat);
					this->mBodyNode->yaw(Ogre::Degree(angleOffset));  // To correct for models facing different directions
				}
			 }
		}
		else
		{
			this->mBodyNode->translate(mDirection * move);
		}
	}
	
}

void 
Robot::update(Ogre::Real deltaTime)
{
	this->updateLocomote(deltaTime);
	this->updateAnimations(deltaTime);
}

void 
Robot::moveTo(GridNode* node)						// Use A* to move to this node from your current spot
{
	mWalkList.push_back(Ogre::Vector3(node->getX(), this->height, node->getY()));
}
