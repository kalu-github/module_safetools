package lib.kalu.safetools;

import android.util.Log;

import androidx.annotation.NonNull;

public final class SafeLogUtil {

    private static final String TAG = "moudle_safetools";

    public static final void log(@NonNull String content) {

        if (!BuildConfig.DEBUG)
            return;

        if (null == content || content.length() == 0)
            return;

        Log.d(TAG, content);
    }
}
