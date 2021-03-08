#ifndef SHADER_H
#define SHADER_H

#include "../Include/Common.h"

class Shader
{
public:
	GLuint GetID();
	bool isLoaded();
	void LoadShader(char* fileName,int _shaderType);
	void Delete();

private:
	GLuint id;
	int type;
	bool loaded;
};

class ShaderProgram
{

public:
	ShaderProgram();
	GLuint GetID();
	GLuint CreateProgram();
	void Delete();
	void LinkProgram();
	void UseProgram();
	void AttachShader(GLuint shaderId);

private:
	GLuint id;
	bool linked;
};
#endif  // SHADER_H
