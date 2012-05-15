/*
  ==============================================================================

    Convolution.h
    Created: 26 Jan 2012 2:44:02pm
    Author:  jpt

  ==============================================================================
*/

#ifndef __CONVOLUTION_H_5B46FADA__
#define __CONVOLUTION_H_5B46FADA__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ffft/FFTReal.h"

class Convolution 
{
public:
    Convolution() noexcept;
    virtual ~Convolution() noexcept;

    void setImpulse(AudioFormatReader* reader) noexcept;

	void process(AudioSampleBuffer& buffer, int numChannel) noexcept;

private :

    void initData() noexcept;

	AudioSampleBuffer convolIn;
    AudioSampleBuffer overlap;
	ffft::FFTReal<float> fft_object;
    HeapBlock<float> impulse;
	HeapBlock<float> resultat;
	bool impulseLoaded;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Convolution);
};
#endif  // __CONVOLUTION_H_5B46FADA__
