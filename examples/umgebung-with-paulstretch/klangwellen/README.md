# KlangWellen

*KlangWellen* is a C++ library for processing audio signals. each processor ( or class ) is designed around a central `process` method that either receives and/or emitts samples.

*KlangWellen* is an header-only library which should make it easier to integrate into projects.

## compiling + running tests

to compile and run tests use the CLI as shown below ( example for `klangwellen-to-wav` + only tested on MacOS with Xcode installed ):

```zsh
$ cd test/
$ clang++ -std=c++17 klangwellen-to-wav.cpp -I../include/ -I./ -o klangwellen-to-wav
$ ./klangwellen-to-wav # run test
$ afplay klangwellen-to-wav.wav # optionally playback WAV with afplay
```

or use the provided shellscript or

## `processor()` interface

*KlangWellen* refrains from implementing `process` interfaces with the know C++ techniques[^1]. however, most processors supply a *conceptual* interface for processing signals in one or more of the following `process` methods:

1. [ ] `float process()` :: generate single **mono** signal
2. [ ] `float process(float)‌` :: single **mono** signal
3. [ ] `void process(Signal&)` :: single **stereo** signal
4. [ ] `void process(float*, uint32_t)` :: block of **mono** signals
5. [ ] `void process(float*, float*, uint32_t)` :: block of **stereo** signal

note, that *1. `float process()`* only generates a signal. *‌2. `float process(float)‌`* is encouraged to respect the input signal. however, in some cases ( in stereo signal generators ) it may overwrite the input signal regardless.

developers are encouraged to only implement the variants that make sense for a specific processors. developers are also encouraged to add a comment to the head of a processor marking those `process` methods are available with an `[x]`.

```cpp
/**
 * PROCESSOR INTERFACE
 *
 * - [ ] float process()
 * - [ ] float process(float)‌
 * - [ ] void process(Signal&)
 * - [ ] void process(float*, uint32_t)
 * - [ ] void process(float*, float*, uint32_t)
 */
```

[^1]: i.e *pure virtual functions* ( i.e `virtual float process() = 0;` ) in a base class that are then used in a derived class ( i.e `float process() override {}‌` ). although this is the preferred *object oriented programming* (OOP) approach, my ( possibly faulty ) research has shown that there is a significant ( considering that the process function is called at very high frequencies ) performance decrease when working with virtual methods. this may not be as important on modern *desktop* machine but surely makes a difference on less powerful microcontroller-based platforms which definitely are a targeted platform for this library.
