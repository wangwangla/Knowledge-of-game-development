#include "renderDelegate.h"

RenderDelegate* RenderDelegate::delegateSingleton = nullptr;

RenderDelegate::RenderDelegate()
{
	handleDelegate = nullptr;
	window = nullptr;

	if (this->initWindow() == -1) {
		cout << "Windows initialize failure!" << endl;
	}
}

void RenderDelegate::render()
{
	if (this->handleDelegate != nullptr) {
		glEnable(GL_DEPTH_TEST);
		this->handleDelegate->render();
	} else {
		cout << "Not initialize handleDelegate!" << endl;
	}
}
void RenderDelegate::setHandleDelegate(Handle* handleDelegate)
{
	this->handleDelegate = handleDelegate;
}

void RenderDelegate::printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}


int RenderDelegate::initWindow()
{
	//glfw��ʼ��
	glfwInit();
	//����OpenGL�����汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	//����OpenGL�Ĵΰ汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//ʹ��OpenGL�ĺ���������޷�ʹ���Ѿ�������OpenGL api��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//���ڲ�����ı�
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//�������ڣ� glfwCreateWindow��int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share��
	this->window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (this->window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//�ٷ��ĵ����ڵ�ǰ�߳��д���ָ�����ڵ�OpenGL�����ģ�һ��ֻ����һ���߳��ϴ���һ�������ģ�����ÿ���߳�һ��ֻ����һ��������
	glfwMakeContextCurrent(this->window);

	//��ʼ��glew	
	//glewExperimental����ΪGL_TRUE�������ʹ���ִ���������û���ĵ���ֻ���ճ��̵̳ģ�
	glewExperimental = GL_TRUE;
	//��ʼ��glew
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" <<std::endl;
		return -1;
	}
	//ָ�����ڵ�����λ�ã����
	glViewport(0, 0, 800, 600);
	return 1;
}

GLFWwindow* RenderDelegate::getWindowHandle()
{
	return this->window;
}

void RenderDelegate::run() {

	//����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�׼����Ҫ�˳�
	while(!glfwWindowShouldClose(this->window)) {
		//�����û��ʲô�¼���������꣬���̣��������ö�Ӧ�ĺ���
		glfwPollEvents();
		//�˴�ִ����Ⱦ����
		this->render();
		//����������
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

RenderDelegate* RenderDelegate::getInstance()
{
	if (RenderDelegate::delegateSingleton == nullptr) {
		RenderDelegate::delegateSingleton = new RenderDelegate();
	}

	return RenderDelegate::delegateSingleton;
}

