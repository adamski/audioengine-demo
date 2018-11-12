#if JUCE_ANDROID

// Ensure that JUCE also includes some jni helpers
#define JUCE_CORE_INCLUDE_JNI_HELPERS 1
#include "../JuceLibraryCode/JuceHeader.h"
#include "DemoAudioEngine.h"

//==============================================================================
static void setCppInstance(JNIEnv* env, jobject javaInstance, DemoAudioEngine* cppInstance);
static DemoAudioEngine* getCppInstance (JNIEnv* env, jobject javaInstance);

//==============================================================================
class DemoAudioEngineAndroidBindings    : private DemoAudioEngine::Listener
{
public:
    DemoAudioEngineAndroidBindings(JNIEnv* env, jobject javaInstance, jobject context)
    {
        Thread::initialiseJUCE (env, context);

        // we are using JUCE GUI components so initialise the juce gui subsystem
        initialiseJuce_GUI();

        audioEngine.reset(new DemoAudioEngine);

        javaCounterpartInstance = env->NewWeakGlobalRef (javaInstance);
        env->SetLongField (javaInstance, DemoAudioEngineJavaClass.cppCounterpartInstance, reinterpret_cast<jlong> (this));

        audioEngine->addListener (this);
    }

    ~DemoAudioEngineAndroidBindings()
    {
        audioEngine->removeListener (this);
    }

private:
    //==============================================================================
    std::unique_ptr<DemoAudioEngine> audioEngine;
    jweak javaCounterpartInstance = nullptr;

    //==============================================================================
    #define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD, CALLBACK)                       \
      CALLBACK (jniConstructAudioEngine,   "jniConstructAudioEngine",   "(Landroid/content/Context;)V") \
      CALLBACK (jniDestroyAudioEngine,     "jniDestroyAudioEngine",     "()V")                          \
                                                                                                        \
      CALLBACK (jniPlay,     "jniPlay",     "(Ljava/lang/String;)V")                                    \
      CALLBACK (jniStop,     "jniStop",     "()V")                                                      \
                                                                                                        \
      CALLBACK (jniPause,    "jniPause",    "()V")                                                      \
      CALLBACK (jniResume,   "jniResume",   "()V")                                                      \
                                                                                                        \
      CALLBACK (jniSetRoomSize,        "jniSetRoomSize",      "(F)V")                                   \
      CALLBACK (jniSetLowpassCutoff,   "jniSetLowpassCutoff", "(F)V")                                   \
                                                                                                        \
      CALLBACK (jniAddWaveform,        "jniAddWaveform",      "(Landroid/view/ViewGroup;)V")            \
      CALLBACK (jniRemoveWaveform,     "jniRemoveWaveform",   "()V")                                    \
                                                                                                        \
      FIELD    (cppCounterpartInstance,    "cppCounterpartInstance", "J")                               \
                                                                                                        \
      METHOD (jniFilePlaybackFinished, "jniFilePlaybackFinished", "()V")                                    
    DECLARE_JNI_CLASS (DemoAudioEngineJavaClass, "com/acme/demoaudioengine/DemoAudioEngine")
    #undef JNI_CLASS_MEMBERS
    
    //==============================================================================
    // glue wrapper to invoke java code
    void filePlaybackFinished() override
    {
        auto* env = getEnv();
        
        LocalRef<jobject> javaThis (env->NewLocalRef (javaCounterpartInstance));
        env->CallVoidMethod (javaThis.get(), DemoAudioEngineJavaClass.jniFilePlaybackFinished);
    }

    //==============================================================================
    // simple glue wrappers to invoke the native code
    static void JNIEXPORT jniConstructAudioEngine (JNIEnv* env, jobject javaInstance, jobject context)
    {
        new DemoAudioEngineAndroidBindings (env, javaInstance, context);
    }
    
    static void JNIEXPORT jniDestroyAudioEngine (JNIEnv* env, jobject javaInstance)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            delete myself;
    }
    
    static void JNIEXPORT jniPlay (JNIEnv* env, jobject javaInstance, jstring url)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine->play (juceString (url).toRawUTF8());
    }
    
    static void JNIEXPORT jniStop (JNIEnv* env, jobject javaInstance)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine->stop();
    }
    
    static void JNIEXPORT jniPause (JNIEnv* env, jobject javaInstance)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine->pause();
    }
    
    static void JNIEXPORT jniResume (JNIEnv* env, jobject javaInstance)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine->resume();
    }
    
    static void JNIEXPORT jniSetRoomSize (JNIEnv* env, jobject javaInstance, float roomSize)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine->setRoomSize (roomSize);
    }
    
    static void JNIEXPORT jniSetLowpassCutoff (JNIEnv* env, jobject javaInstance, float cutoff)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine->setLowpassCutoff (cutoff);
    }

    static void JNIEXPORT jniAddWaveform (JNIEnv* env, jobject javaInstance, jobject viewGroup)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine->addWaveformComponentToNativeParentView (viewGroup);
    }

    static void JNIEXPORT jniRemoveWaveform (JNIEnv* env, jobject javaInstance)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            myself->audioEngine->removeWaveformComponentFromNativeParentView ();
    }
    
    //==============================================================================
    static DemoAudioEngineAndroidBindings* getCppInstance (JNIEnv* env, jobject javaInstance)
    {
        return reinterpret_cast<DemoAudioEngineAndroidBindings*> (env->GetLongField (javaInstance,
                                                                                     DemoAudioEngineJavaClass.cppCounterpartInstance));
    }
    
    static void setCppInstance(JNIEnv* env, jobject javaInstance, DemoAudioEngine* cppInstance)
    {
        env->SetLongField(javaInstance, DemoAudioEngineJavaClass.cppCounterpartInstance, (jlong) cppInstance);
    }
};

DemoAudioEngineAndroidBindings::DemoAudioEngineJavaClass_Class DemoAudioEngineAndroidBindings::DemoAudioEngineJavaClass;

#endif // JUCE_ANDROID
