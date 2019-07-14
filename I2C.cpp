#include"I2C.h"

bool I2C::Send(U08 device, U08 reg, U08 value) {
    bool result = Success;
    Origin::I2C::Start();
    result &= Origin::I2C::Send(device << 1 | Write);
    result &= Origin::I2C::Send(reg);
    result &= Origin::I2C::Send(value);
    Origin::I2C::End();
    return result;
}
bool I2C::Send(U08 device, const U08 * buffer, UInt length) {
    bool result = Success;
    Origin::I2C::Start();
    result &= Origin::I2C::Send(device << 1 | Write);

    while (length--) {
        result &= Origin::I2C::Send(buffer[0]);
        buffer++;
    }

    Origin::I2C::End();
    return result;
}
bool I2C::Receive(U08 device, U08 reg, U08 * buffer, UInt length) {
    bool result = Success;
    Origin::I2C::Start();
    result &= Origin::I2C::Send(device << 1 | Write);
    result &= Origin::I2C::Send(reg);
        
    if (result == false) {
        return result;
    }

    Origin::I2C::Restart();
    Origin::I2C::Send(device << 1 | Read);
    Origin::I2C::Receive(buffer, length);
    Origin::I2C::End();
    return result;
}
UInt I2C::Receive(U08 device, U08 reg) {
    U08 value;
    if (Receive(device, reg, & value, 1) == Fail){
        return NotFound;
    }
    return value;
}
