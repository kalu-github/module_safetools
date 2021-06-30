package lib.kalu.safetools;

final class SafeConstant {

    static final String XPOSED_FILES = "/files";
    static final String XPOSED_DATA_DATA = "/data/data";
    static final String XPOSED_DATA_USER = "/data/user";

    static final String ROOT_SBIN_SU = "/sbin/su";
    static final String ROOT_SBIN_BIN_SU = "/system/bin/su";
    static final String ROOT_SBIN_XBIN_SU = "/system/xbin/su";
    static final String ROOT_DATA_LOCAL_XBIN_SU = "/data/local/xbin/su";
    static final String ROOT_DATA_LOCAL_BIN_SU = "/data/local/bin/su";
    static final String ROOT_SYSTEM_SD_XBIN_SU = "/system/sd/xbin/su";
    static final String ROOT_SYSTEM_BIN_FAILSAFE_SU = "/system/bin/failsafe/su";
    static final String ROOT_DATA_LOCAL_SU = "/data/local/su";
    static final String[] ROOT_ARRAY = new String[]{
            SafeConstant.ROOT_SBIN_SU,
            SafeConstant.ROOT_SBIN_BIN_SU,
            SafeConstant.ROOT_SBIN_XBIN_SU,
            SafeConstant.ROOT_DATA_LOCAL_XBIN_SU,
            SafeConstant.ROOT_DATA_LOCAL_BIN_SU,
            SafeConstant.ROOT_SYSTEM_SD_XBIN_SU,
            SafeConstant.ROOT_SYSTEM_BIN_FAILSAFE_SU,
            SafeConstant.ROOT_DATA_LOCAL_SU};
}
