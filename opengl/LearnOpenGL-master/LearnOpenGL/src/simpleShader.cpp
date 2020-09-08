#include "simpleShader.h"

SimpleShader::SimpleShader()
{
	this->VAO = 0;
	this->shaderPorgram = 0;
	// Shaders1
	this->vertexShaderSource1 = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
	this->fragmentShaderSource1 = "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

	// Shaders2
	this-> vertexShaderSource2 = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
	this->fragmentShaderSource2 = "#version 330 core\n"
    "out vec4 color;\n"
	"uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "color = ourColor;\n"
    "}\n\0";

	//��ʼ��vao�� shader
	this->initVAO();

	this->initShader();
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
void SimpleShader::initTriangleVAO()
{
		//���ö�������
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f, 0.5f,0.0f,
	};
	//���ɶ��㻺��������
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
	//�󶨻��������ͣ�һ��������������԰󶨶������������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//�Ѷ������ݸ��Ƶ������ڴ��С�
	/*
	*GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	*GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	*GL_STREAM_DRAW ������ÿ�λ���ʱ����ı�
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���ö�������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//���VAO
	glBindVertexArray(0);
}

void SimpleShader::initVAO()
{
	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f,   // ���Ͻ�
		0.5f, -0.5f, 0.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f   // ���Ͻ�
	};

	GLuint indices[] = { // ��ʼ��0!
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	GLuint VBO;
	GLuint EBO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void SimpleShader::initShader()
{
	//����������ɫ��
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &this->vertexShaderSource1, NULL);
	glCompileShader(vertexShader);

	//����Ƭ����ɫ��
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &this->fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader);

	//������ɫ���������(Shader Program Object)
	//����ɫ������Ϊһ������͵��ڰ�ÿ����ɫ����������ӵ���һ����ɫ��������

	this->shaderPorgram = glCreateProgram();
	glAttachShader(this->shaderPorgram, vertexShader);
	glAttachShader(this->shaderPorgram, fragmentShader);
	glLinkProgram(this->shaderPorgram);
	//ɾ��������ɫ��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void SimpleShader::render()
{
	//�˴�ִ����Ⱦ����
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(this->shaderPorgram);

	GLfloat timeValue = glfwGetTime();
	GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
	GLint vertexColorLocation = glGetUniformLocation(this->shaderPorgram, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

}
