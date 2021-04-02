## About
Tomahawk is a cross-platform C++14 framework to create any type of application from a unified interface. It provides a set of common tools, so that users can focus on making apps without having to reinvent the wheel.

![CMake](https://github.com/romanpunia/tomahawk/workflows/CMake/badge.svg)

## Features
#### Core
+ Thread-safe event queue (with adjustable workers count)
+ File system
+ Process management
+ Timers
+ Memory management
+ OS functionality
+ String utils
+ Separate any-value serializator
+ Key-value storage documents
+ XML/JSON serialization (plus custom JSONB format)
+ Switchable logging system
+ Adaptable dependency system
+ Fast spinlock mutex without CPU time wasting
+ Switchable thread-safe fixed-size memory pool (malloc, realloc, free)
+ Ref. counting (opt. with new/delete) for ownership management
+ Async/await promise-like object to handle chains of async data
#### Math
+ Vertices
+ Vectors
+ Matrices
+ Quaternions
+ Joints (bones)
+ Animation keys
+ Rigid body physics
+ Soft body physics
+ Constraint physics
+ Physics simulator
+ Regular expressions (custom)
+ Cryptography (MD5, SHA1, SHA256, AES256)
+ Encoding (HYBI10, Base64, URI)
+ Templated math utils
+ Common utils for computations
+ Strong random functions
+ Collision detection
+ Mesh evaluation
+ File preprocessor (include, pragma, define, ifdef/else/endif)
+ Transform hierarchy system
+ SIMD optimisations included
#### Audio
+ Configurable audio playback
+ Positional sound with optional velocity
+ Multiple playback
+ Stream filtering
+ Lowpass filter
+ Bandpass filter
+ Highpass filter
+ Effects system is available
+ Reverb effect
+ Chorus effect
+ Distortion effect
+ Echo effect
+ Flanger effect
+ Frequency-shifter effect
+ Vocal morpher effect
+ Pitch-shifter effect
+ Ring modulator effect
+ Autowah effect
+ Equalizer effect
+ Compressor
#### Network
+ Socket abstraction
+ Async/sync sockets with IO queue
+ SSL/TLS support
+ Socket server and connection abstraction
+ Configurable server router
+ IO multiplexer
+ Socket client abstraction
+ BSON type support for data transfer
+ MongoDB support for database manipulations
+ PostgreSQL support for database manipulations
+ File transfer compression
+ Async/sync HTTP 1.1 server/client support
+ Async/sync WebSocket server support
+ Connection session support
+ Async/sync SMTP client support
+ Polling mechanisms: select, poll, epoll, kqueue, iocp
#### Scripting
+ Angel Script packed into library
+ Template abstraction over virtual machine
+ Support for real threads
+ Compiler with simple preprocessor (defines, include, pragmas)
+ Built-in switchable default interfaces
+ Promise-like async data type support
+ JIT compiler support for non-ARM platforms
+ Module system to add bindings as include files
+ Symbolic imports to import functions from C/C++ libraries directly to script
+ Pointer wrapper to work directly with raw pointers
+ Strings with pointer conversion support to work with C char arrays
+ Debugger interface
+ Standard library
+ Tomahawk bindings (WIP)
#### Graphics
+ Configurable windowing (activity) system
+ Input detection (keyboard, cursor, controller, joystick, multi-touch)
+ Render backend abstraction over DirectX 11 and OpenGL (WIP)
+ Shader materials
+ Structured shading system
+ Standard library for shaders
+ Default and skinned meshes
+ Element instancing for big particle systems
+ Renderer without shaders
+ Simple shader preprocessor for include/pragma directives
+ Switchable render backend
#### GUI
+ Serializable GUI system
+ CSS paradigm-based styling system with overriding support
+ HTML-like GUI declarations
+ Logical nodes for conditional rendering
+ Read/write dynamic properties
+ Various widgets
+ Layouting system
+ Font system
+ Dynamic trees (and recursive)
+ Based on RmlUi
#### Engine
+ Thread-safe scene graph
+ Async/sync content management with processors
+ Entity system
+ Component system
+ Render system to handle any type of visualisation per camera
+ Data serialization
+ Built-in processors for many types of data
+ Built-in components for different simulations
+ Built-in renderers for different visualisations
+ Built-in shader code for every renderer
#### Built-in renderers
+ Skinned, default and soft-body models
+ Particle systems
+ Deferred decals
+ PBR Lighting with shadows
+ PBR Surfaces (aka env. mapping, O(1) recursive reflections included)
+ PBR global illumination (radiance, reflections and ambient occlusion)
+ Bloom for emissive materials
+ Screen-space reflections
+ Screen-space ambient occlusion
+ Screen-space GI (simplified)
+ Depth of field
+ Tone mapping
+ Glitch effect
+ UI rendering
#### Built-in components
+ Rigid body
+ Acceleration (force applier for rigid bodies)
+ Slider constraint (constraints for rigid bodies)
+ Audio source
+ Audio listener
+ Skin animator (for skinned models)
+ Key animator (for any entity)
+ Element system (particle buffer)
+ Element animator (for particle systems)
+ Free-look (rotation with cursor, for cameras originally)
+ Fly (direction-oriented movement with input keys, for cameras originally)
+ Model
+ Skinned model
+ Point light (lamp)
+ Spot light (flashlight)
+ Line light (sun)
+ Probe light (can cast reflections to entities)
+ Camera (with rendering system that holds renderers)
#### Built-in processors
+ Scene graph processor
+ Audio clip processor (WAVE, OGG)
+ Texture 2d processor (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
+ Shader processor (render backend dependent, code preprocessor included)
+ Model processor (with Assimp's import options, if supported)
+ Skinned model processor (with Assimp's import options, if supported)
+ Document processor (XML, JSON, binary)
+ Server processor (for HTTP server to load router config)

*Note: some functionality might be stripped without needed dependencies. Also exceptions were not used, it's more C-like with return codes.*
## Cross platform
+ Windows 7/8/8.1/10+ x64/x86
+ Raspberian 3+ ARM
+ Solaris 9+ x64/x86
+ FreeBSD 11+ x64/x86
+ Ubuntu 16.04+ x64/x86
+ MacOS Catalina 10.15+ x64

## Building
*Tomahawk uses CMake as building system. Because windows doesn't have default include/src folders [Microsoft's Vcpkg](https://github.com/Microsoft/vcpkg) is suggested but not required.*
1. Install [CMake](https://cmake.org/install/).
2. Install dependencies listed below to have all the functionality.
3. Execute CMake command to generate the files or use CMake GUI if you have one.
> cmake [params] [tomahawk's directory]

There are several build options for this project.
+ **TH_INFO** to allow informational logs, defaults to true
+ **TH_WARN** to allow warning logs, defaults to true
+ **TH_ERROR** to allow error logs, defaults to true
+ **TH_RESOURCE** to embed resources from **/lib/shaders** to this project, defaults to true 

## Linking
Tomahawk has support for CMake's install command, to link it with your project you can use CMake as usual.

## Resources
Tomahawk has embedded resources. They are located at **/lib/shaders**. Resources will be packed to **/src/core/shaders.h** and **/src/core/shaders.cpp** at CMake's configuration stage. If you want to disable resource embedding then shaders must not use standard library otherwise error will be raised.

## Core built-in dependencies from **/lib/internal**
These are used widely and presents useful features
* [Bullet Physics](https://github.com/bulletphysics/bullet3)
* [AngelScript](https://sourceforge.net/projects/angelscript/)
* [Wepoll](https://github.com/piscisaureus/wepoll)
* [RmlUi](https://github.com/mikke89/RmlUi)
* [Tiny File Dialogs](https://github.com/native-toolkit/tinyfiledialogs)
* [RapidXML](https://github.com/discordapp/rapidxml)
* [RapidJSON](https://github.com/tencent/rapidjson)
* [STB](https://github.com/nothings/stb)
* [Vector Class](https://github.com/vectorclass/version1)

## Optional dependencies from **/lib/external**
These are recommended to be installed, but are not required to. Every entry has **install.sh** script.
* [OpenSSL](https://github.com/openssl/openssl)
* [GLEW](https://github.com/nigels-com/glew)
* [Zlib](https://github.com/madler/zlib)
* [Assimp](https://github.com/assimp/assimp)
* [MongoC](https://github.com/mongodb/mongo-c-driver)
* [Libpq](https://github.com/postgres/postgres/tree/master/src/interfaces/libpq)
* [OpenAL Soft](https://github.com/kcat/openal-soft)
* [SDL2](https://www.libsdl.org/download-2.0.php)

## Platform dependencies from **/lib/external**
These are resolved automatically.
* [OpenGL](https://github.com/KhronosGroup/OpenGL-Registry)
* [D3D11](https://www.microsoft.com/en-us/download/details.aspx?id=6812)

## License
Tomahawk is licensed under the MIT license

## Known Issues
This project is under development, bugs aren't the rare thing. Also dependency installation process can by quite tricky on some platforms like MacOS. OpenGL is not working properly, sooner or later it will be fully implemented, but it requires too much work for one person. There is about 60 shaders written in HLSL, they must be also implemented in GLSL for OpenGL to work properly. Script interface covers less than 5% of Tomahawk's abilities, bindings are wasting too much time to write, sorry :)