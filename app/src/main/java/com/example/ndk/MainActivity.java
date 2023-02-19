package com.example.ndk;

import androidx.appcompat.app.AppCompatActivity;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.example.ndk.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {



    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;

        // 获取签名
//        PackageInfo packageInfo = null;
//        try {
//            packageInfo = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
//        } catch (PackageManager.NameNotFoundException e) {
//            e.printStackTrace();
//        }
//        Signature[] signatures = packageInfo.signatures;
//        Log.e("TAG",signatures[0].toCharsString());

        // 注：参数的加密应该是在运行时框架中如HttpUtils，不每次加密，这里写在了编译时很 low
        String signature = SignatureUtils.signatureParams("userName=jillian&userPwd=123456");
        // signature作为参数给到服务器，服务器会生成同样的密文，对加密的字符串进行比较

        tv.setText(SignatureUtils.signatureParams("userName=jillian&userPwd=123456"));
    }
  }