package lib.kalu.tool

import android.net.Uri
import android.util.Base64
import java.nio.charset.StandardCharsets
import java.security.MessageDigest
import java.util.UUID
import javax.crypto.Cipher
import javax.crypto.Mac
import javax.crypto.spec.SecretKeySpec


object Box {

    init {
        System.loadLibrary("tool")
    }

    private external fun _getFlag1(): ByteArray?

    private external fun _getFlag2(): ByteArray?

    private external fun _aesEncrypt(
        data: ByteArray?,
        key: ByteArray?,
        useBase64: Boolean = true
    ): ByteArray?

    private external fun _aesDecrypt(
        data: ByteArray?,
        key: ByteArray?,
        useBase64: Boolean = true
    ): ByteArray?

    private external fun _base64Encrypt(data: ByteArray?): ByteArray?

    private external fun _base64Decrypt(data: ByteArray?): ByteArray?

    private external fun _md5(data: ByteArray?): String?

    private external fun _hamcSha265(data: ByteArray?, key: ByteArray): String?

    fun aesEncrypt(data: ByteArray?, useJNI: Boolean = true): ByteArray {
        try {
            if (null == data)
                throw Exception("error: data null")
            if (useJNI) {
                return _aesEncrypt(data, _getFlag1()) ?: throw Exception("error: _aesDecrypt fail")
            } else {
                // AES/ECB/PKCS5Padding加密
                val secretKey = SecretKeySpec(_getFlag1(), "AES")
                val cipher = Cipher.getInstance("AES/ECB/PKCS5Padding")
                cipher.init(Cipher.ENCRYPT_MODE, secretKey)
                val encryptedBytes = cipher.doFinal(data)
                // 转换为Base64编码
                return base64Encrypt(encryptedBytes, false)
                    ?: throw Exception("error: base64Encrypt fail")
            }
        } catch (e: Exception) {
            LogUtil.print("Box", "aesEncrypt -> Exception ${e.message}")
            return byteArrayOf()
        }
    }

    fun aesDecrypt(data: ByteArray?, useJNI: Boolean = true): ByteArray {
        try {
            if (null == data)
                throw Exception("error: data null")
            if (useJNI) {
                return _aesDecrypt(data, _getFlag1()) ?: throw Exception("error: _aesDecrypt fail")
            } else {
                // base64解密
                val result_base64 =
                    base64Decrypt(data, false) ?: throw Exception("error: base64Decrypt fail")
                // AES/ECB/PKCS5Padding解密
                val secretKey = SecretKeySpec(_getFlag1(), "AES")
                val cipher = Cipher.getInstance("AES/ECB/PKCS5Padding")
                cipher.init(Cipher.DECRYPT_MODE, secretKey)
                return cipher.doFinal(result_base64)
            }
        } catch (e: Exception) {
            LogUtil.print("Box", "aesDecrypt -> Exception ${e.message}")
            return byteArrayOf()
        }
    }

    fun base64Encrypt(data: ByteArray?, useJNI: Boolean = true): ByteArray? {
        try {
            if (null == data)
                throw Exception("error: data null")
            if (useJNI) {
                return _base64Encrypt(data) ?: throw Exception("error: _base64Encrypt fail")
            } else {
                return Base64.encode(data, Base64.DEFAULT)
            }
        } catch (e: Exception) {
            LogUtil.print("Box", "base64Encrypt -> Exception ${e.message}")
            return null
        }
    }

    fun base64Decrypt(data: ByteArray?, useJNI: Boolean = true): ByteArray? {
        try {
            if (null == data)
                throw Exception("error: data null")
            if (useJNI) {
                return _base64Decrypt(data) ?: throw Exception("error: _base64Decrypt fail")
            } else {
                return Base64.decode(data, Base64.DEFAULT)
            }
        } catch (e: Exception) {
            LogUtil.print("Box", "base64Decrypt -> Exception ${e.message}")
            return null
        }
    }

    fun md5(data: ByteArray?, useJNI: Boolean = true): String? {

        try {
            if (null == data)
                throw Exception("error: data null")
            if (useJNI) {
                return _md5(data) ?: throw Exception("error: _md5Encrypt fail")
            } else {
                val md = MessageDigest.getInstance("MD5")
                val bytes = md.digest(data)
                if (bytes.isEmpty())
                    throw Exception("error: bytes isEmpty")
                val result = StringBuilder()
                for (byte in bytes) {
                    result.append(String.format("%02x", byte))
                }
                return result.toString()
            }
        } catch (e: Exception) {
            LogUtil.print("Box", "md5 -> Exception ${e.message}")
            return null
        }
    }

    fun hmacSha265(key: ByteArray?, data: ByteArray?, useJNI: Boolean = true): String? {
        try {
            if (null == key)
                throw Exception("error: key null")
            if (null == data)
                throw Exception("error: data null")
            if (useJNI) {
                return _hamcSha265(data, key)
                    ?: throw Exception("error: _hamcSha265 fail")
            } else {
                // 算法名称
                val algorithm = "HmacSHA256"
                // 创建 Mac 实例
                val mac = Mac.getInstance(algorithm)
                // 生成 SecretKeySpec
                val secretKeySpec = SecretKeySpec(key, algorithm)
                // 初始化 Mac
                mac.init(secretKeySpec)
                // 计算 HMAC 值
                val hmacBytes = mac.doFinal(data)
                val result = java.lang.StringBuilder()
                for (b in hmacBytes) {
                    result.append(String.format("%02x", b))
                }
                return result.toString()
            }
        } catch (e: Exception) {
            LogUtil.print("Box", "hmacSha265 -> Exception ${e.message}")
            return null
        }
    }

    /**
     * ### 请求参数
     * | 参数名      | 类型   | 必填 | 说明                       |
     * | ----------- | ------ | ---- | -------------------------- |
     * | token       | string | 是   | 访问令牌                   |
     * | ts          | int    | 是   | 时间戳（秒）               |
     * | nonce       | string | 是   | 随机字符串                 |
     * | path         | string | 是   | 资源URL（m3u8/ts）         |
     * | sign        | string | 是   | 签名                       |
     * | deviceId    | string | 是   | 设备唯一标识               |
     * | osModel     | string | 否   | 设备型号                   |
     * | osBrand     | string | 否   | 设备品牌                   |
     * | os          | string | 否   | 操作系统                   |
     * | osVersion   | string | 否   | 操作系统版本               |
     * | appVersion  | string | 否   | 应用版本                   |
     * | uid         | string | 是   | 用户唯一标识               |
     * | clientIP    | string | 是   | 客户端IP地址               |
     * | vid         | string | 是   | 视频ID                     |
     * | epId        | string | 否   | 剧集ID（如有）             |
     * ### 签名建议
     * - 所有参数（除sign外）按顺序拼接，未传用空字符串，最后拼接secret_key：
     *   ```
     *   sign_str = token + ts + nonce + path + deviceId + osModel + osBrand + os + osVersion + appVersion + uid + clientIP + vid + epId + secret_key
     *   sign = HMAC_SHA256(sign_str, secret_key)
     *   ```
     */

    fun formatUrl(
        baseUrl: String,
        vid: String,
        epId: String
    ): String {

        try {
            LogUtil.print("Box", "formatUrl1 -> baseUrl = $baseUrl")
            val builder = java.lang.StringBuilder()
            builder.append(baseUrl)
            builder.append("?token=")
            builder.append("&ts=")
            builder.append("&nonce=")
            builder.append("&path=")
            val deviceId = BoxUtils.getDeviceId()
            builder.append("&deviceId=")
            builder.append(deviceId)
            builder.append("&osModel=")
            val osModel = BoxUtils.getOsModel()
            builder.append(osModel)
            builder.append("&osBrand=")
            val osBrand = BoxUtils.getOsBrand()
            builder.append(osBrand)
            builder.append("&os=")
            val os = BoxUtils.getOs()
            builder.append(os)
            builder.append("&osVersion=")
            val osVersion = BoxUtils.getOsVersionName()
            builder.append(osVersion)
            builder.append("&appVersion=")
            val appVersion = BoxUtils.getAppVersionName()
            builder.append(appVersion)
            builder.append("&uid=")
            val uid = BoxUtils.getUid()
            builder.append(uid)
            builder.append("&clientIP=")
            val clientIP = BoxUtils.getClientIp()
            builder.append(clientIP)
            builder.append("&vid=")
            builder.append(vid)
            builder.append("&epId=")
            builder.append(epId)
            builder.append("&sign=")
            val newUrl = builder.toString()
            LogUtil.print("Box", "formatUrl1 -> newUrl = $newUrl")
            return newUrl
        } catch (e: Exception) {
            LogUtil.print("Box", "formatUrl1 -> Exception  ${e.message}")
            return baseUrl
        }
    }

    /**
     * ### 请求参数
     * | 参数名      | 类型   | 必填 | 说明                       |
     * | ----------- | ------ | ---- | -------------------------- |
     * | token       | string | 是   | 访问令牌                   |
     * | ts          | int    | 是   | 时间戳（秒）               |
     * | nonce       | string | 是   | 随机字符串                 |
     * | path         | string | 是   | 资源URL（m3u8/ts）         |
     * | sign        | string | 是   | 签名                       |
     * | deviceId    | string | 是   | 设备唯一标识               |
     * | osModel     | string | 否   | 设备型号                   |
     * | osBrand     | string | 否   | 设备品牌                   |
     * | os          | string | 否   | 操作系统                   |
     * | osVersion   | string | 否   | 操作系统版本               |
     * | appVersion  | string | 否   | 应用版本                   |
     * | uid         | string | 是   | 用户唯一标识               |
     * | clientIP    | string | 是   | 客户端IP地址               |
     * | vid         | string | 是   | 视频ID                     |
     * | epId        | string | 否   | 剧集ID（如有）             |
     * ### 签名建议
     * - 所有参数（除sign外）按顺序拼接，未传用空字符串，最后拼接secret_key：
     *   ```
     *   sign_str = token + ts + nonce + path + deviceId + osModel + osBrand + os + osVersion + appVersion + uid + clientIP + vid + epId + secret_key
     *   sign = HMAC_SHA256(sign_str, secret_key)
     *   ```
     */
    fun formatUrl(url: String): String {
        try {
            LogUtil.print("Box", "formatUrl2 -> url = $url")

            val uri = Uri.parse(url)

            val builder = java.lang.StringBuilder()
            val indexOf = url.indexOf("?")
            val baseUrl = url.substring(0, indexOf)
            LogUtil.print("Box", "formatUrl2 -> baseUrl = $baseUrl")
            builder.append(baseUrl)

            val query = uri.query
            LogUtil.print("Box", "formatUrl2 -> query = $query")
            val names = uri.queryParameterNames
            val sign_str = java.lang.StringBuilder()
            for (i in names.indices) {
                // token
                if (i == 0) {
                    val token = BoxUtils.getToken()
                    LogUtil.print("Box", "formatUrl2 -> i = 0, name = token, value = $token")
                    sign_str.append(token)
                    builder.append("?token=$token")
                }
                // ts
                else if (i == 1) {
                    val ts = BoxUtils.getTs()
                    LogUtil.print("Box", "formatUrl2 -> i = 1, name = ts, value = $ts")
                    sign_str.append(ts)
                    builder.append("&ts=$ts")
                }
                // nonce
                else if (i == 2) {
                    val nonce = UUID.randomUUID().toString().replace("-", "") + System.nanoTime()
                    LogUtil.print("Box", "formatUrl2 -> i = 2, name = nonce, value = $nonce")
                    sign_str.append(nonce)
                    builder.append("&nonce=$nonce")
                }
                // path
                else if (i == 3) {
                    val path = uri.path
                    LogUtil.print("Box", "formatUrl2 -> i = 3, name = path, value = $path")
                    sign_str.append(path)
                    builder.append("&path=$path")
                }
                // sign
                else if (i == 14) {

                    //
                    val secret_key = md5(_getFlag2())
                    LogUtil.print("Box", "formatUrl2 -> secret_key = $secret_key")
                    sign_str.append(secret_key)
                    LogUtil.print("Box", "formatUrl2 -> sign_str = $sign_str")

                    val bytesKey = secret_key!!.toByteArray(StandardCharsets.UTF_8)
                    val bytesStr = sign_str.toString().toByteArray(StandardCharsets.UTF_8)

                    val sign =
                        hmacSha265(bytesKey, bytesStr)
                    LogUtil.print("Box", "formatUrl2 -> i = 14, name = sign, value = $sign")
                    builder.append("&sign=$sign")

                    val secret_key2 = md5(_getFlag2(), false)
                    LogUtil.print("Box", "formatUrl2 -> secret_key2 = $secret_key2")
                    val sign2 =
                        hmacSha265(bytesKey, bytesStr, false)
                    LogUtil.print("Box", "formatUrl2 -> i = 14, name = sign2, value = $sign2")
                }
                // default
                else {
                    val name = names.elementAt(i)
                    val value = uri.getQueryParameter(name)
                    LogUtil.print("Box", "formatUrl2 -> i = $i, name = $name, value = $value")
                    sign_str.append(value)
                    builder.append("&$name=$value")
                }
            }

            val newUrl = builder.toString()
            LogUtil.print("Box", "formatUrl2 -> newUrl = $newUrl")
            return newUrl
        } catch (e: Exception) {
            LogUtil.print("Box", "formatUrl2 -> Exception  ${e.message}")
            return url
        }
    }
}
