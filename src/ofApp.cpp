#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 
	
	ofSetVerticalSync(true);
    ofBackgroundHex(0x000000);

    // GUI 
    gui.setup("PYRAMIDAL LIGHT"); // most of the time you don't need a name
	gui.add(filled.setup("fill", true));
	gui.add(radius.setup("radius", 250, 1, 2500));
    bHide = false;
	
    // AUDIO-IN
	soundStream.printDeviceList();
	
	int bufferSize = 256;

	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
	
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;


	ofSoundStreamSettings settings;

	// if you want to set the device id to be different than the default
	// auto devices = soundStream.getDeviceList();
	// settings.device = devices[4];

	// you can also get devices for an specific api
	// auto devices = soundStream.getDevicesByApi(ofSoundDevice::Api::PULSE);
	// settings.device = devices[0];

	// or get the default device for an specific api:
	// settings.api = ofSoundDevice::Api::PULSE;

	// or by name
	auto devices = soundStream.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setInDevice(devices[0]);
	}

	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 2;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);

}

//--------------------------------------------------------------
void ofApp::update(){
	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

	//lets record the volume into an array
	volHistory.push_back( scaledVol );
	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}

    ofSetCircleResolution(90);
}

//--------------------------------------------------------------
void ofApp::draw(){
	
    //ofBackgroundGradient(ofColor::white, ofColor::gray);

	// ofFill();
    // ofSetColor(0, 0, 0, 21); 
    // ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    ofFill();
    ofSetColor(0,0,0,11);
    ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());

	ofNoFill();

    
	// draw the average volume:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(0, 0, 0);
			
		ofSetColor(225);
		//ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
		ofDrawRectangle(50, 50, ofGetWidth()-100, ofGetHeight()-100);
		
        if(filled){
		    ofFill();
	    } else {
		    ofNoFill();
	    }
		//ofSetColor(245, 58, 135);	
		ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, (scaledVol*3) * radius);
			
		ofPopMatrix();
	ofPopStyle();
	
	drawCounter++;

    if(!bHide){
		gui.draw();
	}
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input){
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;	

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (size_t i = 0; i < input.getNumFrames(); i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :) 
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
    
    if(key == 'h'){
		bHide = !bHide;
	}

	if( key == 's' ){
		soundStream.start();
	}
	
	if( key == 'e' ){
		soundStream.stop();
	}
}