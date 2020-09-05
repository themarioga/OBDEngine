#include "OBDShaderProgram.h"

#include "Graphics/TEntity.h"

OBDShaderProgram::OBDShaderProgram(ResourceGLSL* vs, ResourceGLSL* fs) {
	vertexShader = new TShader(vs);
	fragmentShader = new TShader(fs);

	pid = glCreateProgram();
	glAttachShader(pid, vertexShader->getShaderID());
	glAttachShader(pid, fragmentShader->getShaderID());
	
	glLinkProgram(pid);

	// Check the program
    GLint Result = GL_FALSE;
    i32 InfoLogLength;
	glGetProgramiv(pid, GL_LINK_STATUS, &Result);
	glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(pid, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cerr << "Error: " << &ProgramErrorMessage[0] << std::endl;
		exit(0);
	}

	paramIDs = TEntity::cache.generateAllIDs(pid);
}

OBDShaderProgram::~OBDShaderProgram() {
	glDetachShader(pid, vertexShader->getShaderID());
	glDeleteShader(vertexShader->getShaderID());
	glDetachShader(pid, fragmentShader->getShaderID());
	glDeleteShader(fragmentShader->getShaderID());
	delete vertexShader;
	delete fragmentShader;
	vertexShader = nullptr;
	fragmentShader = nullptr;
}
    
std::vector<GLuint> OBDShaderProgram::getParamIDs() {
	return paramIDs;
}
    
GLuint OBDShaderProgram::getShaderProgram() {
	return pid;
}