/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINEDITOR_H_712EC9DE__
#define __PLUGINEDITOR_H_712EC9DE__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class SylfelCabAudioProcessorEditor  : public AudioProcessorEditor,
									   public Timer,
                                       public ButtonListener
{
public:
    SylfelCabAudioProcessorEditor (SylfelCabAudioProcessor* ownerFilter);
    ~SylfelCabAudioProcessorEditor();

    //==============================================================================
    // This is just a standard Juce paint method...
    void paint (Graphics& g);
    void resized();
    void timerCallback();
    void buttonClicked (Button * button);
private: 
	ToggleButton bypassBtn;
    Label label;
    TextButton choiceButton;

	SylfelCabAudioProcessor* getProcessor() const
    {
        return static_cast <SylfelCabAudioProcessor*> (getAudioProcessor());
    }
};


#endif  // __PLUGINEDITOR_H_712EC9DE__
