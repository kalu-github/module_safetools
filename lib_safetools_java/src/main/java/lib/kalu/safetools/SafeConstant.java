package lib.kalu.safetools;

final class SafeConstant {

    static final int SIGNATURE_HASH = -1758486939;
    static final String SIGNATURE_MD5 = "35e530a0941fddb8a743c6dd2fb78970";

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


    static final String EMULATOR_DRIVERS_GOLDFISH = "goldfish";
    static final String[] EMULATOR_ARGS_DRIVERS_VALUE = new String[]{
            SafeConstant.EMULATOR_DRIVERS_GOLDFISH};
    static final String EMULATOR_DRIVERS_PROC = "/proc/tty/drivers";
    static final String EMULATOR_DRIVERS_CPUINFO = "/proc/cpuinfo";
    static final String[] EMULATOR_ARGS_DRIVERS = new String[]{
            SafeConstant.EMULATOR_DRIVERS_PROC,
            SafeConstant.EMULATOR_DRIVERS_CPUINFO};
    static final String EMULATOR_GENERIC = "generic";
    static final String EMULATOR_DEVICE = "device";
    static final String EMULATOR_ANDROID = "android";
    static final String EMULATOR_UNKNOWN = "unknown";
    static final String EMULATOR_ANDROID_SDK_BUILT_FOR_X86_1 = "android sdk built for x86";
    static final String EMULATOR_ANDROID_SDK_BUILT_FOR_X86_2 = "android_sdk_built_for_x86";
    static final String EMULATOR_EMULATOR = "emulator";
    static final String EMULATOR_GOOGLE_SDK = "google_sdk";
    static final String EMULATOR_SDK = "sdk";
    static final String EMULATOR_SDK_X86 = "sdk_x86";
    static final String EMULATOR_VBOX86P = "vbox86p";
    static final String EMULATOR_GENYMOTION = "genymotion";
    static final String EMULATOR_BLUESTACKS = "bluestacks";
    static final String[] EMULATOR_ARGS_SDK = new String[]{
            SafeConstant.EMULATOR_ANDROID,
            SafeConstant.EMULATOR_UNKNOWN,
            SafeConstant.EMULATOR_ANDROID_SDK_BUILT_FOR_X86_1,
            SafeConstant.EMULATOR_ANDROID_SDK_BUILT_FOR_X86_2,
            SafeConstant.EMULATOR_EMULATOR,
            SafeConstant.EMULATOR_GOOGLE_SDK,
            SafeConstant.EMULATOR_SDK,
            SafeConstant.EMULATOR_SDK_X86,
            SafeConstant.EMULATOR_VBOX86P,
            SafeConstant.EMULATOR_GENYMOTION,
            SafeConstant.EMULATOR_BLUESTACKS};
    static final String EMULATOR_INTEL = "intel";
    static final String EMULATOR_AMD = "amd";
    static final String EMULATOR_SYSTEM_BIN_CAT = "/system/bin/cat";
    static final String EMULATOR_PROC_CPUINFO = "/proc/cpuinfo";
    static final String[] EMULATOR_ARGS_CPU = new String[]{
            SafeConstant.EMULATOR_SYSTEM_BIN_CAT,
            SafeConstant.EMULATOR_PROC_CPUINFO};
}
