#include "Shader_object.h"

#include <fstream>


//Constructor / Destructor
Shader_object::Shader_object(string path_vs, string path_fs){
	//---------------------------

	// Create the shaders program
	this->program_ID = glCreateProgram();

	// Compile & check Shaders-
	GLuint vs = shader_compilation(path_vs, GL_VERTEX_SHADER);
	GLuint fs = shader_compilation(path_fs, GL_FRAGMENT_SHADER);

	//Link program
	glLinkProgram(program_ID);

	//Detach shaders for keep memory ressources
	glDetachShader(program_ID, vs);
	glDeleteShader(vs);
	glDetachShader(program_ID, fs);
	glDeleteShader(fs);

	//---------------------------
}
Shader_object::Shader_object(string path_vs, string path_fs, string path_gs){
	//---------------------------

	// Create the shaders program
	this->program_ID = glCreateProgram();

	// Compile & check Shaders-
	GLuint vs = shader_compilation(path_vs, GL_VERTEX_SHADER);
	GLuint fs = shader_compilation(path_fs, GL_FRAGMENT_SHADER);
	GLuint gs = shader_compilation(path_gs, GL_GEOMETRY_SHADER);

	//Link program
	glLinkProgram(program_ID);

	//Detach shaders for keep memory ressources
	glDetachShader(program_ID, vs);
	glDeleteShader(vs);
	glDetachShader(program_ID, fs);
	glDeleteShader(fs);
	glDetachShader(program_ID, gs);
	glDeleteShader(gs);

	//---------------------------
}
Shader_object::~Shader_object(){
	//---------------------------

	glDeleteProgram(program_ID);

	//---------------------------
}

void Shader_object::use(){
	//---------------------------

	glUseProgram(program_ID);

	//---------------------------
}
void Shader_object::build_shader(GLuint& program_ID, string path_vs, string path_fs){
	string path_gs = "../src/Engine/Shader/shader_scene.gs";
	//---------------------------

	// Create the shaders program
	program_ID = glCreateProgram();

	// Compile & check Shaders-
	GLuint vs = shader_compilation(path_vs, GL_VERTEX_SHADER);
	GLuint gs = shader_compilation(path_gs, GL_GEOMETRY_SHADER);
	GLuint fs = shader_compilation(path_fs, GL_FRAGMENT_SHADER);

	//Link program
	glLinkProgram(program_ID);

	//Detach shaders for keep memory ressources
	glDetachShader(program_ID, vs);
	glDeleteShader(vs);
	glDetachShader(program_ID, gs);
	glDeleteShader(gs);
	glDetachShader(program_ID, fs);
	glDeleteShader(fs);

	//---------------------------
}
GLuint Shader_object::shader_compilation(string file_path, GLenum shaderType){
	GLuint shaderID = glCreateShader(shaderType);
	//---------------------------

	//Read Shader_object
	std::ifstream shaderStream(file_path.c_str(), std::ios::in);
	string shaderCode;
	if(shaderStream.is_open()){
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
	}

	//Compile Shader_object
	int success, InfoLogLength;
	char const* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer , NULL);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	//Bind to program
	glAttachShader(program_ID, shaderID);

	//---------------------------
	return shaderID;
}
