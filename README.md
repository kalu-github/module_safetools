#
#### Demo

![image](https://github.com/153437803/cmake_tools/blob/master/image20210202113900.gif )

#
#### jni签名校验
```
com.kalu.encryption
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

![image](https://github.com/153437803/cmake_tools/blob/master/image20210202114038.png )
![image](https://github.com/153437803/cmake_tools/blob/master/image20210202114106.png )

#
#### 检测app签名信息

![image](https://github.com/153437803/cmake_tools/blob/master/image20210202114126.png )

#
#### 检测机器是否root

![image](https://github.com/153437803/cmake_tools/blob/master/image20210202114139.png )

#
#### 检测模拟器

![image](https://github.com/153437803/cmake_tools/blob/master/image20210202114150.png )

#
#### 检测Xposed

![image](https://github.com/153437803/cmake_tools/blob/master/image20210202114205.png )

#
#### todo list
```
1. jni 字符串加密解密 - 100%
2. app 签名校验 - 100%
3. app xpose框架检测 - 90%, 暂未支持android多用户
```
