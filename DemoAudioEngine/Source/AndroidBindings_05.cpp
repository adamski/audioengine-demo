#if JUCE_ANDROID

// Ensure that JUCE also includes some jni helpers
#define JUCE_CORE_INCLUDE_JNI_HELPERS 1
#include "../JuceLibraryCode/JuceHeader.h"
#include "DemoAudioEngine.h"

//==============================================================================
static void setCppInstance(JNIEnv* env, jobject javaInstance, DemoAudioEngine* cppInstance);
static DemoAudioEngine* getCppInstance (JNIEnv* env, jobject javaInstance);

//==============================================================================
class DemoAudioEngineAndroidBindings
{
public:
    DemoAudioEngineAndroidBindings()
    {
        audioEngine.setPlaybackFinishedCallback ([this] () { filePlaybackFinished(); });
    }

    ~DemoAudioEngineAndroidBindings()
    {
        audioEngine.setPlaybackFinishedCallback ({});
    }

private:
    //==============================================================================
    ScopedJuceInitialiser_GUI juceInitialiser;
    DemoAudioEngine audioEngine;

    //==============================================================================
#define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD, CALLBACK)                       \
      CALLBACK (constructAudioEngine,   "constructAudioEngine",   "(Landroid/content/Context;)V")       \
      CALLBACK (destroyAudioEngine,     "destroyAudioEngine",     "()V")                                \
                                                                                                        \
      CALLBACK (play,     "play",     "(Ljava/lang/String;)V")                                          \
      CALLBACK (stop,     "stop",     "()V")                                                            \
                                                                                                        \
      CALLBACK (pause,    "pause",    "()V")                                                            \
      CALLBACK (resume,   "resume",   "()V")                                                            \
                                                                                                        \
      CALLBACK (setRoomSize,        "setRoomSize",      "(F)V")                                         \
      CALLBACK (setLowpassCutoff,   "setLowpassCutoff", "(F)V")                                         \
                                                                                                        \
      CALLBACK (addWaveformComponentToNativeParentView, "addWaveformComponentToNativeParentView", "(Landroid/view/ViewGroup;)V") \
      CALLBACK (removeWaveformComponentFromNativeParentView, "removeWaveformComponentFromNativeParentView", "()V")               \
                                                                                                        \
      FIELD    (cppCounterpartInstance,    "cppCounterpartInstance", "J")
    DECLARE_JNI_CLASS (DemoAudioEngineJavaClass, "com/acme/androidaudioenginedemo/DemoAudioEngine")
#undef JNI_CLASS_MEMBERS

    //==============================================================================
    // simple glue wrappers to invoke the native code
    static void JNICALL constructAudioEngine (JNIEnv* env, jobject javaInstance, jobject context)
    {
        Thread::initialiseJUCE (env, context);

        auto* cppCounterpartObject = new DemoAudioEngineAndroidBindings;
        env->SetLongField (javaInstance,DemoAudioEngineJavaClass.cppCounterpartInstance, reinterpret_cast<jlong> (cppCounterpartObject));
    }

    static void JNICALL destroyAudioEngine (JNIEnv* env, jobject javaInstance)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            delete myself;
    }

    static void JNICALL play (JNIEnv* env, jobject javaInstance, jstring url)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine.play (juceString (url).toRawUTF8());
    }

    static void JNICALL stop (JNIEnv* env, jobject javaInstance)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine.stop();
    }

    static void JNICALL pause (JNIEnv* env, jobject javaInstance)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine.pause();
    }

    static void JNICALL resume (JNIEnv* env, jobject javaInstance)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine.resume();
    }

    static void JNICALL setRoomSize (JNIEnv* env, jobject javaInstance, float roomSize)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine.setRoomSize (roomSize);
    }

    static void JNICALL setLowpassCutoff (JNIEnv* env, jobject javaInstance, float cutoff)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine.setLowpassCutoff (cutoff);
    }

    static void JNICALL addWaveformComponentToNativeParentView (JNIEnv* env, jobject javaInstance, jobject viewGroup)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine.addWaveformComponentToNativeParentView (viewGroup);
    }

    static void JNICALL removeWaveformComponentFromNativeParentView (JNIEnv* env, jobject javaInstance)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine.removeWaveformComponentFromNativeParentView ();
    }

    //==============================================================================
    void filePlaybackFinished()
    {
        // TODO!
    }

    //==============================================================================
    static DemoAudioEngineAndroidBindings* getCppInstance (JNIEnv* env, jobject javaInstance)
    {
        return reinterpret_cast<DemoAudioEngineAndroidBindings*> (env->GetLongField (javaInstance,
                                                                                     DemoAudioEngineJavaClass.cppCounterpartInstance));
    }
};

DemoAudioEngineAndroidBindings::DemoAudioEngineJavaClass_Class DemoAudioEngineAndroidBindings::DemoAudioEngineJavaClass;

#endif // JUCE_ANDROID
