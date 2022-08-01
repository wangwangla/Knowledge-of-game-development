package kw.test.demo;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.scenes.scene2d.utils.Layout;

public class ChineseFont extends ApplicationAdapter{
	// spritchBatch
	private SpriteBatch spriteBatch;
	//create font
	private BitmapFont font;
	@Override
	public void create() {
		// TODO Auto-generated method stub
		super.create();
		spriteBatch = new SpriteBatch();
		font = new BitmapFont(Gdx.files.internal("Demo.fnt"),Gdx.files.internal("Demo.png"),false);
	}
	@Override
	public void render() {
		// TODO Auto-generated method stub
		super.render();
		Gdx.gl.glClearColor(1.0f, 0f, 0f, 1.0f);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
		spriteBatch.begin();
		font.draw(spriteBatch, "¼ûÏÍ\nË¼Æë", 30, 70);
		spriteBatch.end();
	}
	@Override
	public void dispose() {
		// TODO Auto-generated method stub
		super.dispose();
		spriteBatch.dispose();
	}

}
