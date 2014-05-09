//
//  UIvPlotter.h
//
//  Created by Patricio Gonzalez Vivo on 10/9/13.
//
#pragma once

#define PEN_UP 0
#define PEN_DOWN 1

#include "UIClass.h"

#include "ofxOsc.h"
#include "ofxJSONElement.h"

struct MotorVal{
    long M1;
    long M2;
    friend bool operator==(const MotorVal& v1, const MotorVal& v2){return (v1.M1==v2.M1)&&(v1.M2==v2.M2);}
    friend bool operator!=(const MotorVal& v1, const MotorVal& v2){return !operator==(v1,v2);}
};

enum Comand{
    MOVE_ABS = 0,
    MOVE_REL = 1,
    LINE_ABS = 2,
    LINE_REL = 3
};

struct Instruction : public ofPoint {
    Comand cmd;
    MotorVal target;
};

class UIvPlotter : public UIClass {
public:
    UIvPlotter();
    
    string      getClassName(){ return "vPLOTTER"; };
    
    bool        checkHostValues();
    
    bool        isHealthyAt(ofPoint _pos);
    ofVec2f     getTension(ofPoint _pos);       //  values between 0-1 are ok
    ofVec2f     getResolution(ofPoint _pos);    //  values between 0-1 are ok
    MotorVal    getStepsFor(const ofPoint &_pos) const;
    ofPoint     getPosFor(const MotorVal &_steps) const;
    
    ofRectangle getPrintingArea(){return ofRectangle(0,0,printingArea.width,printingArea.height);}
    
    void        print(ofPolyline _polyline);
    void        print(vector<ofPolyline> _paths);
    
    void        update();
    void        draw();
    
private:
    void        setupUI();
    void        guiEvent(ofxUIEventArgs &e);
    
    ofPoint     calcPointB(float a, float b, float c)const;
    bool        addInstruction(Comand _command, ofPoint _pos);
    bool        exeInstruction(Instruction _inst);
    
    vector<Instruction> instructions;

//    ofxUITextInput  *textField;
    string      host;
    int         oscPort;
    
    ofxOscSender sender;
    
    ofRectangle printingArea;
    ofPoint     M1,M2;
    ofPoint     currentPos;
    MotorVal    steps;
    MotorVal    target;
    float       m2s;
    int         motorsDistance, pulleyRadius, stepsPerRotation,penUp,penDown;
    
    bool        penState;
    bool        bPlotting;
    bool        bUpdate;
};
