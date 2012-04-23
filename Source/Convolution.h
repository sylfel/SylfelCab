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
    Convolution();
    ~Convolution();

    void setImpulse(File file);

	void process(AudioSampleBuffer& buffer);

private :
	AudioSampleBuffer convolIn;
    AudioSampleBuffer overlap;
    AudioSampleBuffer tempBuffer;
	ffft::FFTReal<float> fft_object;
	float* impulse;
	float* resultat;
	bool impulseLoaded;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Convolution);
};
#endif  // __CONVOLUTION_H_5B46FADA__
