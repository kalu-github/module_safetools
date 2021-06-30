package lib.kalu.safetools;

import android.content.Context;
import android.os.Build;

import androidx.annotation.Keep;

import java.io.File;

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

        String fingerprint = Build.FINGERPRINT.toLowerCase();
        SafeLogUtil.log("checkEmulator => fingerprint = " + fingerprint);
        if (fingerprint.contains("android") || fingerprint.contains("unknown"))
            return false;

        String model = Build.MODEL.toLowerCase();
        SafeLogUtil.log("checkEmulator => model = " + model);
        if (model.contains("google_sdk") || model.contains("emulator") || model.contains("android sdk built for x86"))
            return false;

        String manufacturer = Build.MANUFACTURER.toLowerCase();
        SafeLogUtil.log("checkEmulator => manufacturer = " + manufacturer);
        if (manufacturer.contains("genymotion"))
            return false;

        String brand = Build.BRAND.toLowerCase();
        String device = Build.DEVICE.toLowerCase();
        SafeLogUtil.log("checkEmulator => brand = " + brand + ", device = " + device);
        if (brand.contains("generic") && device.startsWith("device"))
            return false;

        String product = Build.PRODUCT.toLowerCase();
        SafeLogUtil.log("checkEmulator => product = " + product);
        if (product.contains("google_sdk"))
            return false;

        return true;
    }

    /**
     * 检测Root
     */
    private static final boolean checkRoot() {

        for (int i = 0; i < SafeConstant.ROOT_ARRAY.length; i++) {
            String s = SafeConstant.ROOT_ARRAY[i];
            File file = new File(s);
            if (null != file && file.exists())
                return false;
        }

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
            if (filePath.startsWith(SafeConstant.XPOSED_DATA_DATA) && !filePath.equals(SafeConstant.XPOSED_DATA_DATA + File.separator + packageName + SafeConstant.XPOSED_FILES)) {
                return false;
            }
            // 异常:多用户
            else if (filePath.startsWith(SafeConstant.XPOSED_DATA_USER)) {

                int position = 0;
                for (int i = 0; i < 3; i++) {
                    position = filePath.indexOf(File.separator, ++position);
                }
                String substring = filePath.substring(0, ++position);

                if (!filePath.equals(substring + packageName + SafeConstant.XPOSED_FILES)) {
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
