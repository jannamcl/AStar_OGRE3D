#ifndef A_STAR_H
#define A_STAR_H  // WHAT HAPPENS IF I GET RID OF THIS?
#include "Grid.h"
#include <list>
#include <map>

class GridNode; // forward declarations
class Grid;

class Anode{ //hold the values for this run of the algorithm
private:
	GridNode* node;	// point back to the original node in the grid
	int Fvalue;		// values for our A* algorithm
	int Gvalue;
	int Hvalue;
	Anode* parent;  // used for A* to point to the previous node
	int AnodeID;
public:
	Anode();
	~Anode(){};
	Anode(GridNode* n, int f = 0, int g = 0, int h = 0, Anode* p = NULL);
	GridNode* getGridNode() {return node;};
	void setFvalue(int f);	// set the A* values
	void setGvalue(int g);
	void setHvalue(int h);
	void setParent(Anode* p){parent = p;};
	Anode* getParent(){return parent;};
	int getFvalue();		// get the A* values
	int getGvalue();
	int getHvalue();
	void setAnodeID(int id);
	int getAnodeID();
	//Anode* getAnode(int id);
};

typedef std::map<int, Anode*> OpenListType;		// using a map to hold the open list

class Astar {
private:
	Grid* grid;				// the environment where the start takes place
	GridNode* start;		// where we're starting
	GridNode* goal;			// where we're trying to reach
	std::map<int, Anode*> openList;		// open list: map of nodes that still need to be examined. int is the node ID
	std::list<Anode*> closedList;	// close list: list of nodes that have already been processed
	// Note: GridNodes contain a pointer to the "parent" node in the path
	std::list<Anode*> path;			// path: holds the final list of nodes that are the path YOU CAN CHANGE THIS TO LIST AND MAKE RUNTIME FASTER
	std::map<int, Anode*>::iterator iter;
	std::list<Anode*>::iterator it;
public:
	Astar();
	~Astar();							// delete any memory allocated
	Astar(Grid* g, GridNode* startNode, GridNode* goalNode);
	void setGrid(Grid* g);				// set the grid to use
	void setStartNode(GridNode* node);	// set starting node
	void setGoalNode(GridNode* node);	// set goal node
	void addNodesToOpen(Anode* node);	// add this node's neighbors to the open list if they are walkable and not already there.  Make parent point to currentNode
	void moveNodeToClosed(Anode* node); // put the node on the closed list
	Anode* getNextNode();				// get the next node that should be considered
	bool findPath();					// run the A* algorithm on the grid and store the result in path
	Anode* getPathNode(int which);		// get the path node by node starting at 0
	std::list<GridNode*> Astar::getPath(int which);

};


#endif