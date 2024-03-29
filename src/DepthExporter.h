#pragma once

#include "ofMain.h"
#include <limits>
#include <fstream>

class DepthExporter {
public:
	static void exportSassi(string filename, vector <ofPoint> giaroni);
private:
	static string getExtension(string filename);

	static void exportPCDFloatVertex(ostream& obj, float x, float y, float z);
	static void exportCSVFloatVertex(ostream& obj, float x, float y, float z);
	static void exportPCDHeader(ostream& obj, int nPoints);
	static void exportPCDSassi(string filename, vector <ofPoint> giaroni);
	static void exportCSVSassi(string filename, vector <ofPoint> giaroni);
};
