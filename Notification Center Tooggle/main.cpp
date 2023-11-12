#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class MyTrayApp : public QApplication {
public:
    MyTrayApp(int argc, char *argv[]) : QApplication(argc, argv) {
        // Create the system tray icon
        trayIcon = new QSystemTrayIcon(QIcon::fromTheme("org.gnome.Settings-notifications"), this);

        // Create a context menu
        QMenu* contextMenu = new QMenu();

        // Create a menu item
        QAction* runCommandAction = new QAction("Run Command", this);
        connect(runCommandAction, &QAction::triggered, this, &MyTrayApp::executeCommand);

        // Add the menu item to the context menu
        contextMenu->addAction(runCommandAction);

        // Set the context menu for the system tray icon
        trayIcon->setContextMenu(contextMenu);

        // Connect the activated signal to executeCommand
        connect(trayIcon, &QSystemTrayIcon::activated, this, &MyTrayApp::executeCommand);

        // Show the system tray icon
        trayIcon->show();
    }

private slots:
    void executeCommand() {
        // Replace "your_command_here" with the actual command you want to run
        system("kill -s USR1 $(pidof deadd-notification-center)");
    }

private:
    QSystemTrayIcon* trayIcon;
};

int main(int argc, char *argv[]) {
    MyTrayApp app(argc, argv);

    return app.exec();
}
