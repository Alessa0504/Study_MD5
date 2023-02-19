package com.example.ndk;

import android.content.Context;

/**
 * @author zouji
 * @Description:
 * @date 2023/2/16
 */
public class SignatureUtils {

    // Used to load the 'ndk' library on application startup.
    static {
        System.loadLibrary("ndk");
    }

    /**
     * native 方法签名参数
     * @param params
     * @return
     */
    public static native String signatureParams(String params);

    /**
     * 校验签名，只允许自己的app可以使用这个so
     * @param context
     * @return
     */
    public static native void signatureVerify(Context context);
}
