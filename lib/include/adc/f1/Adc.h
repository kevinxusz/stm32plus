/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {


  /**
   * Non-template base class for the ADC peripheral
   */

  class Adc {

    protected:
     ADC_TypeDef *_peripheralAddress;
     ADC_InitTypeDef _init;
     Adc *_master;
     uint8_t _injectedChannelCount;
     bool _calibrated;

     static uint8_t _regularChannelRank[3];        // we can have multiple channel feature instances and multiple ADCs
     static uint8_t _injectedChannelRank[3];

    public:
      Adc(ADC_TypeDef *peripheralAddress,Adc *master);

      void enablePeripheral() const;
      void disablePeripheral() const;

      void calibrate() const;

      // get and increment the rank for an ADC

      uint8_t getAndIncrementRegularChannelRank(uint8_t adcNumber);
      uint8_t getAndIncrementInjectedChannelRank(uint8_t adcNumber);

      void incrementInjectedChannelCount(uint8_t amountToAdd);

      // regular conversion data access functions

      void startRegularConversion() const;
      bool hasRegularConversionStarted() const;
      bool hasRegularConversionFinished() const;
      uint16_t getRegularConversionValue() const;

      // cast operators

      operator ADC_TypeDef *();
      operator ADC_InitTypeDef *();
  };


  /**
   * Constructor
   * @param peripheralAddress The peripheral address
   */

  inline Adc::Adc(ADC_TypeDef *peripheralAddress,Adc *master)
    : _peripheralAddress(peripheralAddress) {

    // store the master ADC if this is dual mode or nullptr if in single

    _master=master;

    // not calibrated yet

    _calibrated=false;

    // initialise the ranks back to 1 so that the channel features are ready

    _regularChannelRank[0]=_regularChannelRank[1]=_regularChannelRank[2]=1;
    _injectedChannelRank[0]=_injectedChannelRank[1]=_injectedChannelRank[2]=1;
    _injectedChannelCount=0;

    // set up the default init values
    // the features can customise this before the AdcPeripheral class uses

    ADC_StructInit(&_init);

    _init.ADC_NbrOfChannel=0;      // nothing yet - the features will increment this as required
    _init.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;  // software trigger (features override this)
  }


  /*
   * (re-)calibrate the ADC. This happens automatically on the first call to enablePeripheral()
   */

  inline void Adc::calibrate() const {

    // reset calibration - this internal operation can take a few cycles

    ADC_ResetCalibration(_peripheralAddress);
    while(ADC_GetResetCalibrationStatus(_peripheralAddress));

    // start the calibration and wait until it's done
    ADC_StartCalibration(_peripheralAddress);
    while(ADC_GetCalibrationStatus(_peripheralAddress));
  }


  /**
   * Get the rank (order) number for a regular channel
   * @param adcNumber The ADC number (1..3)
   * @return the current rank number
   */

  inline uint8_t Adc::getAndIncrementRegularChannelRank(uint8_t adcNumber) {
    return _regularChannelRank[adcNumber-1]++;
  }


  /**
   * Get the rank (order) number for an injected channel
   * @param adcNumber The ADC number (1..3)
   * @return the current rank number
   */

  inline uint8_t Adc::getAndIncrementInjectedChannelRank(uint8_t adcNumber) {
    return _injectedChannelRank[adcNumber-1]++;
  }


  /**
   * Return the ADC peripheral address
   */

  inline Adc::operator ADC_TypeDef *() {
    return _peripheralAddress;
  }


  /**
   * Return the ADC init structure
   */

  inline Adc::operator ADC_InitTypeDef *() {
    return &_init;
  }


  /**
   * Enable the ADC
   */

  inline void Adc::enablePeripheral() const {

    ADC_Cmd(_peripheralAddress,ENABLE);

    if(!_calibrated)
      calibrate();
  }


  /**
   * Disable the ADC
   */

  inline void Adc::disablePeripheral() const {
    ADC_Cmd(_peripheralAddress,DISABLE);
  }


  /**
   * Start the conversion by software command. This will set SWSTART in CR2.
   */

  inline void Adc::startRegularConversion() const {
    ADC_SoftwareStartConvCmd(_peripheralAddress,ENABLE);
  }


  /**
   * Return true if a software conversion has started. SWSTART in CR2 is cleared
   * by the MCU when the conversion starts
   * @return true if the conversion has started.
   */

  inline bool Adc::hasRegularConversionStarted() const {
    return ADC_GetSoftwareStartConvStatus(_peripheralAddress)==RESET;
  }


  /**
   * Return true if a conversion has finished. Returns the status of the EOC flag
   * @return true if the conversion has finished.
   */

  inline bool Adc::hasRegularConversionFinished() const {
    return ADC_GetFlagStatus(_peripheralAddress,ADC_FLAG_EOC)==SET;
  }


  /**
   * Get the result of a regular conversion
   * @return The conversion result
   */

  inline uint16_t Adc::getRegularConversionValue() const {
    return ADC_GetConversionValue(_peripheralAddress);
  }


  /**
   * Increase the number of injected channels
   */

  inline void Adc::incrementInjectedChannelCount(uint8_t amountToAdd) {
    _injectedChannelCount+=amountToAdd;
  }
}
