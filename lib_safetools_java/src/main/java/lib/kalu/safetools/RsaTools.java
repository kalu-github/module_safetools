package lib.kalu.safetools;

import androidx.annotation.NonNull;
import androidx.annotation.RawRes;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.Map;

import javax.crypto.Cipher;

/**
 * desc
 */
public class RsaTools {

    private final static String KEYFACTORY_ALGORITHM = "RSA";
    private final static String SIGNATURE_ALGORITHM = "Sha256WithRSA";
    private final static String CIPHER_TRANSFORMATION = "RSA/ECB/PKCS1Padding";

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
            Signature signature = Signature.getInstance(SIGNATURE_ALGORITHM);
            signature.initVerify(publicKey);
            signature.update(data.getBytes());
            byte[] bytes = sign.getBytes();
            byte[] decodeBytes;
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                decodeBytes = java.util.Base64.getDecoder().decode(bytes);
//                LogUtil.e("testRRR", "jdk");
            } else {
//                LogUtil.e("testRRR", "android");
                decodeBytes = android.util.Base64.decode(bytes, android.util.Base64.DEFAULT);
            }
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
            Signature signature = Signature.getInstance(SIGNATURE_ALGORITHM);
            signature.initSign(privateKey);
            signature.update(data.getBytes());
            byte[] bytes = signature.sign();
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                return java.util.Base64.getEncoder().encodeToString(bytes);
            } else {
                return android.util.Base64.encodeToString(bytes, android.util.Base64.DEFAULT).replaceAll("\n", "");
            }
        } catch (Exception e) {
            SafeLogUtil.log(e.getMessage());
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
            Cipher cipher = Cipher.getInstance(CIPHER_TRANSFORMATION);
            cipher.init(Cipher.ENCRYPT_MODE, publicKey);
            byte[] bytes = cipher.doFinal(data.getBytes(StandardCharsets.UTF_8));
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                return java.util.Base64.getEncoder().encodeToString(bytes);
            } else {
                return android.util.Base64.encodeToString(bytes, android.util.Base64.DEFAULT).replaceAll("\n", "");
            }
        } catch (Exception e) {
            SafeLogUtil.log(e.getMessage());
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
            Cipher cipher = Cipher.getInstance(CIPHER_TRANSFORMATION);
            cipher.init(Cipher.DECRYPT_MODE, privateKey);
            byte[] decode;
            byte[] bytes = data.getBytes(StandardCharsets.UTF_8);
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                decode = java.util.Base64.getDecoder().decode(bytes);
            } else {
                decode = android.util.Base64.decode(bytes, android.util.Base64.DEFAULT);
            }
            byte[] doFinal = cipher.doFinal(decode);
            return new String(doFinal, StandardCharsets.UTF_8).replaceAll("\n", "");
        } catch (Exception e) {
            SafeLogUtil.log(e.getMessage());
            return null;
        }
    }

    /********************************/

    private static byte[] getBytes(@NonNull String key) {
        try {
            byte[] bytes = key.getBytes(StandardCharsets.UTF_8);
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                return java.util.Base64.getDecoder().decode(bytes);
            } else {
                return android.util.Base64.decode(bytes, android.util.Base64.DEFAULT);
            }
        } catch (Exception e) {
            SafeLogUtil.log(e.getMessage());
            return null;
        }
    }

    private static PublicKey generatePublic(@NonNull byte[] keyBytes) {
        try {
            X509EncodedKeySpec keySpec = new X509EncodedKeySpec(keyBytes);
            KeyFactory keyFactory = KeyFactory.getInstance(KEYFACTORY_ALGORITHM);
            PublicKey pubKey = keyFactory.generatePublic(keySpec);
            return pubKey;
        } catch (Exception e) {
            SafeLogUtil.log(e.getMessage());
            return null;
        }
    }

    private static PrivateKey generatePrivate(@NonNull byte[] keyBytes) {
        try {
            PKCS8EncodedKeySpec keySpec = new PKCS8EncodedKeySpec(keyBytes);
            KeyFactory keyFactory = KeyFactory.getInstance(KEYFACTORY_ALGORITHM);
            PrivateKey priKey = keyFactory.generatePrivate(keySpec);
            return priKey;
        } catch (Exception e) {
            SafeLogUtil.log(e.getMessage());
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
            KeyPairGenerator generator = KeyPairGenerator.getInstance(KEYFACTORY_ALGORITHM);
            generator.initialize(1024);
            KeyPair keyPair = generator.generateKeyPair();
            return keyPair;
        } catch (Exception e) {
            SafeLogUtil.log(e.getMessage());
            return null;
        }
    }

    public static String getKey(@RawRes int id) {
        try {
            InputStreamReader inputReader = new InputStreamReader(SafeContentProvider.mContext.getResources().openRawResource(id), StandardCharsets.UTF_8);
            BufferedReader bufReader = new BufferedReader(inputReader);
            String info;
            StringBuilder builder = new StringBuilder();
            while ((info = bufReader.readLine()) != null) {
                builder.append(info);
            }
            return builder.toString();
        } catch (Exception e) {
            SafeLogUtil.log(e.getMessage());
            return null;
        }
    }

    public static String createSign(@NonNull Map<CharSequence, Object> map) {
        return createSign(map, false);
    }

    public static String createSign(@NonNull Map<CharSequence, Object> map, boolean isFromH5) {

        // 验签
        ArrayList<String> list = new ArrayList<>();
        Iterator<Map.Entry<CharSequence, Object>> iterator = map.entrySet().iterator();
        while (iterator.hasNext()) {
            Map.Entry<CharSequence, Object> next = iterator.next();
            if (null != next.getKey() && next.getKey().length() > 0 && null != next.getValue() && next.getValue().toString().length() > 0) {
                list.add(next.getKey().toString());
            }
        }
        Collections.sort(list);

        StringBuilder builder = new StringBuilder();
        for (int i = 0; i < list.size(); i++) {
            CharSequence key = list.get(i);
            if (null == key || key.length() == 0)
                continue;
            if (!isFromH5 && "sessionId".equals(key))
                continue;
            if (!isFromH5 && "key".equals(key))
                continue;
            if ("text".equals(key))
                continue;
            Object value = map.get(key);
            if (null == value)
                continue;
            String valueOf = String.valueOf(value);
            if (valueOf.length() == 0)
                continue;
//            if (value instanceof JsonNull)
//                continue;
//            if (value instanceof JsonElement)
//                continue;
//            if (value instanceof JsonObject)
//                continue;
//            if (value instanceof JsonArray)
//                continue;
            if (valueOf.trim().startsWith("{") && valueOf.trim().endsWith("}"))
                continue;
            if (valueOf.trim().startsWith("[") && valueOf.trim().endsWith("]"))
                continue;

            if (builder.length() != 0) {
                builder.append("&");
            }
            if (value instanceof Boolean) {
                builder.append(key + (((Boolean) value) ? "=1" : "=0"));
            } else {
                builder.append(key + "=" + value);
            }
//            try {
//                String newString = new GsonBuilder().serializeNulls().disableHtmlEscaping().create().toJson(value);
//                StringBuilder stringBuilder = new StringBuilder();
//                for (int j = 0; j < newString.length(); j++) {
//                    char charAt = newString.charAt(j);
//                    if ('\"' == charAt)
//                        continue;
//                    if (':' == charAt) {
//                        stringBuilder.append("=");
//                    } else if (',' == charAt) {
//                        stringBuilder.append(", ");
//                    } else {
//                        stringBuilder.append(charAt);
//                    }
//                }
//                String toString = stringBuilder.toString();
//                builder.append(key + "=" + toString);
//            } catch (Exception e) {
//                builder.append(key + "=" + value);
//            }
        }

        //String toJson = new GsonBuilder().serializeNulls().disableHtmlEscaping().create().toJson(map);
        //SafeLogUtil.log("sign_map = " + toJson);
        String client_private_key = getKey(R.raw.client_private_key);
        SafeLogUtil.log("client_private_key = " + client_private_key);
        String sign_data = builder.toString();
        SafeLogUtil.log( "sign_data = " + sign_data);
        String sign = sign(sign_data, client_private_key);
//        LogUtil.e("testRRR", "sign = " + sign);
//        String client_public_key = getKey(R.raw.client_public_key);
//        LogUtil.e("testRRR", "client_public_key = " + client_public_key);
//        boolean sign_verify = RSAUtil.verify(sign_data, client_public_key, sign);
//        LogUtil.e("testRRR", "sign_verify = " + sign_verify);
        return sign;
    }
}
