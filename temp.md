# JNI Interface Documentation

This document provides an overview of the various functions and classes exposed to the software interfacing with the JNI library.

## Classes

### Session

The `Session` class represents a session for managing devices and their interactions.

#### Methods

- `createSession()`: Creates a new session and returns a pointer to the session object.
- `destroySession(sessionPtr: Long)`: Destroys the session object.
- `scan(sessionPtr: Long)`: Scans for available devices and returns the number of devices found.
- `addAll(sessionPtr: Long)`: Adds all available devices to the session and returns the number of devices added.
- `run(sessionPtr: Long, samples: Long)`: Runs the session for the specified number of samples.
- `cancel(sessionPtr: Long)`: Cancels the session.
- `flush(sessionPtr: Long)`: Flushes the session.

### Device

The `Device` class represents a device managed by the session.

#### Methods

- `createDevice(sessionPtr: Long, serial: String)`: Creates a device object for the specified serial number and returns a pointer to the device object.
- `destroyDevice(devicePtr: Long)`: Destroys the device object.
- `setMode(devicePtr: Long, channel: Int, mode: Int)`: Sets the mode for the specified channel of the device.
- `getMode(devicePtr: Long, channel: Int)`: Gets the mode for the specified channel of the device.
- `write(devicePtr: Long, channel: Int, buffer: FloatArray, cyclic: Boolean)`: Writes data to the specified channel of the device.
- `read(devicePtr: Long, samples: Int, buffer: FloatArray)`: Reads data from the device.
- `setLed(devicePtr: Long, leds: Int)`: Sets the LED state of the device.
- `setAdcMux(devicePtr: Long, adcMux: Int)`: Sets the ADC multiplexer state of the device.

## Usage Example

Here is an example of how to use the JNI library in a Java application:

```java
import com.example.libsmu.Session;
import com.example.libsmu.Device;

public class Main {
    static {
        System.loadLibrary("libsmu_jni");
    }

    public static void main(String[] args) {
        Session session = new Session();
        session.scan();
        session.addAll();

        Device device = session.getDevice("serial_number");
        device.setMode(0, Device.SVMI);
        device.write(0, new float[]{1.0f, 2.0f, 3.0f}, false);

        float[] buffer = new float[1024];
        int samples = device.read(0, buffer);
        for (int i = 0; i < samples; i++) {
            System.out.println(buffer[i]);
        }

        session.run(1000);
        session.cancel();
        session.flush();
    }
}
```

## Kotlin Example

To use the JNI library in a Kotlin application with modern Gradle, follow these steps:

1. Add the JNI library to your project.

In your `build.gradle.kts` file, add the following dependencies:

```kotlin
dependencies {
    implementation(files("libs/libsmu_jni.so"))
}
```

2. Create a Kotlin class to interact with the JNI library.

Create a new Kotlin file, `Libsmu.kt`, and add the following code:

```kotlin
package com.example.libsmu

class Libsmu {
    companion object {
        init {
            System.loadLibrary("libsmu_jni")
        }
    }

    external fun createSession(): Long
    external fun destroySession(sessionPtr: Long)
    external fun scan(sessionPtr: Long): Int
    external fun addAll(sessionPtr: Long): Int
    external fun run(sessionPtr: Long, samples: Long): Int
    external fun cancel(sessionPtr: Long): Int
    external fun flush(sessionPtr: Long)
    external fun createDevice(sessionPtr: Long, serial: String): Long
    external fun destroyDevice(devicePtr: Long)
    external fun setMode(devicePtr: Long, channel: Int, mode: Int): Int
    external fun getMode(devicePtr: Long, channel: Int): Int
    external fun write(devicePtr: Long, channel: Int, buffer: FloatArray, cyclic: Boolean): Int
    external fun read(devicePtr: Long, samples: Int, buffer: FloatArray): Int
    external fun setLed(devicePtr: Long, leds: Int): Int
    external fun setAdcMux(devicePtr: Long, adcMux: Int): Int
}
```

3. Use the `Libsmu` class in your Kotlin application.

In your `MainActivity.kt` file, add the following code:

```kotlin
package com.example.libsmu

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle

class MainActivity : AppCompatActivity() {
    private lateinit var libsmu: Libsmu

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        libsmu = Libsmu()

        val sessionPtr = libsmu.createSession()
        libsmu.scan(sessionPtr)
        libsmu.addAll(sessionPtr)

        val devicePtr = libsmu.createDevice(sessionPtr, "serial_number")
        libsmu.setMode(devicePtr, 0, 1) // Set mode to SVMI
        libsmu.write(devicePtr, 0, floatArrayOf(1.0f, 2.0f, 3.0f), false)

        val buffer = FloatArray(1024)
        val samples = libsmu.read(devicePtr, 3, buffer)
        for (i in 0 until samples) {
            println(buffer[i])
        }

        libsmu.run(sessionPtr, 1000)
        libsmu.cancel(sessionPtr)
        libsmu.flush(sessionPtr)
    }
}
```
