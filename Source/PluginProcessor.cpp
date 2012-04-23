/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Tgconvol2AudioProcessor::Tgconvol2AudioProcessor()
{
    bypass = 0.0f;
    impulsePath = "C:/dev/perso/TGConvol/Impulse/goc.wav";
    convolution.setImpulse(impulsePath);
}

Tgconvol2AudioProcessor::~Tgconvol2AudioProcessor()
{
}

//==============================================================================
const String Tgconvol2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int Tgconvol2AudioProcessor::getNumParameters()
{
    return Parameters::totalNumParams;
}

float Tgconvol2AudioProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case bypassParam:    return bypass;
        default:            return 0.0f;
    }
}

void Tgconvol2AudioProcessor::setParameter (int index, float newValue)
{
	// This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case bypassParam:    bypass = newValue;  break;
        default:            break;
    }
}

const String Tgconvol2AudioProcessor::getParameterName (int index)
{
    return String::empty;
}

const String Tgconvol2AudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String Tgconvol2AudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String Tgconvol2AudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool Tgconvol2AudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool Tgconvol2AudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool Tgconvol2AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Tgconvol2AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int Tgconvol2AudioProcessor::getNumPrograms()
{
    return 0;
}

int Tgconvol2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Tgconvol2AudioProcessor::setCurrentProgram (int index)
{
}

const String Tgconvol2AudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void Tgconvol2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

void Tgconvol2AudioProcessor::setImpulsePath(File file) 
{
    impulsePath = file.getFileName();
    convolution.setImpulse(file);
}

//==============================================================================
void Tgconvol2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Tgconvol2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void Tgconvol2AudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    if (bypass == 0.0f) {
        convolution.process(buffer);
    }

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool Tgconvol2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Tgconvol2AudioProcessor::createEditor()
{
    return new Tgconvol2AudioProcessorEditor (this);
}

//==============================================================================
void Tgconvol2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Tgconvol2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Tgconvol2AudioProcessor();
}
