#pragma once

#define M_RSV_BITS(type,bits) private: type __RSV_ ## __LINE__ : bits; public:

typedef unsigned char      U08;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;
typedef char               I08;
typedef short              I16;
typedef int                I32;
typedef long long          I64;

typedef float              F32;
typedef double             F64;
typedef const char *       Text;

template<U32 = 4> 
union UIntType {
    typedef U32 Result;
};

template<> 
union UIntType<8> {
    typedef U64 Result;
};

template<U32 = 4> 
union IntType {
    typedef I32 Result;
};

template<> 
union IntType<8> {
    typedef I64 Result;
};

typedef UIntType<sizeof(void *)>::Result UInt;
typedef IntType<sizeof(void *)>::Result Int;

constexpr bool Enable = true;
constexpr bool Disable = false;
constexpr bool Success = true;
constexpr bool Fail = false;
constexpr bool IsSystem64 = sizeof(void *) == 8;
constexpr bool IsSystem32 = sizeof(void *) == 4;
constexpr UInt NotFound = (UInt)-1;
enum Level : UInt { L, H };

//FAKER
#ifdef _MSC_BUILD
    enum {
        SDA,
        SCL,
    };
    void delayMicroseconds(U32) {

    }
    void pinMode(U32, U32) {

    }
    void digitalWrite(U32, U32) {

    }
    U32 digitalRead(U32) {
        return 0;
    }
#else
    #include <Arduino.h>
#endif

enum PinMode {
    InputMode,
    OutputMode,
};

class Pin{
    U32 pin;
public:
    Pin() {
        pin = (U32)-1;
    }
    Pin(U32 pin) : 
        Pin(pin, OutputMode) {
    }
    Pin(U32 pin, PinMode mode) {
        this->pin = pin;
        pinMode(pin, mode);
    }
    void TurnToInputMode(){
        pinMode(pin, InputMode);
    }
    void TurnToOutputMode(){
        pinMode(pin, OutputMode);
    }
    void operator=(Level value) {
        digitalWrite(pin, (U32)value);
    }
    operator Level() {
        return (Level)digitalRead(pin);
    }
};