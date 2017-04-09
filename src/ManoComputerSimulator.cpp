#include "ManoComputerSimulator.h"
#include "ui_ManoComputerSimulator.h"

ManoComputerSimulator::ManoComputerSimulator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManoComputerSimulator)
{
    ui->setupUi(this);
    cpu16BitsController.reset(new CPU16BitsController(*ui->instructionCycleTableWidget, *ui->memoryTableWidget, *ui->descriptionArea));
}

ManoComputerSimulator::~ManoComputerSimulator(){}

/**
 * @brief Event handler for the "Fetch Instruction" button.
 */
void ManoComputerSimulator::on_fetchInstructionBtn_clicked()
{
    cpu16BitsController->fetch();
}

/**
 * @brief Event handler for the "Decode Instruction" button.
 */
void ManoComputerSimulator::on_decodeInstructionBtn_clicked()
{
    cpu16BitsController->decode((QWidget*)this);
}

/**
 * @brief Event handler for the "Execute Instruction" button.
 */
void ManoComputerSimulator::on_executeInstructionBtn_clicked(){
   cpu16BitsController->execute((QWidget*)this);
   cpu16BitsController->updateDescriptionTextEdit(ui->descriptionOfComboBox->currentIndex());
}

/**
 * @brief Event handler for the "Description of" combox box.
 * @param step indicates the transfer statement whose
 *        description will be displayed.
 */
void ManoComputerSimulator::on_descriptionOfComboBox_activated(int step){
    cpu16BitsController->updateDescriptionTextEdit(step);
}

