#ifndef IOT33PIO_HZCOUNTER_H
#define IOT33PIO_HZCOUNTER_H

#include <Arduino.h>

class HzCounter : public Printable {
  public:
    void reset() {
      counts = 0;
      since_ms = millis();
    }
    void tick();
    uint32_t getHz() const {
      return counts * 1000ul / (millis() - since_ms);
    }
    size_t printTo(Print &p) const override;
  private:
    void trim();
    static constexpr uint32_t maxCounts = 20000;
    uint32_t counts;
    uint32_t since_ms;
};

inline void HzCounter::tick() {
  if (++counts > maxCounts) {
    trim();
  };
}

inline void HzCounter::trim() {
  uint32_t now_ms = millis();
  uint32_t elapsed_ms = now_ms - since_ms;
  counts = counts / 2;
  since_ms = now_ms - elapsed_ms / 2;
}
size_t HzCounter::printTo(Print &p) const {
  return (counts ? p.print(getHz()) : p.print("??")) + p.print("Hz");
}

#endif //IOT33PIO_HZCOUNTER_H
