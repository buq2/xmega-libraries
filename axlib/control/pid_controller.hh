#ifndef AXLIB_CONTROL_PIX_HH
#define AXLIB_CONTROL_PIX_HH

namespace axlib {

template <uint32_t buffer_length>
class PidController
{
 public:
    PidController();
    void SetSetPoint(const float set_point);
    void SetP(const float p);
    void SetI(const float i);
    void SetD(const float d);
    void Update(const float value);

    void SetControlChangeMinMax(const float min, const float max);
    bool ModifyControl(float *control);
    float GetSetPoint() const;
    void SetDeadZone(const float dead_zone);
 private:
    float dead_zone_;
    float set_point_;
    float p_;
    float i_;
    float d_;
    float change_min_;
    float change_max_;
    float buffer_[buffer_length];
    uint32_t idx_;
    bool is_valid_;
}; //PidController

} //namespace axlib

#include "axlib/control/pid_controller.hpp"

#endif //ifndef AXLIB_CONTROL_PIX_HH
