#pragma once

#include "ofMain.h"
//#include "ofxSimpleGuiToo.h"
#include "ofxUI.h"
#include "DepthExporter.h"
#include <libnoise/noise.h>

using namespace noise;

class testApp : public ofBaseApp{

	public:

        ofxUICanvas *gui;
        void exit();
        void guiEvent(ofxUIEventArgs &e);

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void updateCloud();
		void esportaFile();
		float calcoloCosti();

		ofEasyCam cam; // add mouse controls for camera movement

		float raggioRotonda;
		float raggioExt;
		float raggioInt;
		float cameraDistance;
		float lunghezzaTotale;

        bool isChanged;

		// gui elements
        bool showGui;
        bool isSetup;
        bool showCircles;
        bool showBasePoints;
        bool showSassi;
        bool showSteli;
        bool showSassiCentro;

        DepthExporter esporta;

        ofMesh sassi;
        ofMesh basi;
        ofMesh sassiCentro;

        ofPoint centro;
        ofPoint origine;
        vector <ofPoint> cloud;
        vector <ofPoint> shadow;
        vector <ofPoint> cloudCentro;

        float baseh;
        float minh;
        float maxh;
        float distx;
        float disty;
        float raggioh;
        float esponenteRaggio;
        float influenzaRaggio;
        float shadowPointRadius;
        float startAngle;
        float stopAngle;
        float startAngleValue;
        float stopAngleValue;

        float areaScavo;
        float spessoreScavo;
        float costoTotale;

        module::Perlin rumore;
        float intensRumore;
        int ottaveRumore;
        float freqRumore;
        bool usaRumore;

};
