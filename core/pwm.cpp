#include "core/pwm.hh"

using namespace axlib;

PwmSimple::PwmSimple(const Port pwm_port, const uint8_t pwm_pin)
    :
      pwm_port_(GetPort(pwm_port)),
      tc0_(GetTimerCounter0(pwm_port)),
      tc1_(GetTimerCounter1(pwm_port)),
      pwm_pin_(pwm_pin)
{
    pwm_port_->DIRSET = pwm_pin_;
    pwm_port_->OUTSET = pwm_pin_;

    if (pwm_pin <= 0b00001111) {
        tc0_->CTRLB |= 0x03; //single slope
        tc0_->CTRLB |= 0b10000; //channel select CCAEN
    } else {
        tc1_->CTRLB | 0x03; //single slope
        tc0_->CTRLB |= 0b10000; //channel select CCAEN
    }
}

void PwmSimple::SetPeriod(const uint16_t per)
{
    if (pwm_pin_ <= 0b00001111) {
        tc0_->PER = per;
    } else {
        tc1_->PER = per;
    }
}

void PwmSimple::SetDutyCycle(const float duty_cycle)
{
    if (pwm_pin_ <= 0b00001111) {
        tc0_->CCABUF = tc0_->PER*duty_cycle;
    } else {
        tc1_->CCABUF = tc1_->PER*duty_cycle;
    }
}

float PwmSimple::GetDutyCycle()
{
    if (pwm_pin_ <= 0b00001111) {
        return (float)tc0_->CCABUF/(float)tc0_->PER;
    } else {
        return (float)tc1_->CCABUF/(float)tc1_->PER;
    }
}

void PwmSimple::SetClock(const uint8_t clk)
{
    clock_ctrl_ = (0x00FF & clk);
    if (pwm_pin_ <= 0b00001111) {
        tc0_->CTRLA |= clock_ctrl_;
    } else {
        tc1_->CTRLA |= clock_ctrl_;
    }
}

void PwmSimple::SetEnabled(const bool enabled)
{
    uint8_t data = 0; // disable
    if (enabled) {
        data = clock_ctrl_;
    }

    if (pwm_pin_ <= 0b00001111) {
        tc0_->CTRLA |= (0x00FF & data);
    } else {
        tc1_->CTRLA |= (0x00FF & data);
    }
}

void PwmSimple::SetCcBuffer(const uint16_t buffer)
{
    if (pwm_pin_ <= 0b00001111) {
        tc0_->CCABUF = buffer;
    } else {
        tc0_->CCABUF = buffer;
    }
}
