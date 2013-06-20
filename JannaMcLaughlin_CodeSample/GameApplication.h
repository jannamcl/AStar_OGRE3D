#ifndef __GameApplication_h_
#define __GameApplication_h_

#include "BaseApplication.h"
#include "Agent.h"
#include "Sinbad.h"
#include "Penguin.h"
#include "Robot.h"
#include <list>

class Agent;
class Sinbad;

class GameApplication : public BaseApplication
{
	
private:

	std::list<Agent *> agentList;
	std::list<Agent*>::iterator iter;

	void loadEnv();			// Load the buildings or ground plane, etc
	void setupEnv();		// Set up lights, shadows, etc	
	void loadObjects();		// Load other propers or objects (e.g. furniture)
	void loadCharacters();	// Load actors, agents or characters
	
	void addTime(Ogre::Real deltaTime);
public:
    GameApplication(void);
    virtual ~GameApplication(void);

protected:
    virtual void createScene(void);
};
// #ifndef __GameApplication_h_
#endif 
