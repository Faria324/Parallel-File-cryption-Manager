#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <gtkmm.h>
#include <atomic>
#include <memory>
#include <chrono>

#include "../processes/ProcessManagement.hpp"
#include "../processes/Task.hpp"

class MainWindow : public Gtk::Window {
public:
    MainWindow();
    ~MainWindow() override;
    int userKey{0};

private:
    // =========================
    // Layout Containers
    // =========================
    Gtk::Box mainBox{Gtk::ORIENTATION_VERTICAL};


    std::ofstream logFile;

    // =========================
    // File chooser & input
    // =========================
    Gtk::FileChooserButton folderChooser;
    Gtk::Entry passwordEntry;

    // =========================
    // Action buttons
    // =========================
    Gtk::Button encryptButton;
    Gtk::Button decryptButton;
    Gtk::Button clearButton;

    // =========================
    // Progress & metrics
    // =========================
    Gtk::ProgressBar progressBar;

    Gtk::Grid metricsGrid;
    Gtk::Frame metricsFrame;

    // Metric labels (POINTERS — IMPORTANT)
    Gtk::Label* totalFilesLabel{nullptr};
    Gtk::Label* processedFilesLabel{nullptr};
    Gtk::Label* failedFilesLabel{nullptr};
    Gtk::Label* runtimeLabel{nullptr};
    Gtk::Label* throughputLabel{nullptr};
    Gtk::Label* memoryLabel{nullptr};

    // =========================
    // Log view
    // =========================
    Gtk::TextView logView;
    Glib::RefPtr<Gtk::TextBuffer> logBuffer;

    // =========================
    // Backend / processing
    // =========================
    ProcessManagement* processManager{nullptr};

    std::atomic<int> totalFiles{0};
    std::atomic<int> processedFiles{0};
    std::atomic<int> failedFiles{0};

    std::chrono::steady_clock::time_point startTime;

    // =========================
    // Helper methods
    // =========================
    Gtk::Label* create_metric(
        const std::string& icon,
        const std::string& text,
        int row
    );

    void apply_modern_theme();

    // Logging
    void append_log(const std::string& message);

    // UI updates
    void update_metrics();
    void update_progress();

    // Signal handlers
    void on_encrypt();
    void on_decrypt();
    void on_clear();

    // Task processing
    void run_tasks(const std::string& folder, bool encrypt);
};

#endif // MAINWINDOW_HPP
