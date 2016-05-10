#include <jni.h>

JNIEXPORT jstring JNICALL
Java_loipn_helloandroidjni_MainActivity_getMsgFromJni(JNIEnv *env, jobject instance) {

    // TODO


    return (*env)->NewStringUTF(env, "Hello From Jni loi");
}

int* rgbData;
int rgbDataSize = 0;

JNIEXPORT void JNICALL Java_loipn_helloandroidjni_CameraBridgeViewBase_YUVtoRBG(JNIEnv * env, jobject obj, jintArray rgb, jbyteArray yuv420sp, jint width, jint height)
{
    int sz;
    int i;
    int j;
    int Y;
    int Cr = 0;
    int Cb = 0;
    int pixPtr = 0;
    int jDiv2 = 0;
    int R = 0;
    int G = 0;
    int B = 0;
    int cOff;
    int w = width;
    int h = height;
    sz = w * h;

    jboolean isCopy;
    jbyte* yuv = (*env)->GetByteArrayElements(env, yuv420sp, &isCopy);
    if(rgbDataSize < sz) {
        int tmp[sz];
        rgbData = &tmp[0];
        rgbDataSize = sz;

    }

    for(j = 0; j < h; j++) {
        pixPtr = j * w;
        jDiv2 = j >> 1;
        for(i = 0; i < w; i++) {
            Y = yuv[pixPtr];
            if(Y < 0) Y += 255;
            if((i & 0x1) != 1) {
                cOff = sz + jDiv2 * w + (i >> 1) * 2;
                Cb = yuv[cOff];
                if(Cb < 0) Cb += 107; else Cb -= 108;
                Cr = yuv[cOff + 1];
                if(Cr < 0) Cr += 107; else Cr -= 108;
            }
            R = Y + Cr + (Cr >> 2) + (Cr >> 3) + (Cr >> 5); //1.406*~1.403
            if(R < 0) R = 0; else if(R > 255) R = 255;
            G = Y - (Cb >> 2) + (Cb >> 4) + (Cb >> 5) - (Cr >> 1) + (Cr >> 3) + (Cr >> 4) + (Cr >> 5);//
            if(G < 0) G = 0; else if(G > 255) G = 255;
            B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6);//1.765~1.770
            if(B < 0) B = 0; else if(B > 255) B = 255;
            rgbData[pixPtr++] = 0xff000000 + (B << 16) + (G << 8) + R;
        }
    }
    (*env)->SetIntArrayRegion(env, rgb, 0, sz, ( jint * ) &rgbData[0] );

    (*env)->ReleaseByteArrayElements(env, yuv420sp, yuv, JNI_ABORT);
}