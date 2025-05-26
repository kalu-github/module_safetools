package lib.kalu.tool;

import android.net.Uri;
import android.util.Base64;

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.*;

import javax.crypto.Cipher;
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;

public class Box {

    static {
        System.loadLibrary("tool");
    }

    private static native byte[] _getFlag1();

    private static native byte[] _getFlag2();

    private static native byte[] _aesEncrypt(byte[] data, byte[] key, boolean useBase64);

    private static native byte[] _aesDecrypt(byte[] data, byte[] key, boolean useBase64);

    private static native byte[] _base64Encrypt(byte[] data);

    private static native byte[] _base64Decrypt(byte[] data);

    private static native String _md5(byte[] data);

    private static native String _hamcSha265(byte[] data, byte[] key);

    public static byte[] aesEncrypt(byte[] data, boolean useJNI) {
        try {
            if (data == null) {
                throw new Exception("error: data null");
            }
            if (useJNI) {
                byte[] result = _aesEncrypt(data, _getFlag1(), true);
                if (result == null) {
                    throw new Exception("error: _aesDecrypt fail");
                }
                return result;
            } else {
                Box box = new Box();
                SecretKeySpec secretKey = new SecretKeySpec(box._getFlag1(), "AES");
                Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
                cipher.init(Cipher.ENCRYPT_MODE, secretKey);
                byte[] encryptedBytes = cipher.doFinal(data);
                byte[] base64Result = base64Encrypt(encryptedBytes, false);
                if (base64Result == null) {
                    throw new Exception("error: base64Encrypt fail");
                }
                return base64Result;
            }
        } catch (Exception e) {
            print("aesEncrypt -> Exception " + e.getMessage());
            return new byte[0];
        }
    }

    public static byte[] aesDecrypt(byte[] data, boolean useJNI) {
        try {
            if (data == null) {
                throw new Exception("error: data null");
            }
            if (useJNI) {
                byte[] result = _aesDecrypt(data, _getFlag1(), true);
                if (result == null) {
                    throw new Exception("error: _aesDecrypt fail");
                }
                return result;
            } else {
                byte[] result_base64 = base64Decrypt(data, false);
                if (result_base64 == null) {
                    throw new Exception("error: base64Decrypt fail");
                }
                Box box = new Box();
                SecretKeySpec secretKey = new SecretKeySpec(box._getFlag1(), "AES");
                Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
                cipher.init(Cipher.DECRYPT_MODE, secretKey);
                return cipher.doFinal(result_base64);
            }
        } catch (Exception e) {
            print("aesDecrypt -> Exception " + e.getMessage());
            return new byte[0];
        }
    }

    public static byte[] base64Encrypt(byte[] data, boolean useJNI) {
        try {
            if (data == null) {
                throw new Exception("error: data null");
            }
            if (useJNI) {
                Box box = new Box();
                byte[] result = box._base64Encrypt(data);
                if (result == null) {
                    throw new Exception("error: _base64Encrypt fail");
                }
                return result;
            } else {
                return Base64.encode(data, Base64.DEFAULT);
            }
        } catch (Exception e) {
            print("base64Encrypt -> Exception " + e.getMessage());
            return null;
        }
    }

    public static byte[] base64Decrypt(byte[] data, boolean useJNI) {
        try {
            if (data == null) {
                throw new Exception("error: data null");
            }
            if (useJNI) {
                Box box = new Box();
                byte[] result = box._base64Decrypt(data);
                if (result == null) {
                    throw new Exception("error: _base64Decrypt fail");
                }
                return result;
            } else {
                return Base64.decode(data, Base64.DEFAULT);
            }
        } catch (Exception e) {
            print("base64Decrypt -> Exception " + e.getMessage());
            return null;
        }
    }

    public static String md5(byte[] data, boolean useJNI) {
        try {
            if (data == null) {
                throw new Exception("error: data null");
            }
            if (useJNI) {
                Box box = new Box();
                String result = box._md5(data);
                if (result == null) {
                    throw new Exception("error: _md5Encrypt fail");
                }
                return result;
            } else {
                MessageDigest md = MessageDigest.getInstance("MD5");
                byte[] bytes = md.digest(data);
                if (bytes.length == 0) {
                    throw new Exception("error: bytes isEmpty");
                }
                StringBuilder result = new StringBuilder();
                for (byte b : bytes) {
                    result.append(String.format("%02x", b));
                }
                return result.toString();
            }
        } catch (Exception e) {
            print("md5 -> Exception " + e.getMessage());
            return null;
        }
    }

    public static String hmacSha265(byte[] key, byte[] data, boolean useJNI) {
        try {
            if (key == null) {
                throw new Exception("error: key null");
            }
            if (data == null) {
                throw new Exception("error: data null");
            }
            if (useJNI) {
                Box box = new Box();
                String result = box._hamcSha265(data, key);
                if (result == null) {
                    throw new Exception("error: _hamcSha265 fail");
                }
                return result;
            } else {
                String algorithm = "HmacSHA256";
                Mac mac = Mac.getInstance(algorithm);
                SecretKeySpec secretKeySpec = new SecretKeySpec(key, algorithm);
                mac.init(secretKeySpec);
                byte[] hmacBytes = mac.doFinal(data);
                StringBuilder result = new StringBuilder();
                for (byte b : hmacBytes) {
                    result.append(String.format("%02x", b));
                }
                return result.toString();
            }
        } catch (Exception e) {
            print("hmacSha265 -> Exception " + e.getMessage());
            return null;
        }
    }

    private static void print(String message) {

    }
}