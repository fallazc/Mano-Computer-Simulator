#ifndef CPU16BITSCONTROLLER_H
#define CPU16BITSCONTROLLER_H

#define NUMBER_TRANSFER_STATEMENTS 7

#include <QtGlobal>
#include <QTextEdit>
#include <QMessageBox>
#include "Utils.h"
#include "CPU16BitsModel.h"
#include "MemoryUnitController.h"

enum class Row { I_V, T0, T1, T2, T3, T4, T5, T6, A_I};

enum class Column {T_S, IR, AC, DR, PC, AR, M_AR, E};

enum class InputControl { LD, INR, CLR};

enum class BusIDs { AR = 1, PC = 2, DR = 3, AC = 4, IR = 5, TR = 6, MEMORY_UNIT = 7};

class CPU16BitsController{
    friend struct CPU16BitsDelegateInstructions;
    typedef void (*DelegateInstruction)(CPU16BitsController &this_);
public:
    CPU16BitsController(QTableWidget &iTableView, QTableWidget &mTableView, QTextEdit &textEdit);

    //
    void fetch();
    void decode(QWidget* parent);
    void execute(QWidget* parent);
    inline void updateDescriptionTextEdit(const int &step){
        descriptionArea.setText(descriptionT[step]);
    }

private:
    bool indirectAddress;
    bool instructionFetched;
    bool instructionDecoded;
    bool instructionExecuted;
    QTextEdit &descriptionArea;
    QTableWidget &instructionTableView;
    QString descriptionT[NUMBER_TRANSFER_STATEMENTS];
    QScopedPointer<CPU16BitsModel> cpu16BitsModel;
    QScopedPointer<MemoryUnitController> memoryUnitController;
    DelegateInstruction executeInstruction;

    quint16 getDataAtAR();

    void updateInitialValuesInTableView();
    void updateAfterInstructionInTableView();
    void writeToMAR(Row step, quint16 data);
    void writeToRegister(Row step, Column cRegister, InputControl type, quint16 value = 0);
    DelegateInstruction getMemoryReferenceFunction(const int &leadingByte);
    DelegateInstruction getRegisterReferenceFunction(const int &instruction);
    void updateInstructionTableView(Row step, Column cRegister, const QString &text);
    void resetTransferStatements();
};

#endif // CPU16BITSCONTROLLER_H
