package lib.kalu.encryption;

import android.content.Context;
import android.widget.Toast;

import androidx.annotation.Keep;

@Keep
public final class ToolUtil {

    static {
        System.loadLibrary("tools");
    }

    /**
     * 加密字符
     *
     * @param str 待加密信息
     * @return 加密后信息
     */
    @Keep
    public static native String aesEncode(String str);

    /**
     * 加密字符
     *
     * @param str            待加密信息
     * @param checkRoot      是否检测机器是否root
     * @param checkEmulator  是否检测模拟器
     * @param checkXposed    是否检测Xposed
     * @param checkSignature 是否检测app签名信息
     * @return 加密后信息
     */
    @Keep
    public static native String aesEncodeMult(String str, boolean checkRoot, boolean checkEmulator, boolean checkXposed, boolean checkSignature);

    /**
     * 解密字符
     *
     * @param str 待解密信息
     * @return 解密信息
     */
    @Keep
    public static native String aesDecode(String str);

    /**
     * 解密字符
     *
     * @param str
     * @param str            待解密信息
     * @param checkRoot      是否检测机器是否root
     * @param checkEmulator  是否检测模拟器
     * @param checkXposed    是否检测Xposed
     * @param checkSignature 是否检测app签名信息
     * @return 解密信息
     */
    @Keep
    public static native String aesDecodeMult(String str, boolean checkRoot, boolean checkEmulator, boolean checkXposed, boolean checkSignature);

    /**
     * 检测签名信息师傅被纂改
     *
     * @return true: 通过, fasle: 未通过
     */
    @Keep
    public static native boolean checkSignature();

    /**
     * 是否存在Root标记
     *
     * @return true: 通过, fasle: 未通过
     */
    @Keep
    public static native boolean checkRoot();


    /**
     * 是否存在模拟器
     *
     * @return true: 通过, fasle: 未通过
     */
    @Keep
    public static native boolean checkEmulator();

    /**
     * 是否存在 xposed
     *
     * @return true: 通过, fasle: 未通过
     */
    @Keep
    public static native boolean checkXposed();
//
//    @Keep
//    static void makeText(Context context, String msg) {
//
//        try {
//            Toast.makeText(context, msg, Toast.LENGTH_SHORT).show();
//        } catch (Exception e) {
//        }
//    }
}