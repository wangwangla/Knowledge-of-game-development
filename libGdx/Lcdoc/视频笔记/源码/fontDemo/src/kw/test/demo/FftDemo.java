package kw.test.demo;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.Sprite;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.graphics.g2d.freetype.FreeTypeFontGenerator;
import com.badlogic.gdx.graphics.g2d.freetype.FreeTypeFontGenerator.FreeTypeBitmapFontData;
import com.badlogic.gdx.graphics.g2d.freetype.FreeTypeFontGenerator.FreeTypeFontParameter;

public class FftDemo  extends ApplicationAdapter{
	//font
	private BitmapFont bitmapFont;
	//generFont
	private FreeTypeFontGenerator fontGenerator;
	//map
	private FreeTypeBitmapFontData fontData;
	//batch
	private SpriteBatch batch;
	
	@Override
	public void create() {
		// TODO Auto-generated method stub
		super.create();
		// read fft file
		fontGenerator = new FreeTypeFontGenerator(Gdx.files.internal("vssetup.ttf"));
		//对 字体 进行 处理
		FreeTypeFontParameter parameter = new FreeTypeFontParameter();
		parameter.size=10;
		parameter.characters=fontGenerator.DEFAULT_CHARS+"见贤思齐";
		parameter.flip = false;
		fontData = fontGenerator.generateData(parameter);
		TextureRegion textureRegion =null;
		/*bitmapFont = new BitmapFont(fontData,,false);*/
		batch=new SpriteBatch();
	}
	@Override
	public void render() {
		// TODO Auto-generated method stub
		super.render();
		Gdx.gl.glClearColor(1.0f, 0f, 0f, 1.0f);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
		batch.begin();
		bitmapFont.draw(batch, "见贤思齐", 60, 59);
		batch.end();
	}
	@Override
	public void dispose() {
		// TODO Auto-generated method stub
		super.dispose();
		 
	}

}
