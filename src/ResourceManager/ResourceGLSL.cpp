#include "ResourceGLSL.h"

ResourceGLSL::ResourceGLSL() {

}

ResourceGLSL::~ResourceGLSL() {
	
}

void ResourceGLSL::load(const char *path) {
    setIdentifier(path);
    std::string file_path(path);
    
    std::size_t vertexFound = file_path.find("vertex");
    std::size_t fragmentFound = file_path.find("fragment");

    if (vertexFound != std::string::npos) {
        type = OBDEnums::ShaderType::VERTEX;
    } else if (fragmentFound != std::string::npos) {
        type = OBDEnums::ShaderType::FRAGMENT;
    } else {
        std::cerr << "Error: tipo de shader desconocido, utiliza la palabra vertex o fragment en el nombre para su reconocimiento." << std::endl;
        exit(0);
    }

    std::ifstream shaderStream(file_path, std::ios::in);
    if (shaderStream.is_open()) {
        std::stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
    } else {
        std::cerr << "Error: imposible abrir el shader, revisa la ruta y los permisos." << std::endl;
        exit(0);
    }
}

void ResourceGLSL::release() {
}

void ResourceGLSL::setIdentifier(const char *i) {
    identifier = i;
}

const char *ResourceGLSL::getIdentifier() {
    return identifier;
}

std::string *ResourceGLSL::getShaderCode() {
    return &shaderCode;
}

OBDEnums::ShaderType ResourceGLSL::getShaderType() {
    return type;
}