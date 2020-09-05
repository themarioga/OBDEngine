#include "OBDEngine.h"

#include "ResourceManager/ResourceOBJ.h"
#include "ResourceManager/ResourceGLSL.h"

TCache TEntity::cache = TCache();

OBDEngine::OBDEngine() {
    rootNode = new TNode();

    // Create default layers
    clSceneNode = new OBDSceneNode(rootNode);
    defaultSceneNode = new OBDSceneNode(rootNode);

    // Resource Manager
    OBDManager = new ResourceManager();
}

OBDEngine::~OBDEngine() {
    //ToDo: revisar destructor
    cameras.clear();
    lights.clear();

    delete rootNode;
}

void OBDEngine::Init(i32 sW, i32 sH) {
    setWindowSize(sW, sH);

	// Viewport
	viewport = glm::vec4(0.0f, 0.0f, windowWidth, windowHeight);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Enable textures
	glEnable(GL_TEXTURE_2D);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

    //Backface culling
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT, GL_FILL);
	
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Antialiasing
	glEnable(GL_MULTISAMPLE);
}

void OBDEngine::End() {
}

void OBDEngine::draw() {
    glUseProgram(TEntity::cache.getID(OBDEnums::OpenGLIDs::CURRENT_PROGRAM_ID));

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Async
	OBDManager->Update();

    // Draw our tree
    rootNode -> draw();

    // Clear light cache
    TEntity::cache.getLights()->clear();
}

#ifdef USEVIDEO
void OBDEngine::InitVideoSystem(){
	av_register_all();
	avformat_network_init();
}
#endif

OBDLight* OBDEngine::createLight(OBDColor color, f32 intensity, f32 ambient, f32 attenuation) {
    OBDLight* lightNode = new OBDLight(clSceneNode, color, intensity, ambient, attenuation);
    lights.push_back(lightNode);
    return lightNode;
}

OBDCamera* OBDEngine::createCamera(f32 far, f32 fov) {
    OBDCamera* cameraNode = new OBDCamera(clSceneNode, windowWidth, windowHeight, far, fov);
    cameras.push_back(cameraNode);
    return cameraNode;
}

OBDSceneNode* OBDEngine::createSceneNode() {
    return new OBDSceneNode(defaultSceneNode);
}

OBDSceneNode* OBDEngine::createSceneNode(OBDSceneNode* layer) {
    return new OBDSceneNode(layer);
}

OBDObject* OBDEngine::createObject(u32 id, std::string mesh, bool autoload) {
    ResourceOBJ *obj = (ResourceOBJ*)OBDManager->getResource(mesh, true);
    ResourceMTL *mtl = (ResourceMTL*)OBDManager->getResource(obj->getDefaultMaterialPath(), true);
    OBDObject *tempObject = new OBDObject(defaultSceneNode, id, obj, mtl);
	if (autoload) loadObjectTexturesFromMTL(tempObject, mtl, true);
    return tempObject;
}

OBDObject* OBDEngine::createObject(OBDSceneNode* layer, u32 id, std::string mesh, bool autoload) {
    ResourceOBJ *obj = (ResourceOBJ*)OBDManager->getResource(mesh, true);
    ResourceMTL *mtl = (ResourceMTL*)OBDManager->getResource(obj->getDefaultMaterialPath(), true);
    OBDObject *tempObject = new OBDObject(layer, id, obj, mtl);
	if (autoload) loadObjectTexturesFromMTL(tempObject, mtl, true);
    return tempObject;
}

OBDAnimation* OBDEngine::createAnimation(std::vector<std::string> objects, std::string material, bool autoload) {
	std::vector<ResourceOBJ*> *objs = new std::vector<ResourceOBJ*>();
	objs->reserve(objects.size());
	for (int i = 0; i< objects.size(); i++) {
		ResourceOBJ *obj = (ResourceOBJ*)OBDManager->getResource(objects[i], true);
		objs->push_back(obj);
	}
    ResourceMTL *mtl = (ResourceMTL*)OBDManager->getResource(material, true);

	OBDAnimation *tempAnim = new OBDAnimation(defaultSceneNode, objs, mtl);
	if (autoload) loadAnimationTexturesFromMTL(tempAnim, mtl, true);
    return tempAnim;
}

OBDAnimation* OBDEngine::createAnimation(OBDSceneNode* layer, std::vector<std::string> objects, std::string material, bool autoload) {
	std::vector<ResourceOBJ*> *objs = new std::vector<ResourceOBJ*>();
	objs->reserve(objects.size());
	for (int i = 0; i< objects.size(); i++) {
		ResourceOBJ *obj = (ResourceOBJ*)OBDManager->getResource(objects[i], true);
		objs->push_back(obj);
	}
    ResourceMTL *mtl = (ResourceMTL*)OBDManager->getResource(material, true);
	
	OBDAnimation *tempAnim = new OBDAnimation(layer, objs, mtl);
	if (autoload) loadAnimationTexturesFromMTL(tempAnim, mtl, true);
    return tempAnim;
}

OBDTerrain *OBDEngine::createTerrain(std::string heightMap, f32 y_offset, f32 y_scale, i32 step) {
	return new OBDTerrain(defaultSceneNode, heightMap, y_offset, y_scale, step);
}

OBDTerrain *OBDEngine::createTerrain(OBDSceneNode* layer, std::string heightMap, f32 y_offset, f32 y_scale, i32 step) {
	return new OBDTerrain(layer, heightMap, y_offset, y_scale, step);
}

OBDBillboard* OBDEngine::createBillboard(OBDSceneNode* layer, OBDShaderProgram *p, glm::vec3 pos, glm::vec2 size) {
    return new OBDBillboard(layer, p, pos, size);
}

#ifdef USEVIDEO
OBDVideo* OBDEngine::createVideo(OBDSceneNode *layer, OBDShaderProgram *p, std::string path){
	return new OBDVideo(layer, p, path);
}
#endif

OBDImage* OBDEngine::createImage(OBDSceneNode *layer, OBDShaderProgram *p, std::string path){
	ResourceIMG *s = (ResourceIMG*)OBDManager->getResource(path, true);
	return new OBDImage(layer, p, s);
}

OBDSkybox* OBDEngine::createSkybox(OBDSceneNode* layer, OBDShaderProgram* program, std::vector<std::string> textures) {
	std::vector<ResourceIMG*> images;
	for (int i = 0; i < textures.size(); i++){
		images.push_back((ResourceIMG*)OBDManager->getResource(textures[i], true));
	}
    return new OBDSkybox(layer, program, images);
}

OBDShaderProgram *OBDEngine::createShaderProgram(std::string programName, std::string vs, std::string fs) {
	ResourceGLSL *s1 = (ResourceGLSL*)OBDManager->getResource(vs, true);
	ResourceGLSL *s2 = (ResourceGLSL*)OBDManager->getResource(fs, true);
	OBDShaderProgram *p = new OBDShaderProgram(s1, s2);
    shaderPrograms.insert(std::pair<std::string, OBDShaderProgram*>(programName, p));
    return p;
}

OBDMaterial *OBDEngine::createMaterial(std::string path, std::string name) {
	ResourceMTL *s = (ResourceMTL*)OBDManager->getResource(path, true);
	return new OBDMaterial(s, name);
}

OBDTexture *OBDEngine::createTexture(OBDEnums::TextureTypes t, std::string fs) {
	ResourceIMG *s = (ResourceIMG*)OBDManager->getResource(fs, true);
	return new OBDTexture(t, s);
}

OBDSceneNode *OBDEngine::createShaderedSceneNode(std::string sN, std::string vs, std::string fs) {
	ResourceGLSL *s1 = (ResourceGLSL*)OBDManager->getResource(vs, true);
	ResourceGLSL *s2 = (ResourceGLSL*)OBDManager->getResource(fs, true);
	OBDShaderProgram *p = new OBDShaderProgram(s1, s2);
    shaderPrograms.insert(std::pair<std::string, OBDShaderProgram*>(sN, p));
	return new OBDSceneNode(new TNode(new TShaderSwapper(p->getShaderProgram()), rootNode));
}

OBDSceneNode *OBDEngine::createShaderedSceneNode(OBDSceneNode *pa, std::string sN, std::string vs, std::string fs) {
	ResourceGLSL *s1 = (ResourceGLSL*)OBDManager->getResource(vs, true);
	ResourceGLSL *s2 = (ResourceGLSL*)OBDManager->getResource(fs, true);
	OBDShaderProgram *p = new OBDShaderProgram(s1, s2);
    shaderPrograms.insert(std::pair<std::string, OBDShaderProgram*>(sN, p));
	TNode *t = new TNode(new TShaderSwapper(p->getShaderProgram()), pa->getLastNode());
	pa->addChild(t);
	return new OBDSceneNode(t);
}

void OBDEngine::loadObjectTexturesFromMTL(OBDObject *obj, ResourceMTL *mtl, bool sync) {
	for (std::map<std::string, OBDMaterial*>::iterator it = obj->getMaterials()->begin(); it != obj->getMaterials()->end(); ++it) {
		std::map<std::string, ResourceMaterial*>::iterator resource = mtl->getResource()->find(it->second->getMaterialName());
		if (resource != mtl->getResource()->end()) {
			if (resource->second->diffuseTextureMap != "") {
				ResourceIMG *tmp = (ResourceIMG*)OBDManager->getResource(resource->second->diffuseTextureMap, sync);
				it->second->setTexture(new OBDTexture(OBDEnums::TextureTypes::TEXTURE_DIFFUSE, tmp));
			}
			if (resource->second->ambientOclusionsTextureMap != "") {
				ResourceIMG *tmp = (ResourceIMG*)OBDManager->getResource(resource->second->ambientOclusionsTextureMap, sync);
				it->second->setTexture(new OBDTexture(OBDEnums::TextureTypes::TEXTURE_OCLUSIONS, tmp));
			}
			if (resource->second->specularTextureMap != "") {
				ResourceIMG *tmp = (ResourceIMG*)OBDManager->getResource(resource->second->specularTextureMap, sync);
				it->second->setTexture(new OBDTexture(OBDEnums::TextureTypes::TEXTURE_SPECULAR, tmp));
			}
			if (resource->second->alphaTextureMap != "") {
				ResourceIMG *tmp = (ResourceIMG*)OBDManager->getResource(resource->second->alphaTextureMap, sync);
				it->second->setTexture(new OBDTexture(OBDEnums::TextureTypes::TEXTURE_ALPHA, tmp));
			}
		}
	}
}

void OBDEngine::loadAnimationTexturesFromMTL(OBDAnimation *obj, ResourceMTL *mtl, bool sync) {
	auto resource = mtl->getResource()->find(obj->getMaterial()->getMaterialName());
	if (resource != mtl->getResource()->end()) {
		if (resource->second->diffuseTextureMap != "") {
			ResourceIMG *tmp = (ResourceIMG*)OBDManager->getResource(resource->second->diffuseTextureMap, sync);
			obj->getMaterial()->setTexture(new OBDTexture(OBDEnums::TextureTypes::TEXTURE_DIFFUSE, tmp));
		}
		if (resource->second->ambientOclusionsTextureMap != "") {
			ResourceIMG *tmp = (ResourceIMG*)OBDManager->getResource(resource->second->ambientOclusionsTextureMap, sync);
			obj->getMaterial()->setTexture(new OBDTexture(OBDEnums::TextureTypes::TEXTURE_OCLUSIONS, tmp));
		}
		if (resource->second->specularTextureMap != "") {
			ResourceIMG *tmp = (ResourceIMG*)OBDManager->getResource(resource->second->specularTextureMap, sync);
			obj->getMaterial()->setTexture(new OBDTexture(OBDEnums::TextureTypes::TEXTURE_SPECULAR, tmp));
		}
		if (resource->second->alphaTextureMap != "") {
			ResourceIMG *tmp = (ResourceIMG*)OBDManager->getResource(resource->second->alphaTextureMap, sync);
			obj->getMaterial()->setTexture(new OBDTexture(OBDEnums::TextureTypes::TEXTURE_ALPHA, tmp));
		}
	}
}

void OBDEngine::registerLight(OBDLight* lightNode) {
    lights.push_back(lightNode);
}

void OBDEngine::registerCamera(OBDCamera* cameraNode) {
    cameras.push_back(cameraNode);
}

void OBDEngine::registerShaderProgram(std::string programName, OBDShaderProgram *r) {
    shaderPrograms.insert(std::pair<std::string, OBDShaderProgram*>(programName, r));
}

void OBDEngine::setCurrentShaderProgram(std::string programName) {
    std::map<std::string, OBDShaderProgram*>::iterator it;
    it = shaderPrograms.find(programName);
    if (it != shaderPrograms.end()) {
        TEntity::cache.setAllIDs(it->second -> getParamIDs());
        TEntity::cache.setID(OBDEnums::OpenGLIDs::CURRENT_PROGRAM_ID, it->second -> getShaderProgram());
    }
}

void OBDEngine::setWindowSize(i32 w, i32 h) {
	windowWidth = w;
	windowHeight = h;
	viewport = glm::vec4(0.0f, 0.0f, windowWidth, windowHeight);
	TEntity::cache.setScreenSize(glm::vec2(w, h));
}

void OBDEngine::setClearColor(OBDColor c) {
	glClearColor(c.r, c.g, c.b, c.a);
}

glm::vec3 OBDEngine::getWorldCoordinatesFromScreen(glm::vec3 screen) {
    screen.y = windowHeight - screen.y; //Invert y
    return glm::unProject(screen, *TEntity::cache.getViewMatrix(), *TEntity::cache.getProjectionMatrix(), viewport);
}
 
glm::vec3 OBDEngine::getScreenCoordinatesFromWorld(glm::vec3 world) {
    return glm::project(world, *TEntity::cache.getViewMatrix(), *TEntity::cache.getProjectionMatrix(), viewport);
}

OBDLine OBDEngine::getRaycastFromScreenCoordinates(glm::vec2 screen) {
    OBDLine l;
    l.start = getWorldCoordinatesFromScreen(glm::vec3(screen.x, screen.y, 0));
    l.end = getWorldCoordinatesFromScreen(glm::vec3(screen.x, screen.y, 1));
    return l;
}

TNode* OBDEngine::getRootNode() {
    return rootNode;
}

OBDSceneNode* OBDEngine::getDefaultLayer() {
    return defaultSceneNode;
}

ResourceManager *OBDEngine::getResourceManager() {
	return OBDManager;
}

OBDShaderProgram *OBDEngine::getRegisteredShaderProgram(std::string sN) {
	auto it = shaderPrograms.find(sN);
	assert(it != shaderPrograms.end());
	return it->second;
}