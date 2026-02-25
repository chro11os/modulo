#include "terminal_ui.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"

#include "../process/read_drive.h"
#include "../utils/create/createFile.h"
#include "../utils/read/readFile.h"
#include "../utils/write/writeFile.h"

using namespace ftxui;

void runTerminalUI() {
  auto screen = ScreenInteractive::Fullscreen();

  // State
  std::string flash_status = "IDLE";
  float flash_progress = 0.0f;
  bool flashing = false;
  std::string log_message = "MODULO TERMINAL FLASHER v1.0 INITIALIZED...";
  std::string selected_file = "N/A";
  std::string selected_drive = "/dev/sdb1 (KINGSTON_16GB)";

  // Flat Black and White colors
  auto color_primary = Color::White;
  auto color_dim = Color::GrayDark;
  auto color_bg = Color::Black;

  // Actions
  auto on_create = [&] {
    log_message = "EXECUTING CREATE_FILE()...";
    selected_file = createFile();
    if (!selected_file.empty()) {
      log_message = "SUCCESS: CREATED " + selected_file;
    } else {
      log_message = "ERROR: FILE CREATION FAILED.";
    }
  };

  auto on_read = [&] {
    if (selected_file == "N/A") {
      log_message = "ERROR: NO ACTIVE IMAGE FILE SELECTED.";
      return;
    }
    log_message = "EXECUTING READ_FILE(" + selected_file + ")...";
    readFile(selected_file);
    log_message = "SUCCESS: READ COMPLETE.";
  };

  auto on_write = [&] {
    if (selected_file == "N/A") {
      log_message = "ERROR: NO ACTIVE IMAGE FILE SELECTED.";
      return;
    }
    log_message = "EXECUTING WRITE_FILE(" + selected_file + ")...";
    writeFile(selected_file);
    log_message = "SUCCESS: WRITE COMPLETE.";
  };

  auto on_read_drive = [&] {
    log_message = "EXECUTING READ_DRIVE(" + selected_drive + ")...";
    read_drive(selected_drive);
    log_message = "SUCCESS: DRIVE SCANNED.";
  };

  auto on_flash = [&] {
    if (selected_file == "N/A") {
      log_message = "ERROR: CANNOT FLASH 'N/A' IMAGE.";
      return;
    }
    if (flashing)
      return;

    flashing = true;
    flash_status = "FLASHING";
    flash_progress = 0.0f;

    std::thread([&] {
      for (int i = 0; i <= 100; ++i) {
        flash_progress = i / 100.0f;
        if (i % 10 == 0)
          log_message = "WRITING BLOCK " + std::to_string(i) + "/100...";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        screen.PostEvent(Event::Custom);
      }
      flash_status = "SUCCESS";
      log_message =
          "SUCCESS: " + selected_file + " FLASHED TO " + selected_drive;
      flashing = false;
      screen.PostEvent(Event::Custom);
    }).detach();
  };

  // Components - Using Simple styling for flat look
  auto btn_create =
      Button(" CREATE IMAGE     ", on_create, ButtonOption::Simple());
  auto btn_read = Button(" VERIFY IMAGE     ", on_read, ButtonOption::Simple());
  auto btn_write =
      Button(" PREPARE IMAGE    ", on_write, ButtonOption::Simple());
  auto btn_read_drive =
      Button(" SCAN TARGET      ", on_read_drive, ButtonOption::Simple());
  auto btn_flash =
      Button(" START FLASHING   ", on_flash, ButtonOption::Simple());
  auto btn_quit = Button(" SHUTDOWN SYSTEM  ", screen.ExitLoopClosure(),
                         ButtonOption::Simple());

  auto layout = Container::Vertical({
      btn_create,
      btn_read,
      btn_write,
      btn_read_drive,
      btn_flash,
      btn_quit,
  });

  auto component = Renderer(layout, [&] {
    auto header = vbox({
        text(" MODULO USB FLASHER ") | bold | center,
        text(" v1.0.4 - FLAT EDITION ") | center | dim,
    });

    auto status_indicator = hbox({
        text(" STATUS: ") | bold,
        text(flashing ? " BUSY " : " READY ") | inverted,
        filler(),
        text(" FLASH: ") | bold,
        text(flash_status) | (flash_status == "SUCCESS" ? bold : dim),
    });

    auto main_content = hbox({
        vbox({
            text(" COMMANDS ") | bold,
            separator(),
            layout->Render() | flex,
        }) | border |
            size(WIDTH, GREATER_THAN, 25),

        vbox({
            text(" DEVICE INFO ") | bold,
            separator(),
            vbox({
                hbox({text(" IMAGE: ") | bold, text(selected_file)}),
                hbox({text(" DRIVE: ") | bold, text(selected_drive)}),
                filler(),
                gauge(flash_progress),
                hbox({text(std::to_string((int)(flash_progress * 100)) + "%") |
                      center}) |
                    flex,
            }) | flex |
                border,
        }) | flex,
    });

    auto log_view = vbox({
                        text(" LOG ") | bold,
                        separator(),
                        text("> " + log_message),
                    }) |
                    border | size(HEIGHT, EQUAL, 5);

    return vbox({
               header,
               separator(),
               status_indicator,
               main_content | flex,
               log_view,
           }) |
           border | bgcolor(color_bg) | color(color_primary);
  });

  screen.Loop(component);
}
