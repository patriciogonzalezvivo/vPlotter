//
//  UIvPlotter.cpp
//  09 - TraceMaker
//
//  Created by Patricio Gonzalez Vivo on 10/9/13.
//
//

#include "UIvPlotter.h"

UIvPlotter::UIvPlotter(){
    motorsDistance      = 500;//1549;
    pulleyRadius        = 5;
    stepsPerRotation    = 800;
    
    host                = "192.168.1.12";
    oscPort             = 101010;

    currentPos.set(0,0);
    
    steps.M1    = 0;
    steps.M2    = 0;
    
    target.M1   = 0;
    target.M2   = 0;
    
    penState    = PEN_UP;
    bPlotting   = false;
    
    checkHostValues();
    sender.setup(host, oscPort);
    
//    textField = NULL;
}

void UIvPlotter::setupUI(){
//    textField = gui->addTextInput("HOST", "", OFX_UI_FONT_SMALL);
    
    gui->addLabel("Config");
    gui->addIntSlider("motorsDistance_(mm)", 0, 2000, &motorsDistance);
    gui->addIntSlider("pulleyRadius_(mm)", 0, 50, &pulleyRadius);
    gui->addIntSlider("stepsPerRotation", 0, 1000, &stepsPerRotation);
    gui->addIntSlider("penUp_(deg)", 0, 180, &penUp);
    gui->addIntSlider("penDown_(deg)", 0, 180, &penDown);
    bool bFalse = false;
    gui->addButton("SYNC", bFalse);
    
    gui->addButton("CLOSE", bFalse);
}

void UIvPlotter::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if (name == "HOST"){
//        host = textField->getTextString();
        checkHostValues();
    } else if(name == "SYNC"){
        checkHostValues();
    } else if(name == "CLOSE"){
        ofxOscMessage m;
        m.setAddress("/stop");
        sender.sendMessage(m);
    } else {
        bUpdate = true;
    }
}

bool UIvPlotter::checkHostValues(){
    ofxJSONElement  response;
    
    std::string url = "http://"+host+":8080/status.json";
    
	if (!response.open(url)){
        cout  << "Failed to parse JSON\n" << endl;
    }
    
    penUp = ofToInt(response["options"]["penUpAngle"].asString());
    penDown = ofToInt(response["options"]["penDownAngle"].asString());
    motorsDistance = ofToInt(response["options"]["motorsDistance"].asString());
    pulleyRadius = ofToInt(response["options"]["pulleyRadius"].asString());
    stepsPerRotation = ofToInt(response["options"]["stepsPerRotation"].asString());
    
    bUpdate = true;
}

bool UIvPlotter::isHealthyAt(ofPoint _pos){
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

MotorVal UIvPlotter::getStepsFor(const ofPoint &_pos)const{
    MotorVal rta;
    rta.M1 = M1.distance(_pos)/m2s;
    rta.M2 = M2.distance(_pos)/m2s;
    return rta;
}

ofPoint UIvPlotter::getPosFor(const MotorVal &_steps)const{
    ofPoint rta;
    float a = _steps.M2 * m2s;
    float b = _steps.M1 * m2s;
    float c = motorsDistance;
    return calcPointB(a, b, c);
}

ofVec2f UIvPlotter::getTension(ofPoint _pos){
    float angleToM1 = atan2(_pos.y-M1.y,_pos.x-M1.x);
    float angleToM2 = atan2(_pos.y-M2.y,M2.x-_pos.x);
    float d = cos(angleToM1)*sin(angleToM2)+sin(angleToM1)*cos(angleToM2);
    float tensionToM1 = cos(angleToM2)/d;
    float tensionToM2 = cos(angleToM1)/d;
    return ofVec2f(tensionToM1,tensionToM2)-ofVec2f(0.5,0.5);
}

ofPoint UIvPlotter::calcPointB(float a, float b, float c)const{
    // law of cosines
    // http://en.wikipedia.org/wiki/Solution_of_triangles
    float angle = acos((b*b+c*c-a*a)/(2*b*c));
    return ofPoint(b*cos(angle)+M1.x,b*sin(angle)+M1.y);
}

ofVec2f UIvPlotter::getResolution(ofPoint _pos){
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

void UIvPlotter::print(ofPolyline _polyline){
    if(_polyline.size()>1){
        addInstruction(MOVE_ABS, _polyline.getVertices()[0]+ printingArea.getTopLeft());
        addInstruction(MOVE_ABS, _polyline.getVertices()[1]+ printingArea.getTopLeft());
    }
    
    for(int j = 0; j < _polyline.getVertices().size(); j++) {
        addInstruction(((j==0)?MOVE_ABS:LINE_ABS), _polyline.getVertices()[j] + printingArea.getTopLeft());
    }
    
    if (instructions.size() > 0) {
        addInstruction(MOVE_ABS, printingArea.getCenter());
        bPlotting = true;
    }
}

void UIvPlotter::print( vector<ofPolyline> _paths ){
    
    for(int i = 0; i < _paths.size(); i++) {
        if(_paths[i].size()>1){
            addInstruction(MOVE_ABS, _paths[i].getVertices()[0]+ printingArea.getTopLeft());
        }
        for(int j = 0; j < _paths[i].getVertices().size(); j++) {
            addInstruction( ((j==0)?MOVE_ABS:LINE_ABS), _paths[i].getVertices()[j] + printingArea.getTopLeft());
        }
    }
    
    if (instructions.size() > 0) {
        addInstruction(MOVE_ABS, printingArea.getCenter());
        bPlotting = true;
    }
}

bool UIvPlotter::addInstruction(Comand _command, ofPoint _pos){
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

void UIvPlotter::update(){
    if(bUpdate){
        m2s = (2 * PI * pulleyRadius) / stepsPerRotation;
        
        M1.set(0, 0);
        M2.set(motorsDistance, 0);
        
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
        
        ofxOscMessage m;
        m.setAddress("/setup");
        m.addIntArg(penUp);
        m.addIntArg(penDown);
        m.addIntArg(motorsDistance);
        m.addIntArg(pulleyRadius);
        m.addIntArg(stepsPerRotation);
        sender.sendMessage(m);
        
        bUpdate = false;
    }
    
    
    if(bEnable){
        if (bPlotting){
            if(instructions.size()>0){
                if (ofGetFrameNum()%2 == 0) {
                    if (exeInstruction(instructions[0])){
                        
                        ofxOscMessage m;
                        if(instructions[0].cmd==LINE_REL){
                            m.setAddress("/line/rel");
                        } else if(instructions[0].cmd==LINE_ABS){
                            m.setAddress("/line/abs");
                        } else if(instructions[0].cmd==MOVE_REL){
                            m.setAddress("/move/rel");
                        } else if(instructions[0].cmd==MOVE_ABS){
                            m.setAddress("/move/abs");
                        }
                        
                        m.addFloatArg(instructions[0].x);
                        m.addFloatArg(instructions[0].y);
                        sender.sendMessage(m);
                        
                        instructions.erase(instructions.begin());
                    }
                }
            } else {
                bPlotting = false;
            }
        }
    }
}

bool UIvPlotter::exeInstruction(Instruction _inst){
    MotorVal t, s;

    //  Set the right PEN
    //
    {
        switch (_inst.cmd) {
            case LINE_REL:
            case LINE_ABS:{
                if(penState == PEN_UP){
                    penState = PEN_DOWN;
                }
            }
                break;
            case MOVE_REL:
            case MOVE_ABS:{
                if(penState == PEN_DOWN){
                    penState = PEN_UP;
                }
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

    //  Make steps
    //
    while (s!=t){
        if( t.M1 - s.M1 != 0){
            s.M1 += dir.M1;
        }
        
        if(t.M2-s.M2 != 0){
            s.M2 += dir.M2;
        }
        steps = s;
        currentPos = getPosFor(s);
    }
    return true;
}

void UIvPlotter::draw(){
    if (bEnable) {
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(-printingArea.x, -printingArea.y);
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
    }
}