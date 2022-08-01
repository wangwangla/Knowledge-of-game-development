## android源码分析
AndroidApplication
安卓应用启动继承这个class类，通过此类就可以创建一个活动，这个方法调用initialize进行初始化，进行配置绘制。


### AndroidAudio
- 他是实现了android的Audio接口。
- 构造函数
	- 通过创建SoundPool来实现的，调用安卓的AudioManager服务
```java
public AndroidAudio (Context context, AndroidApplicationConfiguration config) {
if (!config.disableAudio) {
soundPool = new SoundPool(config.maxSimultaneousSounds, AudioManager.STREAM_MUSIC, 100);
manager = (AudioManager)context.getSystemService(Context.AUDIO_SERVICE);
if (context instanceof Activity) {
((Activity)context).setVolumeControlStream(AudioManager.STREAM_MUSIC);
```
- dispose
将数组中的music都执行dispose方法，使用锁的方式一个一个的执行
```java
public void dispose () {
	if (soundPool == null) {
		return;
	}
	synchronized (musics) {
		// gah i hate myself.... music.dispose() removes the music from the list...
		ArrayList<AndroidMusic> musicsCopy = new ArrayList<AndroidMusic>(musics);
		for (AndroidMusic music : musicsCopy) {
			music.dispose();
		}
	}
	soundPool.release();
}
```
- pause
当执行声明之前pause方法的时候，如果是正在播放，那么就将状态设置为true，如不是那就是false。
```java
protected void pause () {
	if (soundPool == null) {
		return;
	}
	synchronized (musics) {
		for (AndroidMusic music : musics) {
			if (music.isPlaying()) {
				music.pause();
				music.wasPlaying = true;					
			} else
				music.wasPlaying = false;
		}
	}
	this.soundPool.autoPause();
}
```
- 增加music
	- 首先判断文件是从哪里来的
	- 创建MediaPlayer
	- 将MediaPlayer放入到集合中。
	```java
	MediaPlayer mediaPlayer = new MediaPlayer();
	mediaPlayer.setDataSource(descriptor.getFileDescriptor(), descriptor.getStartOffset(), descriptor.getLength());
	mediaPlayer.prepare();
	AndroidMusic music = new AndroidMusic(this, mediaPlayer);
	synchronized (musics) {
		musics.add(music);
	}

	```
```java
/** {@inheritDoc} */
	@Override
	public Music newMusic (FileHandle file) {
		if (soundPool == null) {
			throw new GdxRuntimeException("Android audio is not enabled by the application config.");
		}
		AndroidFileHandle aHandle = (AndroidFileHandle)file;

		MediaPlayer mediaPlayer = new MediaPlayer();

		if (aHandle.type() == FileType.Internal) {
			try {
				AssetFileDescriptor descriptor = aHandle.assets.openFd(aHandle.path());
				mediaPlayer.setDataSource(descriptor.getFileDescriptor(), descriptor.getStartOffset(), descriptor.getLength());
				descriptor.close();
				mediaPlayer.prepare();
				AndroidMusic music = new AndroidMusic(this, mediaPlayer);
				synchronized (musics) {
					musics.add(music);
				}
				return music;
			} catch (Exception ex) {
				throw new GdxRuntimeException("Error loading audio file: " + file
					+ "\nNote: Internal audio files must be placed in the assets directory.", ex);
			}
		} else {
			try {
				mediaPlayer.setDataSource(aHandle.file().getPath());
				mediaPlayer.prepare();
				AndroidMusic music = new AndroidMusic(this, mediaPlayer);
				synchronized (musics) {
					musics.add(music);
				}
				return music;
			} catch (Exception ex) {
				throw new GdxRuntimeException("Error loading audio file: " + file, ex);
			}
		}

	}

```
- 




public final class AndroidAudio implements Audio {
	private final SoundPool soundPool;
	private final AudioManager manager;
	protected final List<AndroidMusic> musics = new ArrayList<AndroidMusic>();


	protected void pause () {
		if (soundPool == null) {
			return;
		}
		synchronized (musics) {
			for (AndroidMusic music : musics) {
				if (music.isPlaying()) {
					music.pause();
					music.wasPlaying = true;					
				} else
					music.wasPlaying = false;
			}
		}
		this.soundPool.autoPause();
	}

	protected void resume () {
		if (soundPool == null) {
			return;
		}
		synchronized (musics) {
			for (int i = 0; i < musics.size(); i++) {
				if (musics.get(i).wasPlaying == true) musics.get(i).play();
			}
		}
		this.soundPool.autoResume();
	}

	/** {@inheritDoc} */
	@Override
	public AudioDevice newAudioDevice (int samplingRate, boolean isMono) {
		if (soundPool == null) {
			throw new GdxRuntimeException("Android audio is not enabled by the application config.");
		}
		return new AndroidAudioDevice(samplingRate, isMono);
	}

	/** {@inheritDoc} */
	@Override
	public Music newMusic (FileHandle file) {
		if (soundPool == null) {
			throw new GdxRuntimeException("Android audio is not enabled by the application config.");
		}
		AndroidFileHandle aHandle = (AndroidFileHandle)file;

		MediaPlayer mediaPlayer = new MediaPlayer();

		if (aHandle.type() == FileType.Internal) {
			try {
				AssetFileDescriptor descriptor = aHandle.assets.openFd(aHandle.path());
				mediaPlayer.setDataSource(descriptor.getFileDescriptor(), descriptor.getStartOffset(), descriptor.getLength());
				descriptor.close();
				mediaPlayer.prepare();
				AndroidMusic music = new AndroidMusic(this, mediaPlayer);
				synchronized (musics) {
					musics.add(music);
				}
				return music;
			} catch (Exception ex) {
				throw new GdxRuntimeException("Error loading audio file: " + file
					+ "\nNote: Internal audio files must be placed in the assets directory.", ex);
			}
		} else {
			try {
				mediaPlayer.setDataSource(aHandle.file().getPath());
				mediaPlayer.prepare();
				AndroidMusic music = new AndroidMusic(this, mediaPlayer);
				synchronized (musics) {
					musics.add(music);
				}
				return music;
			} catch (Exception ex) {
				throw new GdxRuntimeException("Error loading audio file: " + file, ex);
			}
		}

	}

	/** {@inheritDoc} */
	@Override
	public Sound newSound (FileHandle file) {
		if (soundPool == null) {
			throw new GdxRuntimeException("Android audio is not enabled by the application config.");
		}
		AndroidFileHandle aHandle = (AndroidFileHandle)file;
		if (aHandle.type() == FileType.Internal) {
			try {
				AssetFileDescriptor descriptor = aHandle.assets.openFd(aHandle.path());
				AndroidSound sound = new AndroidSound(soundPool, manager, soundPool.load(descriptor, 1));
				descriptor.close();
				return sound;
			} catch (IOException ex) {
				throw new GdxRuntimeException("Error loading audio file: " + file
					+ "\nNote: Internal audio files must be placed in the assets directory.", ex);
			}
		} else {
			try {
				return new AndroidSound(soundPool, manager, soundPool.load(aHandle.file().getPath(), 1));
			} catch (Exception ex) {
				throw new GdxRuntimeException("Error loading audio file: " + file, ex);
			}
		}
	}

	/** {@inheritDoc} */
	@Override
	public AudioRecorder newAudioRecorder (int samplingRate, boolean isMono) {
		if (soundPool == null) {
			throw new GdxRuntimeException("Android audio is not enabled by the application config.");
		}
		return new AndroidAudioRecorder(samplingRate, isMono);
	}

	/** Kills the soundpool and all other resources */
	public void dispose () {
		if (soundPool == null) {
			return;
		}
		synchronized (musics) {
			// gah i hate myself.... music.dispose() removes the music from the list...
			ArrayList<AndroidMusic> musicsCopy = new ArrayList<AndroidMusic>(musics);
			for (AndroidMusic music : musicsCopy) {
				music.dispose();
			}
		}
		soundPool.release();
	}
}



## net


```java
@Override
public boolean openURI (String URI) {
	boolean result = false;
	final Uri uri = Uri.parse(URI);
	Intent intent = new Intent(Intent.ACTION_VIEW, uri);
	PackageManager pm = app.getContext().getPackageManager();
	//这句是匹配一个活动，如果匹配成功就执行，否则就返回一个false
	if (pm.resolveActivity(intent, PackageManager.MATCH_DEFAULT_ONLY) != null) {
		app.runOnUiThread(new Runnable() {
			@Override
			public void run () {
				Intent intent = new Intent(Intent.ACTION_VIEW, uri);
				// LiveWallpaper and Daydream applications need this flag
				if (!(app.getContext() instanceof Activity))
					intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
				app.startActivity(intent);
			}
		});
		result = true;
	}
	return result;
}
```

## 网络返回一个byte
最原始的网络请求
- **创建一个URL**
- **获取连接**
- 设置请求方式
- 设置超时时间
- 读取的最大时间
- **读取流**
- **写入reader**
- **将结果返回**
```java
HttpURLConnection connection = null;
BufferedReader reader = null;
try{

	URL url = new URL("https://www.baidu.com");//新建URL
	connection = (HttpURLConnection)url.openConnection();//发起网络请求
	connection.setRequestMethod("GET");//请求方式
	connection.setConnectTimeout(8000);//连接最大时间
	connection.setReadTimeout(8000);//读取最大时间
	InputStream in = connection.getInputStream();
	reader = new BufferedReader(new InputStreamReader(in));//写入reader
	StringBuilder response = new StringBuilder();
	String line;
	while((line = reader.readLine()) != null){
		response.append(line);
	}
	showResponse(response.toString());//更新ui
```

- 安卓网络请求方式，在创建的时候，根据默认配置创建一个最大线程数，通过池创建
```java
public NetJavaImpl (int maxThreads) {
	executorService = new ThreadPoolExecutor(0, maxThreads,
			60L, TimeUnit.SECONDS,
			new SynchronousQueue<Runnable>(),
			new ThreadFactory() {
				@Override
				public Thread newThread(Runnable r) {
					Thread thread = new Thread(r, "NetThread");
					thread.setDaemon(true);
					return thread;
				}
			});
	connections = new ObjectMap<HttpRequest, HttpURLConnection>();
	listeners = new ObjectMap<HttpRequest, HttpResponseListener>();
}
```
- 创建连接
	- 判断URL是否为null.
	- 获取请求方法
	- 如果是get就拼接URL,否则就只设置url.
	- 获取连接
	- 对连接进行设置，包括方法和头
	- 设置监听，如果取消，那么就取消监听
	- 然后提交请求
	
	
```java
public void sendHttpRequest (final HttpRequest httpRequest, final HttpResponseListener httpResponseListener) {
		final HttpURLConnection connection = (HttpURLConnection)url.openConnection();
		// should be enabled to upload data.
		final boolean doingOutPut = method.equalsIgnoreCase(HttpMethods.POST) || method.equalsIgnoreCase(HttpMethods.PUT) || method.equalsIgnoreCase(HttpMethods.PATCH);
		connection.setDoOutput(doingOutPut);
		connection.setDoInput(true);
		connection.setRequestMethod(method);
		HttpURLConnection.setFollowRedirects(httpRequest.getFollowRedirects());

		putIntoConnectionsAndListeners(httpRequest, httpResponseListener, connection);

		// Headers get set regardless of the method
		for (Map.Entry<String, String> header : httpRequest.getHeaders().entrySet())
			connection.addRequestProperty(header.getKey(), header.getValue());

		// Set Timeouts
		connection.setConnectTimeout(httpRequest.getTimeOut());
		connection.setReadTimeout(httpRequest.getTimeOut());

		executorService.submit(new Runnable() {
			@Override
			public void run() {
				try {
					// Set the content for POST and PUT (GET has the information embedded in the URL)
					if (doingOutPut) {
						// we probably need to use the content as stream here instead of using it as a string.
						String contentAsString = httpRequest.getContent();
						if (contentAsString != null) {
							OutputStreamWriter writer = new OutputStreamWriter(connection.getOutputStream(), "UTF8");
							try {
								writer.write(contentAsString);
							} finally {
								StreamUtils.closeQuietly(writer);
							}
						} else {
							InputStream contentAsStream = httpRequest.getContentStream();
							if (contentAsStream != null) {
								OutputStream os = connection.getOutputStream();
								try {
									StreamUtils.copyStream(contentAsStream, os);
								} finally {
									StreamUtils.closeQuietly(os);
								}
							}
						}
					}

					connection.connect();

					final HttpClientResponse clientResponse = new HttpClientResponse(connection);
					try {
						HttpResponseListener listener = getFromListeners(httpRequest);

						if (listener != null) {
							listener.handleHttpResponse(clientResponse);
						}
						removeFromConnectionsAndListeners(httpRequest);
					} finally {
						connection.disconnect();
					}
				} catch (final Exception e) {
					connection.disconnect();
					try {
						httpResponseListener.failed(e);
					} finally {
						removeFromConnectionsAndListeners(httpRequest);
					}
				}
			}
		});
	} catch (Exception e) {
		try {
			httpResponseListener.failed(e);
		} finally {
			removeFromConnectionsAndListeners(httpRequest);
		}
		return;
	}
}

```

### 处理方式
得到输出流，输入流设置缓存区，将输出流返回，转化为数组的形式。

```jaba
private InputStream getInputStream () {
	try {
		return connection.getInputStream();
	} catch (IOException e) {
		return connection.getErrorStream();
	}
}
```

return StreamUtils.copyStreamToByteArray(input, connection.getContentLength());

```java
/** Copy the data from an {@link InputStream} to a byte array. The stream is not closed. */
public static byte[] copyStreamToByteArray (InputStream input) throws IOException {
	return copyStreamToByteArray(input, input.available());
}

	/** Copy the data from an {@link InputStream} to a byte array. The stream is not closed.
 * @param estimatedSize Used to allocate the output byte[] to possibly avoid an array copy. */
public static byte[] copyStreamToByteArray (InputStream input, int estimatedSize) throws IOException {
	ByteArrayOutputStream baos = new OptimizedByteArrayOutputStream(Math.max(0, estimatedSize));
	copyStream(input, baos);
	return baos.toByteArray();
}

//得到输出流，输入流设置缓存区，将输出流返回
public static int copyStream (InputStream input, ByteBuffer output, byte[] buffer) throws IOException {
	int startPosition = output.position(), total = 0, bytesRead;
	while ((bytesRead = input.read(buffer)) != -1) {
		BufferUtils.copy(buffer, 0, output, bytesRead);
		total += bytesRead;
		output.position(startPosition + total);
	}
	output.position(startPosition);
	return total;
}
```


## socket
- 是否连接
public boolean isConnected ();
- 读取参数
public InputStream getInputStream ();
- 返回一个流用于读写
public OutputStream getOutputStream ();
- 返回地址
public String getRemoteAddress ();

#### 实现
```java
// create the socket
socket = new java.net.Socket();
//对socket的惨呼进行设置
applyHints(hints); // better to call BEFORE socket is connected!
// and connect...
InetSocketAddress address = new InetSocketAddress(host, port);
if (hints != null) {
socket.connect(address, hints.connectTimeout);
else
socket.connect(address);
```
#### ServerSocket
```java
//创建server
server = new java.net.ServerSocket();
//设置参数
if (hints != null) {
server.setPerformancePreferences(hints.performancePrefConnectionTime, hints.performancePrefLatency,
hints.performancePrefBandwidth);
server.setReuseAddress(hints.reuseAddress);
server.setSoTimeout(hints.acceptTimeout);
server.setReceiveBufferSize(hints.receiveBufferSize);
}
//绑定地址
InetSocketAddress address;
if( hostname != null ) {
address = new InetSocketAddress(hostname, port); 
} else {
address = new InetSocketAddress(port);
}
if (hints != null) {
server.bind(address, hints.backlog);
} else {
server.bind(address);
}
//等待请求，堵塞的
@Override
public Socket accept (SocketHints hints) {
	try {
		return new NetJavaSocketImpl(server.accept(), hints);
	} catch (Exception e) {
		throw new GdxRuntimeException("Error accepting socket.", e);
	}
}
@Override
public void dispose () {
	if (server != null) {
		try {
			server.close();
			server = null;
		} catch (Exception e) {
			throw new GdxRuntimeException("Error closing server.", e);
	}
}
```

