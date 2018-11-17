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
        audioEngine.setPlaybackFinishedCallback([this] () { filePlaybackFinished(); });
    }

    ~DemoAudioEngineAndroidBindings()
    {
        audioEngine.setPlaybackFinishedCallback({});
    }

private:
    //==============================================================================
    ScopedJuceInitialiser_GUI juceInitialiser;
    DemoAudioEngine audioEngine;

    //==============================================================================
    #define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD, CALLBACK)                       \
      // TODO Step 2: comment this back in
      // CALLBACK (constructAudioEngine,   "constructAudioEngine",   "(Landroid/content/Context;)V")       \
      // CALLBACK (destroyAudioEngine,     "destroyAudioEngine",     "()V")                                \

    DECLARE_JNI_CLASS (DemoAudioEngineJavaClass, "com/acme/androidaudioenginedemo/DemoAudioEngine")
    #undef JNI_CLASS_MEMBERS

    //==============================================================================
    static void JNICALL constructAudioEngine (JNIEnv* env, jobject javaInstance, jobject context)
    {
        Thread::initialiseJUCE (env, context);

        auto* cppCounterpartObject = new DemoAudioEngineAndroidBindings;

        // TODO Step 2: store the c++ instance pointer in a member variable in the java class
        // Use env->SetLongField(javaInstance, DemoAudioEngineJavaClass.<your-field-name-here>, fieldValue);
    }

    static void JNICALL destroyAudioEngine (JNIEnv* env, jobject javaInstance)
    {
        if (auto* myself = getCppInstance (env, javaInstance))
            delete myself;
    }

    //==============================================================================
    // simple glue wrappers to invoke the native code

    //==============================================================================
    void filePlaybackFinished()
    {
        // TODO!
    }

    //==============================================================================
    static DemoAudioEngineAndroidBindings* getCppInstance (JNIEnv* env, jobject javaInstance)
    {
        // TODO Step 2: call env->GetLongField (env, javaInstance, DemoAudioEngineJavaClass.<your-field-name>);
        return nullptr;
    }
};

DemoAudioEngineAndroidBindings::DemoAudioEngineJavaClass_Class DemoAudioEngineAndroidBindings::DemoAudioEngineJavaClass;

#endif // JUCE_ANDROID
