package com.libs.wenhaoxia.lametest;

import android.util.Log;

/**
 * Created by wenhaoxia on 2017/7/20.
 */

public class LameLib {

    static {
        System.loadLibrary("lame-lib");
    }

    public static final int ERROR_CODE_FILE_NOT_FOUND = 1;

    public native void convertWav2Mp3(String wavFilePath, String mp3FilePath);

    public native String version();

    protected final void setProgress(int progress, int total) {
        Log.d("Lame", "t:"+total+" p:"+progress);
    }

    protected final void onError(int errorCode, String errorMessage) {
        Log.e("Lame", "code:"+ errorCode + "  message:" +errorMessage);
    }
}
