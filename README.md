#
#### Demo

![image](https://github.com/153437803/moudle_safetools/blob/master/image20210202113900.gif )

#
#### 配置说明
```
1. jni签名信息
- com.kalu.jnitools
- lib_jnitools/src/main/cpp/tool/signaturetool.h

2. jni日志开关
- 0 关闭日志 1 打开日志
- lib_jnitools/src/main/cpp/tool/debugtool.h
```

#
#### 加密字符
```
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
```

#
#### 解密字符
```
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
 * @param str            待解密信息
 * @param checkRoot      是否检测机器是否root
 * @param checkEmulator  是否检测模拟器
 * @param checkXposed    是否检测Xposed
 * @param checkSignature 是否检测app签名信息
 * @return 解密信息
 */
@Keep
public static native String aesDecodeMult(String str, boolean checkRoot, boolean checkEmulator, boolean checkXposed, boolean checkSignature);
```

#
#### 检测app签名信息
```
/**
 * 检测签名信息师傅被纂改
 *
 * @return true: 通过, fasle: 未通过
 */
@Keep
public static native boolean checkSignature();
```

#
#### 检测机器是否root
```
/**
 * 是否存在Root标记
 *
 * @return true: 通过, fasle: 未通过
 */
@Keep
public static native boolean checkRoot();
```

#
#### 检测模拟器
```
/**
 * 是否存在模拟器
 *
 * @return true: 通过, fasle: 未通过
 */
@Keep
public static native boolean checkEmulator();
```

#
#### 检测Xposed
```
/**
 * 是否存在 xposed
 *
 * @return true: 通过, fasle: 未通过
 */
@Keep
public static native boolean checkXposed();
```

#
#### todo list
```
1. jni 字符串加密解密 - 100%
2. app 签名校验 - 100%
3. app xpose框架检测 - 90%, 暂未支持android多用户
```
