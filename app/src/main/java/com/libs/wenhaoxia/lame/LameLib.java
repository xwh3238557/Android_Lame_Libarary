package com.libs.wenhaoxia.lame;

import android.support.annotation.Nullable;

/**
 * Created by wenhaoxia on 2017/7/20.
 */

public class LameLib {

    static {
        System.loadLibrary("lame-lib");
    }

    public interface Callback {
        void onStart();

        void onProgressChange(long progress, long total);

        void onError(int errorCode, String errorMessage);

        void onFinished();
    }

    @Nullable
    private final Callback callback;

    public LameLib(@Nullable Callback callback) {
        this.callback = callback;
    }

    public static final int ERROR_CODE_FILE_NOT_FOUND = 1;

    public native void convertWav2Mp3(String wavFilePath, String mp3FilePath);

    public native String version();

    protected final void onProgressChange(long progress, long total) {
        if (callback != null)
            callback.onProgressChange(progress, total);
    }

    protected final void onError(int errorCode, String errorMessage) {
        if (callback != null)
            callback.onError(errorCode, errorMessage);
    }

    protected final void onFinished() {
        if (callback != null)
            callback.onFinished();
    }

    protected final void onStart() {
        if(callback != null)
            callback.onStart();
    }
}
