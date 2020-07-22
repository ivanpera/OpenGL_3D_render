#pragma once
#include "Lib.h"

class ShaderMaker
{
    public:
		static GLuint createProgram(char const* vertexfilename, char const* fragmentfilename);
		static char* readShaderSource(const char* shaderFile);
	
    private:
		ShaderMaker() { }
};