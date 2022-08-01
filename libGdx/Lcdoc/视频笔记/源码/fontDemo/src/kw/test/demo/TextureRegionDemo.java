package kw.test.demo;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.g2d.TextureRegion;

public class TextureRegionDemo extends ApplicationAdapter {
	// texture
	private Texture texture;
	// spritchBatch
	private SpriteBatch spriteBatch;
	// textureRegion
	private TextureRegion textureRegion;
	@Override
	public void create() {
		// TODO Auto-generated method stub
		super.create();
		texture = new Texture(Gdx.files.internal("1.png"));
		spriteBatch = new SpriteBatch();
		textureRegion = new TextureRegion(texture,0,0,30,30);
	}
	@Override
	public void render() {
		// TODO Auto-generated method stub
		super.render();
		Gdx.gl.glClearColor(1.0f, 0f, 0f, 1.0f);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
		spriteBatch.begin();
		spriteBatch.draw(textureRegion, 0, 0);
		spriteBatch.end();
	}
	@Override
	public void dispose() {
		// TODO Auto-generated method stub
		super.dispose();
		spriteBatch.dispose();
		texture.dispose();
	}

}
