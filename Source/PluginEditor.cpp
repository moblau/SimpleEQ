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
    : AudioProcessorEditor (&p), processor (p),
    peakFreqSliderAttachment(processor.apvts, "Peak Freq", peakFreqSlider ),
    peakGainSliderAttachment(processor.apvts, "Peak Gain", peakGainSlider ),
    peakQualitySliderAttachment(processor.apvts, "Peak Quality", peakQualitySlider ),
    lowCutFreqSliderAttachment(processor.apvts, "LowCut Freq", lowCutFreqSlider ),
    highCutFreqSliderAttachment(processor.apvts, "HighCut Freq", highCutFreqSlider ),
    lowCutSlopeSliderAttachment(processor.apvts, "LowCut Slope", lowCutSlopeSlider ),
    highCutSlopeSliderAttachment(processor.apvts, "HighCut Slope", highCutSlopeSlider )

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
    
    using namespace juce;
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);

    auto bounds = getLocalBounds();
    auto responseArea = bounds.removeFromTop(bounds.getHeight()*.33);
    
    auto w = responseArea.getWidth();
    
    auto & lowCut = monoChain.get<ChainPositions::LowCut>();
    auto & peak = monoChain.get<ChainPositions::Peak>();
    auto & highCut = monoChain.get<ChainPositions::HighCut>();
    
    auto sampleRate = processor.getSampleRate();
    
    std::vector<double> mags;
    
    mags.resize(w);
    for ( int i = 0; i < w; ++i){
        double mag = 1.f;
        auto freq = mapToLog10(double(i)/double(w), 20.0,20000.0);
        
        if ( !lowCut.isBypassed<0>()  )
        {
            mag*= peak.coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !lowCut.isBypassed<1>()  )
        {
            mag*= peak.coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !lowCut.isBypassed<2>()  )
        {
            mag*= peak.coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !lowCut.isBypassed<3>()  )
        {
            mag*= peak.coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        
        if ( !highCut.isBypassed<0>()  )
        {
            mag*= peak.coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !highCut.isBypassed<1>()  )
        {
            mag*= peak.coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !highCut.isBypassed<2>()  )
        {
            mag*= peak.coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !highCut.isBypassed<3>()  )
        {
            mag*= peak.coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( monoChain.isBypassed<ChainPositions::Peak>() )
        {
            mag*= peak.coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        mags[i] = Decibels::gainToDecibels(mag);
    }
    
    Path responseCurve;
    
    const double outputMin = responseArea.getBottom();
    const double outputMax = responseArea.getY();
    auto map = [outputMin,outputMax](double input)
    {
        return jmap(input, -24.0, 24.0,outputMin,outputMax);
    };
    
    responseCurve.startNewSubPath(responseArea.getX(), map(mags.front() ));
    
    for ( size_t i = 1; i < mags.size(); ++i )
    {
        responseCurve.lineTo(responseArea.getX()+i, map(mags[i]));
    }
    
    g.setColour(Colours::orange);
    g.drawRoundedRectangle(responseArea.toFloat(),4.f,1.f);
    g.setColour(Colours::white);
    g.strokePath(responseCurve,PathStrokeType(2.f)); 
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


void SimpleEqAudioProcessorEditor::parameterValueChanged(int parameterIndex, float newValue)
{
    parametersChanged.set(true);
}

void SimpleEqAudioProcessorEditor::timerCallback()
{
    if ( parametersChanged.compareAndSetBool(false,true)){
        //update monochain
        //signal repaint
    }
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
