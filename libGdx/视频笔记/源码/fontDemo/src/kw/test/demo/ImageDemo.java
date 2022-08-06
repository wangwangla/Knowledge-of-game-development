package kw.test.demo;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.Image;

public class ImageDemo extends ApplicationAdapter{
	private Stage stage;
	private Texture texture;
	private TextureRegion textureRegion;
	private Image image;
	@Override
	public void create() {
		// TODO Auto-generated method stub
		super.create();
		stage = new Stage();
		texture = new Texture(Gdx.files.internal("1.png"));
		textureRegion = new TextureRegion();
		textureRegion.setRegion(texture);
		image = new Image(textureRegion);
		stage=new Stage();
		stage.addActor(image);
	}
	@Override
	public void render() {
		// TODO Auto-generated method stub
		super.render();
		stage.act();
		stage.draw();
	}

}
