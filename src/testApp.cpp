#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{

    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    //glEnable(GL_DEPTH_TEST);
    ofEnableAlphaBlending();

    isChanged = false;
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
    raggioExt = prevRaggioExt = raggioRotonda-0.75;
    raggioInt = prevRaggioInt = 10.0;
    cameraDistance = raggioExt*2;
    baseh = prevBaseh = 0.5;
    minh = prevMinh = 0.25;
    maxh = prevMaxh = 4.0;
    distx = prevDistx = 0.5;
    disty = prevDisty = 0.5;
    shadowPointRadius = 0.02;
    raggioh = prevRaggioh = 0.0;
    esponenteRaggio = prevEsponenteRaggio = 1.0;
    influenzaRaggio = prevInfluenzaRaggio = 0.0;

    // this sets the camera's distance from the object
    cam.setDistance(cameraDistance);



    // GUI STUFF ---------------------------------------------------

    // general page
    gui.addTitle("rotonda Raucedo");
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
    gui.addSlider("altezza base", baseh, 0.0, 10.0);
    gui.addSlider("altezza min", minh, 0.0, 1.0);
    gui.addSlider("altezza max", maxh, 0.0, 5.0);
    gui.addTitle("distanza dal centro");
    gui.addSlider("influenza raggio", raggioh, -1.0, 1.0);
    gui.addSlider("esponente", esponenteRaggio, -3.0, 3.0);
    gui.addSlider("influenza", influenzaRaggio, 0.0, 1.0);
    gui.addTitle("spaziatura griglia");
    gui.addSlider("spaziatura x", distx, 0.0, 0.5);
    gui.addSlider("spaziatura y", disty, 0.0, 0.5);


    gui.show();
    cam.disableMouseInput();

    sassi.setMode(OF_PRIMITIVE_POINTS);
    basi.setMode(OF_PRIMITIVE_POINTS);

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
    basi.addVertices(shadow);
    sassi.addVertices(cloud);

    ofBackground(120,100,100);
}

//--------------------------------------------------------------
void testApp::update()
{

    // this sets the camera's distance from the object
    cam.setDistance(cameraDistance);

    if (distx != prevDistx || disty != prevDisty || raggioExt != prevRaggioExt || raggioInt != prevRaggioInt || baseh != prevBaseh || raggioh != prevRaggioh || esponenteRaggio != prevEsponenteRaggio || influenzaRaggio != prevInfluenzaRaggio || minh != prevMinh || maxh != prevMaxh)
    {
        isChanged = true;
    }

    // update point-cloud if necessary
    updateCloud();
}

//--------------------------------------------------------------
void testApp::draw()
{

    cam.begin();

    ofNoFill();
    ofSetColor(255,255,255,255);
    ofSetLineWidth(1.2);
    ofSetCircleResolution(100);
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
        glPointSize(1);
        glEnable(GL_DEPTH_TEST);
        basi.drawVertices();
        glDisable(GL_DEPTH_TEST);
    }

    if (showSteli)
    {
        // disegna steli
        ofSetColor(0,0,0,128);
        ofColor coloreSteli(0,0,0,128);
        ofSetLineWidth(0.1);
        for(int i=0; i<shadow.size(); i++)
        {
            ofPolyline steli;
            steli.addVertex(shadow[i]);
            steli.lineTo(cloud[i]);
            //glEnable(GL_DEPTH_TEST);
            steli.draw();
            //glDisable(GL_DEPTH_TEST);
            steli.clear();
        }
    }

    if (showSassi)
    {
        // disegna sassi
        ofSetColor(255,255,255,255);
        ofFill();
        glPointSize(3);
        glEnable(GL_DEPTH_TEST);
        sassi.drawVertices();
        glDisable(GL_DEPTH_TEST);
        ofNoFill();
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
void testApp::updateCloud()
{

    if (isChanged)
    {
        shadow.clear();
        cloud.clear();
        basi.clear();
        sassi.clear();
        for(float i=0; i<raggioExt*2; i += distx)
        {
            for(float j=0; j<raggioExt*2; j += disty)
            {
                float x = i-raggioExt;
                float y = j-raggioExt;

                float distCentro = ofDist(x, y,centro.x, centro.y);

                if(distCentro <= raggioExt && distCentro >= raggioInt)
                {
                    ofPoint puntoBase;
                    ofPoint puntoSasso;
                    puntoBase.x = x;
                    puntoBase.y = y;
                    puntoBase.z = 0;
                    puntoSasso.x = x;
                    puntoSasso.y = y;
                    float influenzaDistanzaCentro = ((distCentro * raggioh) * pow(influenzaRaggio, esponenteRaggio));
                    float altezza = (baseh + influenzaDistanzaCentro/baseh);
                    if(altezza < minh) {altezza = minh;}
                    else if(altezza > maxh) {altezza = maxh;}
                    puntoSasso.z = altezza;

                    shadow.push_back(puntoBase);
                    cloud.push_back(puntoSasso);
                }
            }
        }
        basi.addVertices(shadow);
        sassi.addVertices(cloud);
        prevDistx = distx;
        prevDisty = disty;
        prevRaggioExt = raggioExt;
        prevRaggioInt = raggioInt;
        prevBaseh = baseh;
        prevRaggioh = raggioh;
        prevEsponenteRaggio = esponenteRaggio;
        prevInfluenzaRaggio = influenzaRaggio;
        prevMinh = minh;
        prevMaxh = maxh;

        isChanged = false;
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
