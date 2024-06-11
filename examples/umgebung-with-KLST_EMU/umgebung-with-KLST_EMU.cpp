#include "Arduino.h"
#include "Klangstrom.h"
#include "KlangstromAudioCodec.h"
#include "KlangstromSerialDebug.h"
#include "KlangstromLEDs.h"
#include "Wavetable.h"

void audioblock(float** input_signal, float** output_signal, uint16_t length);

SerialDebug console;
Klangstrom  klangstrom;
AudioCodec  audiocodec;
LEDs        leds;
uint8_t     fLEDIntensity = 0;

float     wavetable[512];
Wavetable oscillator{wavetable, 512, 48000};

void setup() {
    /* init section */
    klangstrom.init();
    console.init();
    console.info();
    console.timestamp();
    console.println("starting init");
    audiocodec.init();
    leds.init(); // TODO interferes with audiocodec

    console.timestamp();
    console.println("finished init");

    /* setup section */
    console.timestamp();
    console.println("starting setup");
    klangstrom.setup();

    Wavetable::fill(wavetable, 512, Wavetable::WAVEFORM_SINE);

    console.timestamp();
    console.println("finished setup");
    console.println("---------------------------------------------------------");

    audiocodec.start();
}

void loop() {
    fLEDIntensity += 10;
    leds.set(0, fLEDIntensity / 255.0);
    leds.toggle(1);
    // console.println("LED 0 : %f", leds.get(0));
    // console.println("LED 1 : %f", leds.get(1));
    oscillator.set_frequency(random(1, 4) * 55);

    klangstrom.loop();
    delay(1000);
}

void audioblock(float** input_signal, float** output_signal, uint16_t length) {
    for (int i = 0; i < length; ++i) {
        output_signal[0][i] = oscillator.process();
        output_signal[1][i] = output_signal[0][i];
    }
}