/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleEqAudioProcessorEditor::SimpleEqAudioProcessorEditor (SimpleEqAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    for (auto* comp : getComps() )
    {
        addAndMakeVisible(comp);
    }
    setSize (600, 400);
}

SimpleEqAudioProcessorEditor::~SimpleEqAudioProcessorEditor()
{
}

//==============================================================================
void SimpleEqAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (30.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void SimpleEqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();
    auto responseArea = bounds.removeFromTop(bounds.getHeight()*.33);
    
    auto lowCutArea = bounds.removeFromLeft(bounds.getWidth()*.33);
    auto highCutArea = bounds.removeFromRight(bounds.getWidth()*.5);
    
    lowCutFreqSlider.setBounds(lowCutArea.removeFromTop(lowCutArea.getHeight()*.5));
    lowCutSlopeSlider.setBounds(lowCutArea);
    
    
    highCutFreqSlider.setBounds(highCutArea.removeFromTop(highCutArea.getHeight()*.5));
    highCutSlopeSlider.setBounds(highCutArea); 
    
    
    peakFreqSlider.setBounds(bounds.removeFromTop(bounds.getHeight()*.33));
    peakGainSlider.setBounds(bounds.removeFromTop(bounds.getHeight()*.5));
    peakQualitySlider.setBounds(bounds);
    
}

std::vector<juce::Component*> SimpleEqAudioProcessorEditor::getComps()
{
    return
    {
        &peakFreqSlider,
        &peakGainSlider,
        &peakQualitySlider,
        &lowCutFreqSlider,
        &highCutFreqSlider,
        &lowCutSlopeSlider,
        &highCutSlopeSlider
    };
}
