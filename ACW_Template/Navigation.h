#pragma once

#include <fstream>
#include <string>
#include <vector>

class Navigation
{
	std::ofstream _outFile;

public:
	//Constructor and destructor
	Navigation();
	~Navigation();

	// Prototypes
	bool BuildNetwork(const std::string& fileNamePlaces, const std::string& fileNameLinks)const;
	bool ProcessCommand(const std::string& commandString);
    double pythagoreanThereom(double x1, double y1, double x2, double y2)const;
	//void Check2(string checkTransportType, vector<int>vectorNode);
};