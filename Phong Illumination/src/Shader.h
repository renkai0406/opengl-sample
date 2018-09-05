#pragma once

#include "stdafx.h"
#include "opengl.h"


using namespace std;

class ShaderManager
{
public:
	~ShaderManager();
	static ShaderManager* SingleInstance();
	static ShaderManager* NewInstance();
	void load(char* vertex_path, char* fragment_path);
	void use();
	void disable();
	void checkGLError();
	void setUniform1i(const GLchar * name, GLint value);
	void setUniform1f(const GLchar * name, GLfloat value);
	void setUniform3fv(const GLchar * name, GLint count, GLfloat * values);
	void setUniformMatrix4fv(const GLchar * name, GLint count, GLfloat * values);

private:
	ShaderManager();
	string loadFile(char* path);
	static ShaderManager* instance;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLint program;
	static const int BUFFER_SIZE = 512;
};

