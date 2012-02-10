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

        DepthExporter esporta;

        ofMesh sassi;
        ofMesh basi;

        ofPoint centro;
        vector <ofPoint> cloud;
        vector <ofPoint> shadow;

        float baseh;
        float minh;
        float maxh;
        float distx;
        float disty;
        float raggioh;
        float esponenteRaggio;
        float influenzaRaggio;
        float shadowPointRadius;

        module::Perlin rumore;
        float intensRumore;
        bool usaRumore;

};
