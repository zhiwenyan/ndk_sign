#include <jni.h>
#include <string>
#include "md5.h"
#include <android/log.h>

static char *EXTRA_SIGNATURE = "steven";
using namespace std;
static int is_verify = 0;
static char *PACKAGE_NAME = "com.steven.ndk2018";
static char *APP_SIGNATURE = "308201dd30820146020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b3009060355040613025553301e170d3137303731383039353335305a170d3437303731313039353335305a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330819f300d06092a864886f70d010101050003818d0030818902818100a8a6ab4af1c8cc5f13c8bd95138cec2acc0eca47f2a5111dfb1fee762e80ab2be87ba2bcbd2a4c490725bf652aac16919d3b4ae5f0b582c83297d6271655a765185f531f39883caa7ac82647173a8e84ee535ec8b2e255fddd87b6fdb952db4d4c721cc2ff0dd4f249da5fc7ea593101a64176e171726021665579a7384e6dd70203010001300d06092a864886f70d010105050003818100a69eb4954a415e2d6f5cef0012c46ade99649204d4d7547b5e9f21316eaea33785c00672ea476413d8159fb3e937a70d20117c33e1233cdbc82dc43cdd94feb3644065d83cd5d8a1b63c4c0ca5e3df24bf5cf1ac63885989c0212afff2b51e54bd6c1eace91457b3f70f7cb834558482253127d0d6e7f81c917669c42320a402";
//ؖƿ86e7548ae0d465136870a52115cd219b
extern "C"
JNIEXPORT jstring JNICALL
Java_com_steven_ndk2018_utils_SignatureUtils_signatureParams(JNIEnv *env, jclass type,
                                                             jstring params_) {

//    if (is_verify == 0) {
//        return env->NewStringUTF("error_signature");
//    }

    const char *params = env->GetStringUTFChars(params_, 0);

    // MD5 签名规则，加点料
    // 1. 字符串前面加点料
    string signature_str(params);
    signature_str.insert(0, EXTRA_SIGNATURE);
    // 2. 后面去掉两位
    signature_str = signature_str.substr(0, signature_str.length() - 2);

    // 3. md5 去加密 C++ 和 Java 是一样的，唯一不同就是需要自己回收内存
    MD5_CTX *ctx = new MD5_CTX();
    MD5Init(ctx);
    MD5Update(ctx, (unsigned char *) signature_str.c_str(), signature_str.length());
    unsigned char digest[16] = {0};
    MD5Final(digest, ctx);

    // 生成 32 位的字符串
    char md5_str[32];
    for (int i = 0; i < 16; i++) {
        // 不足的情况下补0 f = 0f, ab = ab
        sprintf(md5_str, "%s%02x", md5_str, digest[i]);
    }

    // 释放资源
    env->ReleaseStringUTFChars(params_, params);
    return env->NewStringUTF(md5_str);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_ndk2018_utils_SignatureUtils_signatureVerify(JNIEnv *env, jclass type,
                                                             jobject context) {

    // 1. 获取包名
    jclass j_clz = env->GetObjectClass(context);
    jmethodID j_mid = env->GetMethodID(j_clz, "getPackageName", "()Ljava/lang/String;");
    jstring j_package_name = (jstring) env->CallObjectMethod(context, j_mid);
    // 2 . 比对包名是否一样
    const char *c_package_name = env->GetStringUTFChars(j_package_name, NULL);
    if (strcmp(c_package_name, PACKAGE_NAME) != 0) {
        return;
    }
    // 3. 获取签名
    // 3.1 获取 PackageManager
    j_mid = env->GetMethodID(j_clz,"getPackageManager","()Landroid/content/pm/PackageManager;");
    jobject pack_manager = env->CallObjectMethod(context,j_mid);
    // 3.2 获取 PackageInfo
    j_clz = env->GetObjectClass(pack_manager);
    j_mid = env->GetMethodID(j_clz,"getPackageInfo","(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject package_info = env->CallObjectMethod(pack_manager,j_mid,j_package_name,0x00000040);
    // 3.3 获取 signatures 数组
    j_clz = env->GetObjectClass(package_info);
    jfieldID j_fid = env->GetFieldID(j_clz,"signatures","[Landroid/content/pm/Signature;");
    jobjectArray signatures = (jobjectArray) env->GetObjectField(package_info, j_fid);
    // 3.4 获取 signatures[0]
    jobject signatures_first = env->GetObjectArrayElement(signatures,0);
    // 3.5 调用 signatures[0].toCharsString();
    j_clz = env->GetObjectClass(signatures_first);
    j_mid = env->GetMethodID(j_clz,"toCharsString","()Ljava/lang/String;");
    jstring j_signature_str = (jstring) env->CallObjectMethod(signatures_first, j_mid);
    const char * c_signature_str = env->GetStringUTFChars(j_signature_str,NULL);
    // 4. 比对签名是否一样
    if (strcmp(c_signature_str, APP_SIGNATURE) != 0) {
        return;
    }
    __android_log_print(ANDROID_LOG_ERROR,"JNI_TAG","签名校验成功: %s",c_signature_str);
    // 签名认证成功
    is_verify = 1;
}