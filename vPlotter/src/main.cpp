#include "ofMain.h"

#include "ofApp.h"
#include "ofAppNoWindow.h"

#include <algorithm>

char* getCmdOption(char ** begin, char ** end, const std::string & option){
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option){
    return std::find(begin, end, option) != end;
}

void printVersion(){
    cout << "vPlotter 0.1" << endl;
    cout << "Copyright (c) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )" << endl;
    cout << "This is free software with ABSOLUTELY NO WARRANTY." << endl;
}

//========================================================================
int main(int argc, char * argv[]){
    
    //  SYSTEM
    //----------------------------------------------
    //
#ifdef TARGET_RASPBERRY_PI
    bool noWindows = true;
    if(cmdOptionExists(argv, argv+argc, "-x")){
        noWindows = false;
    }
#else
    bool noWindows = false;
#endif
    ofAppNoWindow noWindowApp;
    if(noWindows){
        ofSetupOpenGL(&noWindowApp, 1024,768, OF_WINDOW);
    } else {
        ofSetupOpenGL(1024,768, OF_WINDOW);
    }
    
    ofApp *app = new ofApp();
    app->sysGpio = true;
    app->bFit = true;
    app->file = "tux.svg";
    app->scale = 100;
    app->rotate = 0;
    app->mmMotorsDistance = 1500;
    app->mmPulleyRadius = 5;
    app->stepsPerRotation = 800;
    app->stepDelay = 1;
    app->degPenUp = 140;
    app->degPenDown = 70;
    app->penDelay = 2000;
    app->oscPort = -1;
    
    if(cmdOptionExists(argv, argv+argc, "-gipo")){
        app->sysGpio = false;
    }
    
    //  HELP
    //
    if(cmdOptionExists(argv, argv+argc, "-v")){
        printVersion();
        return 0;
    }
    
    if(cmdOptionExists(argv, argv+argc, "-h")){
        printVersion();
        cout << endl;
        cout << "Usage: vPlotter [options] -i file.svg" << endl;
        cout << endl;
        cout << "Options:" << endl;
        cout << "   -s [0-100]  Scale the image. Default: " << app->scale << endl;
        cout << "   -r [deg]    Rotate the image. Default: " << app->rotate << endl;
        cout << "   -nf         Don't fit." << endl;
        cout << "   -d [mm]     Distance between motors in mm. Default: " << app->mmMotorsDistance << endl;
        cout << "   -pr [mm]    Pulley Radius in mm. Default: " << app->mmPulleyRadius << endl;
        cout << "   -spr [stp]  Steps per Rotation. Default: " << app->stepsPerRotation << endl;
        cout << "   -sd [ms]    Step Delay. Default: " << app->stepDelay << endl;
        cout << "   -pu [deg]   Pen Up Angle. Default: " << app->degPenUp << endl;
        cout << "   -pd [deg]   Pen Down Angle. Default: " << app->degPenDown << endl;
        cout << "   -delay [ms] Pen Delay. Default: " << app->penDelay << endl;
        cout << "   -gipo       Direct GPIO connection. Not throught /dev/ files. Require Root priviledge" << endl;
        cout << "   -x          X Debug Windows. Default: "<< noWindows << endl;
        cout << "   -o [port]   Stream mode to OSC Port" << endl;
        return 0;
    }
    
    //  INPUT SVG file
    //
    char * filename = getCmdOption(argv, argv+argc, "-i");
    char * oscPort = getCmdOption(argv, argv+argc, "-o");

    if (filename){
        app->file = filename;
    }
    else if(oscPort){
        app->oscPort = ofToInt(oscPort);
    }
#ifdef TARGET_RASPBERRY_PI
    else {
        cout << "Usage: vPlotter [options] -i file.svg" << endl;
        return 0;
    }
#endif
    
    if(cmdOptionExists(argv, argv+argc, "-nf")){
        app->bFit = false;
    }
    
    char * scale = getCmdOption(argv, argv+argc, "-s");
    if(scale){
        app->scale = ofToFloat(scale);
    }
    
    char * rotate = getCmdOption(argv, argv+argc, "-r");
    if(rotate){
        app->rotate = ofToFloat(rotate);
    }
    
    //  MOTORS
    //
    char * d = getCmdOption(argv, argv+argc, "-d");
    if(d){
        app->mmMotorsDistance = ofToInt(d);
    }
    
    char * pr = getCmdOption(argv, argv+argc, "-pr");
    if(pr){
        app->mmPulleyRadius = ofToInt(pr);
    }

    char * spr = getCmdOption(argv, argv+argc, "-spr");
    if(spr){
        app->stepsPerRotation = ofToInt(spr);
    }
    
    char * sd = getCmdOption(argv, argv+argc, "-sd");
    if(sd){
        app->stepDelay = ofToFloat(sd);
    }
    
    //  PEN
    //
    char * pu = getCmdOption(argv, argv+argc, "-pu");
    if(pu){
        app->degPenUp = ofToInt(pu);
    }
    
    char * pd = getCmdOption(argv, argv+argc, "-pd");
    if(pd){
        app->degPenDown = ofToInt(pd);
    }
    
    char * pdelay = getCmdOption(argv, argv+argc, "-delay");
    if(pdelay){
        app->penDelay = ofToFloat(pdelay);
    }

    ofRunApp( app );
}
