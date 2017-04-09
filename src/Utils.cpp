#include "Utils.h"

/**
 * @brief Convert a base 10 number to any base, "0x" and "0b" will be added
 *        if the new base is 16(hexadecimal) or 2(binary).
 * @param value base 10 integer to convert to another base
 * @param fieldWidth is the length of the string to be returned.
 * @param base is the new base to in which the number will be represented
 * @return a QString representation of the converted number
 */
QString intToQString(const int &value, const int &fieldWidth, const int &base){
    QString prefix = "";

    if(base == 16)
        prefix.append("0x");
    else if(base == 2)
        prefix.append("0b");

    return prefix.append(QString("%1").arg(value, fieldWidth, base, QChar('0')).toUpper());
}
