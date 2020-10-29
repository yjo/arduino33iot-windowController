#ifndef IOT33PIO_SLATSMOTOR_H
#define IOT33PIO_SLATSMOTOR_H

#include <Servo.h>
#include "util.h"

class SlatsMotor {
  public:
    void init(int pinNumber);

    void service();

    enum class Mode : uint8_t {
        closed, open, boo, stop
    };

    Mode mode = Mode::boo;

    void setMode(Mode mode, Print &out = Serial);

  private:
    struct ServoInstr {
      const int16_t endAngle_deg;
      const uint32_t minDuration_ms;
      const uint32_t maxDuration_ms;
    };

    static constexpr int16_t SLATS_OPEN_deg = 30;
    static constexpr int16_t SLATS_CLOSED_deg = 150;

    static constexpr ServoInstr servoProg[] = {
        // Keep closed for 30s to 90s
        {SLATS_CLOSED_deg, 90 * 1000ul, 90 * 1000ul},

        // Open at max speed, hold for (the remainder of) 3sec
        {SLATS_OPEN_deg},
        {SLATS_OPEN_deg,   3 * 1000ul},

        // Close over 5s
        {SLATS_CLOSED_deg, 5 * 1000ul},
    };

    static constexpr uint16_t servoProgSize = sizeof(servoProg) / sizeof(servoProg[0]);

    Servo servo;
    ServoInstr const *currentInstr = &servoProg[0];

    uint32_t currentInstrStart_ms = 0;
    uint32_t currentInstrEnd_ms = 0;
    int currentInstrStartAngle_deg = servoProg[0].endAngle_deg;
    int angle_deg = currentInstrStartAngle_deg;

    void serviceProgram();

    void programTick(uint32_t now_ms);
};

#endif //IOT33PIO_SLATSMOTOR_H
