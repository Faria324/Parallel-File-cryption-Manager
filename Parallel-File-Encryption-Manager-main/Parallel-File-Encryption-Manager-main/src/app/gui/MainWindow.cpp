#include "MainWindow.hpp"
#include <filesystem>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/sysinfo.h>

namespace fs = std::filesystem;

MainWindow::MainWindow()
: folderChooser("Select Folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER) {
    set_title("Secure File Processor");
    set_default_size(1000, 700);
    
    // Apply modern clean theme
    apply_modern_theme();
    
    // Main container setup
    add(mainBox);
    mainBox.set_spacing(12);
    mainBox.set_border_width(16);
    
    // ========== HEADER SECTION ==========
    Gtk::Box* headerBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 8);
    headerBox->set_halign(Gtk::ALIGN_CENTER);
    
    Gtk::Label* titleLabel = Gtk::make_managed<Gtk::Label>("Parallel File Encryption Manager");
    Pango::FontDescription titleFont;
    titleFont.set_family("Sans");
    titleFont.set_weight(Pango::WEIGHT_BOLD);
    titleFont.set_size(20 * PANGO_SCALE);
    titleLabel->override_font(titleFont);
    titleLabel->override_color(Gdk::RGBA("#2c3e50"));
    
    Gtk::Label* subtitleLabel = Gtk::make_managed<Gtk::Label>("Encrypt and decrypt files securely");
    subtitleLabel->override_color(Gdk::RGBA("#7f8c8d"));
    
    headerBox->pack_start(*titleLabel, Gtk::PACK_SHRINK);
    headerBox->pack_start(*subtitleLabel, Gtk::PACK_SHRINK);
    mainBox.pack_start(*headerBox, Gtk::PACK_SHRINK);
    
    // ========== CONFIGURATION CARD ==========
    Gtk::Frame* configFrame = Gtk::make_managed<Gtk::Frame>();
    configFrame->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
    Gtk::Box* configBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 16);
    configBox->set_border_width(20);
    
    // Folder selection
Gtk::Box* folderBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 12);

Gtk::Label* folderTitleLabel = Gtk::make_managed<Gtk::Label>("Source Folder");
folderTitleLabel->set_xalign(0.0f);
folderTitleLabel->set_size_request(150, -1);

Pango::FontDescription sectionFont;
sectionFont.set_weight(Pango::WEIGHT_SEMIBOLD);
folderTitleLabel->override_font(sectionFont);

//REQUIRED for folder selection
folderChooser.set_action(Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

// Optional but recommended
folderChooser.set_hexpand(true);
folderChooser.set_tooltip_text("Select folder containing files to process");
folderChooser.set_title("Select Source Folder");

// Pack
folderBox->pack_start(*folderTitleLabel, Gtk::PACK_SHRINK);
folderBox->pack_start(folderChooser, Gtk::PACK_EXPAND_WIDGET);

configBox->pack_start(*folderBox, Gtk::PACK_SHRINK);


// ================== Key input ==================
Gtk::Box* keyBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 12);

// Label
Gtk::Label* keyTitleLabel = Gtk::make_managed<Gtk::Label>("Encryption Key");
keyTitleLabel->set_xalign(0);
keyTitleLabel->set_size_request(150, -1);
keyTitleLabel->override_font(sectionFont);

// Password entry
passwordEntry.set_visibility(false);
passwordEntry.set_hexpand(true);
passwordEntry.set_placeholder_text("Enter secure key...");
passwordEntry.set_tooltip_text("Enter key for encryption/decryption");

passwordEntry.get_style_context()->add_class("entry");
passwordEntry.get_style_context()->add_class("entry-secondary");

// Initial icon: locked (password hidden)
passwordEntry.set_icon_from_icon_name("dialog-password-symbolic", Gtk::ENTRY_ICON_SECONDARY);

// Toggle visibility when icon is clicked
passwordEntry.signal_icon_press().connect(
    [this](Gtk::EntryIconPosition pos, const GdkEventButton*) {
        if (pos == Gtk::ENTRY_ICON_SECONDARY) {
            bool visible = passwordEntry.get_visibility();

            // Toggle visibility
            passwordEntry.set_visibility(!visible);

            // Toggle icon: lock → unlocked
            passwordEntry.set_icon_from_icon_name(
                visible ? "dialog-password-symbolic" : "dialog-password-symbolic",
                Gtk::ENTRY_ICON_SECONDARY
            );
        }
    });


// Pack widgets
keyBox->pack_start(*keyTitleLabel, Gtk::PACK_SHRINK);
keyBox->pack_start(passwordEntry, Gtk::PACK_EXPAND_WIDGET);

// Add to config box
configBox->pack_start(*keyBox, Gtk::PACK_SHRINK);
// ===============================================

    // Action buttons
    Gtk::Box* buttonBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 12);
    buttonBox->set_halign(Gtk::ALIGN_CENTER);
    buttonBox->set_margin_top(10);
    
    // Encrypt button
    encryptButton.set_label("ENCRYPT");
    encryptButton.set_tooltip_text("Encrypt all supported files");
    encryptButton.get_style_context()->add_class("suggested-action");
    encryptButton.set_size_request(120, 40);
    
    // Decrypt button
    decryptButton.set_label("DECRYPT");
    decryptButton.set_tooltip_text("Decrypt all supported files");
    decryptButton.set_size_request(120, 40);
    
    // Clear button
    clearButton.set_label("CLEAR");
    clearButton.set_tooltip_text("Reset all fields");
    clearButton.set_size_request(120, 40);
    
    buttonBox->pack_start(encryptButton, Gtk::PACK_SHRINK);
    buttonBox->pack_start(decryptButton, Gtk::PACK_SHRINK);
    buttonBox->pack_start(clearButton, Gtk::PACK_SHRINK);
    configBox->pack_start(*buttonBox, Gtk::PACK_SHRINK);
    
    configFrame->add(*configBox);
    mainBox.pack_start(*configFrame, Gtk::PACK_SHRINK);
    
    // ========== PROGRESS SECTION ==========
    Gtk::Frame* progressFrame = Gtk::make_managed<Gtk::Frame>();
    progressFrame->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
    Gtk::Box* progressBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 8);
    progressBox->set_border_width(16);
    
    Gtk::Label* progressTitleLabel = Gtk::make_managed<Gtk::Label>("Processing Progress");
    progressTitleLabel->set_xalign(0);
    progressTitleLabel->override_font(sectionFont);
    
    progressBar.set_show_text(true);
    progressBar.set_text("Ready");
    progressBar.set_hexpand(true);
    progressBar.set_margin_top(8);
    
    progressBox->pack_start(*progressTitleLabel, Gtk::PACK_SHRINK);
    progressBox->pack_start(progressBar, Gtk::PACK_SHRINK);
    progressFrame->add(*progressBox);
    mainBox.pack_start(*progressFrame, Gtk::PACK_SHRINK);
    
    // ========== METRICS SECTION ==========
    Gtk::Frame* metricsFrame = Gtk::make_managed<Gtk::Frame>();
    metricsFrame->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
    Gtk::Grid* metricsGrid = Gtk::make_managed<Gtk::Grid>();
    metricsGrid->set_border_width(16);
    metricsGrid->set_row_spacing(12);
    metricsGrid->set_column_spacing(24);
    
    // Create metric labels with icons
    auto create_metric = [&](const std::string& iconName, const std::string& text, int row) -> Gtk::Label& {
    Gtk::Box* metricBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 8);

    Gtk::Image* iconImg = Gtk::make_managed<Gtk::Image>();
    iconImg->set_from_icon_name(iconName, Gtk::ICON_SIZE_BUTTON); // safe way
    iconImg->set_pixel_size(20);

    Gtk::Label* label = Gtk::make_managed<Gtk::Label>(text);
    label->set_xalign(0);

    metricBox->pack_start(*iconImg, Gtk::PACK_SHRINK);
    metricBox->pack_start(*label, Gtk::PACK_SHRINK);

    metricsGrid->attach(*metricBox, row % 3, row / 3, 1, 1);

    return *label;
};
totalFilesLabel     = &create_metric("text-x-generic-symbolic", "Total: 0", 0);
processedFilesLabel = &create_metric("emblem-ok-symbolic", "Processed: 0", 1);
failedFilesLabel    = &create_metric("task-due-symbolic", "Failed: 0", 2);
runtimeLabel        = &create_metric("appointment-new-symbolic", "Runtime: 0s", 3);
throughputLabel     = &create_metric("system-run-symbolic", "Throughput: 0/s", 4);
memoryLabel         = &create_metric("drive-harddisk-symbolic", "Memory: 0 MB", 5);

metricsFrame->add(*metricsGrid);
mainBox.pack_start(*metricsFrame, Gtk::PACK_SHRINK);


    
    // ========== LOG SECTION ==========
    Gtk::Frame* logFrame = Gtk::make_managed<Gtk::Frame>();
    logFrame->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
    Gtk::Box* logHeaderBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 8);
    logHeaderBox->set_border_width(8);
    
    Gtk::Label* logTitleLabel = Gtk::make_managed<Gtk::Label>("Activity Log");
    logTitleLabel->set_xalign(0);
    logTitleLabel->override_font(sectionFont);
    
    logHeaderBox->pack_start(*logTitleLabel, Gtk::PACK_EXPAND_WIDGET);
    logFrame->set_label_widget(*logHeaderBox);
    
    // Log buffer setup
    logBuffer = Gtk::TextBuffer::create();
    logView.set_buffer(logBuffer);
    logView.set_editable(false);
    logView.set_wrap_mode(Gtk::WRAP_WORD_CHAR);
    logView.set_pixels_above_lines(4);
    logView.set_pixels_below_lines(4);
    logView.set_left_margin(8);
    logView.set_right_margin(8);
    logView.set_top_margin(8);
    logView.set_bottom_margin(8);
    
    Gtk::ScrolledWindow* scrollWin = Gtk::make_managed<Gtk::ScrolledWindow>();
    scrollWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    scrollWin->set_min_content_height(150);
    scrollWin->add(logView);
    
    logFrame->add(*scrollWin);
    mainBox.pack_start(*logFrame, Gtk::PACK_EXPAND_WIDGET);
    
    // ========== SIGNAL CONNECTIONS ==========
    encryptButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_encrypt));
    decryptButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_decrypt));
    clearButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_clear));
    
    folderChooser.signal_selection_changed().connect([this]() {
        std::string folder = folderChooser.get_filename();
        if (!folder.empty()) {
            append_log("Folder selected: " + folder);
        }
    });
    
    // Initial log message
    append_log("Application started");
    append_log("Select a folder and enter encryption key to begin");
    
    show_all_children();

    // Open log file in append mode
logFile.open("application_log.txt", std::ios::app);
if (!logFile.is_open()) {
    append_log("Error: Failed to open log file for writing");
}

}

MainWindow::~MainWindow() {
    if (processManager) delete processManager;
    
    if (logFile.is_open()) {
        logFile.close();
    }
}

void MainWindow::append_log(const std::string &message) {
    auto time = std::time(nullptr);
    auto localTime = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(localTime, "%H:%M:%S");

    Glib::ustring timestamp = "<span foreground='#7f8c8d' size='small'>" + ss.str() + "</span>";
    Glib::ustring logMessage = message;

    // Color code messages for UI
    if (message.find("Error") != std::string::npos) {
        logMessage = "<span foreground='#e74c3c'>" + logMessage + "</span>";
    } else if (message.find("Starting") != std::string::npos || 
               message.find("completed") != std::string::npos) {
        logMessage = "<span foreground='#27ae60'>" + logMessage + "</span>";
    } else if (message.find("Submitted") != std::string::npos) {
        logMessage = "<span foreground='#3498db'>" + logMessage + "</span>";
    }

    // **Thread-safe insertion in GTK**
    Glib::signal_idle().connect_once([this, timestamp, logMessage]() {
        auto buffer = logBuffer;
        buffer->insert_markup(buffer->end(), timestamp + "  " + logMessage + "\n");

        // Auto-scroll
        auto mark = buffer->create_mark(buffer->end());
        logView.scroll_to(mark);
    });

    // ✅ Write log to file
    if (logFile.is_open()) {
        logFile << ss.str() << "  " << message << std::endl;
        logFile.flush(); // ensure it's written immediately
    }
}

void MainWindow::update_metrics() {
    totalFilesLabel->set_text("Total: " + std::to_string(totalFiles.load()));
    processedFilesLabel->set_text("Processed: " + std::to_string(processedFiles.load()));
    failedFilesLabel->set_text("Failed: " + std::to_string(failedFiles.load()));

    auto now = std::chrono::steady_clock::now();
    double seconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() / 1000.0;

    runtimeLabel->set_text("Runtime: " + std::to_string(seconds).substr(0, 4) + "s");

    double throughput = seconds > 0
        ? (processedFiles.load() + failedFiles.load()) / seconds
        : 0;

    throughputLabel->set_text("Throughput: " + std::to_string((int)throughput) + "/s");

    struct sysinfo info;
    sysinfo(&info);
    long memMB = (info.totalram - info.freeram) / 1024 / 1024;
    memoryLabel->set_text("Memory: " + std::to_string(memMB) + " MB");
}

void MainWindow::update_progress() {
    double fraction = 0.0;
    if (totalFiles > 0) {
        fraction = static_cast<double>(processedFiles.load() + failedFiles.load()) / totalFiles.load();
    }
    
    progressBar.set_fraction(fraction);
    
    // Update progress text
    if (fraction == 0.0) {
        progressBar.set_text("Ready");
    } else if (fraction < 1.0) {
        int percent = static_cast<int>(fraction * 100);
        progressBar.set_text("Processing: " + std::to_string(percent) + "%");
    } else {
        progressBar.set_text("Completed!");
    }

    update_metrics();
}

void MainWindow::on_clear() {
    logBuffer->set_text("");
    passwordEntry.set_text("");
    folderChooser.unselect_all();
    progressBar.set_fraction(0.0);
    progressBar.set_text("Ready");
    processedFiles = 0;
    failedFiles = 0;
    totalFiles = 0;
    
    // Reset metric labels
    totalFilesLabel->set_text("Total: 0");
    processedFilesLabel->set_text("Processed: 0");
    failedFilesLabel->set_text("Failed: 0");
    runtimeLabel->set_text("Runtime: 0s");
    throughputLabel->set_text("Throughput: 0/s");
    memoryLabel->set_text("Memory: 0 MB");

    if (processManager) {
        delete processManager;
        processManager = nullptr;
    }
    
    append_log("Cleared all settings and logs");
}

void MainWindow::on_encrypt() {
    std::string folder = folderChooser.get_filename();
    std::string key = passwordEntry.get_text();
    
    if (folder.empty()) {
        append_log("Error: Please select a folder first");
        return;
    }
    
    if (key.empty()) {
        append_log("Error: Please enter an encryption key");
        return;
    }
    
    append_log("Starting encryption for folder: " + folder);
    run_tasks(folder, true);
}

void MainWindow::on_decrypt() {
    std::string folder = folderChooser.get_filename();
    std::string key = passwordEntry.get_text();
    
    if (folder.empty()) {
        append_log("Error: Please select a folder first");
        return;
    }
    
    if (key.empty()) {
        append_log("Error: Please enter a decryption key");
        return;
    }
    
    append_log("Starting decryption for folder: " + folder);
    run_tasks(folder, false);
}

void MainWindow::run_tasks(const std::string& folder, bool encrypt) {
    if (!fs::exists(folder) || !fs::is_directory(folder)) {
        append_log("Error: Invalid folder path");
        return;
    }

    // Scan folder for supported files
    int count = 0;
    std::vector<std::string> fileList;

    try {
        for (auto& entry : fs::recursive_directory_iterator(folder)) {
            if (!entry.is_regular_file()) continue;
            auto ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            if (ext == ".txt" || ext == ".log" || ext == ".csv" || ext == ".pdf") {
                count++;
                fileList.push_back(entry.path().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        append_log("Error accessing folder: " + std::string(e.what()));
        return;
    }

    if (count == 0) {
        append_log("No supported files found (.txt, .log, .csv, .pdf)");
        return;
    }

    totalFiles = count;
    processedFiles = 0;
    failedFiles = 0;

    append_log("Found " + std::to_string(count) + " file(s) to process");
    append_log(encrypt ? "Starting encryption..." : "Starting decryption...");

    // Read user key from entry
    try {
        userKey = std::stoi(passwordEntry.get_text());
    } catch (...) {
        append_log("Invalid key entered.");
        return;
    }

    // Reset previous manager
    if (processManager) delete processManager;
    processManager = new ProcessManagement(std::min(static_cast<unsigned int>(count), std::thread::hardware_concurrency()));
    processManager->userKey = userKey;  // <<< PASS KEY HERE

    startTime = std::chrono::steady_clock::now();

    // Setup task completion callback
    processManager->onTaskComplete = [this](bool success) {
        if (success) {
            processedFiles++;
            append_log("Task succeeded");
        } else {
            failedFiles++;
            append_log("Task failed");
        }
        // Update progress safely in main GTK thread
        Glib::signal_idle().connect_once(sigc::mem_fun(*this, &MainWindow::update_progress));
    };

    // Submit tasks to the queue
    for (const auto& filePath : fileList) {
        Action taskAction = encrypt ? Action::ENCRYPT : Action::DECRYPT;
        auto task = std::make_unique<Task>(filePath, taskAction);

        if (!processManager->submitToQueue(std::move(task))) {
            append_log("Queue full. Skipping: " + fs::path(filePath).filename().string());
            failedFiles++;
        } else {
            append_log("Submitted: " + fs::path(filePath).filename().string());
        }
    }

    // Disable buttons while processing
    encryptButton.set_sensitive(false);
    decryptButton.set_sensitive(false);
    clearButton.set_sensitive(false);

    // Progress monitoring thread
    std::thread([this]() {
        while (processManager->hasPendingTasks() || processedFiles + failedFiles < totalFiles) {
            Glib::signal_idle().connect_once(sigc::mem_fun(*this, &MainWindow::update_progress));
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        // Final update
        Glib::signal_idle().connect_once(sigc::mem_fun(*this, &MainWindow::update_progress));

        // Re-enable buttons and log completion
        Glib::signal_idle().connect_once([this]() {
            encryptButton.set_sensitive(true);
            decryptButton.set_sensitive(true);
            clearButton.set_sensitive(true);
            append_log("All tasks completed!");
            append_log("Successfully processed: " + std::to_string(processedFiles) +
                       ", Failed: " + std::to_string(failedFiles));
        });
    }).detach();
}

void MainWindow::apply_modern_theme() {
    auto cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_data(R"(
        /* =========================
           Main window
        ========================== */
    
            window {
    background-color: rgba(117, 177, 217, 1.0);
    font-family: Sans;
}


        /* =========================
           Labels
        ========================== */
        label {
            color: #2c3e50;
        }

        /* =========================
           Frames
        ========================== */
        frame {
            border-radius: 8px;
            border: 1px solid #dee2e6;
            background-color: #ffffff;
        }

        frame > border {
            border-radius: 8px;
            border: 1px solid #dee2e6;
        }

        /* =========================
           Buttons
        ========================== */
        button {
            border-radius: 6px;
            border: 1px solid #dee2e6;
            background-color: #ffffff;
            padding: 6px 12px;
            font-weight: bold;
            color: #495057;
            box-shadow: inset 0 1px 0 rgba(255,255,255,0.6);
        }

        button:hover {
            background-color: #f1f3f5;
            border-color: #adb5bd;
        }

        button:active {
            background-color: #e9ecef;
        }

        button.suggested-action {
            background-color: #3498db;
            color: #ffffff;
            border-color: #2980b9;
        }

        button.suggested-action:hover {
            background-color: #2980b9;
        }

        /* =========================
           Entries
        ========================== */
       /* =========================
   Entries
========================= */
/* Base entry */
entry {
    border-radius: 6px;
    border: 1px solid #dee2e6;
    padding: 6px 8px;
    background-color: #ffffff;
    color: #495057;
}

/* Encryption Key entry */
entry.key-entry {
    background-color: #f5faff;
    border-color: #cfe2ff;
}

/* When focused */
entry.key-entry:focus {
    background-color: #e8f2ff;
    border-color: #3498db;
}
        entry:disabled {
            background-color: #f8f9fa;
            color: #adb5bd;
        }

        entry:hover {
    background-color: #f0f4f8;
}

        /* =========================
           File chooser button
        ========================== */
        filechooserbutton button {
            border-radius: 6px;
            border: 1px solid #dee2e6;
            background-color: #ffffff;
        }

        /* =========================
           Progress bar
        ========================== */
        progressbar {
            min-height: 20px;
        }

        progressbar trough {
            background-color: #e9ecef;
            border-radius: 8px;
        }

        progressbar progress {
            background-color: #3498db;
            border-radius: 8px;
        }

        /* =========================
           Text view
        ========================== */
        textview {
            border-radius: 6px;
            border: 1px solid #dee2e6;
            background-color: #ffffff;
            font-family: Monospace;
            font-size: 12px;
        }

        textview text {
            color: #495057;
        }

        /* =========================
           Images (icons)
        ========================== */
        image {
            color: #6c757d;
        }

        /* =========================
           Status indicators
        ========================== */
        .success {
            color: #27ae60;
        }

        .error {
            color: #e74c3c;
        }

        .warning {
            color: #f39c12;
        }
 entry.key-entry image {
    color: #3498db; /* blue, always visible */
}

entry.key-entry image:hover {
    color: #1d6fa5;
}

entry.key-entry image:active {
    color: #0b3d73;
}

/* Targets the GtkImage inside the entry secondary icon */
entry entry.secondary image {
    color: black; /* default color */
}

entry entry.secondary image:hover {
    color: #007acc; /* color on hover */
}

        /* =========================
           Section titles
        ========================== */
        .section-title {
            font-weight: bold;
            color: #2c3e50;
        }
    )");

    auto screen = Gdk::Screen::get_default();
    Gtk::StyleContext::add_provider_for_screen(
        screen,
        cssProvider,
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
}
