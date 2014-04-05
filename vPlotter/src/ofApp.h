#pragma once
#include "ofMain.h"

#include "ofxOsc.h"
#include "ofxSvg.h"
#include "vPlotter.h"

#include "FtfMirror.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

#ifdef TARGET_RASPBERRY_PI
    FrameBufferCopier fbcp;
#endif
    
    ofxOscReceiver  receiver;
    
    vPlotter plotter;
    vector<ofPolyline> shape;
    ofRectangle printArea;
    
    float   stepDelay,penDelay;
    float   scale,rotate;
    
    int     degPenUp,degPenDown;
    int     mmMotorsDistance,mmPulleyRadius,stepsPerRotation;
    int     oscPort;
    
    string  file;
    bool    bFit;
    bool    sysGpio;
};
