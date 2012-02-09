#include "DepthExporter.h"


string DepthExporter::getExtension(string filename) {
	int i = filename.rfind('.');
	if (i != string::npos) {
		return filename.substr(i + 1);
	} else {
		return "";
	}
}


void DepthExporter::exportSassi(string filename, vector <ofPoint> giaroni) {
	string extension = getExtension(filename);
	if (extension == "pcd") {
		exportPCDSassi(filename, giaroni);
	}
	else if (extension == "csv")
    {
        exportCSVSassi(filename, giaroni);
    }
}


inline void DepthExporter::exportCSVFloatVertex(ostream& obj, float x, float y, float z) {
	obj << x << " , " << y << " , " << z << endl;
}

inline void DepthExporter::exportPCDFloatVertex(ostream& obj, float x, float y, float z) {
	obj << x << " " << y << " " << z << endl;
}

inline void DepthExporter::exportPCDHeader(ostream& obj, int nPoints) {

    obj << "VERSION .7" << endl;
    obj <<"FIELDS x y z" << endl;
    obj <<"SIZE 4 4 4" << endl;
    obj << "TYPE F F F" << endl;
    obj << "COUNT 1 1 1" << endl;
    obj << "WIDTH " << nPoints << endl;
    obj << "HEIGHT 1" << endl;
    obj << "VIEWPOINT 0 0 0 1 0 0 0" << endl;
    obj << "POINTS " << nPoints << endl;
    obj << "DATA ascii" << endl;

}


void DepthExporter::exportPCDSassi(string filename, vector <ofPoint> giaroni) {
	ofstream obj;
	obj.open(ofToDataPath(filename).c_str(), ios::out);
	if (obj.is_open()) {

        exportPCDHeader(obj, giaroni.size());

		for (int j = 0; j < giaroni.size(); j++)
            {
                exportPCDFloatVertex(obj, giaroni[j].x, giaroni[j].y, giaroni[j].z);
		}
	}
}

void DepthExporter::exportCSVSassi(string filename, vector <ofPoint> giaroni) {
	ofstream obj;
	obj.open(ofToDataPath(filename).c_str(), ios::out);
	if (obj.is_open()) {

		for (int j = 0; j < giaroni.size(); j++)
            {
                exportCSVFloatVertex(obj, giaroni[j].x, giaroni[j].y, giaroni[j].z);
		}
	}
}



