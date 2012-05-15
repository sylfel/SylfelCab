/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SylfelCabAudioProcessor::SylfelCabAudioProcessor()
{
    bypass = 0.0f;
    audioFormatManager.registerBasicFormats();
}

SylfelCabAudioProcessor::~SylfelCabAudioProcessor()
{
}

//==============================================================================
const String SylfelCabAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int SylfelCabAudioProcessor::getNumParameters()
{
    return SylfelCabAudioProcessor::Parameters::totalNumParams;
}

float SylfelCabAudioProcessor::getParameter (int index)
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

void SylfelCabAudioProcessor::setParameter (int index, float newValue)
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

const String SylfelCabAudioProcessor::getParameterName (int index)
{
    return String::empty;
}

const String SylfelCabAudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String SylfelCabAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String SylfelCabAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool SylfelCabAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool SylfelCabAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool SylfelCabAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SylfelCabAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int SylfelCabAudioProcessor::getNumPrograms()
{
    return 0;
}

int SylfelCabAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SylfelCabAudioProcessor::setCurrentProgram (int index)
{
}

const String SylfelCabAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void SylfelCabAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void SylfelCabAudioProcessor::setImpulsePath(File file) 
{
    impulsePath = file.getFileName();
    AudioFormatReader* reader = audioFormatManager.createReaderFor(file);
    convolution.setImpulse(reader);
    delete reader;
}

//==============================================================================
void SylfelCabAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SylfelCabAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void SylfelCabAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    if (bypass == 0.0f) {
        for (int i = 0 ; i < getNumInputChannels(); ++i) {
            convolution.process(buffer, i);
        }
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
bool SylfelCabAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SylfelCabAudioProcessor::createEditor()
{
    return new SylfelCabAudioProcessorEditor (this);
}

//==============================================================================
void SylfelCabAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SylfelCabAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SylfelCabAudioProcessor();
}
