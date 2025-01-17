#include <jni.h>
#include <libsmu/libsmu.hpp>

extern "C" {

JNIEXPORT jlong JNICALL Java_com_example_libsmu_Session_createSession(JNIEnv* env, jobject obj) {
    return reinterpret_cast<jlong>(new smu::Session());
}

JNIEXPORT void JNICALL Java_com_example_libsmu_Session_destroySession(JNIEnv* env, jobject obj, jlong sessionPtr) {
    delete reinterpret_cast<smu::Session*>(sessionPtr);
}

JNIEXPORT jint JNICALL Java_com_example_libsmu_Session_scan(JNIEnv* env, jobject obj, jlong sessionPtr) {
    return reinterpret_cast<smu::Session*>(sessionPtr)->scan();
}

JNIEXPORT jint JNICALL Java_com_example_libsmu_Session_addAll(JNIEnv* env, jobject obj, jlong sessionPtr) {
    return reinterpret_cast<smu::Session*>(sessionPtr)->add_all();
}

JNIEXPORT jint JNICALL Java_com_example_libsmu_Session_run(JNIEnv* env, jobject obj, jlong sessionPtr, jlong samples) {
    return reinterpret_cast<smu::Session*>(sessionPtr)->run(samples);
}

JNIEXPORT jint JNICALL Java_com_example_libsmu_Session_cancel(JNIEnv* env, jobject obj, jlong sessionPtr) {
    return reinterpret_cast<smu::Session*>(sessionPtr)->cancel();
}

JNIEXPORT void JNICALL Java_com_example_libsmu_Session_flush(JNIEnv* env, jobject obj, jlong sessionPtr) {
    reinterpret_cast<smu::Session*>(sessionPtr)->flush();
}

JNIEXPORT jlong JNICALL Java_com_example_libsmu_Device_createDevice(JNIEnv* env, jobject obj, jlong sessionPtr, jstring serial) {
    const char* serialStr = env->GetStringUTFChars(serial, nullptr);
    smu::Session* session = reinterpret_cast<smu::Session*>(sessionPtr);
    for (auto dev : session->m_available_devices) {
        if (dev->m_serial == serialStr) {
            env->ReleaseStringUTFChars(serial, serialStr);
            return reinterpret_cast<jlong>(dev);
        }
    }
    env->ReleaseStringUTFChars(serial, serialStr);
    return 0;
}

JNIEXPORT void JNICALL Java_com_example_libsmu_Device_destroyDevice(JNIEnv* env, jobject obj, jlong devicePtr) {
    // No need to delete the device, as it is managed by the session
}

JNIEXPORT jint JNICALL Java_com_example_libsmu_Device_setMode(JNIEnv* env, jobject obj, jlong devicePtr, jint channel, jint mode) {
    return reinterpret_cast<smu::Device*>(devicePtr)->set_mode(channel, mode);
}

JNIEXPORT jint JNICALL Java_com_example_libsmu_Device_getMode(JNIEnv* env, jobject obj, jlong devicePtr, jint channel) {
    return reinterpret_cast<smu::Device*>(devicePtr)->get_mode(channel);
}

JNIEXPORT jint JNICALL Java_com_example_libsmu_Device_write(JNIEnv* env, jobject obj, jlong devicePtr, jint channel, jfloatArray buffer, jboolean cyclic) {
    jsize length = env->GetArrayLength(buffer);
    jfloat* elements = env->GetFloatArrayElements(buffer, nullptr);
    std::vector<float> buf(elements, elements + length);
    env->ReleaseFloatArrayElements(buffer, elements, 0);
    return reinterpret_cast<smu::Device*>(devicePtr)->write(buf, channel, cyclic);
}

JNIEXPORT jint JNICALL Java_com_example_libsmu_Device_read(JNIEnv* env, jobject obj, jlong devicePtr, jint samples, jfloatArray buffer) {
    std::vector<std::array<float, 4>> buf;
    int readSamples = reinterpret_cast<smu::Device*>(devicePtr)->read(buf, samples, 0, false);
    jsize length = readSamples * 4;
    if (env->GetArrayLength(buffer) < length) {
        return -1; // Buffer too small
    }
    jfloat* elements = env->GetFloatArrayElements(buffer, nullptr);
    for (int i = 0; i < readSamples; ++i) {
        for (int j = 0; j < 4; ++j) {
            elements[i * 4 + j] = buf[i][j];
        }
    }
    env->ReleaseFloatArrayElements(buffer, elements, 0);
    return readSamples;
}

JNIEXPORT jint JNICALL Java_com_example_libsmu_Device_setLed(JNIEnv* env, jobject obj, jlong devicePtr, jint leds) {
    return reinterpret_cast<smu::Device*>(devicePtr)->set_led(leds);
}

JNIEXPORT jint JNICALL Java_com_example_libsmu_Device_setAdcMux(JNIEnv* env, jobject obj, jlong devicePtr, jint adcMux) {
    return reinterpret_cast<smu::Device*>(devicePtr)->set_adc_mux(adcMux);
}

} // extern "C"
