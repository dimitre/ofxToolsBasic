#include "polar.h"

//	float pointsPerMeter = 100.0;
const float pointsPerMeter = 1.0;

//ofCamera camera3d;
ofEasyCam camera3d;
glm::vec3 cameraLook3d;
glm::vec3 cameraLookUp = { 0.f, 1.0f, 0.0f };

ofNode lookNode;

ofxMicroUI *uiCam = &u.uis["cam"];

// precisa vir antes do loadMaster pra poder fazer os presets antes.
void setupCam_3d() {
	ofAddListener(uiCam->uiEvent, this, &ofApp::dmtrCamUIEvent_3d);
}


void begin3d() {
//	glShadeModel(uiLuz->pBool["shadeFlat"] ? GL_FLAT : GL_SMOOTH);
	if (uiC->pBool["is3d"]) {
		beginCam_3d();
	}
}

void end3d() {
	if (uiC->pBool["is3d"]) {
		endCam_3d();
	}
}


//--------------------------------------------------------------
void beginCam_3d(float eyeDistance = 0) {
	
	if (uiCam->pBool["disableDepthTest"]) {
		ofDisableDepthTest();
	} else {
		ofEnableDepthTest();
	}

	float cameraX = uiCam->pEasy["cameraX"];
	float cameraZ = uiCam->pEasy["cameraZ"];
	
	camera3d.setNearClip(0.01);
	camera3d.setFarClip(1600);

	// 1.70, a altura de um adulto em p�

	if (uiCam->pBool["cameraPolar"]) {
		float a = uiCam->pEasy["cameraAngle"] + 90.0;
		float d = uiCam->pEasy["cameraDist"];
		cameraX = r2x(a, d);
		cameraZ = r2y(a, d);
		cameraX += lookNode.getPosition().x;
		cameraZ += lookNode.getPosition().z;
	}
	
	cameraX += eyeDistance;
	
	if (!u.pBool["mouseCamera"]) {
		camera3d.setPosition(cameraX,
						   uiCam->pEasy["cameraY"],
						   cameraZ);
		
		cameraLook3d = glm::vec3(uiCam->pEasy["lookX"],
							   uiCam->pEasy["lookY"],
							   uiCam->pEasy["lookZ"]);
		
		lookNode.setPosition(cameraLook3d);
		
//		cameraLookPosition3d = glm::vec3(uiCam->pEasy["lookX"],
//									   (uiCam->pEasy["lookY"]+100),
//									   uiCam->pEasy["lookZ"]);
		
		camera3d.lookAt(lookNode, cameraLookUp);
		//camera3d.lookAt( cameraLook3d, cameraLookPosition3d );
	}

	camera3d.setFov(uiCam->pEasy["cameraFov"]);
	camera3d.begin();
	ofPushMatrix();
	
	float rotX = uiCam->pEasy["accelX"];
	float rotY = uiCam->pEasy["accelY"];
	float rotZ = uiCam->pEasy["accelZ"];
	// float rotation = ofGetFrameNum()/9.0f;
	
	if (uiCam->pBool["rotCam"]) {
		uiCam->pFloat["rotX_accum"] += uiCam->pFloat["rotCamXAuto"];
		uiCam->pFloat["rotY_accum"] += uiCam->pFloat["rotCamYAuto"];
		uiCam->pFloat["rotZ_accum"] += uiCam->pFloat["rotCamZAuto"];
	}

	ofRotateXDeg(rotX + uiCam->pEasy["rotCamX"] + uiCam->pFloat["rotX_accum"]);
	ofRotateYDeg(rotY + uiCam->pEasy["rotCamY"] + uiCam->pFloat["rotY_accum"]);
	ofRotateZDeg(rotZ + uiCam->pEasy["rotCamZ"] + uiCam->pFloat["rotZ_accum"]);
}

//--------------------------------------------------------------
void endCam_3d() {
	ofPopMatrix();
	camera3d.end();
	ofDisableDepthTest();
}


//--------------------------------------------------------------
void dmtrCamUIEvent_3d(ofxMicroUI::element & e) {
	if (!e._settings->presetIsLoading) {

//			cout << e.name << endl;
		if (e.name == "resetLook") {
			uiCam->getSlider("lookX")->set(0.0);
//			uiCam->getSlider("lookY")->set(1.6);
			uiCam->getSlider("lookY")->set(0.0);
			uiCam->getSlider("lookZ")->set(0.0);
		}
		
		else if (e.name == "resetAutoRotate") {
			uiCam->pFloat["rotX_accum"] = uiCam->pFloat["rotY_accum"] = uiCam->pFloat["rotZ_accum"] = 0;
			//uiCam->pEasy["rotX_accum"] = uiCam->pEasy["rotY_accum"] = uiCam->pEasy["rotZ_accum"] = 0;
		}

		else if (e.name == "resetCam") {
			uiCam->pFloat["rotX_accum"] = uiCam->pFloat["rotY_accum"] = uiCam->pFloat["rotZ_accum"] = 0;
			//uiCam->pEasy["rotX_accum"] = uiCam->pEasy["rotY_accum"] = uiCam->pEasy["rotZ_accum"] = 0;

			uiCam->getSlider("rotCamX")->set(0);
			uiCam->getSlider("rotCamY")->set(0);
			uiCam->getSlider("rotCamZ")->set(0);

			uiCam->getSlider("rotCamXAuto")->set(0);
			uiCam->getSlider("rotCamYAuto")->set(0);
			uiCam->getSlider("rotCamZAuto")->set(0);
		}
	}
		
	if (e.name == "ortho") {
		if (*e.b) {
			camera3d.enableOrtho();
		} else {
			camera3d.disableOrtho();
		}
	}
}
