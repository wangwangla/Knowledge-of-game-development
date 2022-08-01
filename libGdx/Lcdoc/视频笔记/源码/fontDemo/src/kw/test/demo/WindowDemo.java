package kw.test.demo;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input;
import com.badlogic.gdx.InputProcessor;
import com.badlogic.gdx.Input.Orientation;
import com.badlogic.gdx.Input.Peripheral;
import com.badlogic.gdx.Input.TextInputListener;
import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Pixmap;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.scenes.scene2d.InputEvent;
import com.badlogic.gdx.scenes.scene2d.InputListener;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.ImageButton;
import com.badlogic.gdx.scenes.scene2d.ui.Window;
import com.badlogic.gdx.scenes.scene2d.ui.Window.WindowStyle;
import com.badlogic.gdx.scenes.scene2d.utils.TextureRegionDrawable;
import com.badlogic.gdx.utils.viewport.StretchViewport;

public class WindowDemo extends ApplicationAdapter{
	private SpriteBatch batch;
	private Texture texture;
	private ImageButton btn_A_OK;
	private ImageButton btn_B_CANCEL;
	private ImageButton btn_SHOW;
	
	private Stage stage;
	private BitmapFont font;
	private Window dio;
	
	
	@Override
	public void render() {
		// TODO Auto-generated method stub
		super.render();
		Gdx.gl.glClearColor(1.0f, 1.0f, 1.0f,1.0f);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
		batch.begin();
		font.setColor(Color.RED);
		font.draw(batch, "XXXX", 200,200);
		batch.end();
		stage.act();
		stage.draw();
	}
	
	@Override
	public void create() {
		// TODO Auto-generated method stub
		super.create();
		stage = new Stage(new StretchViewport(480, 320));
		batch = new SpriteBatch();
		font = new BitmapFont(Gdx.files.internal("Demo.fnt"), Gdx.files.internal("Demo.png"),false);
		setButton();
		setListener();
		setWindow();
		stage.addActor(btn_SHOW);
		Gdx.input.setInputProcessor(stage);
	}
	public void  setButton(){
		TextureRegionDrawable showUp = new TextureRegionDrawable(new TextureRegion(new Texture(Gdx.files.internal("up.png"))));
		TextureRegionDrawable showDown = new TextureRegionDrawable(new TextureRegion(new Texture(Gdx.files.internal("down.png"))));
		
		TextureRegionDrawable A_Up = new TextureRegionDrawable(new TextureRegion(new Texture(Gdx.files.internal("up.png"))));
		TextureRegionDrawable A_Down = new TextureRegionDrawable(new TextureRegion(new Texture(Gdx.files.internal("down.png"))));
	
		TextureRegionDrawable B_Up = new TextureRegionDrawable(new TextureRegion(new Texture(Gdx.files.internal("up.png"))));
		TextureRegionDrawable B_Down = new TextureRegionDrawable(new TextureRegion(new Texture(Gdx.files.internal("down.png"))));
		
		btn_A_OK = new ImageButton(A_Up,A_Down);
		btn_B_CANCEL = new ImageButton(B_Up,B_Down);
		btn_SHOW = new ImageButton(showUp,showDown);
		
		
	}
	
	public void setListener(){
		btn_SHOW.addListener(new InputListener() {
			@Override
			public boolean touchDown(InputEvent event, float x, float y, int pointer, int button) {
				// TODO Auto-generated method stub
				stage.addActor(dio);
				return true;
				
			}
		});
		btn_A_OK.addListener(new InputListener() {
			@Override
			public boolean touchDown(InputEvent event, float x, float y, int pointer, int button) {
				// TODO Auto-generated method stub
				Gdx.app.exit();
				return true;
				
			}
		});
		btn_B_CANCEL.addListener(new InputListener() {
			@Override
			public boolean touchDown(InputEvent event, float x, float y, int pointer, int button) {
				// TODO Auto-generated method stub
				dio.remove();
				return true;
			}
		});
	}
	
	public void setWindow(){
		TextureRegionDrawable textureRegionDrawable = new TextureRegionDrawable(new TextureRegion(new Texture(Gdx.files.internal("1.png"))));
		WindowStyle style = new WindowStyle(font,Color.RED,textureRegionDrawable);
		dio  = new Window("Demo", style);
		//设置宽高
		dio.setWidth(Gdx.graphics.getWidth()/1.5f);
		dio.setHeight(Gdx.graphics.getHeight()/1.5f);
		
		//设置位置
		dio.setPosition(100, 80);
		//设置是否可以移动
		dio.setModal(true);
		//加入按钮
		btn_A_OK.setPosition(Gdx.graphics.getWidth()/10, 0);
		btn_B_CANCEL.setPosition(Gdx.graphics.getWidth()/3, 0);
		
		dio.addActor(btn_A_OK);
		dio.addActor(btn_B_CANCEL);
	}
}
