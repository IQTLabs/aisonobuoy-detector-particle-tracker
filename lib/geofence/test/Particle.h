#pragma once

#include <functional>

// List of all defined system errors
#define SYSTEM_ERROR_NONE                   (0)
#define SYSTEM_ERROR_UNKNOWN                (-100)
#define SYSTEM_ERROR_BUSY                   (-110)
#define SYSTEM_ERROR_NOT_SUPPORTED          (-120)
#define SYSTEM_ERROR_NOT_ALLOWED            (-130)
#define SYSTEM_ERROR_CANCELLED              (-140)
#define SYSTEM_ERROR_ABORTED                (-150)
#define SYSTEM_ERROR_TIMEOUT                (-160)
#define SYSTEM_ERROR_NOT_FOUND              (-170)
#define SYSTEM_ERROR_ALREADY_EXISTS         (-180)
#define SYSTEM_ERROR_TOO_LARGE              (-190)
#define SYSTEM_ERROR_NOT_ENOUGH_DATA        (-191)
#define SYSTEM_ERROR_LIMIT_EXCEEDED         (-200)
#define SYSTEM_ERROR_END_OF_STREAM          (-201)
#define SYSTEM_ERROR_INVALID_STATE          (-210)
#define SYSTEM_ERROR_IO                     (-220)
#define SYSTEM_ERROR_WOULD_BLOCK            (-221)
#define SYSTEM_ERROR_FILE                   (-225)
#define SYSTEM_ERROR_NETWORK                (-230)
#define SYSTEM_ERROR_PROTOCOL               (-240)
#define SYSTEM_ERROR_INTERNAL               (-250)
#define SYSTEM_ERROR_NO_MEMORY              (-260)
#define SYSTEM_ERROR_INVALID_ARGUMENT       (-270)
#define SYSTEM_ERROR_BAD_DATA               (-280)
#define SYSTEM_ERROR_OUT_OF_RANGE           (-290)
#define SYSTEM_ERROR_DEPRECATED             (-300)
#define SYSTEM_ERROR_COAP                   (-1000)
#define SYSTEM_ERROR_COAP_4XX               (-1100)
#define SYSTEM_ERROR_COAP_5XX               (-1132)
#define SYSTEM_ERROR_AT_NOT_OK              (-1200)
#define SYSTEM_ERROR_AT_RESPONSE_UNEXPECTED (-1210)

#include "spark_wiring_vector.h"

typedef uint32_t system_tick_t;

using namespace spark;

class SystemClass {
public:
    SystemClass() : _tick(0) {}

    system_tick_t Uptime() const {
        return (system_tick_t)_tick;
    }

    unsigned uptime() const {
        return _tick / 1000;
    }

    uint64_t millis() const {
        return _tick;
    }

    void inc(int i = 1) {
        _tick += i;
    }

private:
    uint64_t _tick;
};

extern SystemClass System;
