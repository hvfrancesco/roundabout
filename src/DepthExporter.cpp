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

void DepthExporter::exportComputo(string filename, vector <ofPoint> giaroni, float areaScavo, float spessoreScavo, float disty, float raggioExt, float raggioInt, float lunghezzaTotale) {


    float nSteli = giaroni.size();
    float volumeScavo = (areaScavo*spessoreScavo);
    float volumeFondaz = nSteli*0.15*0.20*disty;
    float volumeStabiliz = areaScavo*0.1;
    float volumeTerra = volumeScavo-volumeFondaz-volumeStabiliz;
    float ferroFondaz = nSteli*disty*4*0.395;

    float costo = 0.0;
    costo += (lunghezzaTotale * 0.222 * 1.7); // steli
    costo += (float)(nSteli/1000)*22.00; // ciottoli su steli
    costo += (((raggioInt*raggioInt*PI)/(0.1*0.1))/1000)*22.00; // ciottoli al centro
    costo += volumeScavo * 15.86; // scavo sbancamento
    costo += volumeStabiliz * 31.65; // stabilizzato
    costo += volumeFondaz*180.00; // cls fondaz
    costo += ferroFondaz*1.7; // ferro fondazioni
    costo += volumeTerra*36.67; // terra riporto
    costo += nSteli*0.5; // manodopera sassi

    float pesoFerro = lunghezzaTotale * 0.222;
    float volumeCiottoli = (float)(nSteli/1000) + (((raggioInt*raggioInt*PI)/(0.1*0.1))/1000);

    ofstream obj;
	obj.open(ofToDataPath(filename).c_str(), ios::out);
	if (obj.is_open()) {
    obj << "Rotonda Raucedo - calcolo costi" << endl << endl;
    obj << "Dati Generali:" << endl;
    obj << "- Raggio Esterno " << raggioExt << " m" << endl;
    obj << "- Raggio Interno " << raggioInt << " m" << endl;
    obj << "- Numero elementi " << nSteli << endl << endl;
    obj << "Stima dei costi:" << endl;
    obj << "- fornitura, lavorazione e posa steli in acciaio " << lunghezzaTotale << " ml x 0,222 kg/ml = " << pesoFerro << " kg - costo: " << pesoFerro * 1.7 << " euro" << endl;
    obj << "- fornitura ciottoli di fiume " << volumeCiottoli << " mc - costo: " << volumeCiottoli * 22.00 << " euro" << endl;
    obj << "- esecuzione scavo di sbancamento " << volumeScavo << " mc - costo: " << volumeScavo * 15.86 << " euro" << endl;
    obj << "- fornitura e posa di stabilizzato " << volumeStabiliz << " mc - costo: " << volumeStabiliz * 31.65 << " euro" << endl;
    obj << "- fornitura cls per fondazioni, compresi casseri e lavorazioni " << volumeFondaz << " mc - costo: " << volumeFondaz*180.00 << " euro" << endl;
    obj << "- acciaio per fondazioni " << ferroFondaz << " kg - costo: " << ferroFondaz*1.7 << " euro" << endl;
    obj << "- fornitura e posa terra di riporto e riempimenti " << volumeTerra << " mc - costo: " << volumeTerra*36.67 << " euro" << endl;
    obj << "- manodopera aggiuntiva - costo a corpo " << nSteli*0.5 << " euro" << endl << endl;
    obj << "----------------------------------------------" << endl;
    obj << "COSTO TOTALE: " << costo << " euro" << endl;

	}


}

