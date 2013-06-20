#include "Penguin.h"

Penguin::Penguin(std::string name, std::string filename, float height, float angleOffset) 
{
	using namespace Ogre;

	if (sceneMgr == NULL)
		return;

	this->height = height - 2;
	this->angleOffset = angleOffset;
	mBodyNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * height);
	mBodyEntity = sceneMgr->createEntity(name, filename);
	mBodyNode->attachObject(mBodyEntity);
	mBodyNode->setScale(0.14f, 0.14f, 0.14f);
	setupAnimations();
	



	mWalkSpeed = 25.0f;
	mDirection = Ogre::Vector3::ZERO;
}

void 
Penguin::setupAnimations()
{
	this->mTimer = 0;
	this->mVerticalVelocity = 0;

	// this is very important due to the nature of the exported animations
	mBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	Ogre::String animNames[] =
		{"amuse"};

	// populate our animation list
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		mAnims[i] = mBodyEntity->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	setBaseAnimation(ANIM_AMUSE);
	setTopAnimation(ANIM_AMUSE);

	// relax the hands since we're not holding anything
	//mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
}

void 
Penguin::setBaseAnimation(AnimID id, bool reset)
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
	
void Penguin::setTopAnimation(AnimID id, bool reset)
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
Penguin::updateAnimations(Ogre::Real deltaTime)
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
Penguin::fadeAnimations(Ogre::Real deltaTime)
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
Penguin::updateLocomote(Ogre::Real deltaTime)
{
	
	if (mDirection == Ogre::Vector3::ZERO)
	{
		if (this->nextLocation()) // a check to see if there is another destination
		{
			// Set walking animation
			this->setBaseAnimation(ANIM_AMUSE, true);
			this->setTopAnimation(ANIM_AMUSE, true);
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
				this->setBaseAnimation(ANIM_AMUSE, true);
				this->setTopAnimation(ANIM_AMUSE, true);
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
Penguin::update(Ogre::Real deltaTime)
{
	this->updateLocomote(deltaTime);
	this->updateAnimations(deltaTime);
}

void 
Penguin::moveTo(GridNode* node)						// Use A* to move to this node from your current spot
{
	mWalkList.push_back(Ogre::Vector3(node->getX(), this->height, node->getY()));
}
