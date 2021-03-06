﻿// OpenGL Shader.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "timeutils.h"
#include "utils.h"
#include "opengl.h"
#include "polygon.h"
#include "light.h"

Cube cube;

ShaderManager *shader_model;

#pragma region 光照 Light
Light light;
//Blinn-Phong or Phong
bool blinn = false;
//平行光 Directional Light
GLboolean ld_enabled = false;

//点光源 Light Point
GLboolean lp_enabled = false;

//聚光灯 Spot Light
GLboolean ls_enabled = true;

#pragma endregion

#pragma region 资源路径 Resource path
char shader_model_path[2][18] = { "shader/model.vert", "shader/model.frag" };
char texture_dif_path[] = {"texture/container.png"};
char texture_spe_path[] = { "texture/container_specular.png" };
//char nano_path[] = {"model/nanosuit/nanosuit.obj"};

#pragma endregion

#pragma region screen
GLint screen_width = 600, screen_height = 600;
GLint view_width = 600, view_height = 600;
#pragma endregion

#pragma region speed
GLfloat move_speed = 1.0f;
GLfloat yaw_speed = 0.01f;
GLfloat pitch_speed = 0.01f;
#pragma endregion

#pragma region camera
vec3 camera_position = vec3(0, 0, 10.0f);
vec3 camera_forward = vec3(0, 0, -1.0f);
vec3 camera_up = vec3(0, 1.0f, 0);
#pragma endregion

#pragma region program 其他变量 Other Attribute
mat4 projection;

GLfloat max_pitch_angle = 45.0f;

GLfloat angle_h = 0;
GLfloat angle_v = 0;

GLint last_x = -1;
GLint last_y = -1;

#pragma endregion

mat4 initCamera() {
	mat4 view = mat4(1.0f);
	view = lookAt(camera_position, camera_position + camera_forward, camera_up);
	view = rotate(view, angle_h, vec3(0, 1.0f, 0));
	view = rotate(view, angle_v, vec3(1.0f, 0, 0));
	return view;
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1.0);
	string log_path = "log/" + TimeUtils::getCompleteTime() + ".log";
	LogUtils::createInstance()->init(log_path.c_str());
	shader_model = ShaderManager::NewInstance();
	shader_model->load(shader_model_path[0], shader_model_path[1]);
	shader_model->use();
	light.init(shader_model);
	cube.init(3.0f);
	cube.initCubeTexture(texture_dif_path, texture_spe_path);
	
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 view = initCamera();
	mat4 model = mat4(1.0f);
	
	shader_model->use();
	shader_model->setUniformMatrix4fv("view", 1, value_ptr(view));
	shader_model->setUniformMatrix4fv("projection", 1, value_ptr(projection));
	shader_model->setUniform3fv("camera_position", 1, value_ptr(camera_position));
	shader_model->setUniform1i("blinn", blinn);
	light.setAllLight(ld_enabled, lp_enabled, ls_enabled);
	
	cube.draw(shader_model, model);
	
	glutSwapBuffers();
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	projection = perspective(radians(45.0f), float(view_width / view_height), 0.1f, 100.0f);
	//projection = ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.5f, 20.0f);
}

void idle() {
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {

}

void mouse_motion(int x, int y) {
	if (last_x < 0) {
		last_x = x;
	}
	if (last_y < 0) {
		last_y = y;
	}
	angle_h += (x - last_x) * yaw_speed;
	angle_v += (y - last_y) * pitch_speed;
	last_x = x;
	last_y = y;
}

void keyboard(unsigned char button, int x, int y) {
	switch (button)
	{
	case 'w':
		camera_position += camera_forward * move_speed;
		break;
	case 's':
		camera_position -= camera_forward * move_speed;
		break;
	case '1':
		ld_enabled = !ld_enabled;
		break;
	case '2':
		lp_enabled = !lp_enabled;
		break;
	case '3':
		ls_enabled = !ls_enabled;
		break;
	case '4':
		blinn = !blinn;
		break;
	default:
		break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("冯氏光照模型");
	GLuint result = glewInit();
	if (result != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(result));
		return 1;
	}
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
