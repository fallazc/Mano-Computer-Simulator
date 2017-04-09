#include "MemoryUnitController.h"

MemoryUnitController::MemoryUnitController(QTableWidget &table)
    : memoryUnitModel(new MemoryUnitModel),
      memoryUnitTableView(table)
{
    //initMemoryUnitView();
    initializeMemoryUnitModel();
}

/**
 * @brief Helper method to initialize and fill the memory unit (model + view)
 *        with randomly generated instructions.
 */
void MemoryUnitController::initializeMemoryUnitModel(){
    int location;
    //Functors declaration
    randomNumberGenerator randomLeadingByte(0x0, 0xE);
    randomNumberGenerator randomMemoryAddress(0x0, 0xFFF);
    randomNumberGenerator randomRegisterInstruction(0x0, 0xB);
    //Register reference instructions
    int registerInstructions[0xC] = { 0x800, 0x400, 0x200, 0x100, 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1 };

    //Initialize the address section of the memory unit view
    QStringList memoryLabels;
    for (location = 0; location < MEMORY_UNIT_SIZE; location++){
        memoryLabels.append(intToQString(location, 3));
        memoryUnitTableView.setItem(location, 0, new QTableWidgetItem);
    }

    memoryUnitTableView.setVerticalHeaderLabels(memoryLabels);

    //Fill memory unit with random instructions
    for(location = 0x0; location < MEMORY_UNIT_SIZE; location++){
        /*Generate a number(0-14) and shift it 3 hexadecimal places to the left
         *to make it the leading hexadecimal(4 bits).
         *0xC or 12(offset in bits) = 4(hexadecimal size in bits) * 3(number of places)
         */
        int instruction = randomLeadingByte() << 0xC;

        if(instruction == 0x7000)
            instruction += registerInstructions[randomRegisterInstruction()];
        else
            instruction += randomMemoryAddress();

        //Update view and model
        write(location, instruction);
    }
}
