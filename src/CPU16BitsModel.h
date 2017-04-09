#ifndef CPU16BITSMODEL_H
#define CPU16BITSMODEL_H

#include <bitset>
#include <QtGlobal>

class CPU16BitsModel{
public:
    inline quint16 getE() { return E.to_ulong(); }
    inline quint16 getDR() { return DR.to_ulong(); }
    inline quint16 getAC() { return AC.to_ulong(); }
	inline bool getACleadingBit() { return AC[15]; }
    inline quint16 getIR() { return IR.to_ulong(); }
    inline quint16 getTR() { return TR.to_ulong(); }
    inline quint16 getAR() { return AR.to_ulong(); }
    inline quint16 getPC() { return PC.to_ulong(); }
    inline quint16 getINPR() { return INPR.to_ulong(); }
    inline quint16 getOUTR() { return OUTR.to_ulong(); }

    inline void setE(const quint16& E) { this->E = std::bitset<1>(E); }
    inline void setDR(const quint16& DR) { this->DR = std::bitset<16>(DR); }
    inline void setAC(const quint16& AC) { this->AC = std::bitset<16>(AC); }
    inline void setIR(const quint16& IR) { this->IR = std::bitset<16>(IR); }
    inline void setTR(const quint16& TR) { this->TR = std::bitset<16>(TR); }

    inline void setAR(const quint16& AR) { this->AR = std::bitset<12>(AR); }
    inline void setPC(const quint16& PC) { this->PC = std::bitset<12>(PC); }

    ///setINPR and setOUTR are not used in the project but I figured I'll leave them here
    ///since they are also registers
    inline void setINPR(const quint8& INPR) { this->INPR = std::bitset<8>(INPR); }
    inline void setOUTR(const quint8& OUTR) { this->OUTR = std::bitset<8>(OUTR); }

    inline void inrAR() { setAR(getAR() + 1); }
    inline void inrPC() { setPC(getPC() + 1); }
    inline void inrDR() { setDR(getDR() + 1); }
    inline void inrAC() { setAC(getAC() + 1); }
    inline void inrTR() { setTR(getTR() + 1); }

    inline void clrAR() { AR.reset(); }
    inline void clrPC() { PC.reset(); }
    inline void clrDR() { DR.reset(); }
    inline void clrAC() { AC.reset(); }
    inline void clrTR() { TR.reset(); }

    inline void logicUnitAND(){ AC = AC & DR; }
    inline void logicUnitCMA() { AC = ~AC; }
    void logicUnitCME() { E = ~E; }

    void arithmeticUnitADD();
    void arithmeticUnitCIR();
    void arithmeticUnitCIL();


private:
    //Registers

    std::bitset<16> DR;
    std::bitset<16> AC;
    std::bitset<16> IR;
    std::bitset<16> TR;
    std::bitset<12> AR;
    std::bitset<12> PC;
    std::bitset<8> INPR;
    std::bitset<8> OUTR;
    std::bitset<1> E;
};

#endif // CPU_16BITSMODEL_H
