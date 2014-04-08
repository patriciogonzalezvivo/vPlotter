#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    // PLOTTER
    //
    plotter.setup(degPenUp,degPenDown,mmMotorsDistance,mmPulleyRadius,stepsPerRotation,sysGpio);
    plotter.stepDelay   = stepDelay;
    plotter.penDelay    = penDelay;
    printArea = plotter.getPrintingArea();
    
    if(oscPort > 0){
        receiver.setup(oscPort);
    } else {
        //  INPUT FILE
        //
        if(!ofFile(file).exists()){
            exit();
        }
        ofxSVG  svg;
        svg.load(file);
        for (int i = 0; i < svg.getNumPath(); i++){
            ofPath p = svg.getPathAt(i);
            // svg defaults to non zero winding which doesn't look so good as contours
            p.setPolyWindingMode(OF_POLY_WINDING_ODD);
            vector<ofPolyline>& lines = p.getOutline();
            for(int j=0;j<(int)lines.size();j++){
                shape.push_back(lines[j].getResampledBySpacing(1));
            }
        }
    
        if(bFit){
            float minX = 1000000000;
            float minY = 1000000000;
            float maxX = 0;
            float maxY = 0;
            for (int i=0; i < shape.size(); i++) {
                for (int j=0; j<shape[i].size(); j++) {
                    ofPoint pos = shape[i][j];
                    if(pos.x < minX)minX = pos.x;
                    if(pos.y < minY)minY = pos.y;
                    if(pos.x > maxX)maxX = pos.x;
                    if(pos.y > maxY)maxY = pos.y;
                }
            }
            float w = (maxX-minX);
            float h = (maxY-minY);
            float ratio = 1;
            ofRectangle area = printArea;
            ratio = w/h;
            //        if(w>h){
            //            area.setFromCenter(printArea.getCenter(), printArea.width, printArea.width*ratio);
            //        } else {
            area.setFromCenter(printArea.getCenter(), printArea.height*ratio, printArea.height);
            //        }
            for (int i=0; i < shape.size(); i++) {
                for (int j=0; j<shape[i].size(); j++) {
                    ofPoint pos = shape[i][j];
                    shape[i][j].x = ofMap(pos.x, minX, maxX, area.x, area.x+area.width);
                    shape[i][j].y = ofMap(pos.y, minY, maxY, area.y, area.y+area.height);
                }
            }
        } else {
            for (int i=0; i < shape.size(); i++) {
                for (int j=0; j<shape[i].size(); j++) {
                    shape[i][j] += printArea.getTopLeft();
                }
            }
        }
        
        if(scale != 100 || rotate != 0){
            ofPoint center = printArea.getCenter();
            float scl = ((float)(scale))/100.0f;
            float rot = ofDegToRad(rotate);
            for (int i=0; i < shape.size(); i++) {
                for (int j=0; j<shape[i].size(); j++) {
                    ofPoint toCenter = shape[i][j] - center;
                    
                    float rad = toCenter.length()*scl;
                    float ang = atan2(toCenter.y,toCenter.x)+rot;
                    shape[i][j].x = center.x+rad*cos(ang);
                    shape[i][j].y = center.y+rad*sin(ang);
                }
            }
        }
        
        //  PRINT
        //
        plotter.print(shape);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(oscPort > 0){
        while(receiver.hasWaitingMessages()){
            ofxOscMessage m;
            receiver.getNextMessage(&m);
            
            if(m.getAddress() == "/setup"){
                degPenUp = m.getArgAsInt32(0);
                degPenDown = m.getArgAsInt32(1);
                mmMotorsDistance = m.getArgAsInt32(2);
                mmPulleyRadius = m.getArgAsInt32(3);
                stepsPerRotation = m.getArgAsInt32(4);
                
                cout << "/setup " << degPenUp << " " << degPenDown << " " << mmMotorsDistance << " " << mmPulleyRadius << " " << stepsPerRotation << endl;
                plotter.setup(degPenUp,degPenDown,mmMotorsDistance,mmPulleyRadius,stepsPerRotation,sysGpio);
                printArea = plotter.getPrintingArea();
            } else if(m.getAddress() == "/move/abs"){
                ofPoint pos;
                pos.x = m.getArgAsFloat(0);
                pos.y = m.getArgAsFloat(1);
                
                if(printArea.inside(pos)){
                    plotter.print(MOVE_ABS, pos);
                }
                
            } else if(m.getAddress() == "/move/rel"){
                ofPoint pos;
                pos.x = m.getArgAsFloat(0);
                pos.y = m.getArgAsFloat(1);
                
                if(printArea.inside(pos)){
                    plotter.print(MOVE_REL, pos);
                }
            } else if(m.getAddress() == "/line/abs"){
                ofPoint pos;
                pos.x = m.getArgAsFloat(0);
                pos.y = m.getArgAsFloat(1);
                
                if(printArea.inside(pos)){
                    plotter.print(LINE_ABS, pos);
                }
            } else if(m.getAddress() == "/line/rel"){
                ofPoint pos;
                pos.x = m.getArgAsFloat(0);
                pos.y = m.getArgAsFloat(1);
                
                if(printArea.inside(pos)){
                    plotter.print(LINE_REL, pos);
                }
            } else if(m.getAddress() == "/stop"){
                cout << "/stop" << endl;
                ofExit();
            }
        }
    } else if(!plotter.isPrinting()){
        ofExit();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);

#ifdef TARGET_RASPBERRY_PI
    ofRectangle area = plotter.getPrintingArea();
    ofPushMatrix();
    ofRotateZ(-90);
    ofTranslate(-area.x,-area.y);
    ofTranslate(-area.width,0);
    
    
    ofSetColor(255);
    for (int i=0; i<shape.size(); i++) {
        shape[i].draw();
    }
    ofScale(2.0, 2.0);
    plotter.draw();
    ofPopMatrix();
    fbcp.Copy();
#else
    ofPushMatrix();
    ofScale(0.5, 0.5);
    ofSetColor(255);
    for (int i=0; i<shape.size(); i++) {
        shape[i].draw();
    }
    ofScale(2.0, 2.0);
    ofPopMatrix();
    plotter.draw();
#endif
}

void ofApp::exit(){
    plotter.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
