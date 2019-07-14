#pragma once
#include"Define.h"

//INNER & DETAIL
namespace Origin {
    struct GlobalStatus {
        U08 FailSafe                      : 1;
        M_RSV_BITS(2);
        U08 TempPrewarning                : 1;
        U08 ChipOverload                  : 1;
        U08 ChipResetOrCommunicationError : 1;
        U08 CommunicationError            : 1;
        U08 GlobalError                   : 1;
        void operator=(U08 value) {
            operator U08 & () = value;
        }
        operator U08 &(){
            return *(U08 *)this;
        }
    };
    class SPI {
        static void DefaultDelay() {
            //250KHz 
            delayMicroseconds(4);
        }
    public:
        Pin Sdi;
        Pin Sdo;
        Pin Cs;
        Pin Clock;
        Level Edge;
        void (* Hold)();

        enum Mode {
            Write                   = 0 << 6,
            Read                    = 1 << 6,
            ReadAndClearStatus      = 2 << 6,
            ReadDeviceInformation   = 3 << 6,
        };
        void Reset() {
            Disable();
            Clock = !Eage;
        }
        Level Rx() {
            Level value;
            Clock = Edge;
            Hold();
            value = Sdi;
            Clock = !Edge;
            Hold();
            return value;
        }
        void Tx(Level value) {
            Sdo = value;
            Clock = !Edge;
            Hold();
            Clock = Edge;
            Hold();
        }
        void Send(const U08 * buffer, UInt length) {
            for (UInt i = 0; i < length; i++) {
                Send(buffer[i]);
            }
        }
        void Send(U08 value) {
            for (UInt j = 0; j < 8; j++, value <<= 1) {
                Tx(value & 0x80 ? H : L);
            }
        }
        void Receive(U08 * buffer, UInt length) {
            for (UInt i = 0; i < length; i++) {
                buffer[i] = Receive();
            }
        }
        U08 Receive(){
            U08 value = 0;
            for (UInt j = 0; j < 8; j++) {
                value = value << 1 | Tx();
            }
            return value;
        }
        void Enabe() {
            Cs = L;
        }
        void Disable() {
            Cs = H;
        }

    };
}

//NORMAL
