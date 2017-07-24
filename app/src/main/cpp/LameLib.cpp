#include "LameLib.h"

extern "C" {

/**
 * the flag to set weather the operation is in process
 */
bool parseFlag = false;

/**
 * check wheather the parsing is in process
 * @return true is in process
 */
bool isParsing() {
    return parseFlag;
}

/**
 * start the process
 */
void startParse() {
    parseFlag = true;
}

/**
 * invoke when the operation end
 */
void endParse() {
    parseFlag = false;
}


void onStart(JNIEnv * env, jobject jobj) {
    jmethodID onStartMethodId = env->GetMethodID(env->GetObjectClass(jobj), "onStart", "()V");
    env->CallVoidMethod(jobj, onStartMethodId);
}

void onProgressChange(JNIEnv *env, jobject jobj, long progress, long total) {
    jclass clazz = env->GetObjectClass(jobj);
    jmethodID setProgressMethodId = env->GetMethodID(clazz, "onProgressChange", "(JJ)V");
    env->CallVoidMethod(jobj, setProgressMethodId, progress, total);
}


void onError(JNIEnv *env, jobject jobj, int errorCode, string errorMessage) {
    jclass clazz = env->GetObjectClass(jobj);
    jstring jErrorMessage = env->NewStringUTF(errorMessage.c_str());
    jmethodID onErrorMethodId = env->GetMethodID(clazz, "onError", "(ILjava/lang/String;)V");
    env->CallVoidMethod(jobj, onErrorMethodId, errorCode, jErrorMessage);
}


JNIEXPORT void JNICALL Java_com_libs_wenhaoxia_lametest_LameLib_convertWav2Mp3
        (JNIEnv *env, jobject jobj, jstring jstrWavFilePath, jstring jstrMp3FilePath) {

    if (isParsing()) {
        onError(env, jobj, ERROR_CODE_PARSING_IS_STARTED_ALREADY,
                "Can not start a parsing operation when it is already in process.");
        return;
    } else {
        startParse();
    }
    const string wavFilePath = JNIUntil::getStringFromJstring(env, jstrWavFilePath);
    const string mp3FilePath = JNIUntil::getStringFromJstring(env, jstrMp3FilePath);

    ifstream wavFStream(wavFilePath.c_str(), ios::binary);
    if (!wavFStream.is_open()) {
        onError(env, jobj, ERROR_CODE_FILE_NOT_FOUND, "File not found");
        endParse();
        return;
    }

    ofstream mp3FStream(mp3FilePath.c_str(), ios::trunc | ios::binary);

    int channels = 2;
    short wavBuffer[8192];
    unsigned char mp3Buffer[8192];

    lame_t lame = lame_init();

    lame_set_in_samplerate(lame, 44100);
    lame_set_out_samplerate(lame, 44100);
    lame_set_num_channels(lame, channels);
    lame_set_VBR(lame, vbr_default);
    lame_init_params(lame);

    int readLength = 0;
    streamsize writeLength = 0;
    wavFStream.seekg(0, ios::end);
    int total = (int) wavFStream.tellg();
    //skip wav header
    wavFStream.seekg(4 * 1024, ios::beg);

    int wavBufferSize = 8192 * sizeof(char) / sizeof(short);

    bool  isFirst = true;
    do {
        if (isFirst) {
            isFirst = false;
            onStart(env, jobj);
        }

        wavFStream.read((char *) wavBuffer, wavBufferSize);
        readLength = (int) wavFStream.gcount();
        writeLength = lame_encode_buffer_interleaved(lame, (short int *) wavBuffer,
                                                     readLength * sizeof(char) / sizeof(short) /
                                                     channels,
                                                     (unsigned char *) mp3Buffer, 8192);
        mp3FStream.write((const char *) mp3Buffer, writeLength);
        streamoff currentProgress = wavFStream.tellg();
        if (currentProgress == -1) {
            //this means the file reached the end
            currentProgress = total;
        }
        onProgressChange(env, jobj, currentProgress, total);
    } while (!wavFStream.eof());

    lame_encode_flush(lame, (unsigned char *) mp3Buffer, 1024);

    onFinished(env, jobj);

    lame_close(lame);
    wavFStream.close();
    mp3FStream.close();

    endParse();
}

void onFinished(JNIEnv *env, jobject jobj) {
    jmethodID  onFinishedMethodID = env->GetMethodID(env->GetObjectClass(jobj), "onFinished", "()V");
    env->CallVoidMethod(jobj, onFinishedMethodID);
}

JNIEXPORT jstring JNICALL Java_com_libs_wenhaoxia_lametest_LameLib_version
        (JNIEnv *env, jobject jobj) {
    return env->NewStringUTF(get_lame_version());
}

}