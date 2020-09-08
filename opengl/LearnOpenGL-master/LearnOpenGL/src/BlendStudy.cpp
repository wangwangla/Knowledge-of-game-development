#include "BlendStudy.h"
#include <SOIL\SOIL.h>

BlendStudy::BlendStudy()
{
	
	
	stencil = new StencilTestStudy();
	grassShader = new ShaderHandle("blendgrass");
	viewPostion = vec3(0.0f, 2.0f, 5.0f);

	this->initVAO();
	this->initTexture();

    vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
    vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
    vegetation.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
    vegetation.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
    vegetation.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->sortData();
}

void BlendStudy::sortData()
{
	for (int i = 0; i < vegetation.size(); i++)
	{
		//�̳���ʹ��length��������֮��ľ��룬��
		float distance = viewPostion.z - vegetation[i].z;
		sorted[distance] = vegetation[i];
	}
}

void BlendStudy::initVAO()
{ 
	GLfloat transparentVertices[] = {
        // Positions         // Texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &this->grassVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), &transparentVertices, GL_STATIC_DRAW);
	
	glBindVertexArray(this->grassVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

void BlendStudy::initTexture()
{
	//�˴��ɰ�����ļ��ز��֣���ȡ��Ϊһ�����������ٴ��������
	//����ô����ԭ���ǣ���д���飬����ӡ��
	glGenTextures(1, &this->grassTexture);
	glBindTexture(GL_TEXTURE_2D, this->grassTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//ʹ��GL_LINEAR_MIPMAP_LINEARʱһ��Ҫע�⣬����Ҫ����glGenrateMipmap(GL_TEXTURE_2D)��������������쳣
	int width;
	int height;
	unsigned char* image = SOIL_load_image("../resource/blending_transparent_window.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void BlendStudy::render()
{
	stencil->render();
	this->renderGrass1();
}

void BlendStudy::renderGrass1()
{
	/*
		ע����Ⱦ�Ĳ��裺Ƭ����ɫ�� -> ģ����� -> ��Ȳ��� -> ��� 

		������Ȳ������ڻ��֮ǰ����ǰ���͸���ڵ��������͸������
		��߱��ڵ����֣�����ͨ����Ȳ��ԣ��ڵ�����Ƭ�ζ��ᱻ��������
		����ɱ��ڵ������޷����л�ϣ����Ա��ֳ��ڵ����ֲ�͸����Ч����
	*/
	glDisable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	
	this->grassShader->UseProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->grassTexture);
	GLuint location = grassShader->getProgram();
	glUniform1i(glGetUniformLocation(grassShader->getProgram(), "ourTexture"),0);
	
	mat4 model(1.0f);
	mat4 view(1.0f);
	mat4 projection(1.0f);

	view = lookAt(vec3(0.0f, 2.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(grassShader->getProgram(), "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(grassShader->getProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));

	glBindVertexArray(this->grassVAO);
	for (int i = 0; i < this->vegetation.size(); i++)
	{
		model = translate(mat4(1.0f), this->vegetation[i]);
		glUniformMatrix4fv(glGetUniformLocation(grassShader->getProgram(), "model"), 1, GL_FALSE, value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
	glEnable(GL_STENCIL_TEST);
}

void BlendStudy::renderGrass2()
{
	/*
		Ϊ���ֲ�������Ȳ�������ڵ����ֵ�͸��Ч���޷���ȷ��ʾ��
		����͸����������õ�����Ⱦ�ķ�ʽ������Զ������������ߵ�
		���������Ⱦ��ǰ�ߵ��������Ⱦ����ߵ�����Ͳ�����Ϊ�ڵ�
		����Ƭ��
	*/
	
	glDisable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	
	this->grassShader->UseProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->grassTexture);
	GLuint location = grassShader->getProgram();
	glUniform1i(glGetUniformLocation(grassShader->getProgram(), "ourTexture"),0);
	
	mat4 model(1.0f);
	mat4 view(1.0f);
	mat4 projection(1.0f);

	view = lookAt(viewPostion, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(grassShader->getProgram(), "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(grassShader->getProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));

	glBindVertexArray(this->grassVAO);
	for (map<float, vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) 
	{
		model = translate(mat4(1.0f), it->second);
		glUniformMatrix4fv(glGetUniformLocation(grassShader->getProgram(), "model"), 1, GL_FALSE, value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
	glEnable(GL_STENCIL_TEST);

}