// ADSR.h
#ifndef ADSR_H
#define ADSR_H

#include "ModulationEngine.h"

class ADSR : public ModulationSource {
public:
    enum State {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

    ADSR() : state(IDLE), value(0.0f), attackTime(0.1f), decayTime(0.1f), sustainLevel(0.7f), releaseTime(0.5f), sampleRate(44100.0f) {}

    void setAttack(float time) { attackTime = time; }
    void setDecay(float time) { decayTime = time; }
    void setSustain(float level) { sustainLevel = level; }
    void setRelease(float time) { releaseTime = time; }

    void noteOn() {
        state = ATTACK;
    }

    void noteOff() {
        state = RELEASE;
    }

    void setSampleRate(float sr) {
        sampleRate = sr;
    }

    void update(float dt) override {
        switch (state) {
            case ATTACK:
                value += dt / attackTime;
                if (value >= 1.0f) {
                    value = 1.0f;
                    state = DECAY;
                }
                break;
            case DECAY:
                value -= dt / decayTime;
                if (value <= sustainLevel) {
                    value = sustainLevel;
                    state = SUSTAIN;
                }
                break;
            case SUSTAIN:
                // Do nothing, value stays at sustainLevel
                break;
            case RELEASE:
                value -= dt / releaseTime;
                if (value <= 0.0f) {
                    value = 0.0f;
                    state = IDLE;
                }
                break;
            case IDLE:
                break;
        }
    }

    float getValue() const override {
        return value;
    }

private:
    State state;
    float value;
    float attackTime;
    float decayTime;
    float sustainLevel;
    float releaseTime;
    float sampleRate;
};

#endif // ADSR_H
