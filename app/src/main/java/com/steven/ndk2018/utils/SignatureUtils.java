package com.steven.ndk2018.utils;

import android.content.Context;

/**
 * Description:
 * Data：8/14/2018-11:24 AM
 *
 * @author yanzhiwen
 */
public class SignatureUtils {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * native 方法签名参数  方法是不能被混淆的, hook 到你调用的方法，自己写个项目
     *
     * @return 签名
     */
    public static native String signatureParams(String params);



    /**
     * 校验签名 ，只允许自己 App 可以使用这个 so
     * @param context
     * @return
     */
    public static native void signatureVerify(Context context);
}
