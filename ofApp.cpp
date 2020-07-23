#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (int k = 0; k < 8; k++) {

		auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
		vector<glm::vec3> log_list;
		for (int i = 0; i < 30; i++) {

			auto location = glm::vec3(
				ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i) * 0.005), 0, 1, -350, 350),
				ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i) * 0.005), 0, 1, -350, 350),
				0);

			log_list.push_back(location);
		}

		if (ofGetFrameNum() % 10 == 0) {

			auto direction = log_list[log_list.size() - 1] - log_list[log_list.size() - 2];
			auto theta = atan2(direction.y, direction.x);
			auto param = glm::length(direction) * 0.5;

			this->ripple_location_list.push_back(log_list[log_list.size() - 1]);
			this->ripple_radius_list.push_back(0);
			this->ripple_param_list.push_back(param);
			this->ripple_theta_list.push_back(theta);
			this->ripple_life_list.push_back(255);
		}

		ofNoFill();
		for (int i = log_list.size() - 1; i > 0; i--) {

			ofSetColor(ofMap(i, 0, log_list.size(), 239, 39));
			ofDrawLine(log_list[i], log_list[i - 1]);
		}

		ofFill();
		ofSetColor(39);
		ofDrawSphere(log_list[log_list.size() - 1], 2);
	}

	for (int i = this->ripple_location_list.size() - 1; i >= 0; i--) {

		this->ripple_radius_list[i] += this->ripple_param_list[i];
		this->ripple_life_list[i] -= 10;

		if (this->ripple_life_list[i] < 0) {

			this->ripple_location_list.erase(this->ripple_location_list.begin() + i);
			this->ripple_radius_list.erase(this->ripple_radius_list.begin() + i);
			this->ripple_param_list.erase(this->ripple_param_list.begin() + i);
			this->ripple_theta_list.erase(this->ripple_theta_list.begin() + i);
			this->ripple_life_list.erase(this->ripple_life_list.begin() + i);

			continue;
		}

		ofNoFill();
		ofSetColor(ofMap(this->ripple_life_list[i], 0, 255, 239, 39));
		ofBeginShape();
		for (int deg = 0; deg < 360; deg += 3) {

			auto vertex = glm::vec3(this->ripple_radius_list[i] * cos(deg * DEG_TO_RAD), this->ripple_radius_list[i] * sin(deg * DEG_TO_RAD), 0);
			auto rotation_x = glm::rotate(glm::mat4(), (float)(PI * 0.5), glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), (float)(this->ripple_theta_list[i] + PI * 0.5), glm::vec3(0, 1, 0));
			vertex = this->ripple_location_list[i] + glm::vec4(vertex, 0) * rotation_y * rotation_x;

			ofVertex(vertex);
		}
		ofEndShape(true);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}