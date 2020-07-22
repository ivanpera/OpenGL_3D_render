#include "Headers/ShaderMaker.h"
#include <iostream>
#include <fstream>
#include <limits>


std::string const SHADER_DIR = "Shaders/";
std::string const EXTENSION = ".glsl";
unsigned int const MAX_PATH_LEN = 500;
char* ShaderMaker::readShaderSource(const char* shaderFile)
{
	char path[MAX_PATH_LEN];
	strcpy(path, SHADER_DIR.c_str());
	strcat(path, shaderFile);
	strcat(path, EXTENSION.c_str());
	FILE* fp = fopen(path, "rb");

	if (fp == NULL) { 
		return NULL; 
	}

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	long pos = size + 1;
	char* buf = new char[pos];
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}

GLuint ShaderMaker::createProgram(char const* vertexfilename, char const* fragmentfilename)
{
	GLenum ErrorCheckValue = glGetError();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// Creiamo gli eseguibili degli shader
	//Leggiamo il codice del Vertex Shader
	GLchar* VertexShader = readShaderSource(vertexfilename);
	//Visualizzo sulla console il CODICE VERTEX SHADER
	//std::cout << VertexShader << "\n";
	//Generiamo un identificativo per il vertex shader
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	//std::cout << vertexShaderId << "\n";
	//Associamo all'identificativo il codice del vertex shader
	glShaderSource(vertexShaderId, 1, (const char**)&VertexShader, NULL);
	//Compiliamo il Vertex SHader
	glCompileShader(vertexShaderId); 
	


	//Leggiamo il codice del Fragment Shader
	const GLchar* FragmentShader = readShaderSource(fragmentfilename);
	//Visualizzo sulla console il CODICE FRAGMENT SHADER
	//cout << FragmentShader;

	//Generiamo un identificativo per il FRAGMENT shader
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&FragmentShader, NULL);
	//Compiliamo il FRAGMENT SHader
	glCompileShader(fragmentShaderId);

	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		std::cout << ErrorCheckValue;
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n",
			gluErrorString(ErrorCheckValue)
		);

		exit(-1);
	}
	//Creiamo un identificativo di un eseguibile e gli colleghiamo i due shader compilati
	GLuint programId = glCreateProgram();

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	
	return programId;
}
