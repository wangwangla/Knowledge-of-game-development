package kw.test.demo;

import com.badlogic.gdx.backends.lwjgl.LwjglApplication;
import com.badlogic.gdx.backends.lwjgl.LwjglApplicationConfiguration;

/**
 * libGdx demo
 * @author Administrator
 *
 */
public class Demo {
	public static void main(String[] args) {
		//����һЩĬ������
		LwjglApplicationConfiguration config = new LwjglApplicationConfiguration();
		//Ҳ�����Զ���һЩ����ֵ
		config.width = 280;
		config.height = 480;
		//�������С����
		config.resizable = false;
		//�˷���������������һ����ApplicationListener,��һ����config����
		new LwjglApplication(new WindowDemo(), config);
	}
}
