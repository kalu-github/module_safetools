package lib.kalu.encryption.cipher;

import androidx.annotation.Keep;

@Keep
public final class CipherTool {

    static {
        System.loadLibrary("kalu-encryption");
    }

    public static native String aesEncode(String str);

    public static native String aesDecode(String str);
}