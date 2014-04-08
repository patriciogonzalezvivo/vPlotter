//
//  vPlotter.h
//
//  Created by Patricio Gonzalez Vivo on 12/1/13.
//
//  Using 2x Pololu A4988 Stepper Driver
//  http://www.pololu.com/product/1182

#pragma once
#include "ofMain.h"

#ifdef TARGET_RASPBERRY_PI
#include "wiringPi.h"
#include "softServo.h"
#endif

//  PINS
//----------------------------
#define SERVO_PIN 18

#define STEP_PIN_M1 4
#define DIR_PIN_M1 17

#define STEP_PIN_M2 23
#define DIR_PIN_M2 22

// STATES
//----------------------------
// pen states
#define PEN_UP 0
#define PEN_DOWN 1
// motor directions
#define DIR_UP 1
#define DIR_DOWN 0
//  Servo conversion values
#define MIN_PULSE_WIDTH -250 //544   // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH 1250 //2400  // the longest pulse sent to a servo

struct MotorVal{
    long M1;
    long M2;
    friend bool operator==(const MotorVal& v1, const MotorVal& v2){return (v1.M1==v2.M1)&&(v1.M2==v2.M2);}
    friend bool operator!=(const MotorVal& v1, const MotorVal& v2){return !operator==(v1,v2);}
};

enum Comand{
    MOVE_ABS = 0,
    MOVE_REL,
    LINE_ABS,
    LINE_REL
};

struct Instruction : public ofPoint {
    Comand cmd;
    MotorVal target;
};

class vPlotter : public ofThread {
public:
    vPlotter();
    
    bool        setup(int _degPenUp = 140 , int _degPenDown = 70, int _mmMotorsDistance = 1500, int _mmPulleyRadius = 5, int _stepsPerRotation = 800, bool _sysGPIO = false);
    
    bool        isPrinting();
    bool        isHealthyAt(ofPoint _pos);
    int         getInstructionsLeft();
    ofRectangle getPrintingArea();
    MotorVal    getStepsFor(const ofPoint &_pos) const;
    ofPoint     getPosFor(const MotorVal &_steps) const;
    ofVec2f     getTension(ofPoint _pos);       //  values between 0-1 are ok
    ofVec2f     getResolution(ofPoint _pos);    //  values between 0-1 are ok
    
    void        print(vector<ofPolyline> _paths);
    void        print(Comand _command, ofPoint _pos);
    
    void        stop();
    void        draw();
    
    float       stepDelay;
    float       penDelay;
    
private:
    ofPoint     calcPointB(float a, float b, float c)const;
    void        threadedFunction();
    
    bool        addInstruction(Comand _command, ofPoint _pos);
    bool        exeInstruction(Instruction _inst);
    
    vector<Instruction> instructions;
    
    float       m2s;
    
    ofRectangle printingArea;
    ofPoint     M1,M2;
    ofPoint     currentPos;
    MotorVal    steps;
    MotorVal    target;
    
    int         motorsDistance, pulleyRadius, stepsPerRotation;
    int         penPosUp, penPosDown;
    
    bool        penState;
    bool        bPlotting;
};