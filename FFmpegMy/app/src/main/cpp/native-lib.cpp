#include <jni.h>
#include <string>

extern "C"
{
#include <libavfilter/avfilter.h>
JNIEXPORT jstring JNICALL
Java_com_ffmpegmy_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    char info[40000] = {0};
    avfilter_register_all();

    AVFilter *f_temp = (AVFilter *)avfilter_next(NULL);
    while(f_temp != NULL) {
        sprintf(info, "%s%s\n", info, f_temp->name);
        f_temp = f_temp->next;
    }
    return env->NewStringUTF(info);
}


}
