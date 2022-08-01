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
		//设置一些默认配置
		LwjglApplicationConfiguration config = new LwjglApplicationConfiguration();
		//也可以自定义一些属性值
		config.width = 280;
		config.height = 480;
		//设置其大小不变
		config.resizable = false;
		//此方法传入两个参数一个是ApplicationListener,另一个是config配置
		new LwjglApplication(new WindowDemo(), config);
	}
}
