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

    lunghezzaTotale = 0.0;

    raggioRotonda = 15.75;
    raggioExt =  raggioRotonda-0.75;
    raggioInt = 10.0;
    cameraDistance = raggioExt*2;
    baseh = 0.5;
    minh = 0.25;
    maxh = 4.0;
    distx = 0.5;
    disty = 0.5;
    shadowPointRadius = 0.02;
    raggioh = 0.0;
    esponenteRaggio = 1.0;
    influenzaRaggio = 0.0;
    intensRumore = 0.0;

    // this sets the camera's distance from the object
    cam.setDistance(cameraDistance);



    // GUI STUFF ---------------------------------------------------

    gui = new ofxUICanvas(5,5,320,ofGetHeight()-5);		//ofxUICanvas(float x, float y, float width, float height)
    gui->addWidgetDown(new ofxUILabel("rotonda Raucedo", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUISlider(304,10,5.0,25.0,raggioExt,"RAGGIO EXT"));
    gui->addWidgetDown(new ofxUISlider(304,10,0.0,24.0,raggioInt,"RAGGIO INT"));
    gui->addWidgetDown(new ofxUISlider(304,10,5.0,250.0,cameraDistance,"CAMERA DIST"));
    gui->addWidgetDown(new ofxUISlider(304,10,0.0,10.0,baseh,"BASEH"));
    gui->addWidgetDown(new ofxUISlider(304,10,0.0,1.0,minh,"HMIN"));
    gui->addWidgetDown(new ofxUISlider(304,10,0.0,5.0,maxh,"HMAX"));
    gui->addWidgetDown(new ofxUILabel("distanza dal centro", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUISlider(304,10,-1.0,1.0,raggioh,"INFLU RAGGIO"));
    gui->addWidgetDown(new ofxUISlider(304,10,-3.0,3.0,esponenteRaggio,"EXP"));
    gui->addWidgetDown(new ofxUISlider(304,10,0.0,1.0,influenzaRaggio,"INTENS RAGGIO"));
    gui->addWidgetDown(new ofxUILabel("spaziatura", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUISlider(304,10,0.0,0.5,distx,"SPAZ X"));
    gui->addWidgetDown(new ofxUISlider(304,10,0.0,0.5,disty,"SPAZ Y"));
    gui->addWidgetDown(new ofxUILabel("rumore", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUIToggle(20, 20, false, "USA RUMORE"));
    gui->addWidgetDown(new ofxUISlider(304,10,0.0,1.0,intensRumore,"INTENS RUMORE"));

    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);

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
                lunghezzaTotale += baseh;
            }
        }
    }
    basi.addVertices(shadow);
    sassi.addVertices(cloud);

    ofBackground(120,100,100);
}


//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "RAGGIO EXT")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        raggioExt = slider->getScaledValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "RAGGIO INT")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        raggioInt = slider->getScaledValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "CAMERA DIST")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        cameraDistance = slider->getScaledValue();
    }
    else if(e.widget->getName() == "BASEH")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        baseh = slider->getScaledValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "HMIN")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        minh = slider->getScaledValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "HMAX")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        maxh = slider->getScaledValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "INFLU RAGGIO")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        raggioh = slider->getScaledValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "EXP")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        esponenteRaggio = slider->getScaledValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "INTENS RAGGIO")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        influenzaRaggio = slider->getScaledValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "SPAZ X")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        distx = slider->getScaledValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "SPAZ Y")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        disty = slider->getScaledValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "USA RUMORE")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        usaRumore = toggle->getValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "INTENS RUMORE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        intensRumore = slider->getScaledValue();
        isChanged = true;
    }
}




//--------------------------------------------------------------
void testApp::update()
{

    // this sets the camera's distance from the object
    cam.setDistance(cameraDistance);

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
    string msg = string("numero elementi: ") + ofToString(shadow.size());
    msg += "  lunghezza totale steli: " + ofToString(lunghezzaTotale) + "\n";
    msg += string("Using mouse inputs to navigate ('g' to toggle): ") + (cam.getMouseInputEnabled() ? "YES" : "NO");
    msg += "\nfps: " + ofToString(ofGetFrameRate(), 2);
    ofDrawBitmapString(msg, 10, ofGetHeight()-50);

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
        lunghezzaTotale = 0.0;
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
                    float altezza = (baseh + influenzaDistanzaCentro*baseh);
                    if(usaRumore)
                    {
                        double valoreRumore = rumore.GetValue(x,y,altezza);
                        altezza = altezza + altezza * valoreRumore * intensRumore;
                    }
                    if(altezza < minh) {altezza = minh;}
                    else if(altezza > maxh) {altezza = maxh;}
                    puntoSasso.z = altezza;

                    lunghezzaTotale += altezza;
                    shadow.push_back(puntoBase);
                    cloud.push_back(puntoSasso);
                }
            }
        }
        basi.addVertices(shadow);
        sassi.addVertices(cloud);

        isChanged = false;
    }

}

//--------------------------------------------------------------
void testApp::exit()
{
    delete gui;
}



//--------------------------------------------------------------

void testApp::esportaFile()
{

    esporta.exportSassi(ofToString("roundabout.pcd"), cloud);
    esporta.exportSassi(ofToString("roundabout.csv"), cloud);
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
        gui->toggleVisible();
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

    case ' ':
        esportaFile();
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
