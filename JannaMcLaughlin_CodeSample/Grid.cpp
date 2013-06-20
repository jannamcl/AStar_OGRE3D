#include "Grid.h"


/*THE GRIDNODE CLASS*/

// default constructor
GridNode::GridNode(){}

// create a node
GridNode::GridNode(int nID, int x, int y, bool isC)
{
	this->nodeID = nID;
	this->xCoord = x;
	this->yCoord = y;
	this->clear = isC;
	
	//contains = ?????????????
}

// destroy a node
GridNode::~GridNode()
{
}

// set the node id
void GridNode::setID(int id)
{
	this->nodeID = id;
}

// set the x coordinate
void GridNode::setX(int x)
{
	this->xCoord = x;
}

	// set the y coordinate
void GridNode::setY(int y)
{
	this->yCoord = y;
}

// get the x and y coordinate of the node
int GridNode::getX()
{
	return this->xCoord;
}

int GridNode::getY()
{
	return this->yCoord;
}

// return the position of this node
Ogre::Vector3 GridNode::getPosition()
{
	Ogre::Vector3 nPosition = Ogre::Vector3(xCoord,yCoord,0);//assumes the Plane is at zero
	return nPosition;
}

// set the node as walkable
void GridNode::setClear()
{
	clear = true;
}

// set the node as occupied
void GridNode::setOccupied()
{
	clear = false;
}

// is the node walkable
bool GridNode::isClear()
{
	return clear;
}


/*THE GRID CLASS*/

// create a grid
Grid::Grid(int numRows, int numCols, int nodeHeight, int nodeDiagonal, int x, int y)
{
		data.resize(numRows, GridRow(numCols));
		//size is now numRows
		//value is now GridRow
		//soo.. data is full of GridRows (if numRows was 4, then there'd be 4 GridRows)

		this->height = nodeHeight;
		this->diag = nodeDiagonal;

		this->rows = numRows; 
		this->cols = numCols; 

		int nodeID; //numRows*numCols*0;

		int defaultX = x;

		for(int r = 0; r < rows; r++)
		{
			nodeID = cols * r;

				for(int c = 0; c < cols; c++)
				{
				
					data[r].data[c] = new GridNode(nodeID, r, c, true);
					data[r].data[c]->setX(x);
					data[r].data[c]->setY(y);

					nodeID++;
					x+= 10;
				}
				x = defaultX;
				y += 10;
		}
}

// destroy a grid
Grid::~Grid()
	{
	}

	// get the node specified 
GridNode* Grid::getNode(int r, int c)
	{
		if(r< 0 || r>= data.size())
			return NULL;
			if(c<0 || c>= data[r].data.size())
				return NULL;
		return data[r].data[c];
	}

GridNode* Grid::getNode(int nodeID)
{
	if(nodeID < 0)
		return NULL;
	for(int r = 0; r < rows; r++)
         {
            for(int c = 0; c < cols; c++)
            {
				if(nodeID <= getNode(r,c)->getID())
					return getNode(r,c);
			}
	}
}

	// get the height and width of the nodes	
int Grid::getHeight()
	{
		return height;
	}

	// get the diagonal of the nodes
int Grid::getDiagonal()
	{
		return diag;
	}

	// set the height of the nodes
void Grid::setHeight(int h)
	{
		height = h;
	}

	// set the Diagonal of the nodes
void Grid::setDiagonal(int d)
	{
		diag = d;
	}

	// get adjacent nodes;
	GridNode* Grid::getNorthNode(GridNode* n)
	{
		int tempID = n->getID();
		tempID -= cols;
		if(getNode(tempID) == NULL)
			return NULL;
		else
			return getNode(tempID);
	}
	GridNode* Grid::getSouthNode(GridNode* n)
	{
		int tempID = n->getID();
		tempID += cols;
		if(getNode(tempID) == NULL)
			return NULL;
		else
			return getNode(tempID);
	}
	GridNode* Grid::getEastNode(GridNode* n)
	{
		int tempID = n->getID();
		tempID += 1;
		if(getNode(tempID) == NULL)
			return NULL;
		else
			return getNode(tempID);
	}
	GridNode* Grid::getWestNode(GridNode* n)
		{
		int tempID = n->getID();
		tempID -= 1;
		if(getNode(tempID) == NULL)
			return NULL;
		else
			return getNode(tempID);
	}
	GridNode* Grid::getNENode(GridNode* n)
	{
		GridNode* temp = getNorthNode(n);
		if(temp == NULL) return NULL;
		int tempID	= temp->getID() + 1;
			return getNode(tempID);
	}
	GridNode* Grid::getNWNode(GridNode* n)
	{
		GridNode* temp = getNorthNode(n);
		if(temp == NULL) return NULL;
		int tempID	= temp->getID() - 1;
			return getNode(tempID);
	}
	GridNode* Grid::getSENode(GridNode* n)
	{
		GridNode* temp = getSouthNode(n);
		if(temp == NULL) return NULL;
		int tempID = temp->getID() + 1;
		if(tempID % cols == 0)
			return NULL;
		return getNode(tempID);
	}
	GridNode* Grid::getSWNode(GridNode* n)
	{
		GridNode* temp = getSouthNode(n);
		if(temp == NULL) return NULL;
		if(temp->getID() % cols == 0)
			return NULL;
		int tempID = temp->getID() - 1;
			return getNode(tempID);
	}

	//get distance between two nodes
	int Grid::getDistance(GridNode* node1, GridNode* node2)
	{
        int x1 = node1->getX();
        int y1 = node1->getY();
        int x2 = node2->getX();
        int y2 = node2->getY();

		int xs = x1 - x2;
		int ys = y1 - y2;
		
		if(xs < 0)
			xs = -xs;
		if(ys < 0)
			ys = -ys;

        return xs + ys;
	}

	// height or diagonal?...assumes node1 and node2 are neighbors
	int Grid::getGcost(GridNode* node1, GridNode* node2)
	{
		
		if(node1->getID() != node2->getID())
			
			if(node1->getID() == node2->getID()-cols || node2->getID() == node1->getID()-cols)
				return height;
			else if(node1->getID() == node2->getID()-1 || node2->getID() == node1->getID()-1)
				return height;
			else
				return diag;
		else //node1 = node2
			return 0;
	}

	// Print a grid to a file.  Good for debugging
	void Grid::printToFile()
	{
		std::ofstream outFile;
		outFile.open("Grid.txt");
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				outFile << this->getNode(i, j)->contains << " ";
			}
			outFile << std::endl;
		}
		outFile.close();
	}