//
//  vPlotter.cpp
//  noWindow
//
//  Created by Patricio Gonzalez Vivo on 12/1/13.
//
//

#include "vPlotter.h"

vPlotter::vPlotter(){
    // delays in ms
    stepDelay   = 1;
    penDelay    = 50;
    
    //  Pos in mm
    currentPos.set(0,0);
    
    //  Pos step
    steps.M1    = 0;
    steps.M2    = 0;
    target.M1   = 0;
    target.M2   = 0;
    
    //  States
    penState    = PEN_UP;
    bPlotting   = false;
}

bool vPlotter::setup(int _penPosUp, int _penPosDown, int _motorsDistance, int _pulleyRadius, int _stepsPerRotation, bool _sysGPIO){
    //  Values
    motorsDistance = _motorsDistance;
    M1.set(0, 0);
    M2.set(motorsDistance, 0);
    pulleyRadius = _pulleyRadius;
    stepsPerRotation = _stepsPerRotation;
    
    //  Set printing Area
    printingArea.set(M1,motorsDistance,motorsDistance);
    ofPoint testPos = printingArea.getCenter();
    bool bSafe = true;
    while (bSafe) {
        testPos.y--;
        bSafe = isHealthyAt(testPos);
    }
    float top = testPos.y;
    testPos = printingArea.getCenter();
    bSafe = true;
    while (bSafe) {
        testPos.x--;
        testPos.y++;
        bSafe = isHealthyAt(testPos);
    }
    float left = testPos.x;
    float width = (printingArea.getCenter().x-left)*2;
    float heigth = (testPos.y-top);
    printingArea.x = left;
    printingArea.y = top;
    printingArea.width = width;
    printingArea.height = heigth;
    
    //  Calibration
    currentPos = printingArea.getCenter();
    m2s = (2 * PI * pulleyRadius) / stepsPerRotation;
    steps = getStepsFor(currentPos);
    target = getStepsFor(currentPos);
    
    //  SERVO SETUP
    //--------------------------------
    if(_penPosUp < 0) _penPosUp = 0;
    if(_penPosUp > 180) _penPosUp = 180;
    penPosUp =  ofMap(_penPosUp,0,180,MIN_PULSE_WIDTH,MAX_PULSE_WIDTH);
    if(_penPosDown < 0) _penPosDown = 0;
    if(_penPosDown > 180) _penPosDown = 180;
    penPosDown = ofMap(_penPosDown,0,180,MIN_PULSE_WIDTH,MAX_PULSE_WIDTH);
    penState = PEN_UP;
    
    //  Setup WiringPi
    //
#ifdef TARGET_RASPBERRY_PI
    if(_sysGPIO){
        if(wiringPiSetupSys() == -1){
            printf("Error on wiringPi setup\n");
            return false;
        }
    } else {
        if(wiringPiSetupGpio() == -1){
            printf("Error on wiringPi setup\n");
            return false;
        }
    }
    
    //  Pins
    pinMode(DIR_PIN_M1, OUTPUT);
    pinMode(STEP_PIN_M1, OUTPUT);
    pinMode(DIR_PIN_M2, OUTPUT);
    pinMode(STEP_PIN_M2, OUTPUT);
    
    digitalWrite(DIR_PIN_M1, LOW);
    digitalWrite(DIR_PIN_M2, LOW);
    
    softServoSetup(SERVO_PIN, -1, -1, -1, -1, -1, -1, -1);
    softServoWrite(SERVO_PIN, penPosUp);
    usleep(penDelay*1000.0f);
#endif
    
    return true;
}

bool vPlotter::isPrinting(){
    return bPlotting;
}

ofRectangle vPlotter::getPrintingArea(){
    return printingArea;
}

bool vPlotter::isHealthyAt(ofPoint _pos){
    ofVec2f tension = getTension(_pos);
    ofVec2f resolution = getResolution(_pos);
    if(tension.x<0||tension.y<0||
       tension.x>=0.8||tension.y>=0.8||
       resolution.x>=0.8||resolution.y>=0.8){
        return false;
    } else {
        return true;
    }
}

int vPlotter::getInstructionsLeft(){
    return instructions.size();
}

MotorVal vPlotter::getStepsFor(const ofPoint &_pos)const{
    MotorVal rta;
    rta.M1 = M1.distance(_pos)/m2s;
    rta.M2 = M2.distance(_pos)/m2s;
    return rta;
}

ofPoint vPlotter::getPosFor(const MotorVal &_steps)const{
    ofPoint rta;
    float a = _steps.M2 * m2s;
    float b = _steps.M1 * m2s;
    float c = motorsDistance;
    return calcPointB(a, b, c);
}

ofVec2f vPlotter::getTension(ofPoint _pos){
    double angleToM1 = atan2(_pos.y-M1.y,_pos.x-M1.x);
    double angleToM2 = atan2(_pos.y-M2.y,M2.x-_pos.x);
    double d = cos(angleToM1)*sin(angleToM2)+sin(angleToM1)*cos(angleToM2);
    double tensionToM1 = cos(angleToM2)/d;
    double tensionToM2 = cos(angleToM1)/d;
    return ofVec2f(tensionToM1,tensionToM2)-ofVec2f(0.5,0.5);
}

ofPoint vPlotter::calcPointB(float a, float b, float c)const{
    // law of cosines
    // http://en.wikipedia.org/wiki/Solution_of_triangles
    double angle = acos((b*b+c*c-a*a)/(2*b*c));
    return ofPoint(b*cos(angle)+M1.x,b*sin(angle)+M1.y);
}

ofVec2f vPlotter::getResolution(ofPoint _pos){
    ofVec2f rta;

    float c = M1.distance(M2);
    float b = M1.distance(_pos);
    float a = M2.distance(_pos);
    
    float err = .00000000001;
    ofPoint pc = calcPointB(a, b, c);
    if((_pos.x-err<pc.x<_pos.x+err) ||
       (_pos.y-err<pc.y<_pos.y+err)){
        float db = _pos.distance(calcPointB(a, b+1, c));
        float da = _pos.distance(calcPointB(a+1, b, c));
        return ofVec2f(da/1.4,db/1.4);
    }else{
        return ofVec2f(2,2);
    }
}

void vPlotter::stop(){
    stopThread();
    bPlotting = false;
    waitForThread(true);
    instructions.clear();
}

void vPlotter::print(vector<ofPolyline> _paths){
    if(isPrinting()){
        stopThread();
        bPlotting = false;
    }
    
    for(int i = 0; i < _paths.size(); i++) {
        for(int j = 0; j < _paths[i].getVertices().size(); j++) {
            addInstruction( ((j==0)?MOVE_ABS:LINE_ABS), _paths[i].getVertices()[j] );
        }
    }
    
    if (instructions.size() > 0) {
        addInstruction(MOVE_ABS, printingArea.getCenter());
        bPlotting = true;
        startThread(true, false);
    }
}

void vPlotter::print(Comand _command, ofPoint _pos){
    bool printing = false;
    
    if (!bPlotting){
        addInstruction(_command, _pos);
        if (instructions.size() > 0) {
            bPlotting = true;
            startThread(true, false);
        }
    } else {
        if(lock()){
            addInstruction(_command, _pos);
            printing = bPlotting;
            unlock();
        }
    }
}

bool vPlotter::addInstruction(Comand _command, ofPoint _pos){
    ofPoint t;
    
    //  ABSolute or RELative positions??
    //
    switch (_command) {
        case MOVE_ABS:
        case LINE_ABS:
            t = _pos;
            break;
        case MOVE_REL:
        case LINE_REL:
            t = _pos + currentPos;
            break;
    }
    
    //  Inside Area
    //
    if (t.x < printingArea.getLeft())   t.x = printingArea.getLeft();
    if (t.x > printingArea.getRight())  t.x = printingArea.getRight();
    if (t.y < printingArea.getTop())    t.y = printingArea.getTop();
    if (t.y > printingArea.getBottom()) t.y = printingArea.getBottom();
    
    //  Make instruction
    //
    Instruction inst;
    inst.set(_pos);
    inst.cmd = _command;
    inst.target = getStepsFor(_pos);
    
    //  Add
    //
    instructions.push_back(inst);
}

void vPlotter::threadedFunction(){
    while( isThreadRunning() != 0 ){
        if (isPrinting()){
            if(getInstructionsLeft()){
                if (exeInstruction(instructions[0])){
                    if(lock()){
                        instructions.erase(instructions.begin());
                        unlock();
                    }
                }
            } else {
                bPlotting = false;
                stopThread();
            }
        }
    }
}

bool vPlotter::exeInstruction(Instruction _inst){
    MotorVal t, s;
    float sd,pd;
    
    //  Set the right PEN
    //
    {
        sd = stepDelay*1000.0f;
        pd = penDelay*1000.0f;
        switch (_inst.cmd) {
            case LINE_REL:
            case LINE_ABS:{

                if(penState == PEN_UP){
#ifdef TARGET_RASPBERRY_PI
                    softServoWrite(SERVO_PIN,penPosDown);
#endif
                    usleep(pd);
                }
                
                penState = PEN_DOWN;
            }
                break;
            case MOVE_REL:
            case MOVE_ABS:{

                if(penState == PEN_DOWN){
#ifdef TARGET_RASPBERRY_PI
                    softServoWrite(SERVO_PIN,penPosUp);
#endif
                    usleep(pd);
                }
                
                penState = PEN_UP;
            }
                break;
        }
        

        
        // Tmp Variables
        t = target = _inst.target;
        s = steps;
    }

    // set directions
    MotorVal dir;
    dir.M1 = (t.M1 > s.M1) ? +1 : -1;
    dir.M2 = (t.M2 > s.M2) ? +1 : -1;
#ifdef TARGET_RASPBERRY_PI
    digitalWrite(DIR_PIN_M1, (t.M1 > s.M1) ? DIR_UP : DIR_DOWN);
	digitalWrite(DIR_PIN_M2, (t.M2 > s.M2) ? DIR_DOWN : DIR_UP);
#endif
    //  Make steps
    //
    while (s!=t){
        if( t.M1 - s.M1 != 0){
#ifdef TARGET_RASPBERRY_PI
            digitalWrite(STEP_PIN_M1, HIGH);
#endif
            s.M1 += dir.M1;
        }
        
        if(t.M2-s.M2 != 0){
#ifdef TARGET_RASPBERRY_PI
            digitalWrite(STEP_PIN_M2, HIGH);
#endif
            s.M2 += dir.M2;
        }
        steps = s;
        currentPos = getPosFor(s);
        usleep(250);
#ifdef TARGET_RASPBERRY_PI
        digitalWrite(STEP_PIN_M1, LOW);
        digitalWrite(STEP_PIN_M2, LOW);
#endif
        usleep(250);
        usleep(sd);
    }
    return true;
}

void vPlotter::draw(){
    if(lock()){
        ofPushMatrix();
        ofPushStyle();
        ofScale(0.5, 0.5); // 1pixel -> 5mm
        ofNoFill();

        ofVec2f tension = getTension(currentPos);
        //  M1
        if (tension.x<0) {
            ofSetColor(0, 0, 255);
        } else if (tension.x>1) {
            ofSetColor(255, 0, 0);
        } else {
            ofSetColor(0,255,0);
        }
        ofRect(M1+ofPoint(-20, -20), 40, 40);
        ofCircle(M1,pulleyRadius);
        ofLine(M1,currentPos);
        ofDrawBitmapString(ofToString(tension.x,2), M1+ofPoint(20,50));
        //  M2
        if (tension.y<0) {
            ofSetColor(0, 0, 255);
        } else if (tension.y>1) {
            ofSetColor(255, 0, 0);
        } else {
            ofSetColor(0,255,0);
        }
        ofRect(M2+ofPoint(-20, -20), 40, 40);
        ofCircle(M2, pulleyRadius);
        ofLine(M2,currentPos);
        ofDrawBitmapString(ofToString(tension.y,2), M2+ofPoint(-60,50));

        //  Printing Area
        ofSetColor(0,255,0);
        ofRect(printingArea);
        {
            ofPushMatrix();
            ofPushStyle();
            ofTranslate(currentPos);
            ofVec2f resolution;
            if(resolution.x < 1 && resolution.y < 1){
                ofSetColor(0,255,0);
            } else {
                ofSetColor(255,0,0);
            }
            ofDrawBitmapString(ofToString(resolution.x,2)+"/"+ofToString(resolution.y,2), -70,50);
            ofNoFill();
            ofCircle(0,0,20);
            if(penState==PEN_UP) ofNoFill();
            else ofFill();
            ofCircle(0,0,8);
            ofLine(-30,0,30,0);
            ofLine(0,-30,0,30);
            ofPopStyle();
            ofPopMatrix();
        }
        
        //  Future Instructions
        ofSetColor(0,255,255);
        ofBeginShape();
        for(int i=0;i<instructions.size();i++){
            ofVertex(instructions[i]);
        }
        ofEndShape();
        
        ofPopStyle();
        ofPopMatrix();
        unlock();
    }
}