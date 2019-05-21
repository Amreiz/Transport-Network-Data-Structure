#pragma once

#include "Node.h"
#include "Arc.h"

char c[255];

Node::Node()
{
	mLatitude = NULL;
	mLongitude = NULL;
	mRefNum = NULL;
}

Node::Node(string &pName, const int pRefNumber, const double pLatitude, const double pLongitude) : mName(pName), mRefNum(pRefNumber), mLatitude(pLatitude), mLongitude(pLongitude)
{

}

Node::~Node()
{
	
}

const string& Node::getName() const
{
	return mName;
}

int Node::getRefNum() const
{
	return mRefNum;
}

double Node::getLatitude() const
{
	return mLatitude;
}

double Node::getLongitude() const
{
	return mLongitude;
}

void Node::setArcs(const vector<Arc*>& arcs)
{
	mArcs = arcs;
}

