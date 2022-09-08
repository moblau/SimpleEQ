/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

ResponseCurveComponent::ResponseCurveComponent(SimpleEqAudioProcessor& p) : processor(p)
{
    const auto& params = processor.getParameters();
    for ( auto param: params)
    {
        param->addListener(this);
    }
    
    startTimerHz(60);
}

ResponseCurveComponent::~ResponseCurveComponent()
{
    const auto& params = processor.getParameters();
    for ( auto param: params)
    {
        param->removeListener(this);
    }
}

void ResponseCurveComponent::parameterValueChanged(int parameterIndex, float newValue)
{
    parametersChanged.set(true);
}

void ResponseCurveComponent::timerCallback()
{
    if ( parametersChanged.compareAndSetBool(false,true)){
        //update monochain
        auto chainSettings = getChainSettings(processor.apvts);
        auto peakCoefficients = makePeakFilter(chainSettings,processor.getSampleRate());
        updateCoefficients(monoChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
        
        auto lowCutCoefficients = makeLowCutFilter(chainSettings,processor.getSampleRate());
        auto highCutCoefficients = makeHighCutFilter(chainSettings,processor.getSampleRate());
        
        updateCutFilter(monoChain.get<ChainPositions::LowCut>(), lowCutCoefficients, chainSettings.lowCutSlope);
        updateCutFilter(monoChain.get<ChainPositions::HighCut>(), highCutCoefficients, chainSettings.highCutSlope);
        repaint();
        
        //signal repaint
    }
}

void ResponseCurveComponent::paint (Graphics& g)
{
    
    using namespace juce;
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
    
    auto responseArea = getLocalBounds();
    
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
        
        if ( !monoChain.isBypassed<ChainPositions::Peak>() )
        {
            mag*= peak.coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !lowCut.isBypassed<0>()  )
        {
            mag*= lowCut.get<0>().coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !lowCut.isBypassed<1>()  )
        {
            mag*= lowCut.get<1>().coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !lowCut.isBypassed<2>()  )
        {
            mag*= lowCut.get<2>().coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !lowCut.isBypassed<3>()  )
        {
            mag*= lowCut.get<3>().coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        
        if ( !highCut.isBypassed<0>()  )
        {
            mag*= highCut.get<0>().coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !highCut.isBypassed<1>()  )
        {
            mag*= highCut.get<1>().coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !highCut.isBypassed<2>()  )
        {
            mag*= highCut.get<2>().coefficients->getMagnitudeForFrequency(freq,sampleRate);
        }
        if ( !highCut.isBypassed<3>()  )
        {
            mag*= highCut.get<3>().coefficients->getMagnitudeForFrequency(freq,sampleRate);
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

//==============================================================================
SimpleEqAudioProcessorEditor::SimpleEqAudioProcessorEditor (SimpleEqAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
    responseCurveComponent(processor),
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
}

void SimpleEqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();
    auto responseArea = bounds.removeFromTop(bounds.getHeight()*.33);
    
    responseCurveComponent.setBounds(responseArea);
    
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
        &highCutSlopeSlider,
        &responseCurveComponent
    };
}
