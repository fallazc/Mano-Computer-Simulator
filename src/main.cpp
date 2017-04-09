#include "ManoComputerSimulator.h"
#include <QApplication>

/**
 * @brief Main entry point for the application.
 * @param argc number of command line arguments.
 * @param argv c_string arguments.
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ManoComputerSimulator w;
    w.show();

    return a.exec();
}
