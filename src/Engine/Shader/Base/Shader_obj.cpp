#include "Shader_obj.h"
#include "Shader_src.h"

#include "../../../Specific/File/Directory.h"
#include "../../../Specific/Function/fct_error.h"

#include <fstream>


//Constructor / Destructor
Shader_obj::Shader_obj(Shader_src* shader_src){
	//---------------------------

	// Create the shaders program
	this->name = shader_src->get_name();
	this->program_ID = glCreateProgram();

	// Compile & check Shaders-
	GLuint vs = shader_compilation(shader_src->get_path_vs(), GL_VERTEX_SHADER);
	GLuint fs = shader_compilation(shader_src->get_path_fs(), GL_FRAGMENT_SHADER);

	//Link program
	glLinkProgram(program_ID);

	//Detach shaders for keep memory ressources
	glDetachShader(program_ID, vs);
	glDeleteShader(vs);
	glDetachShader(program_ID, fs);
	glDeleteShader(fs);

	//---------------------------
}
Shader_obj::Shader_obj(string name, string path_vs, string path_fs){
	//---------------------------

	// Create the shaders program
	this->name = name;
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
Shader_obj::Shader_obj(string name, string path_vs, string path_fs, string path_gs){
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
Shader_obj::~Shader_obj(){
	//---------------------------

	glDeleteProgram(program_ID);

	//---------------------------
}

void Shader_obj::use(){
	//---------------------------

	glUseProgram(program_ID);

	//---------------------------
}
void Shader_obj::build_shader(GLuint& program_ID, string path_vs, string path_fs){
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
GLuint Shader_obj::shader_compilation(string file_path, GLenum shaderType){
	//---------------------------

	//First, check if file exists
	if(is_file_exist(file_path) == false){
		error_fileDoesNotExists(file_path);
	}

	//Create shader
	GLuint shaderID = glCreateShader(shaderType);

	//Read Shader_obj
	std::ifstream shaderStream(file_path.c_str(), std::ios::in);
	string shaderCode;
	if(shaderStream.is_open()){
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
	}

	//Compile Shader_obj
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
