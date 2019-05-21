#include "Navigation.h"
#include "Arc.h"
#include "Node.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Vectors
vector<Arc*> arcs;
vector<Arc*> arcsTemp;
vector<Arc*> visitedArcs;
vector<Arc*> getNextNode;
vector<Node*> nodes;
vector<Node*> allNonVisitedNodes;
vector<Node*> allVisitedNodes;
vector<Node*> nextNodes;
vector<string> stringsToSplit;
vector<double> arcDistances;
vector<double>tempDistances;

// Pointers
Node* startNode = new Node;
Node* endNode = new Node;
Node* previousNode = new Node;
Node* currentNode = new Node;
Node* startingPoint = new Node();
Node* destinationPoint = new Node();
Arc* currentArc = new Arc;

// Variables
bool visitedStartNode;
bool isLink;
bool isDiff;
bool onPath;
bool pointToStart;
bool pointToDestination;

// Converts latitude/longitude into eastings/northings
extern void LLtoUTM(const double Lat, const double Long, double &UTMNorthing, double &UTMEasting);

Navigation::Navigation() : _outFile("Output.txt")
{
}

Navigation::~Navigation()
{
}

// Finds the linear distance between to coordinates
// PARAMS - Takes in 2 sets of northings and eastings

double Navigation::pythagoreanThereom(const double x1, const double y1, const double x2, const double y2)const	
{
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));;
}

// Checks if the mode of transport is feasible between links from the arcs
// PARAMS: - Takes in a mode of transport

vector<string> CheckMode(const string &mode)
{
	vector<string>checkRoute;	// Allowed routes will be pushed into here

	if (mode == "Ship" || mode == "Rail")	// if mode is allowed
	{
		checkRoute.push_back(mode);	// push into allowed routes vector
	}

	else if (mode == "Bus")
	{
		checkRoute.push_back(mode);
		checkRoute.push_back("Ship");
	}
	else if (mode == "Car")
	{
		checkRoute.push_back(mode);
		checkRoute.push_back("Bus");
		checkRoute.push_back("Ship");
	}
	else if (mode == "Bike")
	{
		checkRoute.push_back(mode);
		checkRoute.push_back("Car");
		checkRoute.push_back("Bus");
		checkRoute.push_back("Rail");
		checkRoute.push_back("Ship");
	}
	else if (mode == "Foot")
	{
		checkRoute.push_back(mode);
		checkRoute.push_back("Bike");
		checkRoute.push_back("Car");
		checkRoute.push_back("Rail");
		checkRoute.push_back("Bus");
		checkRoute.push_back("Ship");
	}

	return checkRoute;
}


bool Navigation::ProcessCommand(const string& commandString)
{
	istringstream inString(commandString);

	// Variables to be used in process command

	int startRef;
	int endRef;
	const int nodeRef = NULL;
	int refNumber1;
	int refNumber2;
	const int findStartRef = NULL;
	const int findEndRef = NULL;

	double northings1;
	double northings2;
	double eastings1;
	double eastings2;
	double finalDistance;
	double currentDistance = 0;
	const double getDistance = NULL;

	string place1, place2, command;
	string const findMode;
	int count = 0;

	char line[255];

	inString >> command;

	if (command == "MaxDist")
	{
		for (int i = 0; i != nodes.size(); i++)		// run through vector of nodes
		{
			LLtoUTM(nodes[i]->getLatitude(), nodes[i]->getLongitude(), northings1, eastings1);		// get first set of coordinates
			place1 = nodes[i]->getName();		// get the name of the first set of coordinates

			for (int j = 0; j != nodes.size(); j++)		// run through vector of nodes again
			{
				LLtoUTM(nodes[j]->getLatitude(), nodes[j]->getLongitude(), northings2, eastings2);		// get second set of coordinates
			}

			finalDistance = pythagoreanThereom(northings1, eastings1, northings2, eastings2) / 1000;		// calculate linear distance between both set of coordinates

			if (finalDistance > currentDistance)		// if current iteration of calculated distance is larger than previous iteration
			{
				place2 = nodes[i]->getName();			// get the name that correlates to the end distance
				currentDistance = finalDistance;		// set new current distance to have largest number
			}
		}

		// output data of command into the appropriate format specified in the text file
		_outFile << command << endl << place2 << "," << place1 << "," << fixed << showpoint << setprecision(3) << currentDistance << endl << endl;

		return true;
	}

	else if (command == "MaxLink")
	{
		for (int i = 0; i != arcs.size(); i++)		// run through vector of arcs
		{
			endRef = arcs[i]->getEnd();
			startRef = arcs[i]->getStart();

			for (int j = 0; j != nodes.size(); j++)		// run through vector of nodes
			{
				if (arcs[i]->getStart() == nodes[j]->getRefNum())	// if starting referance number from the arcs is equal to current iteration of node referance number
				{
					LLtoUTM(nodes[j]->getLatitude(), nodes[j]->getLongitude(), northings1, eastings1);	// get first set of coordinates

					for (int k = 0; k != nodes.size(); k++)		// run through vector of nodes again
					{
						if (arcs[i]->getEnd() == nodes[k]->getRefNum())		// if arcs end referance is equal to the nodes start ref num
						{
							LLtoUTM(nodes[k]->getLatitude(), nodes[k]->getLongitude(), northings2, eastings2);	// get second set of coordinates

							finalDistance = pythagoreanThereom(northings1, eastings1, northings2, eastings2) / 1000;	// calculate linear distance between both set of coordinates

							arcDistances.push_back(finalDistance);		// save distances into vector used later in other commands
							arcs[i]->setDistance(finalDistance);	

							if (finalDistance > currentDistance)	// if current iteration of calculated distance is larger than previous iteration
							{
								currentDistance = finalDistance;	// set new current distance to have largest number
								startRef = arcs[i]->getStart();		// get start ref of the largest number
								endRef = arcs[i]->getEnd();			// get end ref of the arc
							}
						}
					}
				}
			}
		}

		// output data of command into the appropriate format specified in the text file
		_outFile << command << endl << startRef << "," << endRef << "," << fixed << showpoint << setprecision(3) << currentDistance << endl << endl;

		return true;
	}

	else if (command == "FindDist")
	{
		inString >> refNumber1;
		inString >> refNumber2;

		for (int i = 0; i < nodes.size(); i++)	// run through vector of nodes
		{
				if (refNumber1 == nodes[i]->getRefNum())	// if first command is equal to current instance of nodes
				{
					startRef = nodes[i]->getRefNum();		// get the start referance
					LLtoUTM(nodes[i]->getLatitude(), nodes[i]->getLongitude(), northings1, eastings1);	// get first set of coordinates
					count++;	//increment
				}

				else if (refNumber2 == nodes[i]->getRefNum())	// if second command is equal to current instance of nodes
				{
					endRef = nodes[i]->getRefNum();
					LLtoUTM(nodes[i]->getLatitude(), nodes[i]->getLongitude(), northings2, eastings2);	// get second set of coordinates
					count++;	//increment
				}

				if (count == 2)		// once the counter has found two sets of coordinates
				{
					finalDistance = pythagoreanThereom(northings1, eastings1, northings2, eastings2) / 1000;	// calculate linear distance between both set of coordinates
					break;	// break out of loop
				}
		}

		// output data of command into the appropriate format specified in the text file
		_outFile << command << " " << refNumber1 << " " << refNumber2 << endl << startRef << "," << endRef << " " << fixed << showpoint << setprecision(3) << finalDistance << endl << endl;

		return true;
	}

	else if (command == "FindNeighbour")
	{
		inString >> refNumber1;

		_outFile << command << " " << refNumber1 << endl;	// output data of command into the appropriate format specified in the text file

		for (int i = 0; i != arcs.size(); i++)		// run through the arcs
		{
			if (arcs[i]->getStart() == refNumber1)
			{
				refNumber2 = arcs[i]->getEnd();
				_outFile << refNumber2 << endl;	// output data of command into the appropriate format specified in the text file
			}

			else if (arcs[i]->getEnd() == refNumber1)
			{
				refNumber2 = arcs[i]->getStart();
				_outFile << refNumber2 << endl;	// output data of command into the appropriate format specified in the text file
			}
		}

		_outFile << endl;

		return true;
	}

	else if (command == "Check")
	{
		string checkMode;
		vector<int> route;

		inString >> checkMode;
		_outFile << command << " " << checkMode << " ";	// output data of command into the appropriate format specified in the text file

		while (inString.good())
		{
			inString.getline(line, 255, ' ');
			istringstream stream(line);
			int numberOfRoutes;
			stream >> numberOfRoutes;
			route.push_back(numberOfRoutes);
		}

		for (int i = 1; i < route.size(); i++)
		{
			_outFile << route[i] << " ";	// output data of command into the appropriate format specified in the text file
		}

		_outFile << endl;	// new line

		for (int i = 1; i < route.size() - 1; i++)
		{
			for (int j = 0; j < arcs.size(); j++)
			{
				if (route[i] == arcs[j]->getStart() && (route[i + 1] == arcs[j]->getEnd()) || (route[i] == arcs[j]->getEnd() && route[i + 1] == arcs[j]->getStart()))
				{
					vector<string> validRoute = CheckMode(checkMode);

					if (find(validRoute.begin(), validRoute.end(), arcs[j]->getMode()) != validRoute.end())
					{
						_outFile << route[i] << "," << route[i + 1] << ",PASS" << endl;	// output data of command into the appropriate format specified in the text file
					}

					else 
					{
						_outFile << route[i] << "," << route[i + 1] << ",FAIL" << endl;	// output data of command into the appropriate format specified in the text file
					}
				}
			}

		}
		_outFile << endl;	// new line
		return true;
	}

	else if (command == "FindRoute")
	{
		return true;
	}

	else if (command == "FindShortestRoute")
	{

	}

	else 
	{
		return false;
	}
}

bool Navigation::BuildNetwork(const string &fileNamePlaces, const string &fileNameLinks) const
{
	fstream finPlaces(fileNamePlaces);
	fstream finLinks(fileNameLinks);

	if (finPlaces.fail() || finLinks.fail())
	{
		return false;
	}

	// Variables

	double bLatitude, bLongitude;
	int bStartRef, bEndRef, bRefNum;
	string place, startRefStr, endRefStr, modeStr, refNumStr, latitudeStr, longitudeStr, readLine;
	string const mode;
	Node *const destination = NULL;

	// splitting the nodes data set and setting it into a vector array

		while(getline(finLinks, readLine))		// while finLinks hasnt reached the end line
		{
			istringstream line(readLine);		// read line

			getline(line, startRefStr, ',');	// split startRef by comma
			getline(line, endRefStr, ',');		// split endRef from comma
			getline(line, modeStr, ',');		// split mode from comma

			bStartRef = stoi(startRefStr);		// convert from splitted string to startRef int
			bEndRef = stoi(endRefStr);			// convert from splitted string to endRef int

			Arc* const a = new Arc(bStartRef, bEndRef, modeStr);		// insantiate new object per line
			arcs.push_back(a);		// store lines into vector
		}

		while (getline(finPlaces, readLine))	// while finPlaces hasnt reached the end line
		{
			istringstream line(readLine);		// read line

			getline(line, place, ',');			// split place by comma
			getline(line, refNumStr, ',');		// split refNum by comma
			getline(line, latitudeStr, ',');	// split lattitude by comma
			getline(line, longitudeStr, ',');	// split longitude by comma

			bRefNum = stoi(refNumStr);			// convert from splitted string to nodeRef int
			bLatitude = stod(latitudeStr);		// convert from splitted string to latitude int
			bLongitude = stod(longitudeStr);	// convert from splitted string to longitude int

			Node *const n = new Node(place, bRefNum, bLatitude, bLongitude);		// instantiate new object per line

			for (int i = 0; i != arcs.size(); i++)		// run through stored arcs vector
			{
				if (bRefNum == arcs[i]->getEnd() || bRefNum == arcs[i]->getStart())		// if the node referance number is equal to the start or end referance from the arcs
				{
					Arc *links = new Arc;		// istantiate a new pointer
					links = arcs[i];		// point to current iteration from the arcs
					arcsTemp.push_back(links);		// push current point to a temporary vector
					n->setArcs(arcsTemp);		// set the arcs to point to the nodes
				}
			}

			arcsTemp.clear();
			nodes.push_back(n);		// store lines into vector
		}

		// bi - directional nodes

		for (int i = 0; i != arcs.size(); i++)		// run through arcs
		{
			for (int j = 0; j != nodes.size(); j++)		// run through the nodes
			{
				if (arcs[i]->getEnd() == nodes[j]->getRefNum())		// if the end referance is equal to the current iteration of the nodes referance number
				{
					                                                                                                                                              
					Node *getN = new Node;		// instantiate a new node pointer
					getN = nodes[j];		// set current node to point to node
					arcs[j]->setDestination(getN);		// nodes set and can traverse back and forth
				}
			}
		}
		return true;
}