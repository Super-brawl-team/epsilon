#ifndef ONLINE_DATA_H
#define ONLINE_DATA_H

#include <apps/i18n.h>
#include <assert.h>

#include <new>

namespace Online {

class OnlineData {
  public:
  constexpr static uint8_t k_numberOfSharedDoubleValues = 4;
  constexpr static uint8_t k_maxNumberOfUnknowns =
      5;  // static_cast<uint8_t>(CompoundOnlineData::Parameter::PY)

  OnlineData(double* sharedValues) : m_sharedValues(sharedValues) {}

  virtual I18n::Message labelForParameter(uint8_t param) const = 0;
  virtual I18n::Message sublabelForParameter(uint8_t param) const = 0;
  virtual I18n::Message menuTitle() const = 0;
  virtual double defaultValue(uint8_t param) const;
  virtual bool checkValue(uint8_t param, double value) const = 0;
  virtual uint8_t numberOfParameters() const = 0;
  virtual uint8_t numberOfDoubleValues() const = 0;
  virtual uint8_t numberOfUnknowns() const = 0;
  virtual I18n::Message dropdownMessageAtIndex(int index) const = 0;
  virtual double computeUnknownValue() = 0;
  virtual void setValue(uint8_t param, double value) {
    assert(param < k_numberOfSharedDoubleValues);
    m_sharedValues[param] = value;
  }
  virtual double getValue(uint8_t param) const {
    assert(param < k_numberOfSharedDoubleValues);
    return m_sharedValues[param];
  }

  void setUnknown(uint8_t param) {
    assert(m_unknown < numberOfUnknowns());
    m_unknown = param;
  }
  uint8_t getUnknown() const { return m_unknown; }
  void resetValues();
  /* For SimpleOnlineData, this param tells if the convention for the number
   * of days per year is 360 (365 otherwise). For CompoundOnlineData, it tells
   * if the payment is made at the beginning of the payment period (end
   * otherwise). */
  bool m_booleanParam;

 protected:
  uint8_t m_unknown;

 private:
  double* m_sharedValues;
};

class SimpleOnlineData : public OnlineData {
 private:
  enum class Parameter : uint8_t {
    n = 0,          // Shared value with CompoundOnlineData::N
    rPct,           // Shared value with CompoundOnlineData::rPct
    P,              // Shared value with CompoundOnlineData::PV
    Int,            // Shared value with CompoundOnlineData::FV
    YearConvention  // Is not a double parameter and can't be unknown
  };

 public:
  SimpleOnlineData(double* sharedValues) : OnlineData(sharedValues) {
    resetValues();
  }

  constexpr static uint8_t k_numberOfParameters = 5;
  static_assert(static_cast<uint8_t>(Parameter::n) == 0,
                "n must be first to match CompoundOnlineData::N.");
  static_assert(static_cast<uint8_t>(Parameter::rPct) == 1,
                "rPct must be second to match CompoundOnlineData::rPct.");
  static_assert(static_cast<uint8_t>(Parameter::P) == 2,
                "P must be third to match CompoundOnlineData::PV.");
  static_assert(static_cast<uint8_t>(Parameter::Int) == 3,
                "Int must be forth to match CompoundOnlineData::FV.");
  static_assert(static_cast<uint8_t>(Parameter::YearConvention) ==
                    k_numberOfParameters - 1,
                "YearConvention must be last.");

  constexpr static uint8_t k_numberOfDoubleValues =
      static_cast<uint8_t>(Parameter::YearConvention);
  static_assert(k_numberOfDoubleValues - k_numberOfSharedDoubleValues == 0,
                "All double values are expected to be shared.");
  constexpr static uint8_t k_numberOfUnknowns = k_numberOfDoubleValues;
  static_assert(k_maxNumberOfUnknowns >= k_numberOfUnknowns,
                "k_maxNumberOfUnknowns is invalid.");

  I18n::Message labelForParameter(uint8_t param) const override;
  I18n::Message sublabelForParameter(uint8_t param) const override;
  I18n::Message menuTitle() const override {
    return I18n::Message::SimpleInterest;
  }
  bool checkValue(uint8_t param, double value) const override;
  uint8_t numberOfParameters() const override { return k_numberOfParameters; }
  uint8_t numberOfDoubleValues() const override {
    return k_numberOfDoubleValues;
  }
  uint8_t numberOfUnknowns() const override { return k_numberOfUnknowns; }
  I18n::Message dropdownMessageAtIndex(int index) const override {
    return index == 0 ? I18n::Message::Finance360 : I18n::Message::Finance365;
  }
  double computeUnknownValue() override;
};

class CompoundOnlineData : public OnlineData {
 private:
  enum class Parameter : uint8_t {
    N = 0,  // Shared value with SimpleOnlineData::n
    rPct,   // Shared value with SimpleOnlineData::rPct
    PV,     // Shared value with SimpleOnlineData::P
    FV,     // Shared value with SimpleOnlineData::Int
    Pmt,
    PY,      // Can't be unknown
    CY,      // Can't be unknown
    Payment  // Is not a double parameter and can't be unknown
  };

 public:
  CompoundOnlineData(double* sharedValues) : OnlineData(sharedValues) {
    resetValues();
  }

  constexpr static uint8_t k_numberOfParameters = 8;
  static_assert(static_cast<uint8_t>(Parameter::N) == 0,
                "N must be first to match SimpleOnlineData::n.");
  static_assert(static_cast<uint8_t>(Parameter::rPct) == 1,
                "rPct must be second to match SimpleOnlineData::rPct.");
  static_assert(static_cast<uint8_t>(Parameter::PV) == 2,
                "PV must be third to match SimpleOnlineData::P.");
  static_assert(static_cast<uint8_t>(Parameter::FV) == 3,
                "FV must be forth to match SimpleOnlineData::Int.");
  static_assert(static_cast<uint8_t>(Parameter::PY) == k_numberOfParameters - 3,
                "PY must be third from last.");
  static_assert(static_cast<uint8_t>(Parameter::CY) == k_numberOfParameters - 2,
                "CY must be second from last.");
  static_assert(static_cast<uint8_t>(Parameter::Payment) ==
                    k_numberOfParameters - 1,
                "Payment must be last.");

  constexpr static uint8_t k_numberOfDoubleValues =
      static_cast<uint8_t>(Parameter::Payment);
  constexpr static uint8_t k_numberOfUnknowns =
      static_cast<uint8_t>(Parameter::PY);
  static_assert(k_maxNumberOfUnknowns >= k_numberOfUnknowns,
                "k_maxNumberOfUnknowns is invalid.");

  I18n::Message labelForParameter(uint8_t param) const override;
  I18n::Message sublabelForParameter(uint8_t param) const override;
  I18n::Message menuTitle() const override {
    return I18n::Message::CompoundInterest;
  }
  double defaultValue(uint8_t param) const override;
  bool checkValue(uint8_t param, double value) const override;
  uint8_t numberOfParameters() const override { return k_numberOfParameters; }
  uint8_t numberOfDoubleValues() const override {
    return k_numberOfDoubleValues;
  }
  uint8_t numberOfUnknowns() const override { return k_numberOfUnknowns; }
  I18n::Message dropdownMessageAtIndex(int index) const override {
    return index == 0 ? I18n::Message::FinanceBeginning
                      : I18n::Message::FinanceEnd;
  }
  double computeUnknownValue() override;
  void setValue(uint8_t param, double value) override;
  double getValue(uint8_t param) const override;

 private:
  double m_values[k_numberOfDoubleValues - k_numberOfSharedDoubleValues];
};

class Data {
 public:
  // By default, select the simple interest data model
  Data()
      /* The cast is there to prevent GCC from warning about m_sharedValues
       * array being used uninitialized */
      : m_compoundOnlineData(static_cast<double*>(m_sharedValues)),
        m_simpleOnlineData(static_cast<double*>(m_sharedValues)),
        m_selectedModel(true) {}
  void reset();
  void setModel(bool selectedModel) { m_selectedModel = selectedModel; }
  OnlineData* OnlineData() {
    return m_selectedModel
               ? static_cast<OnlineData*>(&m_simpleOnlineData)
               : static_cast<OnlineData*>(&m_compoundOnlineData);
  }

 private:
  CompoundOnlineData m_compoundOnlineData;
  SimpleOnlineData m_simpleOnlineData;
  double m_sharedValues[OnlineData::k_numberOfSharedDoubleValues];
  bool m_selectedModel;
};
}  
#endif
