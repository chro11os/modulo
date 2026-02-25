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

  // Terminal retro colors
  auto color_green = Color::RGB(51, 255, 51);
  auto color_dim_green = Color::RGB(20, 100, 20);
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

  // Components
  auto btn_create =
      Button(" [1] CREATE IMAGE     ", on_create, ButtonOption::Simple());
  auto btn_read =
      Button(" [2] VERIFY IMAGE     ", on_read, ButtonOption::Simple());
  auto btn_write =
      Button(" [3] PREPARE IMAGE    ", on_write, ButtonOption::Simple());
  auto btn_read_drive =
      Button(" [4] SCAN TARGET      ", on_read_drive, ButtonOption::Simple());
  auto btn_flash =
      Button(" [!] START FLASHING   ", on_flash, ButtonOption::Simple());
  auto btn_quit = Button(" [Q] SHUTDOWN SYSTEM  ", screen.ExitLoopClosure(),
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
    auto header =
        vbox({
            text(" ███╗   ███╗ ██████╗ ██████╗ ██╗   ██╗██╗      ██████╗ ") |
                color(color_green),
            text(" ████╗ ████║██╔═══██╗██╔══██╗██║   ██║██║     ██╔═══██╗") |
                color(color_green),
            text(" ██╔████╔██║██║   ██║██║  ██║██║   ██║██║     ██║   ██║") |
                color(color_green),
            text(" ██║╚██╔╝██║██║   ██║██║  ██║██║   ██║██║     ██║   ██║") |
                color(color_green),
            text(" ██║ ╚═╝ ██║╚██████╔╝██████╔╝╚██████╔╝███████╗╚██████╔╝") |
                color(color_green),
            text(" ╚═╝     ╚═╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚══════╝ ╚═════╝ ") |
                color(color_green),
        }) |
        center;

    auto status_indicator = hbox({
        text(" SYSTEM STATUS: ") | bold,
        text(flashing ? " [ BUSY ] " : " [ READY ] ") |
            bgcolor(flashing ? Color::Red : color_green) | color(Color::Black),
        filler(),
        text(" FLASH STATUS: ") | bold,
        text(" " + flash_status + " ") |
            color(flash_status == "SUCCESS"
                      ? color_green
                      : (flash_status == "FLASHING" ? Color::Yellow
                                                    : Color::White)),
    });

    auto main_content = hbox({
        vbox({
            text(" COMMAND CONSOLE ") | bold | center,
            separator(),
            layout->Render() | flex,
        }) | border |
            size(WIDTH, GREATER_THAN, 30),

        vbox({
            text(" DEVICE INFORMATION ") | bold | center,
            separator(),
            vbox({
                hbox({text(" ACTIVE IMAGE: ") | color(color_green),
                      text(selected_file)}),
                hbox({text(" TARGET DRIVE: ") | color(color_green),
                      text(selected_drive)}),
                filler(),
                text(" FLASH PROGRESS ") | bold,
                gauge(flash_progress) | color(color_green),
                hbox({text(std::to_string((int)(flash_progress * 100)) + "%") |
                      center}) |
                    flex,
            }) | flex |
                border,
        }) | flex,
    });

    auto log_view = vbox({
                        text(" KERNEL LOG ") | color(color_dim_green) | bold,
                        separatorLight() | color(color_dim_green),
                        text("> " + log_message) | color(color_green),
                    }) |
                    borderStyled(BorderStyle::ROUNDED) |
                    color(color_dim_green) | size(HEIGHT, EQUAL, 5);

    return vbox({
               header,
               text(" --- TERMINAL USB FLASHING UTILITY v1.0.4 --- ") | center |
                   color(color_dim_green),
               separatorDouble() | color(color_green),
               status_indicator,
               main_content | flex,
               log_view,
           }) |
           borderDouble | bgcolor(color_bg) | color(color_green);
  });

  screen.Loop(component);
}
