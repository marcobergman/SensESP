#ifndef _wiring_helpers_H_
#define _wiring_helpers_H_

#include "sensesp_app.h"
#include "sensors/sensor.h"
#include "vedirect_parser.h"

class VEDirectInput : public Sensor {
  public:
    VEDirectInput(Stream* rx_stream, uint8_t chargernumber);
    virtual void enable() override final;
    VEDirectData vedirect_data_;
  private:
    Stream* rx_stream_;
    VEDirectParser vedirect_parser_;
};

VEDirectInput* setup_vedirect(Stream* rx_stream, uint8_t chargernumber);

#endif
