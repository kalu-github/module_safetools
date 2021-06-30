package lib.kalu.safetools;

import android.util.Log;

import androidx.annotation.NonNull;

final class SafeLogUtil {

    private static final String TAG = "moudle_safetools";

    static final void log(@NonNull String content) {

        if (!BuildConfig.DEBUG)
            return;

        if (null == content || content.length() == 0)
            return;

        Log.d(TAG, content);
    }
}
