## 源码分析

- 游戏启动

  ```java
  public void initialize (ApplicationListener listener,AndroidApplicationConfiguration config) 
  {
     		//初始化
  		init(listener, config, false);
  }	
  ```

- 初始化代码

  ```java
  private void init (ApplicationListener listener, AndroidApplicationConfiguration config, boolean isForView) {
  		if (this.getVersion() < MINIMUM_SDK) {
  		}
      //创建日志
  	setApplicationLogger(new AndroidApplicationLogger());
  	//得到graphics
      graphics = new AndroidGraphics(this, config, config.resolutionStrategy == null ? new FillResolutionStrategy()
  			: config.resolutionStrategy);
      //输入
  	input = AndroidInputFactory.newAndroidInput(this, this, graphics.view, config);
  	//音频
      audio = new AndroidAudio(this, config);
  	this.getFilesDir(); // workaround for Android bug #10515463
  	files = new AndroidFiles(this.getAssets(), this.getFilesDir().getAbsolutePath());
  	net = new AndroidNet(this, config);
  	this.listener = listener;
  	this.handler = new Handler();
  	this.useImmersiveMode = config.useImmersiveMode;
  	this.hideStatusBar = config.hideStatusBar;
  	this.clipboard = new AndroidClipboard(this);
  
  	// Add a specialized audio lifecycle listener
      //加入音乐循环监听
  	addLifecycleListener(new LifecycleListener() {
  		@Override
  		public void resume () {
  			// No need to resume audio here
  		}
  		@Override
  		public void pause () {
  			audio.pause();
  		}
  		@Override
  		public void dispose () {
  			audio.dispose();
  		}
  	});
  	Gdx.app = this;
  	Gdx.input = this.getInput();
  	Gdx.audio = this.getAudio();
  	Gdx.files = this.getFiles();
  	Gdx.graphics = this.getGraphics();
  	Gdx.net = this.getNet();
  	if (!isForView) {
  		try {
  			requestWindowFeature(Window.FEATURE_NO_TITLE);
  		} catch (Exception ex) {
  			log("AndroidApplication", "Content already displayed, cannot request FEATURE_NO_TITLE", ex);
  		}
  		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);		getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
  			setContentView(graphics.getView(), createLayoutParams());
  		}
  	createWakeLock(config.useWakelock);
  	hideStatusBar(this.hideStatusBar);
  	useImmersiveMode(this.useImmersiveMode);
  	if (this.useImmersiveMode && getVersion() >= Build.VERSION_CODES.KITKAT) {
  		try {
  			Class<?> vlistener = Class.forName("com.badlogic.gdx.backends.android.AndroidVisibilityListener");
  			Object o = vlistener.newInstance();
  				Method method = vlistener.getDeclaredMethod("createListener", AndroidApplicationBase.class);
  				method.invoke(o, this);
  			} catch (Exception e) {
  				log("AndroidApplication", "Failed to create AndroidVisibilityListener", e);
  			}
  		}
  		
  	// detect an already connected bluetooth keyboardAvailable
  	if (getResources().getConfiguration().keyboard != Configuration.KEYBOARD_NOKEYS)
  		this.getInput().keyboardAvailable = true;
  	}
  ```

  



### 日志

- 调用

  ```java
  setApplicationLogger(new AndroidApplicationLogger());
  ```

  

- 代码

```java
public class AndroidApplicationLogger implements ApplicationLogger {

	@Override
	public void log (String tag, String message) {
		Log.i(tag, message);
	}

	@Override
	public void log (String tag, String message, Throwable exception) {
		Log.i(tag, message, exception);
	}

	@Override
	public void error (String tag, String message) {
		Log.e(tag, message);
	}

	@Override
	public void error (String tag, String message, Throwable exception) {
		Log.e(tag, message, exception);
	}

	@Override
	public void debug (String tag, String message) {
		Log.d(tag, message);
	}

	@Override
	public void debug (String tag, String message, Throwable exception) {
		Log.d(tag, message, exception);
	}
}

```

### Gride

- 创建

  ```java
  graphics = new AndroidGraphics(this, config, config.resolutionStrategy == null ? new FillResolutionStrategy()
  			: config.resolutionStrategy);
  ```

  

- 实现

  ```java
  public AndroidGraphics (AndroidApplicationBase application, AndroidApplicationConfiguration config,
  	ResolutionStrategy resolutionStrategy, boolean focusableView) {
  	this.config = config;
  	this.app = application;
      //创建View
  	view = createGLSurfaceView(application, resolutionStrategy);
  	preserveEGLContextOnPause();
  	if (focusableView) {
  		view.setFocusable(true);
  		view.setFocusableInTouchMode(true);
  	}
  }
  ```

  

  - createGLSurfaceView 

  ```java
  protected View createGLSurfaceView (AndroidApplicationBase application, final ResolutionStrategy resolutionStrategy) {
  	if (!checkGL20()) throw new GdxRuntimeException("Libgdx requires OpenGL ES 2.0");
  	//这个之后在说
  	EGLConfigChooser configChooser = getEglConfigChooser();
  	int sdkVersion = android.os.Build.VERSION.SDK_INT;
  	if (sdkVersion <= 10 && config.useGLSurfaceView20API18) {
  		GLSurfaceView20API18 view = new GLSurfaceView20API18(application.getContext(), resolutionStrategy);
  	if (configChooser != null)
  			view.setEGLConfigChooser(configChooser);
  	else
  				view.setEGLConfigChooser(config.r, config.g, config.b, config.a, config.depth, config.stencil);
  			view.setRenderer(this);
  			return view;
  		} else {
  			GLSurfaceView20 view = new GLSurfaceView20(application.getContext(), resolutionStrategy, config.useGL30 ? 3 : 2);
  			if (configChooser != null)
  				view.setEGLConfigChooser(configChooser);
  			else
  				view.setEGLConfigChooser(config.r, config.g, config.b, config.a, config.depth, config.stencil);
  			view.setRenderer(this);
  			return view;
  		}
  	}
  ```

  - View.setRender(this);

    ```java
     public void setRenderer(Renderer renderer) {
            checkRenderThreadState();
            if (mEGLConfigChooser == null) {
                mEGLConfigChooser = new SimpleEGLConfigChooser(true);
            }
            if (mEGLContextFactory == null) {
                mEGLContextFactory = new DefaultContextFactory();
            }
            if (mEGLWindowSurfaceFactory == null) {
                mEGLWindowSurfaceFactory = new DefaultWindowSurfaceFactory();
            }
            mRenderer = renderer;
            mGLThread = new GLThread(mThisWeakRef);
            mGLThread.start();
        }
    
    ```

    

  - 实现

    ```java
    GLSurfaceView20 view = new GLSurfaceView20(application.getContext(), resolutionStrategy, config.useGL30 ? 3 : 2);
    
    
    	private void init (boolean translucent, int depth, int stencil) {
    
    		/*
    		 * By default, GLSurfaceView() creates a RGB_565 opaque surface. If we want a translucent one, we should change the
    		 * surface's format here, using PixelFormat.TRANSLUCENT for GL Surfaces is interpreted as any 32-bit surface with alpha by
    		 * SurfaceFlinger.
    		 */
    		if (translucent) {
    			this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
    		}
    
    		/*
    		 * Setup the context factory for 2.0 rendering. See ContextFactory class definition below
    		 */
    		setEGLContextFactory(new ContextFactory());
    
    		/*
    		 * We need to choose an EGLConfig that matches the format of our surface exactly. This is going to be done in our custom
    		 * config chooser. See ConfigChooser class definition below.
    		 */
    		setEGLConfigChooser(translucent ? new ConfigChooser(8, 8, 8, 8, depth, stencil) : new ConfigChooser(5, 6, 5, 0, depth,
    			stencil));
    
    		/* Set the renderer responsible for frame rendering */
    	}
    
    ```

    



## Input

Input的操作顺序是，在Application中将Input获取到，在drawFrame方法中进行get。

Input方法在初始化的时候传入一个Processor，传入一个stage，然后将stage传入到当前的Processor中，通过调用父类的get方法将（我认为这里将processor传入到了系统中）

在AndroidInput构造函数中，它将事件注册，比如点击，触摸、键盘等注册，用户处理的时候是通过复写父类的方法实现onTouch和键盘监听方法，通过一个XXx方法将系统的event和当前Android Input传入，进行状态设置。

- 输入

  ```java
  	public AndroidInput (Application activity, Context context, Object view, AndroidApplicationConfiguration config) {
  		// we hook into View, for LWPs we call onTouch below directly from
  		// within the AndroidLivewallpaperEngine#onTouchEvent() method.
  		if (view instanceof View) {
  			View v = (View)view;
  			v.setOnKeyListener(this);
  			v.setOnTouchListener(this);
  			v.setFocusable(true);
  			v.setFocusableInTouchMode(true);
  			v.requestFocus();
  		}
  		this.config = config;
  		this.onscreenKeyboard = new AndroidOnscreenKeyboard(context, new Handler(), this);
  
  		for (int i = 0; i < realId.length; i++)
  			realId[i] = -1;
  		handle = new Handler();
  		this.app = activity;
  		this.context = context;
  		this.sleepTime = config.touchSleepTime;
  		touchHandler = new AndroidMultiTouchHandler();
  		hasMultitouch = touchHandler.supportsMultitouch(context);
  
  		vibrator = (Vibrator)context.getSystemService(Context.VIBRATOR_SERVICE);
  
  		int rotation = getRotation();
  		DisplayMode mode = app.getGraphics().getDisplayMode();
  		if (((rotation == 0 || rotation == 180) && (mode.width >= mode.height))
  			|| ((rotation == 90 || rotation == 270) && (mode.width <= mode.height))) {
  			nativeOrientation = Orientation.Landscape;
  		} else {
  			nativeOrientation = Orientation.Portrait;
  		}
  
  		// this is for backward compatibility: libGDX always caught the circle button, original comment:
  		// circle button on Xperia Play shouldn't need catchBack == true
  		keysToCatch.add(Keys.BUTTON_CIRCLE);
  	}
  
  ```

  输入的时候通过

  ```java
  touchHandler = new AndroidMultiTouchHandler();
  hasMultitouch = touchHandler.supportsMultitouch(context);
  
  ---------
  	public boolean supportsMultitouch (Context activity) {
  		return activity.getPackageManager().hasSystemFeature("android.hardware.touchscreen.multitouch");
  	}
  }
  ```

  实现方式

  ```java
  public class AndroidMultiTouchHandler implements AndroidTouchHandler {
  	public void onTouch (MotionEvent event, AndroidInput input) {
  		final int action = event.getAction() & MotionEvent.ACTION_MASK;
  		int pointerIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
  		int pointerId = event.getPointerId(pointerIndex);
  
  		int x = 0, y = 0;
  		int realPointerIndex = 0;
  		int button = Buttons.LEFT;
  
  		long timeStamp = System.nanoTime();
  		synchronized (input) {
  			switch (action) {
  			case MotionEvent.ACTION_DOWN:
  			case MotionEvent.ACTION_POINTER_DOWN:
  				realPointerIndex = input.getFreePointerIndex(); // get a free pointer index as reported by Input.getX() etc.
  				if (realPointerIndex >= AndroidInput.NUM_TOUCHES) break;
  				input.realId[realPointerIndex] = pointerId;
  				x = (int)event.getX(pointerIndex);
  				y = (int)event.getY(pointerIndex);
  				if (android.os.Build.VERSION.SDK_INT >= 14) button = toGdxButton(event.getButtonState());
  				if (button != -1) postTouchEvent(input, TouchEvent.TOUCH_DOWN, x, y, realPointerIndex, button, timeStamp);
  				input.touchX[realPointerIndex] = x;
  				input.touchY[realPointerIndex] = y;
  				input.deltaX[realPointerIndex] = 0;
  				input.deltaY[realPointerIndex] = 0;
  				input.touched[realPointerIndex] = (button != -1);
  				input.button[realPointerIndex] = button;
  				input.pressure[realPointerIndex] = event.getPressure(pointerIndex);
  				break;
  
  			case MotionEvent.ACTION_UP:
  			case MotionEvent.ACTION_POINTER_UP:
  			case MotionEvent.ACTION_OUTSIDE:
  				realPointerIndex = input.lookUpPointerIndex(pointerId);
  				if (realPointerIndex == -1) break;
  				if (realPointerIndex >= AndroidInput.NUM_TOUCHES) break;
  				input.realId[realPointerIndex] = -1;
  				x = (int)event.getX(pointerIndex);
  				y = (int)event.getY(pointerIndex);
  				button = input.button[realPointerIndex];
  				if (button != -1) postTouchEvent(input, TouchEvent.TOUCH_UP, x, y, realPointerIndex, button, timeStamp);
  				input.touchX[realPointerIndex] = x;
  				input.touchY[realPointerIndex] = y;
  				input.deltaX[realPointerIndex] = 0;
  				input.deltaY[realPointerIndex] = 0;
  				input.touched[realPointerIndex] = false;
  				input.button[realPointerIndex] = 0;
  				input.pressure[realPointerIndex] = 0;
  				break;
  
  			case MotionEvent.ACTION_CANCEL:
  				for (int i = 0; i < input.realId.length; i++) {
  					input.realId[i] = -1;
  					input.touchX[i] = 0;
  					input.touchY[i] = 0;
  					input.deltaX[i] = 0;
  					input.deltaY[i] = 0;
  					input.touched[i] = false;
  					input.button[i] = 0;
  					input.pressure[i] = 0;
  				}
  				break;
  
  			case MotionEvent.ACTION_MOVE:
  				int pointerCount = event.getPointerCount();
  				for (int i = 0; i < pointerCount; i++) {
  					pointerIndex = i;
  					pointerId = event.getPointerId(pointerIndex);
  					x = (int)event.getX(pointerIndex);
  					y = (int)event.getY(pointerIndex);
  					realPointerIndex = input.lookUpPointerIndex(pointerId);
  					if (realPointerIndex == -1) continue;
  					if (realPointerIndex >= AndroidInput.NUM_TOUCHES) break;
  					button = input.button[realPointerIndex];
  					if (button != -1)
  						postTouchEvent(input, TouchEvent.TOUCH_DRAGGED, x, y, realPointerIndex, button, timeStamp);
  					else
  						postTouchEvent(input, TouchEvent.TOUCH_MOVED, x, y, realPointerIndex, 0, timeStamp);
  					input.deltaX[realPointerIndex] = x - input.touchX[realPointerIndex];
  					input.deltaY[realPointerIndex] = y - input.touchY[realPointerIndex];
  					input.touchX[realPointerIndex] = x;
  					input.touchY[realPointerIndex] = y;
  					input.pressure[realPointerIndex] = event.getPressure(pointerIndex);
  				}
  				break;
  			}
  		}
  		Gdx.app.getGraphics().requestRendering();
  	}
  
  	private void logAction (int action, int pointer) {
  		String actionStr = "";
  		if (action == MotionEvent.ACTION_DOWN)
  			actionStr = "DOWN";
  		else if (action == MotionEvent.ACTION_POINTER_DOWN)
  			actionStr = "POINTER DOWN";
  		else if (action == MotionEvent.ACTION_UP)
  			actionStr = "UP";
  		else if (action == MotionEvent.ACTION_POINTER_UP)
  			actionStr = "POINTER UP";
  		else if (action == MotionEvent.ACTION_OUTSIDE)
  			actionStr = "OUTSIDE";
  		else if (action == MotionEvent.ACTION_CANCEL)
  			actionStr = "CANCEL";
  		else if (action == MotionEvent.ACTION_MOVE)
  			actionStr = "MOVE";
  		else
  			actionStr = "UNKNOWN (" + action + ")";
  		Gdx.app.log("AndroidMultiTouchHandler", "action " + actionStr + ", Android pointer id: " + pointer);
  	}
  
  	private int toGdxButton (int button) {
  		if (button == 0 || button == 1) return Buttons.LEFT;
  		if (button == 2) return Buttons.RIGHT;
  		if (button == 4) return Buttons.MIDDLE;
  		if (button == 8) return Buttons.BACK;
  		if (button == 16) return Buttons.FORWARD;
  		return -1;
  	}
  
  	private void postTouchEvent (AndroidInput input, int type, int x, int y, int pointer, int button, long timeStamp) {
  		TouchEvent event = input.usedTouchEvents.obtain();
  		event.timeStamp = timeStamp;
  		event.pointer = pointer;
  		event.x = x;
  		event.y = y;
  		event.type = type;
  		event.button = button;
  		input.touchEvents.add(event);
  	}
  
  	public boolean supportsMultitouch (Context activity) {
  		return activity.getPackageManager().hasSystemFeature("android.hardware.touchscreen.multitouch");
  	}
  }
  
  ```

通过系统的MotionEvent获取当前按下的坐标。





## libGdx的game

```java
public abstract class Game implements ApplicationListener {
	protected Screen screen;
	public void dispose () {}
	public void pause (){}
	public void resume (){}
	public void render (){}
	public void resize (int width, int height) {}
	public void setScreen (Screen screen){}
	public Screen getScreen (){}
}
```
从上面看game就是提供一个对screen的管理，他实现了声明周期方法之外，还提供了对screen的操作。
之前也说过，ApplicationAdapter对所有方法进行了一个空实现。
目的是为了子类可以更少的写出所有的方法。

## ApplicationListener它是一个接口，定义了一下常需要使用的方法
```java
public interface ApplicationListener {
	/** 第一次调用的是时候使用. */
	public void create ();

	/** 当他的大小发生改变的时候.他可以在灭有pause状态但是绝不会调用create之前发生*/
	public void resize (int width, int height);

	/** 当应用程序展示它自己的时候Called Called when the {@link Application} should render itself. */
	public void render ();

	/** 
	当应用程序pause的时候，通常在不活跃或者不可见的时候，也可以在销毁之前。
	Called when the {@link Application} is paused, usually when it's not active or visible 
	on-screen. An Application is also paused before it is destroyed. */
	public void pause ();

	/** 当处于resume状态，或者是在失去焦点的时候 
	Called when the {@link Application} is resumed from a paused state, usually when it regains focus. */
	public void resume ();

	/** Called when the {@link Application} is destroyed. Preceded by a call to {@link #pause()}. */
	public void dispose ();
}
```

## Audio
```java
/** 
这个接口是用来创建和管理audio资源的，他可以获取到目录，允许去调用音频硬件设备和AudioRecorder接口，可以创建并且可以
播放

所有创建的via接口需要调用dispose。

注意：所有的都会自动调用pause和resume，当应用程序调用Application的pause和resume方法，

public interface Audio {
	创建一个音频设备或者stereo，不在的使用的时候需要将其dispose  
	当不可以创建 的时候会抛出异常
	public AudioDevice newAudioDevice (int samplingRate, boolean isMono);

	创建一个音频的解码器，它也需要调用dispose
	public AudioRecorder newAudioRecorder (int samplingRate, boolean isMono);

	 创建需要一个FileHandler，
	 注意：完成数据是通过RAM加载的，所以不要使用达到文件，目前的大小限制是1M
	 支持的格式是WAV,MP3,OGG
	 它也需要调用dispose
		不能加载也会抛出异常
	public Sound newSound (FileHandle fileHandle);

	通过流晚间创建一个实体，它支持的格式是 WAV, MP3 and OGG.他需要进行调用dispose，并且会自动调用
	pause和resume
	public Music newMusic (FileHandle file);
}
```
## file
```java
/** 提供一个标准的文件系统, classpath, Android SD card, and Android assets directory.*/
public interface Files {
	/** （指示） how to resolve a path to a file.
	public enum FileType {
		 根目录相对位置的classpath
		 但是通常他是不可以完成安卓的，比如说{@link Audio#newSound(FileHandle)} and{@link Audio#newMusic(FileHandle)}.
		Classpath,

		 相对位置asset目录在安卓并且在应用程序的根目录在桌面应用
		 如果找不到，应该检查一下classpath,在JWS或者applets应用，他一般是可以在找到的。
		Internal,

		一般是在SD卡，或者是当前桌面的家目录。
		External,

		/** Path that is a fully qualified, absolute filesystem path. To ensure portability across platforms use absolute files only
		 * when absolutely (heh) necessary. */
		Absolute,

		/** Path relative to the private files directory on Android and to the application's root directory on the desktop. */
		Local;
	}

	返回FIleHandle,目录找不到会抛出异常。
	public FileHandle getFileHandle (String path, FileType type);

	/** Convenience method that returns a {@link FileType#Classpath} file handle. */
	public FileHandle classpath (String path);

	/** Convenience method that returns a {@link FileType#Internal} file handle. */
	public FileHandle internal (String path);

	/** Convenience method that returns a {@link FileType#External} file handle. */
	public FileHandle external (String path);

	/** Convenience method that returns a {@link FileType#Absolute} file handle. */
	public FileHandle absolute (String path);

	/** Convenience method that returns a {@link FileType#Local} file handle. */
	public FileHandle local (String path);

	/** Returns the external storage path directory. This is the SD card on Android and the home directory of the current user on
	 * the desktop. */
	public String getExternalStoragePath ();

	/** Returns true if the external storage is ready for file IO. Eg, on Android, the SD card is not available when mounted for use
	 * with a PC. */
	public boolean isExternalStorageAvailable ();

	/** Returns the local storage path directory. This is the private files directory on Android and the directory of the jar on the
	 * desktop. */
	public String getLocalStoragePath ();

	/** Returns true if the local storage is ready for file IO. */
	public boolean isLocalStorageAvailable ();
}
```

## Graphics
```java
public interface Graphics {
	枚举了不同类型的实现
	public enum GraphicsType {
		AndroidGL, LWJGL, WebGL, iOSGL, JGLFW, Mock, LWJGL3
	}

	/** Describe a fullscreen display mode */
	设置全屏展示模式
	public class DisplayMode {
		/** the width in physical pixels **/
		public final int width;
		/** the height in physical pixles **/
		public final int height;
		/** the refresh rate in Hertz **/
		public final int refreshRate;
		/** the number of bits per pixel, may exclude alpha **/
		public final int bitsPerPixel;

		protected DisplayMode (int width, int height, int refreshRate, int bitsPerPixel) {
			this.width = width;
			this.height = height;
			this.refreshRate = refreshRate;
			this.bitsPerPixel = bitsPerPixel;
		}

		public String toString () {
			return width + "x" + height + ", bpp: " + bitsPerPixel + ", hz: " + refreshRate;
		}
	}

	/** Describes a monitor
	 *
	 * @author badlogic
	 */
	public class Monitor {
		public final int virtualX;
		public final int virtualY;
		public final String name;

		protected Monitor (int virtualX, int virtualY, String name) {
			this.virtualX = virtualX;
			this.virtualY = virtualY;
			this.name = name;
		}
	}

	/** Class describing the bits per pixel, depth buffer precision, stencil precision and number of MSAA samples. */
	public static class BufferFormat {
		/* number of bits per color channel */
		public final int r, g, b, a;
		/* number of bits for depth and stencil buffer */
		public final int depth, stencil;
		/** number of samples for multi-sample anti-aliasing (MSAA) **/
		public final int samples;
		/** whether coverage sampling anti-aliasing is used. in that case you have to clear the coverage buffer as well! */
		public final boolean coverageSampling;

		public BufferFormat (int r, int g, int b, int a, int depth, int stencil, int samples, boolean coverageSampling) {
			this.r = r;
			this.g = g;
			this.b = b;
			this.a = a;
			this.depth = depth;
			this.stencil = stencil;
			this.samples = samples;
			this.coverageSampling = coverageSampling;
		}

		public String toString () {
			return "r: " + r + ", g: " + g + ", b: " + b + ", a: " + a + ", depth: " + depth + ", stencil: " + stencil
				+ ", num samples: " + samples + ", coverage sampling: " + coverageSampling;
		}
	}

	/** Returns whether OpenGL ES 3.0 is available. If it is you can get an instance of {@link GL30} via {@link #getGL30()} to
	 * access OpenGL ES 3.0 functionality. Note that this functionality will only be available if you instructed the
	 * {@link Application} instance to use OpenGL ES 3.0!
	 *
	 * @return whether OpenGL ES 3.0 is available */
	public boolean isGL30Available ();

	/** @return the {@link GL20} instance */
	public GL20 getGL20 ();

	/** @return the {@link GL30} instance or null if not supported */
	public GL30 getGL30 ();

	/** Set the GL20 instance **/
	public void setGL20 (GL20 gl20);

	/** Set the GL30 instance **/
	public void setGL30 (GL30 gl30);

	/** @return the width of the client area in logical pixels. */
	public int getWidth ();

	/** @return the height of the client area in logical pixels */
	public int getHeight ();

	/** @return the width of the framebuffer in physical pixels */
	public int getBackBufferWidth ();

	/** @return the height of the framebuffer in physical pixels */
	public int getBackBufferHeight ();

	/** Returns the id of the current frame. The general contract of this method is that the id is incremented only when the
	 * application is in the running state right before calling the {@link ApplicationListener#render()} method. Also, the id of
	 * the first frame is 0; the id of subsequent frames is guaranteed to take increasing values for 2<sup>63</sup>-1 rendering
	 * cycles.
	 * @return the id of the current frame */
	public long getFrameId ();

	/** @return the time span between the current frame and the last frame in seconds. Might be smoothed over n frames. */
	public float getDeltaTime ();

	/** @return the time span between the current frame and the last frame in seconds, without smoothing **/
	public float getRawDeltaTime ();

	/** @return the average number of frames per second */
	public int getFramesPerSecond ();

	/** @return the {@link GraphicsType} of this Graphics instance */
	public GraphicsType getType ();

	/** @return the {@link GLVersion} of this Graphics instance */
	public GLVersion getGLVersion ();

	/** @return the pixels per inch on the x-axis */
	public float getPpiX ();

	/** @return the pixels per inch on the y-axis */
	public float getPpiY ();

	/** @return the pixels per centimeter on the x-axis */
	public float getPpcX ();

	/** @return the pixels per centimeter on the y-axis. */
	public float getPpcY ();

	/** This is a scaling factor for the Density Independent Pixel unit, following the same conventions as
	 * android.util.DisplayMetrics#density, where one DIP is one pixel on an approximately 160 dpi screen. Thus on a 160dpi screen
	 * this density value will be 1; on a 120 dpi screen it would be .75; etc.
	 *
	 * @return the logical density of the Display. */
	public float getDensity ();

	/** Whether the given backend supports a display mode change via calling {@link Graphics#setFullscreenMode(DisplayMode)}
	 *
	 * @return whether display mode changes are supported or not. */
	public boolean supportsDisplayModeChange ();

	/** @return the primary monitor **/
	public Monitor getPrimaryMonitor();

	/** @return the monitor the application's window is located on */
	public Monitor getMonitor();

	/** @return the currently connected {@link Monitor}s */
	public Monitor[] getMonitors();

	/** @return the supported fullscreen {@link DisplayMode}(s) of the monitor the window is on */
	public DisplayMode[] getDisplayModes ();

	/** @return the supported fullscreen {@link DisplayMode}s of the given {@link Monitor} */
	public DisplayMode[] getDisplayModes(Monitor monitor);

	/** @return the current {@link DisplayMode} of the monitor the window is on. */
	public DisplayMode getDisplayMode ();

	/** @return the current {@link DisplayMode} of the given {@link Monitor} */
	public DisplayMode getDisplayMode (Monitor monitor);

	/** Sets the window to full-screen mode.
	 *
	 * @param displayMode the display mode.
	 * @return whether the operation succeeded. */
	public boolean setFullscreenMode (DisplayMode displayMode);

	/** Sets the window to windowed mode.
	 *
	 * @param width the width in pixels
	 * @param height the height in pixels
	 * @return whether the operation succeeded*/
	public boolean setWindowedMode (int width, int height);

	/** Sets the title of the window. Ignored on Android.
	 *
	 * @param title the title. */
	public void setTitle (String title);

	/** Sets the window decoration as enabled or disabled. On Android, this will enable/disable
	 *  the menu bar.
	 *
	 *  Note that immediate behavior of this method may vary depending on the implementation. It
	 *  may be necessary for the window to be recreated in order for the changes to take effect.
	 *  Consult the documentation for the backend in use for more information.
	 *
	 *  Supported on all GDX desktop backends and on Android (to disable the menu bar).
	 *
	 * @param undecorated true if the window border or status bar should be hidden. false otherwise.
	 */
	public void setUndecorated (boolean undecorated);

	/** Sets whether or not the window should be resizable. Ignored on Android.
	 *
	 *  Note that immediate behavior of this method may vary depending on the implementation. It
	 *  may be necessary for the window to be recreated in order for the changes to take effect.
	 *  Consult the documentation for the backend in use for more information.
	 *
	 *  Supported on all GDX desktop backends.
	 *
	 * @param resizable
	 */
	public void setResizable (boolean resizable);

	/** Enable/Disable vsynching. This is a best-effort attempt which might not work on all platforms.
	 *
	 * @param vsync vsync enabled or not. */
	public void setVSync (boolean vsync);

	/** @return the format of the color, depth and stencil buffer in a {@link BufferFormat} instance */
	public BufferFormat getBufferFormat ();

	/** @param extension the extension name
	 * @return whether the extension is supported */
	public boolean supportsExtension (String extension);

	/** Sets whether to render continuously. In case rendering is performed non-continuously, the following events will trigger a
	 * redraw:
	 *
	 * <ul>
	 * <li>A call to {@link #requestRendering()}</li>
	 * <li>Input events from the touch screen/mouse or keyboard</li>
	 * <li>A {@link Runnable} is posted to the rendering thread via {@link Application#postRunnable(Runnable)}. In the case
	 * of a multi-window app, all windows will request rendering if a runnable is posted to the application. To avoid this, 
	 * post a runnable to the window instead. </li>
	 * </ul>
	 *
	 * Life-cycle events will also be reported as usual, see {@link ApplicationListener}. This method can be called from any
	 * thread.
	 *
	 * @param isContinuous whether the rendering should be continuous or not. */
	public void setContinuousRendering (boolean isContinuous);

	/** @return whether rendering is continuous. */
	public boolean isContinuousRendering ();

	/** Requests a new frame to be rendered if the rendering mode is non-continuous. This method can be called from any thread. */
	public void requestRendering ();

	/** Whether the app is fullscreen or not */
	public boolean isFullscreen ();

	/** Create a new cursor represented by the {@link com.badlogic.gdx.graphics.Pixmap}. The Pixmap must be in RGBA8888 format,
	 * width & height must be powers-of-two greater than zero (not necessarily equal) and of a certain minimum size (32x32 is a safe bet),
	 * and alpha transparency must be single-bit (i.e., 0x00 or 0xFF only). This function returns a Cursor object that can be set as the 
	 * system cursor by calling {@link #setCursor(Cursor)} .
	 *
	 * @param pixmap the mouse cursor image as a {@link com.badlogic.gdx.graphics.Pixmap}
	 * @param xHotspot the x location of the hotspot pixel within the cursor image (origin top-left corner)
	 * @param yHotspot the y location of the hotspot pixel within the cursor image (origin top-left corner)
	 * @return a cursor object that can be used by calling {@link #setCursor(Cursor)} or null if not supported */
	public Cursor newCursor (Pixmap pixmap, int xHotspot, int yHotspot);

	/** Only viable on the lwjgl-backend and on the gwt-backend. Browsers that support cursor:url() and support the png format (the
	 * pixmap is converted to a data-url of type image/png) should also support custom cursors. Will set the mouse cursor image to
	 * the image represented by the {@link com.badlogic.gdx.graphics.Cursor}. It is recommended to call this function in the main render thread, and maximum one time per frame.
	 *
	 * @param cursor the mouse cursor as a {@link com.badlogic.gdx.graphics.Cursor} */
	public void setCursor (Cursor cursor);

	/**
	 * Sets one of the predefined {@link SystemCursor}s
	 */
	public void setSystemCursor(SystemCursor systemCursor);
}
```

## Input
```java

```

### InputAdapter

仅仅是实现了InputProcessor的接口，里面么有啥实现。

```java
public class InputAdapter implements InputProcessor {
	public boolean keyDown (int keycode) {
		return false;
	}

	public boolean keyUp (int keycode) {
		return false;
	}

	public boolean keyTyped (char character) {
		return false;
	}

	public boolean touchDown (int screenX, int screenY, int pointer, int button) {
		return false;
	}

	public boolean touchUp (int screenX, int screenY, int pointer, int button) {
		return false;
	}

	public boolean touchDragged (int screenX, int screenY, int pointer) {
		return false;
	}

	@Override
	public boolean mouseMoved (int screenX, int screenY) {
		return false;
	}

	@Override
	public boolean scrolled (int amount) {
		return false;
	}
}
```










//资源管理器
public AssetManager () {
	this(new InternalFileHandleResolver());
}

/** Creates a new AssetManager with all default loaders. */
public AssetManager (FileHandleResolver resolver) {
	setLoader(BitmapFont.class, new BitmapFontLoader(resolver));
	setLoader(Music.class, new MusicLoader(resolver));
	setLoader(Pixmap.class, new PixmapLoader(resolver));
	setLoader(Sound.class, new SoundLoader(resolver));
	setLoader(TextureAtlas.class, new TextureAtlasLoader(resolver));
	setLoader(Texture.class, new TextureLoader(resolver));
	setLoader(Skin.class, new SkinLoader(resolver));
	setLoader(ParticleEffect.class, new ParticleEffectLoader(resolver));
	setLoader(com.badlogic.gdx.graphics.g3d.particles.ParticleEffect.class,
		new com.badlogic.gdx.graphics.g3d.particles.ParticleEffectLoader(resolver));
	setLoader(PolygonRegion.class, new PolygonRegionLoader(resolver));
	setLoader(I18NBundle.class, new I18NBundleLoader(resolver));
	setLoader(Model.class, ".g3dj", new G3dModelLoader(new JsonReader(), resolver));
	setLoader(Model.class, ".g3db", new G3dModelLoader(new UBJsonReader(), resolver));
	setLoader(Model.class, ".obj", new ObjLoader(resolver));
	executor = new AsyncExecutor(1);
}



## 加载之后，等待finshLoading
```java
public void finishLoading () {
	log.debug("Waiting for loading to complete...");
	while (!update())
		//停下来等待结束了，在一起执行。
		ThreadUtils.yield();
	log.debug("Loading complete.");
}
```