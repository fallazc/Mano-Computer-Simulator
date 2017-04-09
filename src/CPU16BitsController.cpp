#include "CPU16BitsDelegateInstructions.cpp"
#include "CPU16BitsController.h"

CPU16BitsController::CPU16BitsController(QTableWidget &iTableView, QTableWidget &mTableView, QTextEdit &textArea)
    : cpu16BitsModel(new CPU16BitsModel),
      indirectAddress(false),
      instructionFetched(false),
      instructionDecoded(false),
      instructionExecuted(true),
      instructionTableView(iTableView),
      descriptionArea(textArea),
      memoryUnitController(new MemoryUnitController(mTableView))
{
    //Initialize empty table entries
    for(int c = 1; c < instructionTableView.columnCount(); c++)
        for(int r = 0; r < instructionTableView.rowCount(); r++)
            instructionTableView.setItem(r, c, new QTableWidgetItem);

    updateInitialValuesInTableView();
    //Update descriptions of T0, T1 and T2 only here since
    //they remain the same regarless of the instruction
    descriptionT[0] = QString("s0s1s2 = %1\nAR.LD <- 1").arg(intToQString((int)BusIDs::PC, 3, 2));;
    descriptionT[1] = QString("s0s1s2 = %1\nMEMORY_UNIT.READ <- 1\nIR.LD <- 0b1\nPC.INR <- 1")
                                    .arg(intToQString((int)BusIDs::MEMORY_UNIT, 3, 2));
    descriptionT[2] = QString("s0s1s2 = %1\nAR.LD <- 1").arg(intToQString((int)BusIDs::IR, 3, 2));
}

///CONTROLLER'S MAIN METHODS

/**
 * @brief Fetch the instruction to be executed from the memory unit
 *        Once the instruction is fetched, the decode button will be
 *        unlocked
 */
void  CPU16BitsController::fetch(){
	
    static randomNumberGenerator randomInt(0, MEMORY_UNIT_SIZE - 1);
    updateInitialValuesInTableView();
    resetTransferStatements();
	//Initial values
    writeToRegister(Row::I_V, Column::PC, InputControl::LD, randomInt());
	//T0
    writeToRegister(Row::T0, Column::AR, InputControl::LD, cpu16BitsModel->getPC());
	//T1
    writeToRegister(Row::T1, Column::IR, InputControl::LD, getDataAtAR());
    writeToRegister(Row::T1, Column::PC, InputControl::INR);

    instructionFetched = true;
    instructionDecoded = false;
    instructionExecuted = false;
}

/**
 * @brief Decode the instruction if it was fetched otherwise it will
 *        display a warning and do nothing. Once the instruction is
 *        decoded, this fucntion will be locked until a new instruction
 *        is fetched.
 * @param parent will be used to create a child window that will display
 *        a warning message.
 */
void  CPU16BitsController::decode(QWidget* parent){
    if(!instructionFetched){
        QMessageBox::warning(parent, "Warning", "You need to fetch an instruction before you can decode it!");
		return;
    }

    if (instructionDecoded){
        QMessageBox::warning(parent, "Warning", "This instruction has already been decoded!");
        return;
    }

    quint16 instruction = cpu16BitsModel->getIR();
    quint8 leadingByte = instruction >> 0xC;

    switch (leadingByte) {
    case 0x7:
        //Register reference instruction
        executeInstruction = getRegisterReferenceFunction(instruction);
		break;
    case 0xF:
        //I/O registers are not part of this project otherwise the function
        //binding will go here
        executeInstruction = nullptr;
		break;
    default:
        //Direct memory reference instruction
        executeInstruction = getMemoryReferenceFunction(leadingByte);
    }

    //Update transfer statements
    writeToRegister(Row::T2, Column::AR, InputControl::LD, cpu16BitsModel->getIR());

    instructionDecoded = true;
}

/**
 * @brief Execute the currently bound instruction. On the instruction is
 *        executed, this function will be locked until a new instruction
 *        is fetched and decoded.
 * @param parent will be used to create a child window that will display
 *        a warning message.
 */
void  CPU16BitsController::execute(QWidget* parent){
    if(!instructionDecoded){
        //Warning message window here
        QMessageBox::warning(parent, "Warning", "You need to decode the instruction before you can execute it!");
        return;
    }

    if (instructionExecuted){
        QMessageBox::warning(parent, "Warning", "This instruction has already been executed!");
        return;
    }

    if(executeInstruction != nullptr){
        if(indirectAddress)
            CPU16BitsDelegateInstructions::runT3(*this);
        executeInstruction(*this);
        updateAfterInstructionInTableView();
        //Lock this method until another instruction is fetched
        executeInstruction = nullptr;
        instructionExecuted = true;
        indirectAddress = false;
    } else {
        QMessageBox::critical(parent, "Error", "NullPointerException in the execute");
        exit(-1);
    }
}

///CONTROLLER'S METHODS TO UPDATE THE VIEW

/**
 * @brief Update the "Initial Values" section in the instruction table
 *        widget with default values in each register except PC which
 *        will be manually fetched.
 */
void CPU16BitsController::updateInitialValuesInTableView(){
    updateInstructionTableView(Row::I_V, Column::IR, intToQString(cpu16BitsModel->getIR(), 4));
    updateInstructionTableView(Row::I_V, Column::AC, intToQString(cpu16BitsModel->getAC(), 4));
    updateInstructionTableView(Row::I_V, Column::DR, intToQString(cpu16BitsModel->getDR(), 4));
    updateInstructionTableView(Row::I_V, Column::E, intToQString(cpu16BitsModel->getE()));

    const quint16 ADDRESS = cpu16BitsModel->getAR();
    updateInstructionTableView(Row::I_V, Column::AR, intToQString(ADDRESS, 3));
    updateInstructionTableView(Row::I_V, Column::M_AR, intToQString(memoryUnitController->read(ADDRESS), 4));
}

/**
 * @brief Update the "After Insruction" section in the instruction table
 *        widget after the fetched isntruction was executed
 */
void CPU16BitsController::updateAfterInstructionInTableView() {
    updateInstructionTableView(Row::A_I, Column::IR, intToQString(cpu16BitsModel->getIR(), 4));
    updateInstructionTableView(Row::A_I, Column::AC, intToQString(cpu16BitsModel->getAC(), 4));
    updateInstructionTableView(Row::A_I, Column::DR, intToQString(cpu16BitsModel->getDR(), 4));
    updateInstructionTableView(Row::A_I, Column::E, intToQString(cpu16BitsModel->getE()));
    updateInstructionTableView(Row::A_I, Column::PC, intToQString(cpu16BitsModel->getPC(), 3));

	const quint16 ADDRESS = cpu16BitsModel->getAR();
    updateInstructionTableView(Row::A_I, Column::AR, intToQString(ADDRESS, 3));
    updateInstructionTableView(Row::A_I, Column::M_AR, intToQString(memoryUnitController->read(ADDRESS), 4));

	
}

/**
 * @brief Helper method to write to the memory unit and update the inatruction
 *        table widget.
 * @param step indicates which transfer statement(row) to update.
 * @param data is what will be written to memory.
 */
void CPU16BitsController::writeToMAR(Row step, quint16 data){
    memoryUnitController->write(cpu16BitsModel->getAR(), data);
    updateInstructionTableView(step, Column::M_AR, QString(intToQString(data, 4)));
}

/**
 * @brief CPU16BitsController::writeToRegister
 * @param step indicates which transfer statement(row) to update.
 * @param cRegister indicates which register(column) to update.
 * @param type indicates which type of operation is required.
 * @param value is what will be written to the register.
 */
void CPU16BitsController::writeToRegister(Row step, Column cRegister, InputControl type, quint16 value) {
    switch (cRegister) {
    case Column::IR:
		//"type" is irrelevant for IR
		cpu16BitsModel->setIR(value);
        updateInstructionTableView(step, cRegister, intToQString(cpu16BitsModel->getIR(), 4));
		break;
    case Column::AC:
        if (type == InputControl::LD)
            cpu16BitsModel->setAC(value);
        else if (type == InputControl::INR)
            cpu16BitsModel->inrAC();
        else
            cpu16BitsModel->clrAC();
        updateInstructionTableView(step, cRegister, intToQString(cpu16BitsModel->getAC(), 4));
		break;
    case Column::DR:
        if (type == InputControl::LD)
            cpu16BitsModel->setDR(value);
        else if (type == InputControl::INR)
            cpu16BitsModel->inrDR();
        else
            cpu16BitsModel->clrDR();
        updateInstructionTableView(step, cRegister, intToQString(cpu16BitsModel->getDR(), 4));
		break;
    case Column::PC:
        if (type == InputControl::LD)
            cpu16BitsModel->setPC(value);
        else if (type == InputControl::INR)
            cpu16BitsModel->inrPC();
        else
            cpu16BitsModel->clrPC();
        updateInstructionTableView(step, cRegister, intToQString(cpu16BitsModel->getPC(), 3));
		break;
    case Column::E:
		//"type" is irrelevant for E
		cpu16BitsModel->setE(value);
        updateInstructionTableView(step, cRegister, intToQString(cpu16BitsModel->getE()));
		break;
    case Column::AR:
		//AR's update triggers M[AR]'s update in the table
        if (type == InputControl::LD)
            cpu16BitsModel->setAR(value);
        else if (type == InputControl::INR)
            cpu16BitsModel->inrAR();
        else
            cpu16BitsModel->clrAR();
		int ar = cpu16BitsModel->getAR();
        updateInstructionTableView(step, cRegister, intToQString(ar, 3));
        updateInstructionTableView(step, Column::M_AR, intToQString(memoryUnitController->read(ar), 4));
		break;
	}
}

/**
 * @brief Helper method for M[AR].
 * @return data located at the memory location stored in AR.
 */
quint16 CPU16BitsController::getDataAtAR() {
    quint16 ar = cpu16BitsModel->getAR();
    return memoryUnitController->read(ar);
}

/**
 * @brief Update the text of a cell in the table.
 * @param step indicates which transfer statement(row) to update(model + view).
 * @param cRegister indicates which register(column) to update(model + view).
 * @param text is the string representaton of the current value in the register.
 */
void CPU16BitsController::updateInstructionTableView(Row step, Column cRegister, const QString &text){
    instructionTableView.item((int)step, (int)cRegister)->setText(text);
}

/**
 * @brief Reset the instruction table widget except the "Initial values" row
 *        when an instruction is fetched as well as the description of the
 *        previous transfer statements from T3 to T6.
 */
void CPU16BitsController::resetTransferStatements(){
    for(int c = 1; c < instructionTableView.columnCount(); c++)
        //Ignore the initial value rows
        for(int r = 1; r < instructionTableView.rowCount(); r++)
            instructionTableView.item(r, c)->setText("");

    //Reset description of table view
    for(int i = 3; i < NUMBER_TRANSFER_STATEMENTS; i++)
        descriptionT[i] = "";

    //Reset the transfer statements
    //Note tht since T0, T1, T2 never change there is no need to reset them
    updateInstructionTableView(Row::T3, Column::T_S, "T3:");
    updateInstructionTableView(Row::T4, Column::T_S, "T4:");
    updateInstructionTableView(Row::T5, Column::T_S, "T5:");
    updateInstructionTableView(Row::T6, Column::T_S, "T6:");

}

///CRONTROLLER'S MEMORY REGISTER INSTRUCTIONS

/**
 * @brief Determines the exact memory reference instruction to bind.
 * @param leadingByte indicates what type of operation is needed.
 * @return the address of the delegate instruction function
 *         to execute.
 */
CPU16BitsController::DelegateInstruction CPU16BitsController::getMemoryReferenceFunction(const int& leadingByte){
    //Direct addressing or Indirect addressing - that is the question.
    indirectAddress = leadingByte > 0x7;

    switch(leadingByte){
    case 0x0: case 0x8:
        return &CPU16BitsDelegateInstructions::AND;
    case 0x1: case 0x9:
        return &CPU16BitsDelegateInstructions::ADD;
    case 0x2: case 0xA:
        return &CPU16BitsDelegateInstructions::LDA;
    case 0x3: case 0xB:
        return &CPU16BitsDelegateInstructions::STA;
    case 0x4: case 0xC:
        return &CPU16BitsDelegateInstructions::BUN;
    case 0x5: case 0xD:
        return &CPU16BitsDelegateInstructions::BSA;
    case 0x6: case 0xE:
        return &CPU16BitsDelegateInstructions::ISZ;
    default:
        //Brace yourself the NullPointerException is coming
        return nullptr;
    }
}

/**
 * @brief Determines the exact register reference instruction to bind.
 * @param instruction previously fetched from memory.
 * @return the address of the delegate instruction function
 *         to execute.
 */
CPU16BitsController::DelegateInstruction CPU16BitsController::getRegisterReferenceFunction(const int &instruction){
    switch(instruction){
    case 0x7800:
        return &CPU16BitsDelegateInstructions::CLA;
	case 0x7400:
        return &CPU16BitsDelegateInstructions::CLE;
	case 0x7200:
        return &CPU16BitsDelegateInstructions::CMA;
	case 0x7100:
        return &CPU16BitsDelegateInstructions::CME;
	case 0x7080:
        return &CPU16BitsDelegateInstructions::CIR;
	case 0x7040:
        return &CPU16BitsDelegateInstructions::CIL;
	case 0x7020:
        return &CPU16BitsDelegateInstructions::INC;
	case 0x7010:
        return &CPU16BitsDelegateInstructions::SPA;
	case 0x7008:
        return &CPU16BitsDelegateInstructions::SNA;
	case 0x7004:
        return &CPU16BitsDelegateInstructions::SZA;
	case 0x7002:
        return &CPU16BitsDelegateInstructions::SZE;
	case 0x7001:
        return &CPU16BitsDelegateInstructions::HLT;
	default:
        //Brace yourself the NullPointerException is coming
		return nullptr;
    }
}
