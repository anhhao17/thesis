#ifndef _indicator_light_h_
#define _indicator_light_h_

namespace indicator{
    enum IndicatorState
    {
        OFF,
        ON,
        PULSING,
        FAIL,
        OK
    };
};


class IndicatorLight
{
private:
    indicator::IndicatorState m_state;
    TaskHandle_t m_taskHandle;

public:
    IndicatorLight();
    void setState(indicator::IndicatorState state);
    indicator::IndicatorState getState();
};

#endif