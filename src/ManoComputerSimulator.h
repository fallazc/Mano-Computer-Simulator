#ifndef MANOCOMPUTERSIMULATOR_H
#define MANOCOMPUTERSIMULATOR_H

#include <QMainWindow>
#include <QTableWidget>
#include "Utils.h"
#include "CPU16BitsController.h"
#include "MemoryUnitController.h"

namespace Ui {
class ManoComputerSimulator;
}

class ManoComputerSimulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManoComputerSimulator(QWidget *parent = 0);
    ~ManoComputerSimulator();

    //Event handlers(listeners)
private slots:
    void on_descriptionOfComboBox_activated(int step);
    void on_fetchInstructionBtn_clicked();
    void on_decodeInstructionBtn_clicked();
    void on_executeInstructionBtn_clicked();

private:
    //Controllers
    QScopedPointer<CPU16BitsController> cpu16BitsController;
    //ui holds all the views(UI objects)
    QSharedPointer<Ui::ManoComputerSimulator> ui;
    //Memory unit table view
};
#endif // MANOCOMPUTERSIMULATOR_H
