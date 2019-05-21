#pragma once

#include <string>
#include <vector>

using namespace std;

class Node;

class Arc
{

public:
	// Constructors
	Arc(const Arc &);
	Arc(int pStart, int pEnd, string &pMode);
	Arc& operator = (const Arc &);
	Arc();

	~Arc();
	
	// Getters
	int getStart() const;
	int getEnd() const;
	double getDistance() const;
	const string& getMode() const;
	Node* getDestinationNode() const;
	Node* getStartNode() const;

	// Setters
	void setDestination(Node* const pDestination);
	void setDistance(const double distance);

private:
	// Member variables
	int mStart;
	int mEnd;
	double mDistance;
	string mMode;
	Node* mDestinationNode;
	Node* mStartNode;
};