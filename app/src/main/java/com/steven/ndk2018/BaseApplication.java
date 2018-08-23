package com.steven.ndk2018;

import android.app.Application;

/**
 * Description:
 * Data：8/14/2018-2:24 PM
 *
 * @author yanzhiwen
 */
public class BaseApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
       //SignatureUtils.signatureVerify(this);
    }
}
