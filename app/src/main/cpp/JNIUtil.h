//
// Created by wenhaoxia on 2017/7/21.
//

#ifndef LAMETEST_JNIUNTIL_H

#include <string>
#include <jni.h>
#include <stdlib.h>

using namespace std;

#define LAMETEST_JNIUNTIL_H



class JNIUntil {

public:
    static string getStringFromJstring(JNIEnv* ,jstring);
};


#endif //LAMETEST_JNIUNTIL_H
