#include "Shader.h"

#include <fstream>


//Constructor / Destructor
Shader::Shader(string vertex_path, string fragme_path){
	//---------------------------

	// Create the shaders program
	ID_program = glCreateProgram();

	// Compile & check Shaders-
	GLuint vs = shader_compilation(vertex_path, GL_VERTEX_SHADER);
	GLuint fs = shader_compilation(fragme_path, GL_FRAGMENT_SHADER);

	//Link program
	glLinkProgram(ID_program);

	//Detach shaders for keep memory ressources
	glDetachShader(ID_program, vs);
	glDeleteShader(vs);
	glDetachShader(ID_program, fs);
	glDeleteShader(fs);

	//---------------------------
}
Shader::~Shader(){
	//---------------------------

	glDeleteProgram(ID_program);

	//---------------------------
}

//Functions
void Shader::use(){
	//---------------------------

	glUseProgram(ID_program);

	//---------------------------
}
GLuint Shader::shader_compilation(string file_path, GLenum shaderType){
	GLuint shaderID = glCreateShader(shaderType);
	//---------------------------

	//Read shader
	std::ifstream shaderStream(file_path.c_str(), std::ios::in);
	string shaderCode;
	if(shaderStream.is_open()){
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
	}

	//Compile shader
	int sucess, InfoLogLength;
	char const* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer , NULL);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &sucess);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	//Bind to program
	glAttachShader(ID_program, shaderID);

	//---------------------------
	return shaderID;
}
