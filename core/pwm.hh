#ifndef AXLIB_CORE_PWM_HH
#define AXLIB_CORE_PWM_HH

#include "core/io.hh"
#include <avr/io.h>

namespace axlib {

class PwmSimple
{
 public:
    PwmSimple(const axlib::Port pwm_port, const uint8_t pwm_pin);

    /// Use one of the following:
    /// TC_CLKSEL_OFF_gc TC off (no clock selected)
    /// TC_CLKSEL_DIV1_gc fCLK,SYS
    /// TC_CLKSEL_DIV2_gc fCLK,SYS / 2
    /// TC_CLKSEL_DIV4_gc fCLK,SYS / 4
    /// TC_CLKSEL_DIV8_gc fCLK,SYS / 8
    /// TC_CLKSEL_DIV64_gc fCLK,SYS / 64
    /// TC_CLKSEL_DIV256_gc fCLK,SYS / 256
    /// TC_CLKSEL_DIV1024_gc fCLK,SYS / 1024
    /// TC_CLKSEL_EV0_gc Event channel 0
    /// TC_CLKSEL_EV1_gc Event channel 1
    /// TC_CLKSEL_EV2_gc Event channel 2
    /// TC_CLKSEL_EV3_gc Event channel 3
    /// TC_CLKSEL_EV4_gc Event channel 4
    /// TC_CLKSEL_EV5_gc Event channel 5
    /// TC_CLKSEL_EV6_gc Event channel 6
    /// TC_CLKSEL_EV7_gc Event channel 7
    void SetClock(const uint8_t clk);
    void SetEnabled(const bool enabled);
    void SetPeriod(const uint16_t per);
    void SetDutyCycle(const float duty_cycle);
    float GetDutyCycle();
    void SetCcBuffer(const uint16_t buffer);
 private:
    PORT_t *pwm_port_;
    TC0_t *tc0_;
    TC1_t *tc1_;
    uint8_t pwm_pin_;
    uint8_t clock_ctrl_;
}; //class SimplePwm

} //namespace axlib

#endif //ifndef AXLIB_CORE_PWM_HH
