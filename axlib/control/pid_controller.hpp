#include "axlib/control/pid_controller.hh"
#include "axlib/core/core.hh"

namespace axlib {

template <uint32_t buffer_length>
PidController<buffer_length>::PidController()
    :
      dead_zone_(0),
      set_point_(0),
      p_(0),
      i_(0),
      d_(0),
      change_min_(-1),
      change_max_(1),
      idx_(0),
      is_valid_(false)
{
    memset(buffer_,0,sizeof(buffer_));
}

template <uint32_t buffer_length>
void PidController<buffer_length>::SetSetPoint(const float set_point)
{
    set_point_ = set_point;
}

template <uint32_t buffer_length>
float PidController<buffer_length>::GetSetPoint() const
{
    return set_point_;
}

template <uint32_t buffer_length>
void PidController<buffer_length>::SetP(const float p)
{
    p_ = p;
}

template <uint32_t buffer_length>
void PidController<buffer_length>::SetI(const float i)
{
    i_ = i;
}

template <uint32_t buffer_length>
void PidController<buffer_length>::SetD(const float d)
{
    d_ = d;
}

template <uint32_t buffer_length>
void PidController<buffer_length>::Update(const float value)
{
    buffer_[idx_] = value;
    idx_ = (idx_+1)%buffer_length;
    if (idx_ == 0) {
        // Buffer filled at least once
        is_valid_ = true;
    }
}

template <uint32_t buffer_length>
void PidController<buffer_length>::SetControlChangeMinMax(const float min, const float max)
{
    change_min_ = min;
    change_max_ = max;
}

template <uint32_t buffer_length>
bool PidController<buffer_length>::ModifyControl(float *control)
{
    if (!is_valid_) {
        return false;
    }

    float change = 0;

    // I
    float i_err = 0;
    for (uint32_t i = 0; i < buffer_length; ++i) {
        i_err += set_point_-buffer_[i];
    }
    i_err = i_err/buffer_length;
    if (axlib::abs(i_err) < dead_zone_) {
        i_err = 0;
    }
    change += i_*i_err;

    // P
    const float latest_err = (set_point_-buffer_[axlib::min(idx_-1,buffer_length-1)]);
    float p_err = latest_err;
//    if (axlib::abs(p_err) < dead_zone_) {
//        p_err = 0;
//    }
    change += p_*p_err;

    // D
    const float oldest_err = set_point_-buffer_[idx_];
    float d_err = (oldest_err-latest_err);
    change += d_*d_err;

    // Final value
    change = axlib::min(change_max_, axlib::max(change, change_min_));

    // And actual change
    *control += change;

    return true;
}

template <uint32_t buffer_length>
void PidController<buffer_length>::SetDeadZone(const float dead_zone)
{
    dead_zone_ = axlib::abs(dead_zone);
}

} //namespace axlib

