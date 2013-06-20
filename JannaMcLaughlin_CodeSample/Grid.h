////////////////////////////////////////////////////////
// Class to hold the grid layout of our environment
// Used for navigation and AI, not graphics

#ifndef GRID_H
#define GRID_H
#include <iostream>
#include <vector>
#include <assert.h>

#include "GameApplication.h"

class GridNode;
class Grid;

class GridNode {
private:
	int nodeID;			// identify for the node
	int xCoord;			// x coordinate
	int yCoord;			// y coordinate
	bool clear;			// is the node walkable?
	
public:
	char contains;		// For printing... B = blocked, S = start, G = goal, numbers = path
	GridNode();			// default constructor
	GridNode(int nID, int x, int y, bool isC = true);	// create a node
	~GridNode();		// destroy a node

	void setID(int id);		// set the node id
	int getID(){return nodeID;};			// get the node ID
	void setX(int x);		// set the x coordinate
	void setY(int y);		// set the y coordinate
	int getX();				// get the x and y coordinate of the node
	int getY();
	Ogre::Vector3 getPosition();	// return the position of this node
	void setClear();		// set the node as walkable
	void setOccupied();		// set the node as occupied
	bool isClear();			// is the node walkable
};

class GridRow {  // helper class
public:
	std::vector<GridNode*> data;
	GridRow(int size) {data.resize(size);};
	~GridRow(){};
};

class Grid {
private:
	std::vector<GridRow> data;  // actually hold the grid data
	int rows;					// number of rows
	int cols;					// number of columns
	int height;					// height and width of each cell (10 for our A* example)
	int diag;					// length of the diagonal of each cell (14 for our A* example)
public:
	Grid(int numRows, int numCols, int nodeHeight = 10, int nodeDiagonal = 14, int x = 0, int y = 0);	// create a grid
	~Grid();					// destroy a grid

	GridNode* getNode(int r, int c);  // get the node specified given row and column
	GridNode* getNode(int nodeID);	  // get the node specified given its ID
	int getHeight();				  // get the height and width of the nodes	
	int getDiagonal();				  // get the diagonal of the nodes
	void setHeight(int h);			  // set the height of the nodes
	void setDiagonal(int d);		  // set the Diagonal of the nodes

	GridNode* getNorthNode(GridNode* n);		  // get adjacent nodes;
	GridNode* getSouthNode(GridNode* n);
	GridNode* getEastNode(GridNode* n);
	GridNode* getWestNode(GridNode* n);
	GridNode* getNENode(GridNode* n);
	GridNode* getNWNode(GridNode* n);
	GridNode* getSENode(GridNode* n);
	GridNode* getSWNode(GridNode* n);

	int getDistance(GridNode* node1, GridNode* node2);  //get distance between between two nodes
	int getGcost(GridNode* node1, GridNode* node2);  // height or diagonal?

	void printToFile();				// Print a grid to a file.  Good for debugging
};

#endif