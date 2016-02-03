#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
    vector< shared_ptr< ofxBox2dRect > >  ballVec;
    vector< shared_ptr< ofxBox2dRect > >  controlRects;
    
    shared_ptr< ofxBox2dCircle >    mCircle, mCirc;
    ofxBox2d                        world;
    
    
    ofxKinect           kinect;
    ofxCvContourFinder contourFinder;
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    float       scaleX, scaleY;
};
