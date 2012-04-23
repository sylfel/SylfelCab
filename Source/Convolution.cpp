/*
  ==============================================================================

    Convolution.cpp
    Created: 26 Jan 2012 2:44:02pm
    Author:  jpt

  ==============================================================================
*/
#include "Convolution.h"

int CST_LENGTH_CONVOL = 16384; //4096 , 8192 , 16384

Convolution::Convolution() :
	fft_object(CST_LENGTH_CONVOL),
        convolIn(1,CST_LENGTH_CONVOL),
        overlap(1,CST_LENGTH_CONVOL),
        tempBuffer(1,CST_LENGTH_CONVOL)
{
	impulseLoaded = false;
	
	convolIn.clear();
    overlap.clear();

	impulse = new float[CST_LENGTH_CONVOL];
	for (int i = 0; i < CST_LENGTH_CONVOL; i++) {
		impulse[i] = 0;
	}
	
	resultat = new float[CST_LENGTH_CONVOL];
	for (int i = 0; i < CST_LENGTH_CONVOL; i++) {
		resultat[i] = 0;
	}
	
}

Convolution::~Convolution()
{
	deleteAndZero(impulse);
	deleteAndZero(resultat);
}

void Convolution::setImpulse(File file)
{
	impulseLoaded = false;
	AudioFormatReader* reader = nullptr;
	AudioSampleBuffer* buffer = nullptr;
	try {
		// Read wav file
		WavAudioFormat wavAudioFormat;
		AudioFormatReader* reader = wavAudioFormat.createReaderFor(file.createInputStream(), true);
		
		// Extract sample (maximum CST_LENGTH_CONVOL / 2 samples)
		buffer = new AudioSampleBuffer(1,CST_LENGTH_CONVOL);
		buffer->clear();
		reader->read(buffer, 0, jmin((int)reader->lengthInSamples, CST_LENGTH_CONVOL / 2), 0 , true, true);
		// do fft to impulse
		fft_object.do_fft(impulse, buffer->getSampleData(0,0));
		impulseLoaded = true;
	} catch (...) {

	}

	if (reader != nullptr) {
		deleteAndZero(reader);
	}
	if (buffer != nullptr) {
		deleteAndZero(buffer);
	}
}

void Convolution::process(AudioSampleBuffer& buffer) 
{
	if (impulseLoaded) {
		int numSamples = buffer.getNumSamples();
        convolIn.clear();
		convolIn.copyFrom(0,0,buffer,0,0, numSamples);
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

        convolIn.addFrom(0,0,overlap,0,0,CST_LENGTH_CONVOL);
        buffer.copyFrom(0,0, convolIn, 0, 0, numSamples);
        
        // buffer.addFrom(0,0,overlap,0,0,numSamples);

        overlap.copyFrom(0,0,convolIn,0,numSamples,CST_LENGTH_CONVOL - numSamples);
	}
}