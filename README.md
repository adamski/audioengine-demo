# audioengine-demo
An example of a multi-platform library using JUCE

### Proper description: TODO!

#### Caveats

The iOS version creates a `.dylib`. Dynamic Libraris are not allowed by Apple for apps submitted to the App Store. They need to be wrapped in a Framework. JUCE currently does not support creating Frameworks. Probably a script could be made to create the correct folder structure for it to be recognised as a Framework. TBC


