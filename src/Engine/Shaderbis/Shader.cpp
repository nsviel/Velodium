#include "Shader.h"

//Constructor / Destructor
Shader::Shader(){
	//---------------------------

	this->shader_dir = "../src/Engine/Shader/";

	//---------------------------
}
Shader::~Shader(){
	//---------------------------

	glDeleteProgram(ID_program);

	//---------------------------
}

//Main function
bool Shader::shader_build(string name){
	//---------------------------

	// Create the shaders program
	ID_program = glCreateProgram();

	// Compile & check Shaders-
	string vertex_path = shader_dir + name + ".vs";
	string fragme_path = shader_dir + name + ".fs";
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
	console.AddLog("sucess" ,"Shaders Initialized...");
	return true;
}
void Shader::run(){
	//---------------------------

	glUseProgram(ID_program);

	//---------------------------
}

//Subfunctions
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
