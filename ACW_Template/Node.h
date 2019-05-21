#pragma once

#include <string>
#include <vector>

using namespace std;

class Arc;

class Node
{

public:
	// Constructors and Destructor
	Node();
	Node(string &pName, int pRefNum, double pLatitude, double pLongitude);
	~Node();

	// Getters 
	const string& getName() const;
	int getRefNum() const;
	double getLatitude() const;
	double getLongitude() const;

	// Setters
	void setArcs(const vector<Arc*> &arcs);

private:
	// Member variables
	vector<Arc*> mArcs;
	string mName;
	int mRefNum;
	double mLatitude;
	double mLongitude;
};