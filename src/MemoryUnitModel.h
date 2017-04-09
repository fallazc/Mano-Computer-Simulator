#ifndef MEMORYUNITMODEL_H
#define MEMORYUNITMODEL_H

#include <QScopedArrayPointer>
#define MEMORY_UNIT_SIZE 4096

class MemoryUnitModel{
    friend class MemoryUnitController;
private:
    QScopedArrayPointer<quint16> RAM;

public:
    MemoryUnitModel() : RAM(new quint16[MEMORY_UNIT_SIZE]) {}
};
#endif // MEMORYUNITMODEL_H
