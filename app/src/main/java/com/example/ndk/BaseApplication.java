package com.example.ndk;

import android.app.Application;

/**
 * @author zouji
 * @Description:
 * @date 2023/2/19
 */
public class BaseApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        SignatureUtils.signatureVerify(this);
    }
}
