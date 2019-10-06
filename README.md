# audioengine-demo
An example of a multi-platform library using JUCE

### Proper description: TODO!

#### Credits

The JUCE project, Android and Windows bindings were originally written by Fabian Renn (@hogliux).

The example was presented as part of a workshop at the Audio Developers Conference 2018. 

#### Caveats

The iOS version creates a `.dylib`. Dynamic Libraris are not allowed by Apple for apps submitted to the App Store. They need to be wrapped in a Framework. JUCE currently does not support creating Frameworks. Probably a script could be made to create the correct folder structure for it to be recognised as a Framework. TBC


#### Gotchas

On Android, if you have proguard set for release builds, it is very likely that it will obfuscate your Java code resulting in crashes when the C++ code can't find a Java method or class member. Adding the following to your `proguard-rules.pro` *should* fix the issue:

```proguard
-keepclasseswithmembernames,includedescriptorclasses class * {
    native <methods>;
}

-keepclassmembers class com.nodeaudio.entonaltuner.TunerCore {
     long cppCounterpartInstance;
}
```
