#pragma once
#include "common.h"
class Material {
public:
	Material()
	{
		name;
		Ns = 0.0f;
		Ni = 0.0f;
		d = 0.0f;
		illum = 0;
		vertexShaderName = "vertexShader";
		fragmentShaderName = "fragmentShader";
	}

	// Material Name
	std::string name;
	// Ambient Color
	myCommon::vec3 Ka;
	// Diffuse Color
	myCommon::vec3 Kd;
	// Specular Color
	myCommon::vec3 Ks;
	// Specular Exponent
	float Ns;
	// Optical Density
	float Ni;
	// Dissolve
	float d;
	// Illumination
	int illum;
	// Ambient Texture Map
	std::string map_Ka;
	// Diffuse Texture Map
	std::string map_Kd;
	// Specular Texture Map
	std::string map_Ks;
	// Specular Hightlight Map
	std::string map_Ns;
	// Alpha Texture Map
	std::string map_d;
	// Bump Map
	std::string map_bump;
	// Vertex Shader name
	std::string vertexShaderName;
	// Fragment Shader name
	std::string fragmentShaderName;
};
