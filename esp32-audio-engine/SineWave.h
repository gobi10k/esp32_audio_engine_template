// SineWave.h
#ifndef SINE_WAVE_H
#define SINE_WAVE_H

#include "AudioEngine.h"
#include "SmoothedParameter.h"
#include <cmath>

class SineWave : public AudioSource {
public:
  SineWave(float frequency = 440.0f, float amplitude = 0.5f)
      : frequency(frequency, 0.02f), amplitude(amplitude, 0.02f), 
        phase(0.0f), sampleRate(44100.0f) {
  }

  void setSampleRate(float sr) override {
    sampleRate = sr;
    frequency.setSampleRate(sr);
    amplitude.setSampleRate(sr);
  }

  void renderBlock(float* buffer, size_t blockSize) override {
    for (size_t i = 0; i < blockSize; i++) {
      phase += 2.0f * M_PI * frequency.next() / sampleRate;
      if (phase >= 2.0f * M_PI) {
        phase -= 2.0f * M_PI;
      }
      buffer[i] = sinf(phase) * amplitude.next();
    }
  }

  void setFrequency(float freq, bool immediate = false) {
    if (immediate) {
      frequency.setTargetImmediate(freq);
    } else {
      frequency.setTarget(freq);
    }
  }

  void setAmplitude(float amp, bool immediate = false) {
    if (immediate) {
      amplitude.setTargetImmediate(amp);
    } else {
      amplitude.setTarget(amp);
    }
  }

  float* getAmplitudePtr() { return amplitude.getPtr(); }

private:
  SmoothedParameter frequency;
  SmoothedParameter amplitude;
  float phase;
  float sampleRate;
};

#endif