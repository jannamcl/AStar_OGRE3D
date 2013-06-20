#include "aStar.h"

/*THE ANODE CLASS*/

Anode::Anode()
{
	node = NULL;	// point back to the original node in the grid
	Fvalue = NULL;		// values for our A* algorithm
	Gvalue = NULL;
	Hvalue = NULL;
	parent = NULL;  // used for A* to point to the previous node
}

Anode::Anode(GridNode* n, int f, int g, int h, Anode* p)
{
	node = n;	// point back to the original node in the grid
	setFvalue(f);		// values for our A* algorithm
	setGvalue(g);
	setHvalue(h);
	parent = p;  // used for A* to point to the previous node
}


// set the A* values
void Anode::setFvalue(int f)
{
	this->Fvalue = f;
}
void Anode::setGvalue(int g)
{
	this->Gvalue = g;
}
void Anode::setHvalue(int h)
{
	this->Hvalue = h;
}



// get the A* values
int Anode::getFvalue()
{
	return Fvalue;	
}
int Anode::getGvalue()
{
	return Gvalue;
}
int Anode::getHvalue()
{
	return Hvalue;
}

void Anode::setAnodeID(int id)
{
	this->AnodeID = id;
}
int Anode::getAnodeID()
{
	return AnodeID;
}

/*
Anode* Anode::getAnode(int id)
{
	Astar openList = Astar::openList;
	for(Astar::iter = openList.begin(); Astar::iter != Astar::openList.end(); Astar::iter++)
	{
		Anode* temp = iter->second;
		if(temp->getAnodeID() == id)
			return temp;
	}
	return NULL;
}
*/

/*THE ASTAR CLASS*/

Astar::Astar()
{
	this->grid = NULL;				// the environment where the start takes place
	this->start = NULL;		// where we're starting
	this->goal = NULL;			// where we're trying to reach
	//this->openList = NULL;		// open list: map of nodes that still need to be examined. int is the node ID
	//this->closedList = NULL;	// close list: list of nodes that have already been processed
	// Note: GridNodes contain a pointer to the "parent" node in the path
//	this->path = NULL;			// path: holds the final list of nodes that are the path
}

// delete any memory allocated
Astar::~Astar()
{
}
Astar::Astar(Grid* grid, GridNode* startNode, GridNode* goalNode)
{
	setGrid(grid);						// the environment where the start takes place
	setStartNode(startNode);		// where we're starting
	setGoalNode(goalNode);			// where we're trying to reach

	int g = grid->getGcost(startNode, startNode);
	int h = grid->getDistance(startNode, goalNode);
	int f = g + h;
	Anode* startAnode = new Anode(startNode, f, g, h, NULL);
	startAnode->setAnodeID(startNode->getID());
	
	//OpenListType = new OpenListType<startNode, goalNode>();
	openList[startAnode->getAnodeID()] = startAnode;
	addNodesToOpen(startAnode);

	// close list: list of nodes that have already been processed
	//this->closedList;	
	
	// Note: GridNodes contain a pointer to the "parent" node in the path
	//this->path;			// path: holds the final list of nodes that are the path ????????????????????
}

// set the grid to use
void Astar::setGrid(Grid* g)
{
	this->grid = g;
}

// set starting node
void Astar::setStartNode(GridNode* node)
{
	this->start = node;
}

// set goal node
void Astar::setGoalNode(GridNode* node)
{
	this->goal = node;
}

// add this node's neighbors to the open list if they are walkable 
//and not already there.  
//Make parent point to currentNode
void Astar::addNodesToOpen(Anode* node)
{
	this->moveNodeToClosed(node);
	int gridNodeID = node->getAnodeID();
	GridNode* gridNode = grid->getNode(gridNodeID);
	GridNode* temp = NULL;
	int cycleNeighbors = 0;
	Anode* tempAnode = NULL;
	
	while(cycleNeighbors < 8)
	{
		switch(cycleNeighbors){
		case 0 : temp = grid->getNWNode(gridNode); break;
		case 1 : temp = grid->getNorthNode(gridNode); break;
		case 2 : temp = grid->getNENode(gridNode); break;
		case 3 : temp = grid->getWestNode(gridNode); break;
		case 4 : temp = grid->getEastNode(gridNode); break;
		case 5 : temp = grid->getSWNode(gridNode); break;
		case 6 : temp = grid->getSouthNode(gridNode); break;
		case 7 : temp = grid->getSENode(gridNode); break;
		}
				 
				 
		
		/* openList[temp->getID()] // Anode pointer if it is on the map*/

		//CHECK TO SEE IF NODE IS ALREADY ON OPEN LIST
		if(temp == NULL)
		{
			cycleNeighbors++;
			continue;
		}
		if(openList.count(temp->getID()) == 0)
		{
			// node not already on the list	 
			if (temp->isClear())
			{
				//the node is clear
				int g = grid->getGcost(gridNode, temp);
				int h = grid->getDistance(temp, this->goal);
				int f = g + h;
				tempAnode = new Anode(temp, f, g, h, node);
				tempAnode->setAnodeID(temp->getID());
				tempAnode->setParent(node);
				openList.insert(std::pair<int, Anode*>(temp->getID(),tempAnode));
				//openList[tempAnode->getAnodeID()] = tempAnode;
			}
		}
		else
		{
			
			/*If an adjacent square is already on the open list, check to see if this path to that square is a better one. 
			In other words, check to see if the G score for that square is lower if we use the current square to get there. 
			If not, don’t do anything. 
			 On the other hand, if the G cost of the new path is lower, change the parent of the adjacent square to the selected square 
			Finally, recalculate both the F and G scores of that square.*/
			int nodeG = node->getGvalue();
			for(iter = openList.begin(); iter != openList.end(); iter++)
			{
				if(iter->second->getAnodeID() == temp->getID())
					tempAnode = iter->second;
			}
			iter = openList.begin();
			int tempG = tempAnode->getGvalue();
	/*		if (tempG < nodeG)//nodeG < tempG
			{
				
				tempAnode->setParent(node);
				int g = grid->getGcost(grid->getNode(tempAnode->getAnodeID()), gridNode);
				int f = g + tempAnode->getHvalue();
				tempAnode->setFvalue(f);
				tempAnode->setGvalue(g);
				
			}*/

		}
		tempAnode = NULL;//safety check!

		cycleNeighbors++;
	}
}

// put the node on the closed list
void Astar::moveNodeToClosed(Anode* node)
{
	//remove node from openList
	std::map<int, Anode*>::iterator iter;
	iter = openList.find(node->getAnodeID());
	if (iter != openList.end())
	{
		openList.erase(iter);
	//put node on closedList
		closedList.push_back(node);
	}
}

// get the next node that should be considered according to lowest f
Anode* Astar::getNextNode()
{
	int tempF = -1; //"Null" for tempF because there should never be a negative value for f
	int f = -1; //"Null" for f because there should never be a negative value for the f
	int bestID;

	for(iter = openList.begin(); iter != openList.end(); iter++)
	{
		Anode* temp = iter->second;
		tempF = temp->getFvalue();
		if (f == -1)
		{
			f = tempF;
		}
		if(tempF < f)
		{
			f = tempF;
			bestID = temp->getAnodeID();
		}
	}
	iter = openList.begin();
	return openList[bestID];
}

// run the A* algorithm on the grid and store the result in path
bool Astar::findPath()
{

	//do path.push back stuff after getting to the goal node
	//when you get to goal node, current node will be goal node
	
//check for isEmpty

/*
	for(iter = openList.begin(); iter != openList.end(); iter++)
	{
		Anode* nextAnode = getNextNode();
		//GridNode* next = grid->getNode(nextAnode->getAnodeID());
		
		this->path.push_back(nextAnode);

		if(nextAnode->getGridNode() == this->goal)
		{
			return true;
		}
		
		addNodesToOpen(nextAnode);
		moveNodeToClosed(nextAnode);
	}*/


	
	for(iter = openList.begin(); iter != openList.end(); iter++)
	{
	Anode* nextAnode = getNextNode();
	moveNodeToClosed(nextAnode);
	addNodesToOpen(nextAnode);

	
		if(nextAnode->getGridNode() == this->goal)
		{
			while(nextAnode->getGridNode() != this->start)
			{
				this->path.push_front(nextAnode);
				nextAnode = nextAnode->getParent();
			}
			return true;
		}
	}


	return false;
}

// get the path node by node starting at 0
Anode* Astar::getPathNode(int which)
{
	/*
	GridNode* retNode = this->start;
	Anode* a = closedList.back();
	for(int i = 0; i < which; i++)
	{
		a = a->getParent();
		retNode = grid->getNode(a->getAnodeID());
	}
		return retNode;
		*/

	/*
	
	int count = 0;
	std::list<Anode*>::iterator it;
	for(it = path.end(); it != path.begin(); it--)
	{
		if(count == which)
		{
			Anode* a = *it;
			return a;
			//return temp;
		}
			count++;
	}
		return NULL;
		*/

	//POP_BACK OR MAYBE FRONT...
	//HARDCODING NOW
	which = 0;
	
	return path.back();
}

// get the path as a list
std::list<GridNode*> Astar::getPath(int which)
{
	std::list<GridNode*> gridPath;
	
	Anode* nextNode = path.back();
	GridNode* nextGridNode = grid->getNode(nextNode->getAnodeID());

	while(nextNode->getParent() != NULL)
	{
		gridPath.push_front(nextGridNode);
		nextNode = nextNode->getParent();
		nextGridNode = grid->getNode(nextNode->getAnodeID());
	}
	//be sure to get the beginning node that doesn't have a parent
	//gridPath.push_front(nextGridNode);

	return gridPath;
}


