#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
				
		void audioIn(ofSoundBuffer & input);
	
		vector <float> left;
		vector <float> right;
		vector <float> volHistory;
		
		int 	bufferCounter;
		int 	drawCounter;
		
		float smoothedVol;
		float scaledVol;
		
		ofSoundStream soundStream;


		// GUI

		void circleResolutionChanged(int & circleResolution);
		//void ringButtonPressed();
		void checkSound(int & soundIn);

		bool bHide;
		ofxFloatSlider radius;
		ofxToggle filled;

		ofxPanel gui;
};
