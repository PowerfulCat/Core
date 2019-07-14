#pragma once
#include"Define.h"

//INNER & DETAIL
namespace Origin {
    class I2C {
        static void DefaultDelay() {
            //100KHz 
            delayMicroseconds(10);
        }
    public:
        Pin Data;
        Pin Clock;
        void (* Delay)();

        I2C() : 
            I2C(SDA, SCL, & DefaultDelay){
        }
        I2C(U32 dataPin, U32 clockPin) : 
            Data(dataPin), Clock(clockPin), Delay(& DefaultDelay){
        }
        I2C(U32 dataPin, U32 clockPin, void (*delay)()): 
            Data(dataPin), Clock(clockPin), Delay(delay) {
            Reset();
        }

        enum Mode{
            Write,
            Read,
        };
        void Start() {
            Data = L;
            Delay();
            Clock = L;
            Delay();
        }
        void Restart() {
            Data = H;
            Clock = H;
            Delay();
            Start();
        }
        void End() {
            Data = L;
            Clock = H;
            Delay();
            Data = H;
            Delay();
        }
        void Reset() {
            Start();
            End();
        }
        Level Rx() {
            Level value;
            Clock = H;
            Delay();
            value = Data;
            Clock = L;
            return value;
        }
        void Tx(Level value) {
            Data = value;
            Clock = H;
            Delay();
            Clock = L;
        }
        bool Send(const U08 * buffer, UInt length) {
            bool result = Success;
            for (U08 temp; length-- != 0; buffer++) {
                temp = buffer[0];
                Data.TurnToOutputMode();
            
                for (UInt i = 0; i < 8; i++) {
                    Tx(temp & 0x80 ? H : L);
                    temp <<= 1;
                }
            
                Data.TurnToInputMode();
            
                //read ack from Data pin
                //value 'H' indicated the fail
                if (Rx()) {
                    result = Fail;
                    break;
                }
            }

            //recover to output mode(default)
            Data.TurnToOutputMode();
            return result;
        }
        void Receive(U08 * buffer, UInt length) {
            while (true) {
                U08 temp = 0;
                Data.TurnToInputMode();
            
                for (UInt i = 0; i < 8; i++) {
                    temp = temp << 1 | Rx();
                }

                buffer[0] = temp;
                buffer++;
                Data.TurnToOutputMode();

                //send nack(H)
                //indicates the end of reception
                if (--length == 0) {
                    Tx(H);
                    break;
                }

                //send ack(L) from master to slavel
                //in order to keep reception sequence
                else {
                    Tx(L);
                }
            }
        }
        bool Send(U08 value) {
            return Send(& value, 1);
        }
        U08 Receive() {
            U08 value;
            Receive(& value, 1);
            return value;
        }
    };
}

//NORMAL
class I2C : protected Origin::I2C{
public:
    I2C() : 
        Origin::I2C(){
    }
    I2C(U32 dataPin, U32 clockPin) : 
        Origin::I2C(dataPin, clockPin){
    }
    I2C(U32 dataPin, U32 clockPin, void (*delay)()) : 
        Origin::I2C(dataPin, clockPin, delay) {
    }
    bool Send(U08 device, U08 reg, U08 value);
    bool Send(U08 device, const U08 * buffer, UInt length);
    bool Receive(U08 device, U08 reg, U08 * buffer, UInt length);
    UInt Receive(U08 device, U08 reg);
};
