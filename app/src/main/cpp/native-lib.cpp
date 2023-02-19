#include <jni.h>
#include <string>
#include <android/log.h>
#include "md5.h"  //引入md5的头文件

using namespace std;

static char* EXTRA_SIGNATURE = "JILLIAN";  //额外附加的字符串
//校验签名
static int is_verify = 0;
static char* PACKAGE_NAME = "com.example.ndk";
static char* APP_SIGNATURE = "308202e4308201cc020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b30090603550406130255533020170d3232303931303035333133375a180f32303532303930323035333133375a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330820122300d06092a864886f70d01010105000382010f003082010a0282010100b1d3b77fa17fdcbd45fb6064b3eb65a69862161caca0a73846f745c673096d7223b0da5537bb1aee131059d2b3981e08a3a74f6ef7f3120438b7a4e580cd630db29eea58f3b4c1ef6ed3b1f12e4c52aa2f1b78cbb09786826b502edf6d80cd1a6141c0e620d1381939c93c55edc7aac05500896ee90255652dcfdf8ff141a3104b14c2d66bb612a976dd3f6e5eec8d1b22b2c0b597f185fa39d58624c15f57b3364d032acd334a769821a060b9f731e123926936647b0b3b1dca8c221cc5200cff06ab3b5d58f90858b7c97f2d5a47b457fe3227d48c926b42a051d027a292e7a4ecfbd7ce273617770e8cc8a14e4a0f02d1cc81d92b081819210b1a4808754f0203010001300d06092a864886f70d010105050003820101003c8c0ca55cbefa322f21632f81a2083cbb8f4eb844fb6a1b61186e68fe8e2a76beda720614d04184dcd08124bee4595ac21f8ccbabe860951bd2b7c3e10910c8c18280e6c30972bb544f1ba01391d57613234bdcaf5e573361da31b9be81bec851c5f58161496506bd701f0a3c5c7bbf313b065da36066d11a52a791b50410d159c27357dd38c270cfb6b5e95472769968e4f51a3c82e41e4033b69f29e5a6b85b303d89830cc1c0e68da0fd9996be898f47fa7aac2f6e9ec6e2767df36676f33b6a7d17e3134e0c0e60673381731d9afb6681e4d7165e32dce53ede05e98028e47268e377486b51ea2eeae82897f103dff4aea462bd02cbc72c39b2708cd87f";

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_ndk_SignatureUtils_signatureParams(JNIEnv *env, jclass clazz, jstring params_) {
    const char *params = env->GetStringUTFChars(params_, 0);

    if (is_verify == 0) {
        return env->NewStringUTF("error_signature");
    }

    // MD5签名规则
    // 1.字符串面前添加
    string signature_str(params);
    signature_str.insert(0, EXTRA_SIGNATURE);
    // 2.去掉后面2位
    signature_str = signature_str.substr(0, signature_str.length()-2);
    // 3.MD5去加密
    MD5 md5(signature_str);
    string result = md5.md5();

    //释放资源
    env->ReleaseStringUTFChars(params_, params);

    return env->NewStringUTF(result.c_str());
}

/**
*   PackageInfo packageInfo = null;
    try {
        packageInfo = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
    } catch (PackageManager.NameNotFoundException e) {
        e.printStackTrace();
    }
    Signature[] signatures = packageInfo.signatures;
    Log.e("TAG",signatures[0].toCharsString());
 */
 // 用C调用以上java代码
extern "C"
JNIEXPORT void JNICALL
Java_com_example_ndk_SignatureUtils_signatureVerify(JNIEnv *env, jclass clazz, jobject context) {
    // 1.获取包名
    jclass j_clz = env->GetObjectClass(context);
    jmethodID j_mid = env->GetMethodID(j_clz, "getPackageName", "()Ljava/lang/String;");
    jstring j_package_name = static_cast<jstring>(env->CallObjectMethod(context, j_mid));
    // 2.比对包名是否一样
    const char * c_package_name = env->GetStringUTFChars(j_package_name, NULL);

    if (strcmp(c_package_name, PACKAGE_NAME) != 0) {   //字符串比较
        return;
    }
    __android_log_print(ANDROID_LOG_ERROR, "JNI_TAG", "包名一致: %s", c_package_name);
    // 3.获取签名
    // 3.1 获取 PackageManager
    j_mid = env->GetMethodID(j_clz, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject package_manager = env->CallObjectMethod(context, j_mid);
    // 3.2 获取 PackageInfo
    j_clz = env->GetObjectClass(package_manager);
    j_mid = env->GetMethodID(j_clz, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject package_info = env->CallObjectMethod(package_manager, j_mid, j_package_name, 0x00000040);  //需要传参:  getPackageName(), PackageManager.GET_SIGNATURES
    // 3.3 获取 signatures 数组
    j_clz = env->GetObjectClass(package_info);
    jfieldID j_fid = env->GetFieldID(j_clz, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatures = static_cast<jobjectArray>(env->GetObjectField(package_info, j_fid));
    // 3.4 获取 signatures[0]
    jobject signatures_first = env->GetObjectArrayElement(signatures, 0);
    // 3.5 调用 signatures[0].toCharsString();
    j_clz = env->GetObjectClass(signatures_first);
    j_mid = env->GetMethodID(j_clz, "toCharsString", "()Ljava/lang/String;");
    jstring j_signature_str = static_cast<jstring>(env->CallObjectMethod(signatures_first, j_mid));
    const char * c_signature_str = env->GetStringUTFChars(j_signature_str, NULL);
    // 4.比对签名是否一样
    if (strcmp(c_signature_str, APP_SIGNATURE) != 0) {   //字符串比较
        return;
    }
    is_verify = 1;  //签名认证成功
    __android_log_print(ANDROID_LOG_ERROR, "JNI_TAG", "签名校验成功: %s", c_signature_str);
}