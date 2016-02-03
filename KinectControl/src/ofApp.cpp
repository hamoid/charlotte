#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFullscreen(true);
    world.init();
    world.createBounds();
    world.registerGrabbing();
    world.setGravity(0, 0 );
    
    mCircle = shared_ptr< ofxBox2dCircle >( new ofxBox2dCircle );
    mCirc   = shared_ptr< ofxBox2dCircle >( new ofxBox2dCircle );
    mCircle->setPhysics(3.0, 0.53, 0.1);
    mCirc->setPhysics(3.0, 0.53, 0.1);
    mCircle->setup( world.getWorld(), ofGetWidth()/2, ofGetHeight()/2, 20 );
    mCirc->setup( world.getWorld(), ofGetWidth()/2, ofGetHeight()/2, 20 );
    
    for( int i = 0; i < 100; i++ )
    {
        ballVec.push_back( shared_ptr< ofxBox2dRect >( new ofxBox2dRect ) );
        ballVec.back()->setPhysics(3.0, 0.9, 0.0);
        ballVec.back()->setup( world.getWorld(), ofGetWidth()/2, ofGetHeight()/2, 20, 20 );
    }
    
    kinect.init(false, false); // disable video image (faster fps)
    kinect.open();
    kinect.update();
    grayImage.allocate(kinect.width, kinect.height);
    
    scaleX  = (float)ofGetWidth() / (float)kinect.width;
    scaleY  = (float)ofGetHeight() / (float)kinect.height;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    grayImage.setFromPixels(kinect.getDepthPixels());
    grayImage.threshold(100);
    
    
    contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
    auto blobs =   contourFinder.blobs;

    controlRects.clear();
    for ( auto & blob : blobs )
    {
        controlRects.push_back( shared_ptr< ofxBox2dRect >( new ofxBox2dRect ) );
        controlRects.back()->setPhysics(3.0, 0.53, 0.1);
        controlRects.back()->setup( world.getWorld(),
                                   blob.boundingRect.getX() * scaleX,
                                   blob.boundingRect.getY() * scaleY,
                                   blob.boundingRect.getWidth() * scaleX,
                                   blob.boundingRect.getHeight() * scaleY );
    }
    world.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground( ofColor::black );
//    kinect.drawDepth(0, 0, ofGetWidth(), ofGetHeight() );
//    kinect.drawDepth(0, 0 );

    mCircle->draw();
    mCirc->draw();
    for ( auto& ballPtr : ballVec )
        ballPtr->draw();
    
    for ( auto& ballPtr : controlRects )
        ballPtr->draw();
//    contourFinder.draw(0, 0, ofGetWidth(), ofGetHeight() );
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
//--------------------------------------------------------------
void ofApp::exit(){
    mCirc.reset();
    mCircle.reset();
    for( auto& ballPtr : ballVec )
        ballPtr.reset();

}
