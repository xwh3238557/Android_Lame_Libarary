//
// Created by wenhaoxia on 2017/7/21.
//

#ifndef LAMETEST_LAMELIB_H

#include <string>
#include <stdlib.h>
#include <jni.h>
#include "lame.h"
#include "fstream"

#include "JNIUtil.h"

using namespace std;

#define ERROR_CODE_FILE_NOT_FOUND 1
#define ERROR_CODE_PARSING_IS_STARTED_ALREADY 2


extern "C" {
bool isParsing();

void startParse();

void endParse();

/**
 * invoke when have some error
 * @param env java env object
 * @param object instance object
 * @param errorCode errorcode of the error
 * @param errorMessage detail message of the error
 */
void onError(JNIEnv *, jobject, int, string);

/**
 * invoke when encoding finished
 * @param env
 * @param jobj
 */
void onFinished(JNIEnv *env, jobject jobj);


/**
 * invoke when we need to tell the java side the parse process
 * @param env java enviroment
 * @param jobj instance object
 * @param progress current dealed size of the file
 * @param total total size of the file
 */
void onProgressChange(JNIEnv *, jobject, long, long);

/**
 * call when it start to encoding
 */
void onStart(JNIEnv *, jobject);

/*
 * Class:     com_libs_wenhaoxia_lametest_LameLib
 * Method:    convertWav2Mp3
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_libs_wenhaoxia_lametest_LameLib_convertWav2Mp3
        (JNIEnv *, jobject, jstring, jstring);

/*
   * Class:     com_libs_wenhaoxia_lametest_LameLib
   * Method:    version
   * Signature: ()Ljava/lang/String;
   */
JNIEXPORT jstring JNICALL Java_com_libs_wenhaoxia_lametest_LameLib_version(JNIEnv *, jobject);

};
#define LAMETEST_LAMELIB_H

#endif //LAMETEST_LAMELIB_H
