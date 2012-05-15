/*
  ==============================================================================

    Convolution.cpp
    Created: 26 Jan 2012 2:44:02pm
    Author:  jpt

  ==============================================================================
*/
#include "Convolution.h"

int CST_LENGTH_CONVOL = 4096; //2048, 4096 , 8192 , 16384

Convolution::Convolution() noexcept :
	fft_object(CST_LENGTH_CONVOL),
        convolIn(1,CST_LENGTH_CONVOL),
        overlap(2,CST_LENGTH_CONVOL)
{
	impulseLoaded = false;
    initData();
}

Convolution::~Convolution() noexcept
{
}

void Convolution::initData() noexcept
{
    convolIn.clear();
    overlap.clear();

    impulse.calloc(CST_LENGTH_CONVOL);
    resultat.calloc(CST_LENGTH_CONVOL);
}

void Convolution::setImpulse(AudioFormatReader* reader) noexcept
{
	impulseLoaded = false;
    ScopedPointer<AudioSampleBuffer> buffer = new AudioSampleBuffer(1,CST_LENGTH_CONVOL);
	buffer->clear();
	reader->read(buffer, 0, jmin((int)reader->lengthInSamples, CST_LENGTH_CONVOL / 2), 0 , true, true);
	// do fft to impulse
	fft_object.do_fft(impulse, buffer->getSampleData(0,0));
	impulseLoaded = true;
}

void Convolution::process(AudioSampleBuffer& buffer, int numChannel) noexcept 
{
	if (impulseLoaded) {
		int numSamples = buffer.getNumSamples();
        convolIn.clear();
		convolIn.copyFrom(0,0,buffer,numChannel,0, numSamples);
		// to frequency domain
		fft_object.do_fft (resultat, convolIn.getSampleData(0,0));     // x (real) --FFT---> f (complex)
		
		// do multiplication
        float temp;
        int iImg = CST_LENGTH_CONVOL / 2;
		for (int i = 0 ; i < (CST_LENGTH_CONVOL / 2); i++) {
			//( a,b) * (c,d) = (ac - bd, ad + bc)
            temp = resultat[i] * impulse[i] - resultat[iImg] * impulse[iImg];
			resultat[iImg] = resultat[i] * impulse[iImg] + resultat[iImg] * impulse[i];
            resultat[i] = temp;
            iImg++;
		}

		// to time domain
		fft_object.do_ifft (resultat, convolIn.getSampleData(0,0));    // f (complex) --IFFT--> x (real)
        // Post-scaling should be done after FFT+IFFT
		fft_object.rescale (convolIn.getSampleData(0,0));

        convolIn.addFrom(0,0,overlap,numChannel,0,CST_LENGTH_CONVOL);
        buffer.copyFrom(numChannel,0, convolIn, 0, 0, numSamples);
        
        overlap.copyFrom(numChannel,0,convolIn,0,numSamples,CST_LENGTH_CONVOL - numSamples);
	}
}