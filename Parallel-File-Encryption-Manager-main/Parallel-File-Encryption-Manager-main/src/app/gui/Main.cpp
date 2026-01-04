#include <gtkmm.h>
#include "MainWindow.hpp"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "com.encrypty.gui");
    MainWindow window;
    return app->run(window);
}
