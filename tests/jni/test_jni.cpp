#include <jni.h>
#include <gtest/gtest.h>
#include <libsmu/libsmu.hpp>

class JNITest : public ::testing::Test {
protected:
    JNIEnv* env;
    JavaVM* jvm;
    jclass sessionClass;
    jclass deviceClass;
    jobject sessionObject;

    virtual void SetUp() {
        JavaVMInitArgs vm_args;
        JavaVMOption options[1];
        options[0].optionString = const_cast<char*>("-Djava.class.path=.");
        vm_args.version = JNI_VERSION_1_6;
        vm_args.nOptions = 1;
        vm_args.options = options;
        vm_args.ignoreUnrecognized = false;

        jint res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
        ASSERT_EQ(res, JNI_OK);

        sessionClass = env->FindClass("com/example/libsmu/Session");
        ASSERT_NE(sessionClass, nullptr);

        jmethodID constructor = env->GetMethodID(sessionClass, "<init>", "()V");
        ASSERT_NE(constructor, nullptr);

        sessionObject = env->NewObject(sessionClass, constructor);
        ASSERT_NE(sessionObject, nullptr);

        deviceClass = env->FindClass("com/example/libsmu/Device");
        ASSERT_NE(deviceClass, nullptr);
    }

    virtual void TearDown() {
        jvm->DestroyJavaVM();
    }
};

TEST_F(JNITest, TestSessionCreation) {
    jmethodID createSession = env->GetMethodID(sessionClass, "createSession", "()J");
    ASSERT_NE(createSession, nullptr);

    jlong sessionPtr = env->CallLongMethod(sessionObject, createSession);
    ASSERT_NE(sessionPtr, 0);
}

TEST_F(JNITest, TestDeviceCreation) {
    jmethodID createSession = env->GetMethodID(sessionClass, "createSession", "()J");
    ASSERT_NE(createSession, nullptr);

    jlong sessionPtr = env->CallLongMethod(sessionObject, createSession);
    ASSERT_NE(sessionPtr, 0);

    jmethodID scan = env->GetMethodID(sessionClass, "scan", "()I");
    ASSERT_NE(scan, nullptr);

    jint devicesFound = env->CallIntMethod(sessionObject, scan);
    ASSERT_GT(devicesFound, 0);

    jmethodID addAll = env->GetMethodID(sessionClass, "addAll", "()I");
    ASSERT_NE(addAll, nullptr);

    jint devicesAdded = env->CallIntMethod(sessionObject, addAll);
    ASSERT_GT(devicesAdded, 0);

    jmethodID createDevice = env->GetMethodID(deviceClass, "createDevice", "(JLjava/lang/String;)J");
    ASSERT_NE(createDevice, nullptr);

    jstring serial = env->NewStringUTF("serial_number");
    jlong devicePtr = env->CallLongMethod(sessionObject, createDevice, sessionPtr, serial);
    ASSERT_NE(devicePtr, 0);
}

TEST_F(JNITest, TestDeviceReadWrite) {
    jmethodID createSession = env->GetMethodID(sessionClass, "createSession", "()J");
    ASSERT_NE(createSession, nullptr);

    jlong sessionPtr = env->CallLongMethod(sessionObject, createSession);
    ASSERT_NE(sessionPtr, 0);

    jmethodID scan = env->GetMethodID(sessionClass, "scan", "()I");
    ASSERT_NE(scan, nullptr);

    jint devicesFound = env->CallIntMethod(sessionObject, scan);
    ASSERT_GT(devicesFound, 0);

    jmethodID addAll = env->GetMethodID(sessionClass, "addAll", "()I");
    ASSERT_NE(addAll, nullptr);

    jint devicesAdded = env->CallIntMethod(sessionObject, addAll);
    ASSERT_GT(devicesAdded, 0);

    jmethodID createDevice = env->GetMethodID(deviceClass, "createDevice", "(JLjava/lang/String;)J");
    ASSERT_NE(createDevice, nullptr);

    jstring serial = env->NewStringUTF("serial_number");
    jlong devicePtr = env->CallLongMethod(sessionObject, createDevice, sessionPtr, serial);
    ASSERT_NE(devicePtr, 0);

    jmethodID setMode = env->GetMethodID(deviceClass, "setMode", "(JII)I");
    ASSERT_NE(setMode, nullptr);

    jint result = env->CallIntMethod(sessionObject, setMode, devicePtr, 0, smu::SVMI);
    ASSERT_EQ(result, 0);

    jmethodID write = env->GetMethodID(deviceClass, "write", "(JI[FZ)I");
    ASSERT_NE(write, nullptr);

    jfloatArray buffer = env->NewFloatArray(3);
    jfloat elements[] = {1.0f, 2.0f, 3.0f};
    env->SetFloatArrayRegion(buffer, 0, 3, elements);

    result = env->CallIntMethod(sessionObject, write, devicePtr, 0, buffer, JNI_FALSE);
    ASSERT_EQ(result, 0);

    jmethodID read = env->GetMethodID(deviceClass, "read", "(JII[F)I");
    ASSERT_NE(read, nullptr);

    jfloatArray readBuffer = env->NewFloatArray(12);
    jint samplesRead = env->CallIntMethod(sessionObject, read, devicePtr, 3, readBuffer);
    ASSERT_EQ(samplesRead, 3);

    jfloat readElements[12];
    env->GetFloatArrayRegion(readBuffer, 0, 12, readElements);

    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(readElements[i * 4], elements[i]);
    }
}
