#include "CPU16BitsDelegateInstructions.h"

/**
 * @brief Execute indirect memory addressing (transfer statement 3)
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::runT3(CPU16BitsController &controller) {
    //Update registers
    controller.writeToRegister(Row::T3, Column::AR, InputControl::LD, controller.getDataAtAR());
    //Update transfer statement
    controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: AR <- M[AR]"));
    //Update description of T3
    controller.descriptionT[3] = QString("s0s1s2 = %1\nMEMORY_UNIT.READ <- 1\nAR.LD <- 1")
                                .arg(intToQString((int)BusIDs::MEMORY_UNIT, 3, 2));
    qDebug(intToQString((int)BusIDs::MEMORY_UNIT, 3, 2).toLatin1());
}

///CPU CONTROLLER'S MEMORY REFERENCE INSTRUCTION

/**
 * @brief And memory word to AC.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::AND(CPU16BitsController &controller){
    //Update transfer statements 4 and its description
    t4DRgetsDataAtAR(controller);
    //Update transfer statements 5 and its description
    controller.cpu16BitsModel->logicUnitAND();
	controller.updateInstructionTableView(Row::T5, Column::AC, intToQString(controller.cpu16BitsModel->getAC(), 4));
    controller.updateInstructionTableView(Row::T5, Column::T_S, QString("T5: AC <- AC ^ DR, SC <- 0"));
    controller.descriptionT[5] = QString("Logic.XOR <- 1, SC <- 0");
}

/**
 * @brief Add memory word to AC.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::ADD(CPU16BitsController &controller){
    //Update transfer statements 4 and its description
     t4DRgetsDataAtAR(controller);
     //Update transfer statements 5 and its description
     controller.cpu16BitsModel->arithmeticUnitADD();
     controller.updateInstructionTableView(Row::T5, Column::AC, intToQString(controller.cpu16BitsModel->getAC(), 4));
	 controller.updateInstructionTableView(Row::T5, Column::E, intToQString(controller.cpu16BitsModel->getE()));
     controller.updateInstructionTableView(Row::T5, Column::T_S, QString("T5: AC <- AC + DR, E<- Cout, SC <- 0"));
     controller.descriptionT[5] = QString("Adder.ADD <- 1, SC <- 0");
}

/**
 * @brief Load memory word to AC.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::LDA(CPU16BitsController &controller){
    //Update transfer statements 4 and its description
     t4DRgetsDataAtAR(controller);
     //Update transfer statements 5 and its description
     controller.writeToRegister(Row::T5, Column::AC, InputControl::LD, controller.cpu16BitsModel->getDR());
     controller.updateInstructionTableView(Row::T5, Column::T_S, QString("T5: AC <- DR, SC <- 0"));
     controller.descriptionT[5] = QString("s0s1s2 = %1\nAC.LD <- 1").arg(intToQString((int)BusIDs::DR, 3, 2));
}

/**
 * @brief Store content of AC in memory.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::STA(CPU16BitsController &controller){
    //Update transfer statements 4 and its description
    controller.writeToMAR(Row::T4, controller.cpu16BitsModel->getAC());
    controller.updateInstructionTableView(Row::T4, Column::T_S, QString("T4: M[AR] <- AC, SC <- 0"));
    controller.descriptionT[4] = QString("s0s1s2 = %1\nMEMORY_UNIT.WRITE <- 1")
                                .arg(intToQString((int)BusIDs::AC, 3, 2));
}

/**
 * @brief Branch unconditionally.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::BUN(CPU16BitsController &controller){
    //Update transfer statements 4 and its description
    controller.writeToRegister(Row::T4, Column::PC, InputControl::LD, controller.cpu16BitsModel->getAR());
    controller.updateInstructionTableView(Row::T4, Column::T_S, QString("T4: PC <- AR, SC <- 0"));
    controller.descriptionT[4] = QString("s0s1s2 = %1\nPC.LD <- 1").arg(intToQString((int)BusIDs::AR, 3, 2));
}

/**
 * @brief Branch and save return address.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::BSA(CPU16BitsController &controller){
    //Update transfer statements 4 and its description
    controller.writeToMAR(Row::T4, controller.cpu16BitsModel->getPC());
    controller.writeToRegister(Row::T4, Column::AR, InputControl::INR);
    controller.updateInstructionTableView(Row::T4, Column::T_S, QString("T4: M[AR] <- PC, AR <- AR + 1"));
    controller.descriptionT[4] = QString("s0s1s2 = %1\nMEMORY_UNIT.WRITE <- 1, AR.INR <- 1")
                                        .arg(intToQString((int)BusIDs::PC, 3, 2));

    //Update transfer statements 5 and its description
    controller.writeToRegister(Row::T5, Column::PC, InputControl::LD, controller.cpu16BitsModel->getAR());
    controller.updateInstructionTableView(Row::T5, Column::T_S, QString("T5: PC <- AR, SC <- 0"));
    controller.descriptionT[5] = QString("s0s1s2 = %1\nPC.LD <- 1").arg(intToQString((int)BusIDs::AR, 3, 2));
}

/**
 * @brief Increment and skip if zero.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::ISZ(CPU16BitsController &controller){
    //Update transfer statements 4 and its description
    t4DRgetsDataAtAR(controller);

    //Update transfer statements 5 and its description
    controller.writeToRegister(Row::T5, Column::DR, InputControl::INR);
    controller.updateInstructionTableView(Row::T5, Column::T_S, QString("T5: DR <- DR + 1"));
    controller.descriptionT[5] = QString("DR.INR <- 1");

    //Update transfer statements 6 and its description
    controller.writeToMAR(Row::T6, controller.cpu16BitsModel->getDR());
    controller.updateInstructionTableView(Row::T6, Column::T_S, QString("T6: M[AR] <- DR, if(DR==0)then(PC<-PC+1), SC <- 0"));
    controller.descriptionT[6] = QString("s0s1s2 = %1\nMEMORY_UNIT.WRITE <- 1")
                                        .arg(intToQString((int)BusIDs::DR, 3, 2));
    if(controller.cpu16BitsModel->getDR() == 0){
        controller.writeToRegister(Row::T5, Column::PC, InputControl::INR);
        controller.descriptionT[6].append(", PC.INR <- 1");
    }
    //SC <- 0
}

///CPU CONTROLLER'S REGISTER REFERENCE INSTRUCTION

/**
 * @brief Clear AC.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::CLA(CPU16BitsController &controller) {
	//Update transfer statements 3 and its description
    controller.writeToRegister(Row::T3, Column::AC, InputControl::CLR);
    controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: AC <- 0"));
    controller.descriptionT[3] = QString("AC.CLR <- 1");
}

/**
 * @brief Clear E.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::CLE(CPU16BitsController &controller) {
	//Update transfer statements 3 and its description
    controller.writeToRegister(Row::T3, Column::E, InputControl::CLR);
    controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: E <- 0"));
    controller.descriptionT[3] = QString("Adder_Logic.E <- 0");
}

/**
 * @brief Complement AC.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::CMA(CPU16BitsController &controller) {
	//Update transfer statements 3 and its description
    controller.cpu16BitsModel->logicUnitCMA();
    controller.writeToRegister(Row::T3, Column::AC, InputControl::LD, controller.cpu16BitsModel->getAC());
    controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: AC <- !AC"));
    controller.descriptionT[3] = QString("Logic.CMA <- 1");
}

/**
 * @brief Complement E.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::CME(CPU16BitsController &controller) {
	//Update transfer statements 3 and its description
    controller.cpu16BitsModel->logicUnitCME();
    controller.writeToRegister(Row::T3, Column::E, InputControl::LD, controller.cpu16BitsModel->getE());
	controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: E <- !E"));
    controller.descriptionT[3] = QString("Logic.CME <- 1");
}

/**
 * @brief Circulate right AC and E.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::CIR(CPU16BitsController &controller) {
	//Update transfer statements 3 and its description
    controller.cpu16BitsModel->arithmeticUnitCIR();
	controller.updateInstructionTableView(Row::T3, Column::AC, intToQString(controller.cpu16BitsModel->getAC(), 4));
    controller.updateInstructionTableView(Row::T3, Column::E, intToQString(controller.cpu16BitsModel->getE()));
	controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: AC <- shr AC, AC(15) <- E, E <- AC(0)"));
    controller.descriptionT[3] = QString("adder.CIR <- 1");
}

/**
 * @brief Circulate left AC and E.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::CIL(CPU16BitsController &controller) {
	//Update transfer statements 3 and its description
    controller.cpu16BitsModel->arithmeticUnitCIL();
	controller.updateInstructionTableView(Row::T3, Column::AC, intToQString(controller.cpu16BitsModel->getAC(), 4));
    controller.updateInstructionTableView(Row::T3, Column::E, intToQString(controller.cpu16BitsModel->getE()));
	controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: AC <- shl AC, AC(0) <- E, E <- AC(15)"));
    controller.descriptionT[3] = QString("adder.CIL <- 1");
}

/**
 * @brief Increment AC.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::INC(CPU16BitsController &controller) {
	//Update transfer statements 3 and its description
	controller.writeToRegister(Row::T3, Column::AC, InputControl::INR);
	controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: AC <- AC + 1"));
    controller.descriptionT[3] = QString("AC.INR <- 1");
}

/**
 * @brief Skip next instruction if AC is positive.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::SPA(CPU16BitsController &controller) {
    controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: if(AR(15) == 0)then PC <- PC + 1"));
	if (controller.cpu16BitsModel->getACleadingBit() == 0) {
		//Update transfer statements 3 and its description
		controller.writeToRegister(Row::T3, Column::PC, InputControl::INR);
		controller.updateInstructionTableView(Row::T3, Column::PC, intToQString(controller.cpu16BitsModel->getPC(), 3));
        controller.descriptionT[3] = QString("PC.INR <- 1");
	}
}

/**
 * @brief Skip next instruction if AC is negative.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::SNA(CPU16BitsController &controller) {
    controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: if(AR(15) == 1)then PC <- PC + 1"));
	if (controller.cpu16BitsModel->getACleadingBit() == 1) {
		//Update transfer statements 3 and its description
		controller.writeToRegister(Row::T3, Column::PC, InputControl::INR);
		controller.updateInstructionTableView(Row::T3, Column::PC, intToQString(controller.cpu16BitsModel->getPC(), 3));
        controller.descriptionT[3] = QString("PC.INR <- 1");
	}
}

/**
 * @brief Skip next instruction if AC is zero.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::SZA(CPU16BitsController &controller) {
    controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: if(AC == 0)then PC <- PC + 1"));
	if (controller.cpu16BitsModel->getAC() == 0) {
		//Update transfer statements 3 and its description
		controller.writeToRegister(Row::T3, Column::PC, InputControl::INR);
		controller.updateInstructionTableView(Row::T3, Column::PC, intToQString(controller.cpu16BitsModel->getPC(), 3));
        controller.descriptionT[3] = QString("PC.INR <- 1");
	}
}

/**
 * @brief Skip next instruction if E is zero.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::SZE(CPU16BitsController &controller) {
    controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: if(E == 0)then PC <- PC + 1"));
	if (controller.cpu16BitsModel->getE() == 0) {
		//Update transfer statements 3 and its description
		controller.writeToRegister(Row::T3, Column::PC, InputControl::INR);
		controller.updateInstructionTableView(Row::T3, Column::PC, intToQString(controller.cpu16BitsModel->getPC(), 3));
        controller.descriptionT[3] = QString("PC.INR <- 1");
	}
}

/**
 * @brief Halt computer.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::HLT(CPU16BitsController &controller) {
	controller.updateInstructionTableView(Row::T3, Column::T_S, QString("T3: S <- 0"));
}

///HELPER METHODS

/**
 * @brief Helper method for M[AR]
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::t4DRgetsDataAtAR(CPU16BitsController &controller){
    controller.writeToRegister(Row::T4, Column::DR, InputControl::LD, controller.getDataAtAR());
    controller.updateInstructionTableView(Row::T4, Column::T_S, QString("T4: DR <- M[AR]\n"));
    controller.descriptionT[4] = QString("s0s1s2 = %1\nMEMORY_UNIT.READ <- 1\nDR.LD <- 1")
                                .arg(intToQString((int)BusIDs::MEMORY_UNIT, 3, 2));
    //SC <- 0
}

/**
 * @brief Helper method to increment the program counter register.
 * @param controller object that need to be updated(view + model).
 */
void CPU16BitsDelegateInstructions::incrementPCatStep(CPU16BitsController &controller, Row step){
    controller.writeToRegister(step, Column::PC, InputControl::INR);
    controller.updateInstructionTableView(step, Column::T_S, QString("T%1: PC <- PC + 1").arg(intToQString((int)step, 1, 10)));
    controller.descriptionT[(int)step] = QString("PC.INR <- 1");
}
