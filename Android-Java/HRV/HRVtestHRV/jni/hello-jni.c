/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
///#include <ArrayHandler.h>


#ifdef __cplusplus
extern "C" {
#endif


/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/HelloJni/HelloJni.java
 *   HRVtestHRV\src\com\Sensor\Sensor.java
 */
///JNIEXPORT jstring Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env, jobject thiz )
JNIEXPORT jstring Java_com_Sensor_Sensor_stringFromJNI( JNIEnv* env, jobject thiz )
{

	///main123();	
    return (*env)->NewStringUTF(env, "Hello from JNI !");
	///return (*env).NewStringUTF("Hello from JNI !");
}


JNIEXPORT jobjectArray Java_com_Sensor_Sensor_byteArrayFromJNI(JNIEnv *env, jobject jobj)
{

    jobjectArray ret;
    int i;

    char message[32];

    for( i = 0; i < 32; i++ ) {
		message[i] = (char)(200+i);
    }

    ret = (jobjectArray)(*env)->NewObjectArray( env, 32, (*env)->FindClass( env, "java/lang/Byte"), 0 );

    for( i = 0; i < 32; i++ )
	{
        (*env)->SetObjectArrayElement( env, ret, i, (jobject)&message[i] );
    }
    return(ret);
}




#ifdef __cplusplus
}
#endif
