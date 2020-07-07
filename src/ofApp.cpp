#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofDisableAlphaBlending();
    ofEnableArbTex();

    cam.setupPerspective();
    
    cap.setup(ofGetWidth(), ofGetHeight());

    render.load("shaders/render");
    updatePos.load("","shaders/update.frag");
    
    image.load("EPcPI9mU8AArjyK.jpeg");
    
    
    // パーティクルの初期設定
    particles.setMode(OF_PRIMITIVE_POINTS);
    for(int i=0;i<ofGetHeight();i++){
        for(int j=0;j<ofGetWidth();j++){
                particles.addVertex(ofVec3f(0,0,0));
                particles.addTexCoord(ofVec2f(j, i));
                particles.addColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
        }
    }
    
    
    //pingPong初期設定
    pingPong.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F, 2);
	float * posAndAge = new float[ofGetWidth() * ofGetHeight() * 4];
    for (int x = 0; x < ofGetWidth(); x++){
        for (int y = 0; y < ofGetHeight(); y++){
            int i = ofGetWidth() * y + x;
            posAndAge[i*4 + 0] = x;
            posAndAge[i*4 + 1] = y;
            posAndAge[i*4 + 2] = 0;
            posAndAge[i*4 + 3] = 0;
        }
    }
    pingPong.src->getTextureReference(0).loadData(posAndAge, ofGetWidth(), ofGetHeight(), GL_RGBA);
    delete [] posAndAge;
    
    
	float * velAndMaxAge = new float[ofGetWidth() * ofGetHeight() * 4];
    for (int x = 0; x < ofGetWidth(); x++){
        for (int y = 0; y < ofGetHeight(); y++){
            int i = ofGetWidth() * y + x;
            velAndMaxAge[i*4 + 0] = 0.0;
            velAndMaxAge[i*4 + 1] = 0.0;
            velAndMaxAge[i*4 + 2] = 0.0;
            velAndMaxAge[i*4 + 3] = ofRandom(1,150);
        }
    }
    pingPong.src->getTextureReference(1).loadData(velAndMaxAge, ofGetWidth(), ofGetHeight(), GL_RGBA);
    delete [] velAndMaxAge;

    
	showTex = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    cap.update();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    float time = ofGetElapsedTimef();

    pingPong.dst->begin();
    // 複数バッファの書き出しを有効化
    pingPong.dst->activateAllDrawBuffers();
    ofClear(0);

    updatePos.begin();

    updatePos.setUniformTexture("u_posAndAgeTex", pingPong.src->getTextureReference(0), 0);
    updatePos.setUniformTexture("u_velAndMaxAgeTex", pingPong.src->getTextureReference(1), 1);
    updatePos.setUniform1f("u_time", time);
    updatePos.setUniform1f("u_timestep", 0.05);
    updatePos.setUniform1f("u_scale", 0.001);
    pingPong.src->draw(0, 0);

    updatePos.end();

    pingPong.dst->end();
    pingPong.swap();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushStyle();
    
    render.begin();
    render.setUniformTexture("u_posAndAgeTex", pingPong.src->getTextureReference(0), 0);
    render.setUniformTexture("capTex", cap.getTexture(), 1);
    render.setUniformTexture("imgTex", image.getTexture(), 2);
    render.setUniform2f("resolution",glm::vec2(ofGetWidth(),ofGetHeight()));


    particles.draw();
    
    render.end();
    
    ofPopStyle();
    
    if(showTex){
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        pingPong.dst->getTextureReference(0).draw(0,0);
        ofDrawBitmapStringHighlight("Position", 0,14);
        pingPong.dst->getTextureReference(1).draw(0,0);
        ofDrawBitmapStringHighlight("Velocity", 0,14);
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key==' '){
        showTex = !showTex;
    }
    
    if(key=='f'){
        ofToggleFullscreen();
    }
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
