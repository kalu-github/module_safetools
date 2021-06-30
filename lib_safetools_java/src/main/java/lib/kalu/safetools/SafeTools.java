package lib.kalu.safetools;

import android.content.Context;

import androidx.annotation.Keep;

@Keep
public final class SafeTools {

    public static final boolean checkSafe() {

        boolean checkSignature = checkSignature();
        if (!checkSignature)
            return false;

        boolean checkEmulator = checkEmulator();
        if (!checkEmulator)
            return false;

        boolean checkRoot = checkRoot();
        if (!checkRoot)
            return false;

        boolean checkXposed = checkXposed();
        if (!checkXposed)
            return false;

        return true;
    }

    /**
     * 检测签名信息
     */
    private static final boolean checkSignature() {
        return true;
    }

    /**
     * 检测模拟器
     */
    private static final boolean checkEmulator() {
        return true;
    }

    /**
     * 检测Root
     */
    private static final boolean checkRoot() {
        return true;
    }

    /**
     * 检测Xposed
     */
    private static final boolean checkXposed() {
        try {

            Context context = SafeToolsContextProvider.mContext;
            String filePath = context.getFilesDir().getAbsolutePath();
            String packageName = context.getPackageName();

            // 异常:单用户
            if (filePath.startsWith("/data/data") && !filePath.equals("/data/data/" + packageName + "/files")) {
                return false;
            }
            // 异常:多用户
            else if (filePath.startsWith("/data/user")) {

                int position = 0;
                for (int i = 0; i < 3; i++) {
                    position = filePath.indexOf("/", ++position);
                }
                String substring = filePath.substring(0, ++position);

                if (!filePath.equals(substring + packageName + "/files")) {
                    return false;
                } else {
                    return true;
                }
            }
            // 正常
            else {
                return true;
            }
        } catch (Exception e) {
            SafeLogUtil.log("checkXposed => " + e.getMessage());
            return true;
        }
    }
}
