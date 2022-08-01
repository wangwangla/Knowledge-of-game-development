package kw.test.demo;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.Label;
import com.badlogic.gdx.scenes.scene2d.ui.Label.LabelStyle;
import com.badlogic.gdx.utils.viewport.StretchViewport;

/**
 * label use 
 * @author Administrator
 *
 */
public class LabelDemo extends ApplicationAdapter{
	Stage stage;
	BitmapFont bitmapFont;
	LabelStyle labelStyle;
	Label label;
	@Override
	public void create() {
		// TODO Auto-generated method stub
		super.create();
		bitmapFont = new BitmapFont(Gdx.files.internal("Demo.fnt"),Gdx.files.internal("Demo.png"),false);
		labelStyle = new LabelStyle(bitmapFont,bitmapFont.getColor());
		label = new Label("¼ûÏÍË¼Æë", labelStyle);
		stage = new Stage(new StretchViewport(480,320));
		stage.addActor(label);
	}
	@Override
	public void render() {
		// TODO Auto-generated method stub
		super.render();
		Gdx.gl.glClearColor(1.0f, 0f, 0f, 1.0f);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
		stage.act();
		stage.draw();	
	}
	
	@Override
	public void dispose() {
		// TODO Auto-generated method stub
		super.dispose();
	}
}
