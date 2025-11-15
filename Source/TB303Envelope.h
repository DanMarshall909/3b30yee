#pragma once

class TB303Envelope
{
public:
    TB303Envelope()
    {
        reset();
    }

    void setSampleRate(double newSampleRate)
    {
        sampleRate = newSampleRate;
    }

    void setDecay(float decaySeconds)
    {
        decay = decaySeconds;
    }

    void setAccent(float accentAmount)
    {
        accent = accentAmount;
    }

    void noteOn(bool isAccented)
    {
        isActive = true;
        currentAccent = isAccented ? accent : 0.0f;
        envelope = 1.0f + currentAccent;
    }

    void noteOff()
    {
        isActive = false;
    }

    void reset()
    {
        envelope = 0.0f;
        isActive = false;
        currentAccent = 0.0f;
    }

    float getNextSample()
    {
        if (isActive && envelope > 0.0f)
        {
            // Exponential decay
            float decayRate = 1.0f - (1.0f / (decay * sampleRate));
            envelope *= decayRate;

            if (envelope < 0.001f)
            {
                envelope = 0.0f;
            }
        }
        else if (!isActive)
        {
            envelope = 0.0f;
        }

        return envelope;
    }

    float getCurrentValue() const
    {
        return envelope;
    }

private:
    double sampleRate = 44100.0;
    float decay = 0.3f;          // Decay time in seconds
    float accent = 0.5f;         // Accent amount (0-1)
    float envelope = 0.0f;
    float currentAccent = 0.0f;
    bool isActive = false;
};
