#ifndef MEMORYUNITCONTROLLER_H
#define MEMORYUNITCONTROLLER_H

#include <QString>
#include <QTableWidget>
#include "Utils.h"
#include "MemoryUnitModel.h"

class MemoryUnitController{

public:
    MemoryUnitController(QTableWidget &table);

    inline int read(const int& location){
        return memoryUnitModel->RAM[location];
    }

    inline void write(const int& location, const int& data){
        memoryUnitModel->RAM[location] = data;
        memoryUnitTableView.item(location, 0)->setText(intToQString(data, 4));
    }

private:
    QScopedPointer<MemoryUnitModel> memoryUnitModel;
    QTableWidget &memoryUnitTableView;
    void initializeMemoryUnitModel();
    void initMemoryUnitView();
};

#endif // MEMORYUNITCONTROLLER_H
