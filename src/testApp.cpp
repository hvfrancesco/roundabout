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
    showSassiCentro = false;

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
    startAngle = 0.0;
    stopAngle = 360.0;
    startAngleValue = 1.0;
    stopAngleValue = 1.0;
    shadowPointRadius = 0.02;
    raggioh = 0.0;
    esponenteRaggio = 1.0;
    influenzaRaggio = 0.0;
    intensRumore = 0.0;
    ottaveRumore = 6;
    freqRumore = 1.0;

    spessoreScavo = 0.35;
    costoTotale = 0.0;

    // this sets the camera's distance from the object
    cam.setDistance(cameraDistance);



    // GUI STUFF ---------------------------------------------------

    gui = new ofxUICanvas(5,5,320,ofGetHeight()-10);		//ofxUICanvas(float x, float y, float width, float height)
    gui->setFontSize(OFX_UI_FONT_SMALL, 5);
    gui->addWidgetDown(new ofxUILabel("rotonda Raucedo", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUISlider(304,8,5.0,25.0,raggioExt,"RAGGIO EXT"));
    gui->addWidgetDown(new ofxUISlider(304,8,0.0,24.0,raggioInt,"RAGGIO INT"));
    gui->addWidgetDown(new ofxUISlider(304,8,5.0,250.0,cameraDistance,"CAMERA DIST"));
    gui->addWidgetDown(new ofxUISlider(304,8,0.0,10.0,baseh,"BASEH"));
    gui->addWidgetDown(new ofxUISlider(304,8,0.0,1.0,minh,"HMIN"));
    gui->addWidgetDown(new ofxUISlider(304,8,0.0,5.0,maxh,"HMAX"));
    gui->addWidgetDown(new ofxUILabel("distanza dal centro", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUISlider(304,8,-1.0,1.0,raggioh,"INFLU RAGGIO"));
    gui->addWidgetDown(new ofxUISlider(304,8,-3.0,3.0,esponenteRaggio,"EXP"));
    gui->addWidgetDown(new ofxUISlider(304,8,0.0,1.0,influenzaRaggio,"INTENS RAGGIO"));
    gui->addWidgetDown(new ofxUILabel("spaziatura", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUISlider(304,8,0.0,1.0,distx,"SPAZ X"));
    gui->addWidgetDown(new ofxUISlider(304,8,0.0,1.0,disty,"SPAZ Y"));
    gui->addWidgetDown(new ofxUISlider(304,8,0.0,360.0,startAngle,"ANGOLO INIZIO"));
    gui->addWidgetDown(new ofxUISlider(304,8,0.0,360.0,stopAngle,"ANGOLO FINE"));
    gui->addWidgetDown(new ofxUISlider(304,8,-1.0,1.0,startAngleValue,"VALORE INIZIO"));
    gui->addWidgetDown(new ofxUISlider(304,8,-1.0,1.0,stopAngleValue,"VALORE FINE"));
    gui->addWidgetDown(new ofxUILabel("rumore", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUIToggle(10, 10, false, "USA RUMORE"));
    gui->addWidgetDown(new ofxUISlider(304,8,0.0,1.0,intensRumore,"INTENS RUMORE"));
    gui->addWidgetDown(new ofxUISlider(304,8,1,6,ottaveRumore,"COMPL RUMORE"));
    gui->addWidgetDown(new ofxUISlider(304,8,0.0,16.0,freqRumore,"FREQ RUMORE"));

    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);

    cam.disableMouseInput();

    sassi.setMode(OF_PRIMITIVE_POINTS);
    basi.setMode(OF_PRIMITIVE_POINTS);

    shadow.clear();
    cloud.clear();
    cloudCentro.clear();
    origine.set(raggioExt, centro.y, 0);
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
                float angolo;
                angolo = (puntoBase-centro).angle(origine-centro);
                if(puntoBase.y < 0) angolo = 360.0 - angolo;
                shadow.push_back(puntoBase);
                cloud.push_back(puntoSasso);
                lunghezzaTotale += baseh;
            }
            else if((ofDist(x, y,centro.x, centro.y) <= raggioExt) && (ofDist(x, y,centro.x, centro.y) < raggioInt))
            {
                ofPoint puntoSasso;
                puntoSasso.x = x;
                puntoSasso.y = y;
                puntoSasso.z = 0;
                cloudCentro.push_back(puntoSasso);
            }
        }
    }
    areaScavo = ((raggioExt*raggioExt*PI)-(raggioInt*raggioInt*PI))/360*(stopAngle-startAngle);
    basi.addVertices(shadow);
    sassi.addVertices(cloud);
    sassiCentro.addVertices(cloudCentro);

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
    else if(e.widget->getName() == "ANGOLO INIZIO")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        startAngle = slider->getScaledValue();
        if(startAngle > stopAngle)
        {
            startAngle = stopAngle;
            slider->setValue(startAngle);
        }
        isChanged = true;
    }
    else if(e.widget->getName() == "ANGOLO FINE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        stopAngle = slider->getScaledValue();
        if(stopAngle < startAngle)
        {
            stopAngle = startAngle;
            slider->setValue(stopAngle);
        }
        isChanged = true;
    }
    else if(e.widget->getName() == "VALORE INIZIO")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        startAngleValue = slider->getScaledValue();
        isChanged = true;
    }
    else if(e.widget->getName() == "VALORE FINE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        stopAngleValue = slider->getScaledValue();
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
    else if(e.widget->getName() == "COMPL RUMORE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        ottaveRumore = slider->getScaledValue();
        rumore.SetOctaveCount(ottaveRumore);
        isChanged = true;
    }
    else if(e.widget->getName() == "FREQ RUMORE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        freqRumore = slider->getScaledValue();
        rumore.SetFrequency(freqRumore);
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
    costoTotale = calcoloCosti();
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

    if (showSassiCentro)
    {
        // disegna sassi
        ofSetColor(255,255,255,255);
        ofFill();
        glPointSize(3);
        glEnable(GL_DEPTH_TEST);
        sassiCentro.drawVertices();
        glDisable(GL_DEPTH_TEST);
        ofNoFill();
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
    msg += "  lunghezza totale steli: " + ofToString(lunghezzaTotale) + " m\n";
    msg += "peso totale acciaio: phi6 " + ofToString(lunghezzaTotale * 0.222) + " Kg (" + ofToString(lunghezzaTotale * 0.222 * 1.7) +" euro) / phi8 "  + ofToString(lunghezzaTotale * 0.395) + " Kg (" + ofToString(lunghezzaTotale * 0.395 * 1.7) + " euro)\n";
    msg += "area occupata: " + ofToString(areaScavo) + " mq" + " - Volume scavo: " + ofToString(areaScavo*spessoreScavo) + " mc" + "\n";
    msg += "costo totale: " + ofToString(costoTotale) + " euro\n";
    msg += string("Using mouse inputs to navigate ('g' to toggle): ") + (cam.getMouseInputEnabled() ? "YES" : "NO");
    msg += "\nfps: " + ofToString(ofGetFrameRate(), 2);
    ofDrawBitmapString(msg, 10, ofGetHeight()-70);

}



//--------------------------------------------------------------
void testApp::updateCloud()
{

    if (isChanged)
    {
        shadow.clear();
        cloud.clear();
        cloudCentro.clear();
        basi.clear();
        sassi.clear();
        sassiCentro.clear();
        lunghezzaTotale = 0.0;
        origine.set(raggioExt, centro.y, 0);
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
                    float angolo;
                    angolo = (puntoBase-centro).angle(origine-centro);
                    if(puntoBase.y < 0) angolo = 360.0 - angolo;
                    if(angolo >= startAngle && angolo <= stopAngle)
                    {
                    puntoSasso.x = x;
                    puntoSasso.y = y;
                    float influenzaDistanzaCentro = ((distCentro * raggioh) * pow(influenzaRaggio, esponenteRaggio));
                    float altezza = (baseh + influenzaDistanzaCentro*baseh);
                    altezza *= (startAngleValue*(1-angolo/360)+stopAngleValue*angolo/360);
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
            else if(distCentro <= raggioExt && distCentro < raggioInt)
            {
                ofPoint puntoSasso;
                puntoSasso.x = x;
                puntoSasso.y = y;
                puntoSasso.z = 0;
                cloudCentro.push_back(puntoSasso);
            }
            }
        }
        areaScavo = ((raggioExt*raggioExt*PI)-(raggioInt*raggioInt*PI))/360*(stopAngle-startAngle);

        basi.addVertices(shadow);
        sassi.addVertices(cloud);
        sassiCentro.addVertices(cloudCentro);

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

float testApp::calcoloCosti()
{
    float nSteli = shadow.size();
    float volumeScavo = (areaScavo*spessoreScavo);
    float volumeFondaz = nSteli*0.15*0.20*disty;
    float volumeStabiliz = areaScavo*0.1;
    float volumeTerra = volumeScavo-volumeFondaz-volumeStabiliz;
    float ferroFondaz = nSteli*disty*4*0.395;

    float costo = 0.0;
    costo += (lunghezzaTotale * 0.222 * 1.7); // steli
    costo += (float)(nSteli/1000)*24.00; // ciottoli su steli
    costo += (((raggioInt*raggioInt*PI)/(0.1*0.1))/1000)*24.00; // ciottoli al centro
    costo += volumeScavo * 15.86; // scavo sbancamento
    costo += volumeStabiliz * 31.65; // stabilizzato
    costo += volumeFondaz*180.00; // cls fondaz
    costo += ferroFondaz*1.7; // ferro fondazioni
    costo += volumeTerra*36.67; // terra riporto
    costo += nSteli*0.5; // manodopera sassi

    return costo;
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

    case 'S':
        showSassiCentro = !showSassiCentro;
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
