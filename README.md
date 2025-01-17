[![coverity status](https://scan.coverity.com/projects/analogdevicesinc-libsmu/badge.svg)](https://scan.coverity.com/projects/analogdevicesinc-libsmu)

# libsmu

libsmu contains abstractions for streaming data to and from USB-connected
analog interface devices, currently supporting the Analog Devices' ADALM1000.
Building off of LibUSB for cross-platform operation, it offers the sourcing of
repeated waveforms, configuration of hardware, and measuring of signals.

Python bindings are also provided in the form of the pysmu module. See
instructions below for how to build them.

# Simple installation
## Conda packages
Conda and Anaconda are cross-platform package-management tools that generally focus around python but can support any language or package generally.
You can find documentation on the libsmu conda package [here](https://wiki.analog.com/university/tools/conda?s[]=libsmu) .

## Linux
Download the specific libsmu .deb package for your Linux distribution from the Releases section. Currently we are supporting Ubuntu 16, 18 and 20. The package name should start with libsmu and contain the OS version.
Go to the folder you downloaded the package in and open a terminal, then run the following command:
```shell
analog@analog:~$ sudo apt install -f ./<libsmu_package_name>.deb
```


## MacOS
Download the specific libsmu .pkg package for your MacOS distribution from the Releases section. Currently we are supporting MacOS 10.15, 11.0. The package name should start with libsmu and contain the MacOS version.

Open a terminal and run the following command which will install only the base library in /Library/Frameworks.
```shell
analog@analog:~$ sudo installer -pkg /path/to/<libsmu_package_name>.pkg -target /
```

A different way to install libsmu on MacOS is by using the .tar.gz located in the Releases section. This will install the .dylib (libraries) into system paths (usr/local) and it will also install the smu CLI.
```shell
tar -xzvf <libsmu_package_name>.tar.gz --strip=3 -C /usr/local
```
Based on this base library installation, you can install the Python bindings manually for the desired Python version. Check the **Python** section below or you can use the wheel packages provided in the official installer. Check out the next section:

## Python bindings for Windows, Linux and MacOS
If you want to install pysmu (the Python bindings for libsmu), you can download the specific wheel for your version.
We provide python wheel packages for the following Python versions: py3.7, py3.8, py3.9, py3.10. You can download the .whl
for your Python version from the official releases or use the ones provided on test.pypi.org (soon from the official pypi.org as well).
On Linux:
```shell
# If you are installing from test.pypi.org:
analog@analog:~$ python3 -m pip install --index-url https://test.pypi.org/simple/ pysmu
# If you are installing the .whl downloaded from our official github release:
analog@analog:~$ python3 -m pip install pysmu-1.x.y-cp3x-cp3x-manylinux_2_24_x86_64.whl
```
On MacOS:
```shell
# If you are installing from test.pypi.org:
analog@analog:~$ python3 -m pip install --index-url https://test.pypi.org/simple/ pysmu
# If you are installing the .whl downloaded from our official github release:
analog@analog:~$ python3 -m pip install pysmu-1.x.y-cp3x-cp3x-macos_10_15_x86_64.whl
```
Please note that in order to use these bindings you need the core C++ library they depend upon. This is not packaged with the pypi release but you can install the [latest release](https://github.com/analogdevicesinc/libsmu/releases/latest) or the latest **untested** binaries from the [master branch](https://ci.appveyor.com/project/analogdevicesinc/libsmu).


# Build instructions for libsmu on Linux
## Install Dependencies
Install prerequisites
```shell
analog@analog:~$ sudo apt-get update
analog@analog:~$ sudo apt-get install libusb-1.0-0-dev libboost-all-dev cmake pkg-config
```
Install to build Python bindings
```shell
analog@analog:~$ sudo apt-get install python3 python3-setuptools python3-pip
analog@analog:~$ python3 -m pip install --upgrade pip
analog@analog:~$ sudo python3 -m pip install cython setuptools build wheel
```
Install to build documentation
```shell
analog@analog:~$ sudo apt-get install doxygen
```

# Build instructions for libsmu on MacOS
## Install Dependencies
Install prerequisites
```shell
analog@analog:~$ brew update
analog@analog:~$ brew install libusb cmake pkg-config boost
(optional) analog@analog:~$ brew link --overwrite boost
```
Install to build Python bindings
```shell
analog@analog:~$ brew install python3
analog@analog:~$ python3 -m pip install --upgrade pip
analog@analog:~$ python3 -m pip install cython setuptools build wheel
```
Setup tools and pip should be included in the "python3" package.

Install to build documentation
```shell
analog@analog:~$ brew install doxygen
```

# Clone, configure and build
```shell
analog@analog:~$ git clone https://github.com/analogdevicesinc/libsmu.git
analog@analog:~$ cd libsmu 
```
Options:
CMake Options       | Default | Description                                    |
------------------- | ------- | ---------------------------------------------- |
`BUILD_CLI`   | ON | Build command line smu application                                |
`BUILD_PYTHON`   | ON | Build python bindings                            |
`WITH_DOC`          | OFF | Generate documentation with Doxygen and Sphinx     |
`BUILD_EXAMPLES`        |  OFF | Build examples                            |
`INSTALL_UDEV_RULES` |  ON | Install a udev rule for detection of USB devices   |
`BUILD_JNI`          | OFF | Build JNI support                                 |

Configure via cmake:
```shell
analog@analog:~$ mkdir build && cd build
analog@analog:~$ cmake .. -DBUILD_PYTHON=ON -DBUILD_JNI=ON
```

Compile:
```shell
analog@analog:~$ make
```

# Install
If **-DBUILD_PYTHON=ON** (from the above options) is specified, this step will also install the Python Bindings using the Python version detected at the CMake configuration step.
```shell
analog@analog:~$ sudo make install
```

# Docs

Doxygen-based documentation is available at
https://analogdevicesinc.github.io/libsmu/.

This can also be built locally if enabled using the CMake option mentioned above.
After make is run, the generated documentation files can then be found in the
html subdir of the build directory.

# Testing

The [Google Test framework](https://github.com/google/googletest) is used to
run various streaming tests. Make sure it's installed on the host system and then use the
following to build and run tests:

```shell
analog@analog:~$ cmake -DBUILD_TESTS=ON ..
analog@analog:~$ make check
```

Note that at least one device should be inserted to the system for the checks
to run properly.

# Python

Python Bindings are enabled by default and can be disabled using the CMake option mentioned above.

Note that this will build only one version of Python for the first supported
implementation it finds installed on the system. To build them for other
versions it's easiest to build them manually via the setup.py script in the
regular python manner if libsmu has already been built and/or installed on the
host machine.

```shell
analog@analog:~$ git clone https://github.com/analogdevicesinc/libsmu.git
analog@analog:~$ cd libsmu/build
analog@analog:~$ cmake -DBUILD_PYTHON=ON .. #this will generate a setup.py file in the current directory
analog@analog:~$ python3 -m build #this will create .whl files in a dist/ directory
# the .whl files can then be installed using "pip install"
```

# JNI

JNI support can be enabled using the CMake option mentioned above.

To build the JNI library, follow the same steps as for building the core library, but add the `-DBUILD_JNI=ON` option to the CMake configuration step.

```shell
analog@analog:~$ git clone https://github.com/analogdevicesinc/libsmu.git
analog@analog:~$ cd libsmu
analog@analog:~$ mkdir build && cd build
analog@analog:~$ cmake .. -DBUILD_JNI=ON
analog@analog:~$ make
analog@analog:~$ sudo make install
```

# Using JNI

To use the JNI library, you need to include the JNI headers and link against the JNI library.

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

# Example targeting a standard Kotlin app with modern Gradle

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

# Linux FAQ

By default, libsmu is installed into various directories inside /usr/local which
implies that the runtime linker cache often needs to be regenerated,
 otherwise runtime linking errors will occur.

Regenerate runtime linker cache after install:
```shell
analog@analog:~$ sudo ldconfig
```

If the same errors still happen, make sure the directory the libsmu library is
installed to is in the sourced files for /etc/ld.so.conf before running
ldconfig.

In addition, the udev rules file (53-adi-m1k-usb.rules) is installed by default
to give regular users access to devices supported by libsmu. Udev has to be
forced to reload its rules files in order to use the new file without rebooting
the system.

Reload udev rules files:
```shell
analog@analog:~$ sudo udevadm control --reload-rules
```

Finally, for python support on Debian/Ubuntu derived distros users will have to
export PYTHONPATH or perform a similar method since hand-built modules are
installed to the site-packages directory (which isn't in the standard search
list) while distro provided modules are placed in dist-packages.

Add pysmu module directory to python search path:
```
$ export PYTHONPATH=/usr/local/lib/python3.7/site-packages:${PYTHONPATH}
```

Note the command will have to be altered for targets with different bitness or
python versions.


# Windows

On Windows, it's easiest to use the provided installers,
[libsmu-setup-x86.exe](https://github.com/analogdevicesinc/libsmu/releases/latest) and
[libsmu-setup-x64.exe](https://github.com/analogdevicesinc/libsmu/releases/latest)
that install either 32 or 64 bit support, respectively. During the
install process options are provided to install drivers and Visual Studio
development support.

Note that after v1.0.3, libsmu Windows installers no longer provide the option to install
Python bindings. The process for installing pysmu on Windows is similar to the one described above for 
MacOS and Linux.

# Building and Running libsmu on Android

## Dependencies

To build the libsmu driver on Android, you will need the following dependencies:

- Android NDK (Native Development Kit)
- CMake
- Ninja (optional, but recommended for faster builds)

## Building libsmu on Android

1. Download and install the Android NDK from the official Android developer website.
2. Set up the Android NDK environment variables:
   ```shell
   export ANDROID_NDK_HOME=/path/to/android-ndk
   export PATH=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
   ```
3. Create a build directory and navigate to it:
   ```shell
   mkdir build-android
   cd build-android
   ```
4. Configure the build using CMake:
   ```shell
   cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
         -DANDROID_ABI=arm64-v8a \
         -DANDROID_PLATFORM=android-21 \
         -DCMAKE_BUILD_TYPE=Release \
         -GNinja ..
   ```
5. Build the libsmu driver:
   ```shell
   ninja
   ```

# Porting Pixelpulse2 to a Multiplatform App Targeting Mobiles

## Dependencies

To build Pixelpulse2 on Android, you will need the following dependencies:

- Android NDK (Native Development Kit)
- CMake
- Ninja (optional, but recommended for faster builds)
- Qt for Android

## Building Pixelpulse2 on Android

1. Download and install the Android NDK from the official Android developer website.
2. Download and install Qt for Android from the official Qt website.
3. Set up the Android NDK environment variables:
   ```shell
   export ANDROID_NDK_HOME=/path/to/android-ndk
   export PATH=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
   ```
4. Create a build directory and navigate to it:
   ```shell
   mkdir build-android
   cd build-android
   ```
5. Configure the build using CMake:
   ```shell
   cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
         -DANDROID_ABI=arm64-v8a \
         -DANDROID_PLATFORM=android-21 \
         -DCMAKE_BUILD_TYPE=Release \
         -GNinja ..
   ```
6. Build Pixelpulse2:
   ```shell
   ninja
   ```
