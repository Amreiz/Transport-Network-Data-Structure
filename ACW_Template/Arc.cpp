#include "Arc.h"
#include "Node.h"
#include <vector>

// Constructors
Arc::Arc()
{
	mDestinationNode = NULL;
	mDistance = NULL;
	mEnd = NULL;
	mStartNode = NULL;
	mStart = NULL;
}

Arc::Arc(int const pStart, int const pEnd, string &pMode) : mStart(pStart), mEnd(pEnd), mDistance(), mMode(pMode), mDestinationNode(),  mStartNode()
{

}
// Destructor 
Arc::~Arc()
{

}

//Getters
int Arc::getStart() const
{
	return mStart;
}

int Arc::getEnd() const
{
	return mEnd;
}

double Arc::getDistance() const
{
	return mDistance;
}

const string& Arc::getMode() const
{
	return mMode;
}

Node *Arc::getDestinationNode() const
{
	return mDestinationNode;
}

Node * Arc::getStartNode() const
{
	return mStartNode;
}

// Setters
void Arc::setDestination(Node * const pDestination)
{
	mDestinationNode = pDestination;
}

void Arc::setDistance(const double distance)
{
	mDistance = distance;
}
