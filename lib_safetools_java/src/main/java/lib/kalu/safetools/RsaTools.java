package lib.kalu.safetools;

import android.util.Base64;

import androidx.annotation.NonNull;

import java.io.ByteArrayOutputStream;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;

import javax.crypto.Cipher;

/**
 * desc
 */
public class RsaTools {

    /**
     * RSA最大加密明文大小
     */
    private static final int MAX_ENCRYPT_BLOCK = 117;

    /**
     * RSA最大解密密文大小
     */
    private static final int MAX_DECRYPT_BLOCK = 128;

    /**
     * Base64-Flag
     */
    private static final int BASE64_FLAG = Base64.NO_WRAP;

    /**
     * UTF-8
     */
    private static final Charset UTF_8 =  StandardCharsets.UTF_8;

    private static final String RSA = "RSA";
    private static final String KEY_VERIFY = "MD5withRSA";

    public static boolean verify(@NonNull String data, @NonNull String publicKey, @NonNull String sign) {
        byte[] keyBytes = getBytes(publicKey);
        return verify(data, keyBytes, sign);
    }

    public static boolean verify(@NonNull String data, @NonNull byte[] publicKey, @NonNull String sign) {
        PublicKey pubKey = generatePublic(publicKey);
        return verify(data, pubKey, sign);
    }

    /**
     * 验签
     *
     * @param data      原始字符串
     * @param publicKey 公钥
     * @param sign      签名
     * @return
     */
    public static boolean verify(@NonNull String data, @NonNull PublicKey publicKey, @NonNull String sign) {
        try {
            Signature signature = Signature.getInstance(KEY_VERIFY);
            signature.initVerify(publicKey);
            signature.update(data.getBytes());
            byte[] bytes = sign.getBytes();
            byte[] decodeBytes = Base64.decode(bytes, BASE64_FLAG);
            return signature.verify(decodeBytes);
        } catch (Exception e) {
            return false;
        }
    }

    /********************************/

    public static String sign(@NonNull String data, @NonNull String privateKey) {
        byte[] keyBytes = getBytes(privateKey);
        return sign(data, keyBytes);
    }

    public static String sign(@NonNull String data, @NonNull byte[] privateKey) {
        PrivateKey priKey = generatePrivate(privateKey);
        return sign(data, priKey);
    }

    /**
     * 签名
     *
     * @return
     */
    public static String sign(@NonNull String data, @NonNull PrivateKey privateKey) {
        try {
            Signature signature = Signature.getInstance(KEY_VERIFY);
            signature.initSign(privateKey);
            signature.update(data.getBytes());
            byte[] bytes = signature.sign();
            byte[] decodeBytes = Base64.decode(bytes, BASE64_FLAG);
            return new String(decodeBytes, UTF_8);
        } catch (Exception e) {
            return null;
        }
    }

    /********************************/

    public static String encrypt(@NonNull String data, @NonNull String publicKey) {
        byte[] bytes = getBytes(publicKey);
        return encrypt(data, bytes);
    }

    public static String encrypt(@NonNull String data, @NonNull byte[] publicKey) {
        PublicKey generatePublic = generatePublic(publicKey);
        return encrypt(data, generatePublic);
    }

    /**
     * 加密
     *
     * @param data      原始字符串
     * @param publicKey 公钥
     * @return
     */
    public static String encrypt(@NonNull String data, @NonNull PublicKey publicKey) {
        try {
            Cipher cipher = Cipher.getInstance(RSA);
            cipher.init(Cipher.ENCRYPT_MODE, publicKey);
            int inputLen = data.getBytes().length;
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            int offset = 0;
            byte[] cache;
            int i = 0;
            // 对数据分段加密
            while (inputLen - offset > 0) {
                if (inputLen - offset > MAX_ENCRYPT_BLOCK) {
                    cache = cipher.doFinal(data.getBytes(), offset, MAX_ENCRYPT_BLOCK);
                } else {
                    cache = cipher.doFinal(data.getBytes(), offset, inputLen - offset);
                }
                out.write(cache, 0, cache.length);
                i++;
                offset = i * MAX_ENCRYPT_BLOCK;
            }
            byte[] encryptedData = out.toByteArray();
            out.close();
            // 获取加密内容使用base64进行编码,并以UTF-8为标准转化成字符串
            // 加密后的字符串
            byte[] encodeBytes = Base64.encode(encryptedData, BASE64_FLAG);
            String s = new String(encodeBytes, UTF_8);
            return s;
        } catch (Exception e) {
            return null;
        }
    }

    /********************************/

    public static String decrypt(@NonNull String data, @NonNull String privateKey) {
        byte[] bytes = getBytes(privateKey);
        return decrypt(data, bytes);
    }

    public static String decrypt(@NonNull String data, @NonNull byte[] privateKey) {
        PrivateKey priKey = generatePrivate(privateKey);
        return decrypt(data, priKey);
    }

    /**
     * 解密
     *
     * @return
     */
    public static String decrypt(@NonNull String data, @NonNull PrivateKey privateKey) {
        try {
            Cipher cipher = Cipher.getInstance(RSA);
            cipher.init(Cipher.DECRYPT_MODE, privateKey);
            byte[] dataBytes = Base64.decode(data, BASE64_FLAG);
            int inputLen = dataBytes.length;
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            int offset = 0;
            byte[] cache;
            int i = 0;
            // 对数据分段解密
            while (inputLen - offset > 0) {
                if (inputLen - offset > MAX_DECRYPT_BLOCK) {
                    cache = cipher.doFinal(dataBytes, offset, MAX_DECRYPT_BLOCK);
                } else {
                    cache = cipher.doFinal(dataBytes, offset, inputLen - offset);
                }
                out.write(cache, 0, cache.length);
                i++;
                offset = i * MAX_DECRYPT_BLOCK;
            }
            byte[] decryptedData = out.toByteArray();
            out.close();
            // 解密后的内容
            return new String(decryptedData, UTF_8);
        } catch (Exception e) {
            return null;
        }
    }

    /********************************/

    private static byte[] getBytes(@NonNull String key) {
        try {
            byte[] bytes = key.getBytes(UTF_8);
            byte[] keyBytes = Base64.decode(bytes, BASE64_FLAG);
            return keyBytes;
        } catch (Exception e) {
            return null;
        }
    }

    private static PublicKey generatePublic(@NonNull byte[] keyBytes) {
        try {
            X509EncodedKeySpec keySpec = new X509EncodedKeySpec(keyBytes);
            KeyFactory keyFactory = KeyFactory.getInstance(RSA);
            PublicKey pubKey = keyFactory.generatePublic(keySpec);
            return pubKey;
        } catch (Exception e) {
            return null;
        }
    }

    private static PrivateKey generatePrivate(@NonNull byte[] keyBytes) {
        try {
            PKCS8EncodedKeySpec keySpec = new PKCS8EncodedKeySpec(keyBytes);
            KeyFactory keyFactory = KeyFactory.getInstance(RSA);
            PrivateKey priKey = keyFactory.generatePrivate(keySpec);
            return priKey;
        } catch (Exception e) {
            return null;
        }
    }

    /**
     * 获取密钥对
     *
     * @return 密钥对
     */
    public static KeyPair getKeyPair() {
        try {
            KeyPairGenerator generator = KeyPairGenerator.getInstance(RSA);
            generator.initialize(1024);
            KeyPair keyPair = generator.generateKeyPair();
            return keyPair;
        } catch (Exception e) {
            return null;
        }
    }
}
