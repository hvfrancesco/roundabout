#pragma once

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"

class testApp : public ofBaseApp{

	public:
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

		ofEasyCam cam; // add mouse controls for camera movement

		float raggioRotonda;
		float raggioExt;
		float raggioInt;
		float cameraDistance;


		// gui elements
        bool showGui;
        bool isSetup;
        bool showCircles;
        bool showBasePoints;
        bool showSassi;
        bool showSteli;

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
        float prevDistx;
        float prevDisty;
        float prevRaggioExt;
        float prevRaggioInt;
        float prevBaseh;
        float raggioh;
        float prevRaggioh;

        float shadowPointRadius;

};
