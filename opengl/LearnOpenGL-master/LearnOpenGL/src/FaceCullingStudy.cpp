#include "FaceCullingStudy.h"


FaceCullingStudy::FaceCullingStudy()
{
	stencil = new StencilTestStudy();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	/*
	GL_BACK��ֻ�޳����档
	GL_FRONT��ֻ�޳����档
	GL_FRONT_AND_BACK���޳���������档
	*/
	glFrontFace(GL_CCW);
	//Ĭ��ֵ��GL_CCW����������ʱ�룬GL_CW����˳ʱ��˳��
}



void FaceCullingStudy::render()
{
	//���޳����������Ⱦ���ܣ��������50%,�����˳���������Ϊ��ʱ��
	stencil->render();
}