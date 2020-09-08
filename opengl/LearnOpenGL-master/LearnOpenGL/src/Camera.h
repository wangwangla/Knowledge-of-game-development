#ifndef _CAMERA_
#define _CAMERA_

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace glm;

class Camera
{
public:
	static vec3 cameraPos;
	static vec3 cameraFront;
	static vec3 cameraUp;
	static bool keys[1024];
	static GLfloat lastX;
	static GLfloat lastY;
	static GLfloat pitchValue; //���ӽ�
	static GLfloat yawValue; //ƫ����
	static GLboolean firstMouse;  //�Ƿ��һ���ƶ���꣬��ֹ��һ�ν���ҳ�棬�������������
	static GLfloat aspect;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	//static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void DoMovement();
	mat4 GetLookAt();
	void setDeltaTime(GLfloat currentTime);
private:
	GLfloat deltaTime;
	GLfloat lastTime;
};

#endif //_CAMERA_