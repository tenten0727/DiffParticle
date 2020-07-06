#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofDisableAlphaBlending();
    
    cam.setupPerspective();
    
    cap.setup(ofGetWidth(), ofGetHeight());
    
    // レンダリング用シェーダーの読み込み

    render.load("shaders/render");

    // アップデート用シェーダーの読み込み

    updatePos.load("","shaders/update.frag");
    
    fboParticle.allocate(ofGetWidth(), ofGetHeight());
    
    // パーティクルの初期設定

    particles.setMode(OF_PRIMITIVE_POINTS);
    for(int i=0;i<ofGetHeight();i++){
        for(int j=0;j<ofGetWidth();j++){
                particles.addVertex(ofVec3f(0,0,0));
                particles.addTexCoord(ofVec2f(j, i)); // Fboのテクスチャー内で、データを読み出す位置を設定
                particles.addColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
        }
    }
    
    // パーティクルの座標・加速度の保存用Fbo
    // RGBA32Fの形式で2つのColorbufferを用意

    pingPong.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F, 2);
    
    // パーティクルの位置と経過時間の初期設定
    
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
    //pingPongBufferに初期値を書き込み

    pingPong.src->getTextureReference(0).loadData(posAndAge, ofGetWidth(), ofGetHeight(), GL_RGBA);
    delete [] posAndAge;
    
    // パーティクルの速度と生存期間の初期設定
    
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

    //pingPongBufferに初期値を書き込み

    pingPong.src->getTextureReference(1).loadData(velAndMaxAge, ofGetWidth(), ofGetHeight(), GL_RGBA);
    delete [] velAndMaxAge;

	showTex = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    cap.update();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    float time = ofGetElapsedTimef();
    
    // パーティクルの発生位置を更新

    prevEmitterPos = emitterPos;
    emitterPos = 300 * ofVec3f(ofSignedNoise(time, 0, 0),ofSignedNoise(0, time, 0),ofSignedNoise(0, 0, time));
    
    // パーティクルの位置を計算

    pingPong.dst->begin();

    // 複数バッファの書き出しを有効化

    pingPong.dst->activateAllDrawBuffers();
    ofClear(0);
    updatePos.begin();

    // パーティクルの位置と経過時間

    updatePos.setUniformTexture("u_posAndAgeTex", pingPong.src->getTextureReference(0), 0);

    // パーティクルの速度と生存期間

    updatePos.setUniformTexture("u_velAndMaxAgeTex", pingPong.src->getTextureReference(1), 1);
    updatePos.setUniform1f("u_time", time);
    updatePos.setUniform1f("u_timestep", 0.05);
    updatePos.setUniform1f("u_scale", 0.005);
    updatePos.setUniform3f("u_emitterPos", emitterPos.x, emitterPos.y, emitterPos.z);
    updatePos.setUniform3f("u_prevEmitterPos", prevEmitterPos.x, prevEmitterPos.y, prevEmitterPos.z);
    pingPong.src->draw(0, 0);
    updatePos.end();
    pingPong.dst->end();
    pingPong.swap();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushStyle();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();
//    fboParticle.begin();
//    ofClear(0);
    render.begin();
    
    // パーティクルの位置と経過時間
    render.setUniformTexture("u_posAndAgeTex", pingPong.src->getTextureReference(0), 0);
    render.setUniformTexture("capTex", cap.getTexture(), 1);
    particles.draw();
    render.end();
//    fboParticle.end();
    
    ofDisablePointSprites();
    ofPopStyle();
    
    if(showTex){
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        pingPong.dst->getTextureReference(0).draw(0,0);
        ofDrawBitmapStringHighlight("Position", 0,14);
//        pingPong.dst->getTextureReference(1).draw(0,0);
//        ofDrawBitmapStringHighlight("Velocity", 0,14);
        ofPopStyle();
    }
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 0,ofGetHeight() - 2);
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
