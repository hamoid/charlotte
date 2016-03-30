#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFullscreen( true );
    ofHideCursor();
    
    sensorKinect_.init(false, false);
    sensorKinect_.open();
    sensorKinect_.update();
    
    string xmlConfigFileName = "Settings.txt";
    
    if( configXML.loadFile( xmlConfigFileName ) ){
        ofLogVerbose() << "XML file loaded";
    }else{
        ofLogError() << "While loading XML file called /data/" << xmlConfigFileName;
    }
    
    
    nearMax   = configXML.getValue( "MIN", 245);
    farMin    = configXML.getValue( "MAX", 255);
    ofLogNotice() << "Min " << nearMax;
    ofLogNotice() << "Max " << farMin;
    
    numPixels_ = sensorKinect_.height * sensorKinect_.width;
    grayImage_.allocate( sensorKinect_.width, sensorKinect_.height );
}

//--------------------------------------------------------------
void ofApp::update(){
    sensorKinect_.update();

    //    mImage_.setFromPixels( sensorKinect_.getDepthPixels() );
    grayImage_.setFromPixels( sensorKinect_.getDepthPixels() );
    
    ofPixels   &pix         = grayImage_.getPixels() ;
    int         numPixels   = pix.size();
    
    for(int i = 0; i < numPixels; i++) {
        if( pix[i] > nearMax && pix[i] < farMin ) {
            pix[i] = 255;
        }
            else {
            pix[i] = 0;
        }
    }
    grayImage_.flagImageChanged();

//    mImage_.setFromPixels( pix );
}

//--------------------------------------------------------------
void ofApp::draw(){

//    sensorKinect_.drawDepth(0, 0, ofGetWidth(), ofGetHeight() );
//    mImage_.draw(0, 0, ofGetWidth(), ofGetHeight() );
    grayImage_.draw(0, 0, ofGetWidth(), ofGetHeight() );
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
