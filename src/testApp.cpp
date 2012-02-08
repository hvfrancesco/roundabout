#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{

    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    glEnable(GL_DEPTH_TEST);
    ofEnableAlphaBlending();

    isSetup = true;
    showCircles = true;
    showBasePoints = true;
    showSassi = true;
    showSteli = true;

    ofSetCircleResolution(100);
    ofEnableSmoothing();

    centro.x = 0.0;
    centro.y = 0.0;
    centro.z = 0.0;

    raggioRotonda = 15.75;
    raggioExt = prevRaggioExt = raggioRotonda-0.5;
    raggioInt = prevRaggioInt = 0.0;
    cameraDistance = raggioExt*2;
    baseh = prevBaseh = 0.5;
    minh = 0.25;
    maxh = 4.0;
    distx = prevDistx = 0.5;
    disty = prevDisty = 0.5;
    shadowPointRadius = 0.02;

    // this sets the camera's distance from the object
    cam.setDistance(cameraDistance);



    // GUI STUFF ---------------------------------------------------

    // general page
    gui.addTitle("Raucedo - concorso rotonda");
    // overriding default theme
    //gui.bDrawHeader = false;
    gui.config->toggleHeight = 16;
    gui.config->buttonHeight = 18;
    gui.config->sliderTextHeight = 18;
    gui.config->titleHeight = 18;
    //gui.config->fullActiveColor = 0x6B404B;
    //gui.config->fullActiveColor = 0x5E4D3E;
    gui.config->fullActiveColor = 0x648B96;
    gui.config->textColor = 0xFFFFFF;
    gui.config->textBGOverColor = 0xDB6800;

    gui.addSlider("raggio esterno", raggioExt, 5.0, 25.0);
    gui.addSlider("raggio interno", raggioInt, 0.0, 24.0);
    gui.addSlider("distanza camera", cameraDistance, 5.0, 250.0);
    gui.addSlider("altezza base", baseh, 0.0, 3.0);
    gui.addSlider("altezza min", minh, 0.0, 1.0);
    gui.addSlider("altezza max", maxh, 0.0, 5.0);
    gui.addSlider("spaziatura x", distx, 0.0, 0.5);
    gui.addSlider("spaziatura y", disty, 0.0, 0.5);


    gui.show();
    cam.disableMouseInput();

    shadow.clear();
    cloud.clear();
    for(float i=0; i<raggioExt*2; i += distx)
    {
        for(float j=0; j<raggioExt*2; j += disty)
        {
            float x = i-raggioExt;
            float y = j-raggioExt;

            if((ofDist(x, y,centro.x, centro.y) <= raggioExt) && (ofDist(x, y,centro.x, centro.y) >= raggioInt))
            {
                ofPoint puntoBase;
                ofPoint puntoSasso;
                puntoBase.x = x;
                puntoBase.y = y;
                puntoBase.z = 0;
                puntoSasso.x = x;
                puntoSasso.y = y;
                puntoSasso.z = baseh;
                shadow.push_back(puntoBase);
                cloud.push_back(puntoSasso);
            }
        }
    }

}

//--------------------------------------------------------------
void testApp::update()
{

    // this sets the camera's distance from the object
    cam.setDistance(cameraDistance);

    if (distx != prevDistx || disty != prevDisty || raggioExt != prevRaggioExt || raggioInt != prevRaggioInt || baseh != prevBaseh)
    {

        shadow.clear();
        cloud.clear();
        for(float i=0; i<raggioExt*2; i += distx)
        {
            for(float j=0; j<raggioExt*2; j += disty)
            {
                float x = i-raggioExt;
                float y = j-raggioExt;

                if((ofDist(x, y,centro.x, centro.y) <= raggioExt) && (ofDist(x, y,centro.x, centro.y) >= raggioInt))
                {
                    ofPoint puntoBase;
                    ofPoint puntoSasso;
                    puntoBase.x = x;
                    puntoBase.y = y;
                    puntoBase.z = 0;
                    puntoSasso.x = x;
                    puntoSasso.y = y;
                    puntoSasso.z = baseh;
                    shadow.push_back(puntoBase);
                    cloud.push_back(puntoSasso);
                }
            }
        }




        prevDistx = distx;
        prevDisty = disty;
        prevRaggioExt = raggioExt;
        prevRaggioInt = raggioInt;
        prevBaseh = baseh;
    }


}

//--------------------------------------------------------------
void testApp::draw()
{

    cam.begin();

    ofNoFill();
    ofSetColor(255,255,255,255);
    ofSetLineWidth(1.2);
    // disegna limite rotonda
    ofCircle(centro.x,centro.y,raggioRotonda);

    if (showCircles)
    {
        // disegna cerchio esterno
        ofSetColor(128,128,128,255);
        ofCircle(centro.x,centro.y,raggioExt);
        // disegna cerchio interno
        if(raggioInt > 0.0)
        {
            ofSetColor(128,128,128,255);
            ofCircle(centro.x,centro.y,raggioInt);
        }
    }

    if (showBasePoints)
    {
        // disegna punti di base
        ofSetColor(0,0,0,200);
        ofSetLineWidth(0.25);
        for(int i=0; i<shadow.size(); i++)
        {
            ofPushMatrix();
            ofTranslate(shadow[i].x, shadow[i].y);
            ofCircle(0, 0, shadowPointRadius);
            ofPopMatrix();
            //ofCircle(shadow[i].x, shadow[i].y, shadowPointRadius);
        }
    }

    if (showSteli)
    {
        // disegna steli
        ofSetColor(0,0,0,128);
        ofColor coloreSteli(0,0,0,128);
        ofSetLineWidth(0.25);


        for(int i=0; i<shadow.size(); i++)
        {
            ofPolyline steli;
            steli.addVertex(shadow[i]);
            steli.lineTo(cloud[i]);
            steli.draw();
            steli.clear();
            //ofCircle(shadow[i].x, shadow[i].y, shadowPointRadius);
        }
    }


    if (showSassi)
    {
        // disegna sassi
        ofSetColor(255,255,255,200);
        ofSetLineWidth(0.15);
        for(int i=0; i<cloud.size(); i++)
        {
            ofPushMatrix();
            ofTranslate(cloud[i].x, cloud[i].y, cloud[i].z);
            ofCircle(0, 0, shadowPointRadius);
            ofPopMatrix();
        }
    }



    cam.end();

    ofSetColor(255);
    string msg = string("numero elementi: ") + ofToString(shadow.size()) + "\n";
    msg += string("Using mouse inputs to navigate ('g' to toggle): ") + (cam.getMouseInputEnabled() ? "YES" : "NO");
    msg += "\nfps: " + ofToString(ofGetFrameRate(), 2);
    ofDrawBitmapString(msg, 10, ofGetHeight()-50);


    if (isSetup)
    {
        // draws gui
        gui.draw();
    }


}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

    switch(key)
    {
    case 'G':
    case 'g':
        if(cam.getMouseInputEnabled())
        {
            cam.disableMouseInput();
        }
        else cam.enableMouseInput();
        gui.toggleDraw();
        break;

    case 'F':
    case 'f':
        ofToggleFullscreen();
        break;

    case 'c':
        showCircles = !showCircles;
        break;

    case 'b':
        showBasePoints = !showBasePoints;
        break;

    case 's':
        showSassi = !showSassi;
        break;

    case 'a':
        showSteli = !showSteli;
        break;
        //case 'g':
        //    gui.toggleDraw();
    }



}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}
