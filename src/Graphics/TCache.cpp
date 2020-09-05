#include "TCache.h"

TCache::TCache() {
    for (i32 i = 0; i < (i32)OBDEnums::OpenGLIDs::PARAMS; i++) {
        parameterIDs.push_back(0);
    }
    lights = new std::vector<glslLight>();
	matrixStack.push(glm::mat4(1.0f));
}

TCache::~TCache() {
    lights->clear();
    delete lights;
    lights = nullptr;
}

std::vector<GLuint> TCache::generateAllIDs(GLuint programID) {
    GLuint paramIDs[OBDEnums::OpenGLIDs::PARAMS];

	// Get a handle for our "MVP" uniform
    paramIDs[OBDEnums::OpenGLIDs::MATRIX_PROJECTION] = glGetUniformLocation(programID, "P");
	paramIDs[OBDEnums::OpenGLIDs::MATRIX_VIEW] = glGetUniformLocation(programID, "V");
	paramIDs[OBDEnums::OpenGLIDs::MATRIX_MODEL] = glGetUniformLocation(programID, "M");
	paramIDs[OBDEnums::OpenGLIDs::MATRIX_MV] = glGetUniformLocation(programID, "MV");
	paramIDs[OBDEnums::OpenGLIDs::MATRIX_MVP] = glGetUniformLocation(programID, "MVP");

	paramIDs[OBDEnums::OpenGLIDs::BUFFER_LIGHT] = glGetUniformBlockIndex(programID, "LightBuffer");
	paramIDs[OBDEnums::OpenGLIDs::BUFFER_MATERIAL] = glGetUniformBlockIndex(programID, "MaterialBuffer");
	paramIDs[OBDEnums::OpenGLIDs::BUFFER_TEXTURE] = glGetUniformBlockIndex(programID, "TextureBuffer");

	glUniformBlockBinding(programID, paramIDs[OBDEnums::OpenGLIDs::BUFFER_LIGHT], 1);
	glUniformBlockBinding(programID, paramIDs[OBDEnums::OpenGLIDs::BUFFER_MATERIAL], 2);
	glUniformBlockBinding(programID, paramIDs[OBDEnums::OpenGLIDs::BUFFER_TEXTURE], 3);

	paramIDs[OBDEnums::OpenGLIDs::LIGHT_AMOUNT] = glGetUniformLocation(programID, "lightAmount");
	paramIDs[OBDEnums::OpenGLIDs::CAMERA_POSITION] = glGetUniformLocation(programID, "cameraPosition");

	paramIDs[OBDEnums::OpenGLIDs::SAMPLER_DIFFUSE] = glGetUniformLocation(programID, "textureDiffuse");
    paramIDs[OBDEnums::OpenGLIDs::SAMPLER_OCLUSIONS] = glGetUniformLocation(programID, "textureOclusions");
	paramIDs[OBDEnums::OpenGLIDs::SAMPLER_SPECULAR] = glGetUniformLocation(programID, "textureSpecular");
	paramIDs[OBDEnums::OpenGLIDs::SAMPLER_ALPHA] = glGetUniformLocation(programID, "textureAlpha");

	// Lights
	glGenBuffers(1, &lightsID);
	glBindBuffer(GL_UNIFORM_BUFFER, lightsID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glslLight) * MAX_LIGHTS, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return std::vector<GLuint>(paramIDs, paramIDs + OBDEnums::OpenGLIDs::PARAMS);
}

void TCache::setAllIDs(std::vector<GLuint> v) {
    parameterIDs = v;
}

void TCache::setID(OBDEnums::OpenGLIDs s, GLuint i) {
    parameterIDs[(i32)s] = i;
}

GLuint TCache::getID(OBDEnums::OpenGLIDs s) {
    return parameterIDs[(i32)s];
}

void TCache::setViewMatrix(glm::mat4 m) {
    viewMatrix = m;
}

glm::mat4 *TCache::getViewMatrix() {
    return &viewMatrix;
}

void TCache::setProjectionMatrix(glm::mat4 m) {
    projectionMatrix = m;
}

glm::mat4 *TCache::getProjectionMatrix() {
    return &projectionMatrix;
}

void TCache::setCameraPosition(glm::vec3 m) {
    cameraPosition = m;
}

glm::vec3 TCache::getCameraPosition() {
    return cameraPosition;
}

GLuint TCache::getLightsBuffer(){
	return lightsID;
}

std::vector<glslLight> *TCache::getLights() {
    return lights;
}

std::stack<glm::mat4>& TCache::getMatrixStack() {
    return matrixStack;
}

void TCache::setScreenSize(glm::vec2 s){
	screenSize = s;
}

glm::vec2 TCache::getScreenSize(){
	return screenSize;
}