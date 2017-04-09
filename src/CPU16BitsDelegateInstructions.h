#ifndef CPU16BITS_DELEGATE_INSTRUCTIONS_H
#define CPU16BITS_DELEGATE_INSTRUCTIONS_H
#include "CPU16BitsController.h"

struct CPU16BitsDelegateInstructions
{
public:
    static void runT3(CPU16BitsController &controller);

    ///Memory reference instructions

    static void AND(CPU16BitsController &controller);
    static void ADD(CPU16BitsController &controller);
    static void LDA(CPU16BitsController &controller);
    static void STA(CPU16BitsController &controller);
    static void BUN(CPU16BitsController &controller);
    static void BSA(CPU16BitsController &controller);
    static void ISZ(CPU16BitsController &controller);

    ///Register reference instructions

    static void CLA(CPU16BitsController &controller);
    static void CLE(CPU16BitsController &controller);
    static void CMA(CPU16BitsController &controller);
    static void CME(CPU16BitsController &controller);
    static void CIR(CPU16BitsController &controller);
    static void CIL(CPU16BitsController &controller);
    static void INC(CPU16BitsController &controller);
    static void SPA(CPU16BitsController &controller);
    static void SNA(CPU16BitsController &controller);
    static void SZA(CPU16BitsController &controller);
    static void SZE(CPU16BitsController &controller);
    static void HLT(CPU16BitsController &controller);

private:
    static void t4DRgetsDataAtAR(CPU16BitsController &controller);
    static void incrementPCatStep(CPU16BitsController &controller, Row step);
};

#endif // CPU16BITS_DELEGATE_INSTRUCTIONS_H
