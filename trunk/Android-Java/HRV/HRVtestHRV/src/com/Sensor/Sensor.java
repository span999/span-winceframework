package com.Sensor;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.hardware.SensorManager;
import android.hardware.SensorListener;
import android.util.Log;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.RectF;

import android.os.Handler;
import android.os.Message;

/**
 * <h3>Application that displays the values of the acceleration sensor graphically.</h3>

<p>This demonstrates the {@link android.hardware.SensorManager android.hardware.SensorManager} class.

<h4>Demo</h4>
OS / Sensors
 
<h4>Source files</h4>
 * <table class="LinkTable">
 *         <tr>
 *             <td >src/com.example.android.apis/os/Sensors.java</td>
 *             <td >Sensors</td>
 *         </tr>
 * </table> 
 */
public class Sensor extends Activity {
    /** Tag string for our debug logs */
    private static final String TAG = "Sensor";

    private SensorManager mSensorManager;
    private GraphView mGraphView;

    public Handler mHandler;
    private int isStart;
	
    protected static final int GUINOTIFIER = 0x108;
    protected static final int STOP = 0x109;
	
    private class GraphView extends View implements SensorListener
    {
        private Bitmap  mBitmap;
        private Paint   mPaint = new Paint();
        private Canvas  mCanvas = new Canvas();
        private Path    mPath = new Path();
        private RectF   mRect = new RectF();
        private float   mLastValues[] = new float[3*2];
        private float   mOrientationValues[] = new float[3];
        private int     mColors[] = new int[3*2];
        private float   mLastX;
        private float   mLastY;
        private float   mScale[] = new float[2];
        private float   mYOffset;
        private float   mMaxX;
        private float   mSpeed = 1.0f;
        private float   mWidth;
        private float   mHeight;
        
        public GraphView(Context context) {
            super(context);
			///pre-set color
            mColors[0] = Color.argb(192, 255, 64, 64);
            mColors[1] = Color.argb(192, 64, 128, 64);
            mColors[2] = Color.argb(192, 64, 64, 255);
            mColors[3] = Color.argb(192, 64, 255, 255);
            mColors[4] = Color.argb(192, 128, 64, 128);
            mColors[5] = Color.argb(192, 255, 255, 64);
			///bit mask for the flag enabling antialiasing 
            mPaint.setFlags(Paint.ANTI_ALIAS_FLAG);
			///Set the rectangle's coordinates to the specified values.
            mRect.set(-0.5f, -0.5f, 0.5f, 0.5f);
			///Append the specified arc to the path as a new contour
            mPath.arcTo(mRect, 0, 180);
        }
        
        @Override
        protected void onSizeChanged(int w, int h, int oldw, int oldh) {
			///create bitmap with screen size
            mBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.RGB_565);
			///Specify a bitmap for the canvas to draw into.
            mCanvas.setBitmap(mBitmap);
			///Fill the entire canvas' bitmap
            mCanvas.drawColor(0xFFFFFFFF);
			
            mYOffset = h * 0.5f;
            mScale[0] = - (h * 0.5f * (1.0f / (SensorManager.STANDARD_GRAVITY * 2)));
            mScale[1] = - (h * 0.5f * (1.0f / (SensorManager.MAGNETIC_FIELD_EARTH_MAX)));
            mWidth = w;
            mHeight = h;
            if (mWidth < mHeight) {
                mMaxX = w;
            } else {
                mMaxX = w-50;
            }
			///for the first draw
            mLastX = mMaxX;
            mLastY = 200;
            super.onSizeChanged(w, h, oldw, oldh);
        }

        @Override
        protected void onDraw(Canvas canvas) {
            synchronized (this) {
                if (mBitmap != null) {
                    final Paint paint = mPaint;
                    final Path path = mPath;
                    ///color of circle outside
                    final int outer = 0xFFC0C0C0;
					///color of circle inside
                    final int inner = 0xFFff7010;
					
					///if we are full
                    if (mLastX >= mMaxX) {
                        mLastX = 0;
                        final Canvas cavas = mCanvas;
                        final float yoffset = mYOffset;
                        final float maxx = mMaxX;
                        final float oneG = SensorManager.STANDARD_GRAVITY * mScale[0];
                        paint.setColor(0xFFAAAAAA);
                        cavas.drawColor(0xFFFFFFFF);
						///draw three base line
                        cavas.drawLine(0, yoffset,      maxx, yoffset,      paint);
                        cavas.drawLine(0, yoffset+oneG, maxx, yoffset+oneG, paint);
                        cavas.drawLine(0, yoffset-oneG, maxx, yoffset-oneG, paint);
                    }
                    canvas.drawBitmap(mBitmap, 0, 0, null);

                    float[] values = mOrientationValues;
                    if (mWidth < mHeight) {
                        float w0 = mWidth * 0.333333f;
                        float w  = w0 - 32;
                        float x = w0*0.5f;
                        for (int i=0 ; i<3 ; i++) {
							///Based on saveFlags, can save the current matrix and clip onto a private stack. S
                            canvas.save(Canvas.MATRIX_SAVE_FLAG);
							///Preconcat the current matrix with the specified translation
                            canvas.translate(x, w*0.5f + 4.0f);
                            canvas.save(Canvas.MATRIX_SAVE_FLAG);
                            paint.setColor(outer);
							///Preconcat the current matrix with the specified scale.
                            canvas.scale(w, w);
							///draw three circle
                            canvas.drawOval(mRect, paint);
                            canvas.restore();
                            canvas.scale(w-5, w-5);
                            paint.setColor(inner);
							///Preconcat the current matrix with the specified rotation.
                            canvas.rotate(-values[i]);
                            canvas.drawPath(path, paint);
                            canvas.restore();
                            x += w0;
                        }
                    } else {
                        float h0 = mHeight * 0.333333f;
                        float h  = h0 - 32;
                        float y = h0*0.5f;
                        for (int i=0 ; i<3 ; i++) {
                            canvas.save(Canvas.MATRIX_SAVE_FLAG);
                            canvas.translate(mWidth - (h*0.5f + 4.0f), y);
                            canvas.save(Canvas.MATRIX_SAVE_FLAG);
                            paint.setColor(outer);
                            canvas.scale(h, h);
                            canvas.drawOval(mRect, paint);
                            canvas.restore();
                            canvas.scale(h-5, h-5);
                            paint.setColor(inner);
                            canvas.rotate(-values[i]);
                            canvas.drawPath(path, paint);
                            canvas.restore();
                            y += h0;
                        }
                    }

                }
            }
        }

        public void onSensorChanged(int sensor, float[] values) {
            //Log.d(TAG, "sensor: " + sensor + ", x: " + values[0] + ", y: " + values[1] + ", z: " + values[2]);
            synchronized (this) {
                if (mBitmap != null) {
                    final Canvas canvas = mCanvas;
                    final Paint paint = mPaint;
                    if (sensor == SensorManager.SENSOR_ORIENTATION) {
                        for (int i=0 ; i<3 ; i++) {
                            mOrientationValues[i] = values[i];
                        }
                    } else {
                        float deltaX = mSpeed;
                        float newX = mLastX + deltaX;

                        int j = (sensor == SensorManager.SENSOR_MAGNETIC_FIELD) ? 1 : 0;
                        for (int i=0 ; i<3 ; i++) {
                            int k = i+j*3;
                            final float v = mYOffset + values[i] * mScale[j];
                            paint.setColor(mColors[k]);
                            canvas.drawLine(mLastX, mLastValues[k], newX, v, paint);
                            mLastValues[k] = v;
                        }
                        if (sensor == SensorManager.SENSOR_MAGNETIC_FIELD)
                            mLastX += mSpeed;
                    }
                    invalidate();
                }
            }
        }

        public void doManualChanged(int sensor, float[] values) {
            //Log.d(TAG, "sensor: " + sensor + ", x: " + values[0] + ", y: " + values[1] + ", z: " + values[2]);
            synchronized (this) {
                if (mBitmap != null) {
                    final Canvas canvas = mCanvas;
                    final Paint paint = mPaint;
                    float NewX;
                    float NewY;
					Byte ECGdataArray[]; 
                /*
                    if (sensor == SensorManager.SENSOR_ORIENTATION) {
                        for (int i=0 ; i<3 ; i++) {
                            mOrientationValues[i] = values[i];
                        }
                    } else {
                        float deltaX = mSpeed;
                        float newX = mLastX + deltaX;

                        int j = (sensor == SensorManager.SENSOR_MAGNETIC_FIELD) ? 1 : 0;
                        for (int i=0 ; i<3 ; i++) {
                            int k = i+j*3;
                            final float v = mYOffset + values[i] * mScale[j];
                            paint.setColor(mColors[k]);
                            canvas.drawLine(mLastX, mLastValues[k], newX, v, paint);
                            mLastValues[k] = v;
                        }
                        if (sensor == SensorManager.SENSOR_MAGNETIC_FIELD)
                            mLastX += mSpeed;
                    }
                */
				
					///get ECG data here
					ECGdataArray = byteArrayFromJNI();
				
                    paint.setColor(0xA27F0000);
                    
                    for( int iLoop=32; iLoop>0; iLoop-- ) {
                
                    	NewX = mLastX + 1;
					/*	
                    	if( iLoop > 16)
                    		NewY = mLastY + 1;
                    	else
                    		NewY = mLastY - 1;
					*/
						NewY = ECGdataArray[iLoop];
						
                    	canvas.drawLine(mLastX, mLastY, NewX, NewY, paint);              	
                    	mLastX = NewX;
                    	mLastY = NewY;
                    }
                    invalidate();
                    ///postInvalidate();
                    Log.d("HRV","invalidate()");
                }
            }
        }


        public void onAccuracyChanged(int sensor, int accuracy) {
            // TODO Auto-generated method stub
            
        }
    }
    
    /**
     * Initialization of the Activity after it is first created.  Must at least
     * call {@link android.app.Activity#setContentView setContentView()} to
     * describe what is to be displayed in the screen.
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // Be sure to call the super class.
        super.onCreate(savedInstanceState);

        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        mGraphView = new GraphView(this);
        setContentView(mGraphView);
/*		
        // Start up the thread running the service.  Note that we create a
        // separate thread because the service normally runs in the process's
        // main thread, which we don't want to block.
        Thread tHALThread = new Thread(null, mTask, "Sensor");
        tHALThread.start();
*/
        mHandler = new Handler() {
			public void handleMessage(Message msg) 
			{
	        	float   mValue[] = new float[3];
	        	
	        	mValue[0] = 0 + 100/10;
	        	mValue[1] = 0 + 100/10;
	        	mValue[2] = 0 + 100/10;
				/* message handle */
				switch (msg.what)
				{ 
					case Sensor.GUINOTIFIER:
						Log.d("HRV", "GUINOTIFIER");
						///tsY=(int)(Math.random()*150+50);
						//Log.d("HRV", "tsY=" + tsY);
						mGraphView.doManualChanged(8, mValue);
						break; 
					case Sensor.STOP:
						Log.d("HRV", "STOP");
						break;
				}	 
				super.handleMessage(msg); 
			}
        };
        
        // Start up the thread running the service.  Note that we create a
        // separate thread because the service normally runs in the process's
        // main thread, which we don't want to block.
        Thread thr = new LooperThread();
        thr.start();
    }

	
 	class LooperThread extends Thread {
		public void run() {
			super.run();
			isStart = 1;
			try {
				///Log.d("HRV", "isStart == " + isStart);
				while (isStart == 1) {
					Thread.sleep(200); 
					Log.d("HRV", "isStart == 1");
					/* post message */
					Message m = new Message();
					m.what = Sensor.GUINOTIFIER;
					Sensor.this.mHandler.sendMessage(m);     
				}	
				Log.d("HRV", "isStart == 0");
        		Message m = new Message();
        		m.what = Sensor.STOP;
        		Sensor.this.mHandler.sendMessage(m);     
			}
			catch(Exception e)	{
				e.printStackTrace();
			}
		}      
    }


    /**
     * The function that runs in our worker thread
     */	
/*	
    Runnable mTask = new Runnable() {
    	private int   mCount = 0;
    	private float   mValue[] = new float[3];
        public void run() {
            // Normally we would do some work here...
            while ( mCount < 2 ) {
                if (true) {
                    try {
                    	mValue[0] = 0 + mCount;
                    	mValue[1] = 0 + mCount;
                    	mValue[2] = 0 + mCount;
                    	mGraphView.doManualChanged(1, mValue);

                    	mValue[0] = 0 + mCount/10;
                    	mValue[1] = 0 + mCount/10;
                    	mValue[2] = 0 + mCount/10;
                    	mGraphView.doManualChanged(8, mValue);
                    } catch (Exception e) {
                    }
                }
                mCount++;
            }        	
            // Done with our work...  stop the service!
            Sensors.this.stopSelf();
        }
    };
    
    public void stopSelf() {
    	
    }
*/    

    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager.registerListener(mGraphView, 
                SensorManager.SENSOR_ACCELEROMETER | 
                SensorManager.SENSOR_MAGNETIC_FIELD | 
                SensorManager.SENSOR_ORIENTATION,
                SensorManager.SENSOR_DELAY_FASTEST);
    }
    
    @Override
    protected void onStop() {
        mSensorManager.unregisterListener(mGraphView);
        super.onStop();
    }

    /* A native method that is implemented by the
     * 'hello-jni' native library, which is packaged
     * with this application.
     */
    public native String  stringFromJNI();
	public native Byte[]  byteArrayFromJNI();

    /* This is another native method declaration that is *not*
     * implemented by 'hello-jni'. This is simply to show that
     * you can declare as many native methods in your Java code
     * as you want, their implementation is searched in the
     * currently loaded native libraries only the first time
     * you call them.
     *
     * Trying to call this function will result in a
     * java.lang.UnsatisfiedLinkError exception !
     */
    public native String  unimplementedStringFromJNI();
	public native Byte[]  unimplementedByteArrayFromJNI();

    /* this is used to load the 'hello-jni' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.HelloJni/lib/libhello-jni.so at
     * installation time by the package manager.
     */
	 
    static {
        System.loadLibrary("hello-jni");
    }
		
	
}
