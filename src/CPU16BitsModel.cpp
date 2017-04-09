#include "CPU16BitsModel.h"

/**
 * @brief Performs a binary addition
 */
void CPU16BitsModel::arithmeticUnitADD(){
    std::bitset<1> cOut(0);
    std::bitset<16> sum(0);
	
    for(quint16 i = 0; i < AC.size(); i++){
        sum[i] = AC[i] ^ DR[i] ^ cOut[0];
        cOut = cOut[0] & (AC[i] ^ DR[i]) | (AC[i] & DR[i]);
    }

    E = cOut;
	AC = sum;
}

/**
 * @brief Performs a circular right shift
 */
void CPU16BitsModel::arithmeticUnitCIR() {
	E[0] = AC[0];
	AC = AC >> 1;
	AC[15] = E[0];
}

/**
 * @brief Performs a circular left shift
 */
void CPU16BitsModel::arithmeticUnitCIL() {
	E[0] = AC[15];
	AC = AC << 1;
	AC[0] = E[0];
}
