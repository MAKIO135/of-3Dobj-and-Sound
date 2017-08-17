/**
 *
 * OFDevCon Example Code Sprint
 * Model Distort Example
 *
 * This example loads a model and distorts it using noise
 *
 * The model is the open source and freely licensed balloon dog by Rob Myers, commissioned by furtherfield:
 * http://www.furtherfield.org/projects/balloon-dog-rob-myers
 *
 * Created by James George for openFrameworks workshop at Waves Festival Vienna sponsored by Lichterloh and Pratersauna
 * Adapted during ofDevCon on 2/23/2012
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate( 60 );
	ofSetVerticalSync( true );
	ofBackground( 50, 50, 50, 0 );

	//we need to call this for textures to work on models
    ofDisableArbTex();
	
	//this makes sure that the back of the model doesn't show through the front
	ofEnableDepthTest();

	//now we load our model
	model.loadModel( "monster-animated-character-X.X" );
	model.setPosition( ofGetWidth() * .5, ofGetHeight() * 0.75, 0 );
	
	//compute center
	mesh = model.getMesh( 0 );
	vector<ofVec3f>& verts = mesh.getVertices();
	for ( unsigned int i = 0; i < verts.size(); i++ ) {
		if ( verts[ i ].x < min.x ) min.x = verts[ i ].x;
		else if ( verts[ i ].x > max.x ) max.x = verts[ i ].x;

		if ( verts[ i ].y < min.y ) min.y = verts[ i ].y;
		else if ( verts[ i ].y > max.y ) max.y = verts[ i ].y;

		if ( verts[ i ].z < min.z ) min.z = verts[ i ].z;
		else if ( verts[ i ].z > max.z ) max.z = verts[ i ].z;
	}

	center = ofVec3f( ( min.x + max.x ) / 2.0, ( min.y + max.y ) / 2.0, ( min.z + max.z ) / 2.0 );
	
	light.enable();
    light.setPosition( model.getPosition() + ofPoint( 0, 0, 1600 ) );

	beat.load( "jdee_beat.mp3" );
	fftSmoothed = new float[ 8192 ];
	for ( int i = 0; i < 8192; i++ ) {
		fftSmoothed[ i ] = 0;
	}
	nBandsToGet = 128;
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSoundUpdate();

	float * val = ofSoundGetSpectrum( nBandsToGet );		// request 128 values for fft
	for ( int i = 0; i < nBandsToGet; i++ ) {
		// let the smoothed calue sink to zero:
		fftSmoothed[ i ] *= 0.7f;
		// take the max, either the smoothed or the incoming:
		if ( fftSmoothed[ i ] < val[ i ] ) fftSmoothed[ i ] = val[ i ];
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255, 255);

	//get the model attributes we need
	ofVec3f scale = model.getScale();
	ofVec3f position = model.getPosition();
	float normalizedScale = model.getNormalizedScale();

	ofTexture texture;
    ofxAssimpMeshHelper& meshHelper = model.getMeshHelper( 0 );
    bool bHasTexture = meshHelper.hasTexture();
    if( bHasTexture ) {
        texture = model.getTextureForMesh(0);
    }
    
	ofMaterial material = model.getMaterialForMesh(0);
	
    ofPushMatrix();
	
	//translate and scale based on the positioning. 
	ofTranslate(position);
	ofTranslate( 0, -200, 0 );
	ofRotate(-ofGetMouseX(), 0, 1, 0);
	ofRotate(90,-1,0,0);

	
	ofScale(normalizedScale, normalizedScale, normalizedScale);
	ofScale(scale.x,scale.y,scale.z);
	
	//modify mesh with some noise
	mesh = model.getMesh( 0 );
	float liquidness = 5;
	float amplitude = mouseY/100.0;
	float speedDampen = 5;		
	vector<ofVec3f>& verts = mesh.getVertices();
	for(unsigned int i = 0; i < verts.size(); i++){
		float s = ofMap( verts[ i ].x, min.x, max.x, -1, 1 );
		int n = ofMap( abs( s ), 1, 0, 0, 10 - 1 );
		verts[ i ] = center + ( verts[ i ] - center ) * ofMap( fftSmoothed[ n ], 0, 1, 1, 1.5 );
	}
		
	//draw the model manually
	if(bHasTexture) texture.bind();
	material.begin();
	//mesh.drawWireframe(); //you can draw wireframe too
	mesh.drawFaces();
	material.end();
	if(bHasTexture) texture.unbind();
	
	ofPopMatrix();


	ofSetColor( 255, 255, 255, 255 );

	float width = ( float ) ( 5 * 128 ) / nBandsToGet;
	for ( int i = 0; i < nBandsToGet; i++ ) {
		ofDrawRectangle( 100 + i*width, ofGetHeight() - 100, width, -( fftSmoothed[ i ] * 200 ) );
	}

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
	beat.play();
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
