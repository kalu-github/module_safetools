package lib.kalu.safetools;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Build;

import androidx.annotation.Keep;

import java.io.File;
import java.security.MessageDigest;

@Keep
public final class SafeTools {

    @Keep
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

        try {
            PackageManager packageManager = SafeContentProvider.mContext.getPackageManager();
            String packageName = SafeContentProvider.mContext.getPackageName();
            PackageInfo packageInfo = packageManager.getPackageInfo(packageName, PackageManager.GET_SIGNATURES);
            Signature[] signs = packageInfo.signatures;
            Signature sign = signs[0];

            SafeLogUtil.log("checkSignature => HASH = " + SafeConstant.SIGNATURE_HASH);
            SafeLogUtil.log("checkSignature => hash = " + sign.hashCode());
            if (SafeConstant.SIGNATURE_HASH != sign.hashCode())
                return false;

            MessageDigest messageDigest;
            StringBuffer md5StrBuff = new StringBuffer();
            messageDigest = MessageDigest.getInstance("MD5");
            messageDigest.reset();
            messageDigest.update(sign.toByteArray());
            byte[] byteArray = messageDigest.digest();
            for (int i = 0; i < byteArray.length; i++) {
                if (Integer.toHexString(0xFF & byteArray[i]).length() == 1) {
                    md5StrBuff.append("0").append(Integer.toHexString(0xFF & byteArray[i]));
                } else {
                    md5StrBuff.append(Integer.toHexString(0xFF & byteArray[i]));
                }
            }

            SafeLogUtil.log("checkSignature => MD5 = " + SafeConstant.SIGNATURE_MD5);
            SafeLogUtil.log("checkSignature => md5 = " + md5StrBuff.toString());
            if (!SafeConstant.SIGNATURE_MD5.equals(md5StrBuff.toString()))
                return false;

            SafeLogUtil.log("checkSignature => auth pass");
            return true;
        } catch (Exception e) {
            SafeLogUtil.log("checkSignature => " + e.getMessage());
            return true;
        }
    }

    /**
     * 检测模拟器
     */
    private static final boolean checkEmulator() {

        // 检测系统是否支持Tel
        boolean isSupportTel = SafeEmulatorUtil.checkEmulatorTel();
        SafeLogUtil.log("checkEmulator => isSupportTel = " + isSupportTel);
        if (!isSupportTel)
            return false;

        // 检测光传感器: 由于光传感器模拟器不容易伪造 在这里判断设备是否存在光传感器来判断是否为模拟器
        boolean isSupportSensor = SafeEmulatorUtil.checkEmulatorSensor();
        SafeLogUtil.log("checkEmulator => isSupportSensor = " + isSupportSensor);
        if (!isSupportSensor)
            return false;

        // 检测虚拟机驱动: 读取驱动文件, 检查是否包含已知的qemu驱动
        boolean isSupportDrivers = SafeEmulatorUtil.checkEmulatorDrivers();
        SafeLogUtil.log("checkEmulator => isSupportDrivers = " + isSupportDrivers);
        if (!isSupportDrivers)
            return false;

        boolean isSupportCpu = SafeEmulatorUtil.checkEmulatorCpu();
        SafeLogUtil.log("checkEmulator => isSupportCpu = " + isSupportCpu);
        if (!isSupportCpu)
            return false;

        String fingerprint = Build.FINGERPRINT.toLowerCase();
        SafeLogUtil.log("checkEmulator => fingerprint = " + fingerprint);

        String model = Build.MODEL.toLowerCase();
        SafeLogUtil.log("checkEmulator => model = " + model);

        String manufacturer = Build.MANUFACTURER.toLowerCase();
        SafeLogUtil.log("checkEmulator => manufacturer = " + manufacturer);

        String product = Build.PRODUCT.toLowerCase();
        SafeLogUtil.log("checkEmulator => product = " + product);

        for (int i = 0; i < SafeConstant.EMULATOR_ARGS_SDK.length; i++) {
            if (fingerprint.contains(SafeConstant.EMULATOR_ARGS_SDK[i]))
                return false;
            if (model.contains(SafeConstant.EMULATOR_ARGS_SDK[i]))
                return false;
            if (manufacturer.contains(SafeConstant.EMULATOR_ARGS_SDK[i]))
                return false;
            if (product.contains(SafeConstant.EMULATOR_ARGS_SDK[i]))
                return false;
        }

        String brand = Build.BRAND.toLowerCase();
        String device = Build.DEVICE.toLowerCase();
        SafeLogUtil.log("checkEmulator => brand = " + brand + ", device = " + device);
        if (brand.contains(SafeConstant.EMULATOR_GENERIC) && device.startsWith(SafeConstant.EMULATOR_DEVICE))
            return false;

        SafeLogUtil.log("checkEmulator => auth pass");
        return true;
    }

    /**
     * 检测Root
     */
    private static final boolean checkRoot() {

        for (int i = 0; i < SafeConstant.ROOT_ARRAY.length; i++) {
            String s = SafeConstant.ROOT_ARRAY[i];
            File file = new File(s);
            if (null != file && file.exists()) {
                SafeLogUtil.log("checkRoot => auth fail");
                return false;
            }
        }

        SafeLogUtil.log("checkRoot => auth pass");
        return true;
    }

    /**
     * 检测Xposed
     */
    private static final boolean checkXposed() {
        try {

            Context context = SafeContentProvider.mContext;
            String filePath = context.getFilesDir().getAbsolutePath();
            String packageName = context.getPackageName();

            // 异常:单用户
            if (filePath.startsWith(SafeConstant.XPOSED_DATA_DATA) && !filePath.equals(SafeConstant.XPOSED_DATA_DATA + File.separator + packageName + SafeConstant.XPOSED_FILES)) {
                SafeLogUtil.log("checkXposed => auth fail");
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
                    SafeLogUtil.log("checkXposed => auth fail");
                    return false;
                } else {
                    SafeLogUtil.log("checkXposed => auth pass");
                    return true;
                }
            }
            // 正常
            else {
                SafeLogUtil.log("checkXposed => auth pass");
                return true;
            }
        } catch (Exception e) {
            SafeLogUtil.log("checkXposed => " + e.getMessage());
            return true;
        }
    }
}
