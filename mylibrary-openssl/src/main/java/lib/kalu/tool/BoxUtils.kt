package lib.kalu.tool

import android.annotation.SuppressLint
import android.content.Context
import android.content.SharedPreferences
import android.net.wifi.WifiManager
import android.os.Build
import android.provider.Settings
import android.util.ArrayMap
import java.net.NetworkInterface
import java.nio.charset.StandardCharsets
import java.util.Collections
import java.util.UUID
import kotlin.math.E


class BoxUtils {

    companion object {

        /**
         * 应用唯一标识，如Android是包名，未来可能还有web、pc等
         */
        @JvmStatic
        private val appId: String = "f5a8c0603d1f4441a24f81bd1e62c0fa"

        @JvmStatic
        private var ethMac: String? = null

        @JvmStatic
        private var wlanMac: String? = null

        @JvmStatic
        private var deviceId: String? = null

        @JvmStatic
        private var versionName: String? = null

        @JvmStatic
        private var versionCode: String? = null

        @JvmStatic
        private var newUrl: String? = null

        @JvmStatic
        fun getNewUrl(): String? {
            return newUrl
        }

        @JvmStatic
        fun setNewUrl(value: String?) {
            this.newUrl = value
        }

        @JvmStatic
        fun getAppId(): String {
            return appId!!
        }

        @JvmStatic
        fun getOsModel(): String {
            return android.os.Build.MODEL ?: ""
        }

        @JvmStatic
        fun getOsBrand(): String {
            return android.os.Build.BOARD ?: ""
        }

        @JvmStatic
        fun getOs(): String {
            //return android.os.Build.VERSION.SDK_INT.toString()
            return "Android"
        }

        @JvmStatic
        fun getOsVersionCode(): String {
            return android.os.Build.VERSION.SDK_INT.toString()
        }

        @JvmStatic
        fun getOsVersionName(): String {
            return android.os.Build.VERSION.RELEASE ?: ""
        }

        @JvmStatic
        fun getAppVersionName(): String {
            if (null == versionName) {
                versionName = BoxProvider.ctx!!.packageManager.getPackageInfo(
                    BoxProvider.ctx!!.packageName,
                    0
                ).versionName
            }
            return versionName!!
        }

        @JvmStatic
        fun getAppVersionCode(): String {
            if (null == versionCode) {
                versionCode = BoxProvider.ctx!!.packageManager.getPackageInfo(
                    BoxProvider.ctx!!.packageName,
                    0
                ).versionCode.toString()
            }
            return versionCode!!
        }

        @JvmStatic
        fun getWlanSsid(): String {
            try {
                return (BoxProvider.ctx!!.getSystemService(Context.WIFI_SERVICE) as WifiManager).connectionInfo.ssid
            } catch (e: Exception) {
                return "null"
            }
        }

        @JvmStatic
        fun getWlanBssid(): String {
            try {
                return (BoxProvider.ctx!!.getSystemService(Context.WIFI_SERVICE) as WifiManager).connectionInfo.bssid
            } catch (e: Exception) {
                return "null"
            }
        }

        @JvmStatic
        fun getWlanMac(): String? {
//            try {
//                return (context.getSystemService(Context.WIFI_SERVICE) as WifiManager).connectionInfo.macAddress
//            } catch (e: Exception) {
//                return "null"
//            }
            if (null == wlanMac) {
                wlanMac = getMac("wlan0");
            }
            return wlanMac
        }

        @JvmStatic
        fun getEthMac(): String? {
            if (null == ethMac) {
                ethMac = getMac("eth0")
            }
            return ethMac
        }

        @JvmStatic
        fun getDeviceId(): String? {
            if (null == deviceId) {
                deviceId = getAndroidId()
            }
            return deviceId
        }


        // "wlan0"
        // "eth0"
        @JvmStatic
        private fun getMac(name: String?): String {
            try {
                if (null == name)
                    throw Exception("error: name null")
                if (name.isEmpty())
                    throw Exception("error: name empty")
                val interfaces: List<NetworkInterface> =
                    Collections.list(NetworkInterface.getNetworkInterfaces())
                if (interfaces.isEmpty())
                    throw Exception("error: interfaces empty")
                for (inter in interfaces) {

                    if (name != inter.name)
                        continue

                    val address = inter.hardwareAddress
                    if (address.isEmpty())
                        continue

                    val buf = StringBuilder()
                    for (b in address) {
                        buf.append(String.format("%02X:", b))
                    }
                    if (buf.length > 0) {
                        buf.deleteCharAt(buf.length - 1) // 删除最后一个冒号
                    }
                    return buf.toString()
                }
                throw Exception("error: not find");
            } catch (e: Exception) {
                return "null"
            }
        }

        @SuppressLint("HardwareIds")
        @JvmStatic
        private fun getAndroidId(): String {
            try {
//                if(Build.VERSION.SDK_INT < Build.VERSION_CODES.N){
//                    return Build.SERIAL;
//                }
//                else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
//                }
//                else{
//
//                }
                val id = Settings.Secure.getString(
                    BoxProvider.ctx!!.getContentResolver(),
                    Settings.Secure.ANDROID_ID
                )
                if (null == id)
                    throw Exception("error: id null")
                if (id.isEmpty())
                    throw Exception("error: id empty")
                return id
            } catch (e: Exception) {
                return "null"
            }
        }


        /*******************/

        @JvmStatic
        private val SP: SharedPreferences =
            BoxProvider.ctx!!.getSharedPreferences("BoxUtils", Context.MODE_PRIVATE)

        @JvmStatic
        private val MAP = ArrayMap<String, String>();

        @JvmStatic
        fun updateToken(
            token: String? = null
        ): Boolean {
            return update(_token = token)
        }

        @JvmStatic
        fun updateRefreshToken(
            refreshToken: String? = null
        ): Boolean {
            return update(_refreshToken = refreshToken)
        }

        @JvmStatic
        fun updateAvatar(
            avatar: String? = null
        ): Boolean {
            return update(_avatar = avatar)
        }

        @JvmStatic
        fun updateEmail(
            email: String? = null
        ): Boolean {
            return update(_email = email)
        }

        @JvmStatic
        fun updateUsername(
            username: String? = null
        ): Boolean {
            return update(_username = username)
        }

        @JvmStatic
        fun updateUid(
            uid: String? = null
        ): Boolean {
            return update(_uid = uid)
        }

        @JvmStatic
        fun updateServerTime(
            serverTime: String? = null
        ): Boolean {
            return update(_serverTime = serverTime)
        }

        @JvmStatic
        fun updateLocalTime(
            localTime: String? = null
        ): Boolean {
            return update(_localTime = localTime)
        }

        @JvmStatic
        fun updateClientIP(
            clientIP: String? = null
        ): Boolean {
            return update(_clientIP = clientIP)
        }

        @JvmStatic
        fun updateTokenExpireSeconds(
            tokenExpireSeconds: String? = null
        ): Boolean {
            return update(_tokenExpireSeconds = tokenExpireSeconds)
        }

        @JvmStatic
        private fun update(
            _clientIP: String? = null,
            _serverTime: String? = null,
            _token: String? = null,
            _refreshToken: String? = null,
            _avatar: String? = null,
            _email: String? = null,
            _username: String? = null,
            _uid: String? = null,
            _localTime: String? = null,
            _uuid: String? = null,
            _tokenExpireSeconds: String? = null,
            _saveMap: Boolean = true,
            _saveSp: Boolean = true
        ): Boolean {
            try {

                // SP
                val editor = SP.edit()
                LogUtil.print("BoxUtils", "update -> editor = $editor")

                // _clientIP
                LogUtil.print("BoxUtils", "update -> _clientIP = $_clientIP")
                if (!_clientIP.isNullOrEmpty()) {
                    if (_saveSp) {
                        editor.putString(clientIP, _clientIP)
                        LogUtil.print("BoxUtils", "update -> _clientIP putString succ")
                    }
                    if (_saveMap) {
                        MAP.put(clientIP, _clientIP)
                        LogUtil.print("BoxUtils", "update -> _clientIP put succ")
                    }
                }

                // _serverTime
                LogUtil.print("BoxUtils", "update -> _serverTime = $_serverTime")
                if (!_serverTime.isNullOrEmpty()) {
                    if (_saveSp) {
                        editor.putString(serverTime, _serverTime)
                        LogUtil.print("BoxUtils", "update -> _serverTime putString succ")
                    }
                    if (_saveMap) {
                        MAP.put(serverTime, _serverTime)
                        LogUtil.print("BoxUtils", "update -> _serverTime put succ")
                    }
                }

                // _token
                LogUtil.print("BoxUtils", "update -> _token = $_token")
                if (!_token.isNullOrEmpty()) {
                    if (_saveSp) {
                        editor.putString(token, _token)
                        LogUtil.print("BoxUtils", "update -> _token putString succ")
                    }
                    if (_saveMap) {
                        MAP.put(token, _token)
                        LogUtil.print("BoxUtils", "update -> _token put succ")
                    }
                }

                // _refreshToken
                LogUtil.print("BoxUtils", "update -> _refreshToken = $_refreshToken")
                if (!_refreshToken.isNullOrEmpty()) {
                    if (_saveSp) {
                        editor.putString(refreshToken, _refreshToken)
                        LogUtil.print("BoxUtils", "update -> _refreshToken putString succ")
                    }
                    if (_saveMap) {
                        MAP.put(refreshToken, _refreshToken)
                        LogUtil.print("BoxUtils", "update -> _refreshToken put succ")
                    }
                }

                // _avatar
                LogUtil.print("BoxUtils", "update -> _avatar = $_avatar")
                if (!_avatar.isNullOrEmpty()) {
                    if (_saveSp) {
                        editor.putString(avatar, _avatar)
                        LogUtil.print("BoxUtils", "update -> _avatar putString succ")
                    }
                    if (_saveMap) {
                        MAP.put(avatar, _avatar)
                        LogUtil.print("BoxUtils", "update -> _avatar put succ")
                    }
                }

                // email
                LogUtil.print("BoxUtils", "update -> _email = $_email")
                if (!_email.isNullOrEmpty()) {
                    if (_saveSp) {
                        editor.putString(email, _email)
                        LogUtil.print("BoxUtils", "update -> _email putString succ")
                    }
                    if (_saveMap) {
                        MAP.put(email, _email)
                        LogUtil.print("BoxUtils", "update -> _email put succ")
                    }
                }

                // _username
                LogUtil.print("BoxUtils", "update -> _username = $_username")
                if (!_username.isNullOrEmpty()) {
                    if (_saveSp) {
                        editor.putString(username, _username)
                        LogUtil.print("BoxUtils", "update -> _username putString succ")
                    }
                    if (_saveMap) {
                        MAP.put(username, _username)
                        LogUtil.print("BoxUtils", "update -> _username put succ")
                    }
                }

                // uid
                LogUtil.print("BoxUtils", "update -> _uid = $_uid")
                if (!_uid.isNullOrEmpty()) {
                    if (_saveSp) {
                        editor.putString(uid, _uid)
                        LogUtil.print("BoxUtils", "update -> _uid putString succ")
                    }
                    if (_saveMap) {
                        MAP.put(uid, _uid)
                        LogUtil.print("BoxUtils", "update -> _uid put succ")
                    }
                }

                // _localTime
                LogUtil.print("BoxUtils", "update -> _localTime = $_localTime")
                if (!_localTime.isNullOrEmpty()) {
                    if (_saveSp) {
                        editor.putString(localTime, _localTime)
                        LogUtil.print("BoxUtils", "update -> _localTime putString succ")
                    }
                    if (_saveMap) {
                        MAP.put(localTime, _localTime)
                        LogUtil.print("BoxUtils", "update -> _localTime put succ")
                    }
                }

                // _uuid
                LogUtil.print("BoxUtils", "update -> _uuid = $_uuid")
                if (!_uuid.isNullOrEmpty()) {
                    if (_saveSp) {
                        editor.putString(uuid, _uuid)
                        LogUtil.print("BoxUtils", "update -> _uuid putString succ")
                    }
                    if (_saveMap) {
                        MAP.put(uuid, _uuid)
                        LogUtil.print("BoxUtils", "update -> _uuid put succ")
                    }
                }

                // _tokenExpireSeconds
                LogUtil.print("BoxUtils", "update -> _tokenExpireSeconds = $_tokenExpireSeconds")
                if (!_tokenExpireSeconds.isNullOrEmpty()) {
                    if (_saveSp) {
                        editor.putString(tokenExpireSeconds, _tokenExpireSeconds)
                        LogUtil.print("BoxUtils", "update -> _tokenExpireSeconds putString succ")
                    }
                    if (_saveMap) {
                        MAP.put(tokenExpireSeconds, _tokenExpireSeconds)
                        LogUtil.print("BoxUtils", "update -> _tokenExpireSeconds put succ")
                    }
                }

                // commit
                editor.commit()

                return true
            } catch (e: Exception) {
                return false
            }
        }

        @JvmStatic
        fun clear(): Boolean {
            try {
                val editor = SP.edit()
                LOGIN_KEY.map {
                    editor.remove(it)
                    MAP.remove(it)
                }

                editor.commit()
                LogUtil.print("BoxUtils", "clear sp succ")

                LogUtil.print("BoxUtils", "clear map succ")

                return true
            } catch (e: Exception) {
                LogUtil.print("BoxUtils", "clear Exception ${e.message}")
                return false
            }
        }

        @JvmStatic
        fun isLogin(): Boolean {
            try {
                val uid = getUid()
                LogUtil.print("BoxUtils", "isLogin -> uid = $uid")
                if (uid.isEmpty())
                    throw Exception("warning: not login uid")
                val token = getToken()
                LogUtil.print("BoxUtils", "isLogin -> token = $token")
                if (token.isEmpty())
                    throw Exception("warning: not login token")
                return true
            } catch (e: Exception) {
                LogUtil.print("BoxUtils", "isLogin -> Exception ${e.message}")
                return false
            }
        }

        @JvmStatic
        fun getUUID(): String {
            return get(uuid)
        }

        @JvmStatic
        fun getClientIp(): String {
            return get(clientIP)
        }

        @JvmStatic
        fun getToken(): String {
            return get(token)
        }

        @JvmStatic
        fun getTokenExpireSeconds(): Long? {
            val second = get(tokenExpireSeconds).toLongOrNull()
            return second
        }

        @JvmStatic
        fun getRefreshToken(): String {
            return get(refreshToken)
        }

        @JvmStatic
        fun getUid(): String {
            return get(uid)
        }

        @JvmStatic
        fun getUsername(): String {
            return get(username)
        }

        @JvmStatic
        fun getEmail(): String {
            return get(email)
        }

        @JvmStatic
        fun getAvatar(): String {
            return get(avatar)
        }

        @JvmStatic
        fun getTs(): String {
            try {
                val server = get(serverTime)
                if (server.isEmpty())
                    throw Exception("error: server isEmpty")
                val local = get(localTime)
                if (local.isEmpty())
                    throw Exception("error: local isEmpty0")
                val curr = (System.currentTimeMillis() / 1000)
                val cast = curr - local.toInt()
                val time = server.toInt() + cast
                return time.toString()
            } catch (e: Exception) {
                return ""
            }
        }

        /*************/

        @JvmStatic
        private val uuid: String = "uuid"

        @JvmStatic
        private val clientIP: String = "clientIP"

        @JvmStatic
        private val token: String = "token"

        @JvmStatic
        private val refreshToken: String = "refreshToken"

        @JvmStatic
        private val tokenExpireSeconds: String = "tokenExpireSeconds"

        @JvmStatic
        private val uid: String = "uid"

        @JvmStatic
        private val username: String = "username"

        @JvmStatic
        private val email: String = "email"

        @JvmStatic
        private val avatar: String = "avatar"

        @JvmStatic
        private val serverTime: String = "serverTime"

        @JvmStatic
        private val localTime: String = "localTime"

        val LOGIN_KEY = listOf(token, refreshToken, avatar, email, username, uid, tokenExpireSeconds)

        @JvmStatic
        private fun get(key: String): String {

            // 一级缓存
            var value = MAP.get(key)
            LogUtil.print("BoxUtils", "get fromMap -> key = $key, value = $value")

            // 二级缓存
            if (value.isNullOrEmpty()) {
                val temp = SP.getString(key, null)
                LogUtil.print("BoxUtils", "get fromSp -> key = $key, value = $value")

                if (temp.isNullOrEmpty() && uuid == key) {
                    // uuid
                    value = Box.md5(
                        (UUID.randomUUID()
                            .toString() + System.currentTimeMillis()).toByteArray()
                    )
                    update(_uuid = value, _saveMap = true, _saveSp = true)
                    LogUtil.print("BoxUtils", "get fromUUID -> key = $key, value = $value")
                } else if (!temp.isNullOrEmpty()) {

                    value = temp
                    if (clientIP == key) {
                        update(_clientIP = value, _saveMap = true, _saveSp = false)
                    } else if (serverTime == key) {
                        update(_serverTime = value, _saveMap = true, _saveSp = false)
                    } else if (token == key) {
                        update(_token = value, _saveMap = true, _saveSp = false)
                    } else if (refreshToken == key) {
                        update(_refreshToken = value, _saveMap = true, _saveSp = false)
                    } else if (tokenExpireSeconds == key) {
                        update(_tokenExpireSeconds = value, _saveMap = true, _saveSp = false)
                    } else if (avatar == key) {
                        update(_avatar = value, _saveMap = true, _saveSp = false)
                    } else if (email == key) {
                        update(_email = value, _saveMap = true, _saveSp = false)
                    } else if (username == key) {
                        update(_username = value, _saveMap = true, _saveSp = false)
                    } else if (uid == key) {
                        update(_uid = value, _saveMap = true, _saveSp = false)
                    } else if (localTime == key) {
                        update(_localTime = value, _saveMap = true, _saveSp = false)
                    } else if (uuid == key) {
                        update(_uuid = value, _saveMap = true, _saveSp = false)
                    }

                    LogUtil.print("BoxUtils", "get fromRetry -> key = $key, value = $value")
                }
            }

            return value ?: ""
        }
    }
}