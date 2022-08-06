package kw.test.demo;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.Button;
import com.badlogic.gdx.scenes.scene2d.ui.ImageButton;
import com.badlogic.gdx.scenes.scene2d.utils.TextureRegionDrawable;

public class DrawableDemo extends ApplicationAdapter{
	
	private Stage stage;
	private TextureRegionDrawable textureRegionDrawable;
	private ImageButton imageButton;
	private TextureRegionDrawable up;
	private TextureRegionDrawable down;
	private TextureRegion buttonUp;
	private TextureRegion buttonDown;
	private Texture texture;
	
	@Override
	public void create() {
		// TODO Auto-generated method stub
		super.create();
	
		buttonDown = new TextureRegion(new Texture(Gdx.files.internal("bu.png")));
		buttonUp = new TextureRegion(new Texture(Gdx.files.internal("1.png")));
		up = new TextureRegionDrawable(buttonUp);
		down = new TextureRegionDrawable(buttonDown);
		imageButton= new ImageButton(up,down);
		stage = new Stage();
		stage.addActor(imageButton);
		Gdx.input.setInputProcessor(stage);
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
	
	
}
