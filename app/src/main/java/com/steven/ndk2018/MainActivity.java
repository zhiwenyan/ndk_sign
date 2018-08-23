package com.steven.ndk2018;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import com.steven.ndk2018.utils.SignatureUtils;


public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //参数的加密应该在运行时架构中
        PackageInfo packageInfo = null;
        try {
            packageInfo = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        Signature[] signatures = packageInfo.signatures;
        Log.e("TAG",signatures[0].toCharsString());

        TextView tv = findViewById(R.id.sample_text);
        String signParam = SignatureUtils.signatureParams("userName=steven&password=123456");
        Log.i("TAG", signParam);
        tv.setText(signParam);
    }
}
