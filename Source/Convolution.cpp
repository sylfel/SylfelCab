/*
  ==============================================================================

    Convolution.cpp
    Created: 26 Jan 2012 2:44:02pm
    Author:  jpt

  ==============================================================================
*/
#include "Convolution.h"

Convolution::Convolution() noexcept
{
	impulseLoaded = false;
}

Convolution::~Convolution() noexcept
{
}

void Convolution::initData(int numberSample) noexcept
{
    if (numberSample < 500) {
        lengthConvol = 2048;
    } else {
        lengthConvol = 4096;
    }

    fft_object = new ffft::FFTReal<float>(lengthConvol);
    convolIn = new AudioSampleBuffer(1,lengthConvol);
    overlap = new AudioSampleBuffer(2,lengthConvol);

    convolIn->clear();
    overlap->clear();

    impulse.calloc(lengthConvol);
    resultat.calloc(lengthConvol);
}

void Convolution::releaseData() noexcept
{
    impulseLoaded = false;
    impulse.free();
    resultat.free();
}

void Convolution::setImpulse(AudioFormatReader* reader) noexcept
{
	impulseLoaded = false;
    ScopedPointer<AudioSampleBuffer> buffer = new AudioSampleBuffer(1,lengthConvol);
	buffer->clear();
	reader->read(buffer, 0, jmin((long)reader->lengthInSamples, lengthConvol / 2), 0 , true, true);
	// do fft to impulse
	fft_object->do_fft(impulse, buffer->getSampleData(0,0));
	impulseLoaded = true;
}

void Convolution::process(AudioSampleBuffer& buffer, int numChannel) noexcept 
{
	if (impulseLoaded) {
		int numSamples = buffer.getNumSamples();
        convolIn->clear();
		convolIn->copyFrom(0,0,buffer,numChannel,0, numSamples);
		// to frequency domain
		fft_object->do_fft (resultat, convolIn->getSampleData(0,0));     // x (real) --FFT---> f (complex)
		
		// do multiplication
        float temp;
        int iImg = lengthConvol / 2;
		for (int i = 0 ; i < (lengthConvol / 2); i++) {
			//( a,b) * (c,d) = (ac - bd, ad + bc)
            temp = resultat[i] * impulse[i] - resultat[iImg] * impulse[iImg];
			resultat[iImg] = resultat[i] * impulse[iImg] + resultat[iImg] * impulse[i];
            resultat[i] = temp;
            iImg++;
		}

		// to time domain
		fft_object->do_ifft (resultat, convolIn->getSampleData(0,0));    // f (complex) --IFFT--> x (real)
        // Post-scaling should be done after FFT+IFFT
		fft_object->rescale (convolIn->getSampleData(0,0));

        convolIn->addFrom(0,0,*overlap,numChannel,0,lengthConvol);
        buffer.copyFrom(numChannel,0, *convolIn, 0, 0, numSamples);
        
        overlap->copyFrom(numChannel,0,*convolIn,0,numSamples,lengthConvol - numSamples);
	}
}