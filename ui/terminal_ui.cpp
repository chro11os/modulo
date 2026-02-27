#include "terminal_ui.h"
#include <iostream>
#include <string>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"

#include "../utils/create/createFile.h"
#include "../utils/read/readFile.h"
#include "../utils/write/writeFile.h"

using namespace ftxui;

void runTerminalUI() {
  auto screen = ScreenInteractive::Fullscreen();

  // State
  std::string log_message = "READY TO TEST UTILS...";
  std::string active_file = "NONE";

  // Inputs
  std::string filename_input_str = "test_file";
  std::string write_content_str = "Hello FTXUI!";
  std::string read_content_str = "(No data yet)";

  // Actions
  auto on_create = [&] {
    log_message = "CALLING CREATEFILE(" + filename_input_str + ")...";
    active_file = createFile(filename_input_str);
    if (!active_file.empty()) {
      log_message = "SUCCESS: FILE CREATED -> " + active_file;
    } else {
      log_message = "ERROR: FILE CREATION FAILED.";
    }
  };

  auto on_read = [&] {
    if (active_file == "NONE") {
      log_message = "ERROR: NO ACTIVE FILE. CREATE ONE FIRST.";
      return;
    }
    log_message = "READING " + active_file + "...";
    read_content_str = readFile(active_file);
    log_message = "READ SUCCESSFUL.";
  };

  auto on_write = [&] {
    if (active_file == "NONE") {
      log_message = "ERROR: NO ACTIVE FILE. CREATE ONE FIRST.";
      return;
    }
    log_message = "WRITING TO " + active_file + "...";
    if (writeFile(active_file, write_content_str) == 0) {
      log_message = "WRITE SUCCESSFUL.";
    } else {
      log_message = "WRITE FAILED.";
    }
  };

  // Components
  Component input_filename = Input(&filename_input_str, "Filename");
  Component input_write = Input(&write_content_str, "Content to write");

  auto btn_create =
      Button(" [1] CREATE FILE ", on_create, ButtonOption::Simple());
  auto btn_read = Button(" [2] READ CONTENT ", on_read, ButtonOption::Simple());
  auto btn_write =
      Button(" [3] WRITE DATA   ", on_write, ButtonOption::Simple());
  auto btn_quit = Button(" [Q] QUIT         ", screen.ExitLoopClosure(),
                         ButtonOption::Simple());

  auto layout = Container::Vertical({
      input_filename,
      btn_create,
      input_write,
      btn_write,
      btn_read,
      btn_quit,
  });

  auto component = Renderer(layout, [&] {
    auto filename_box = vbox({
        text(" FILENAME: ") | bold,
        input_filename->Render() | border | color(Color::White),
    });

    auto write_box = vbox({
        text(" TEXT TO WRITE: ") | bold,
        input_write->Render() | border | color(Color::White),
    });

    auto read_box = vbox({
        text(" READ DATA BOX: ") | bold,
        vbox({
            paragraph(read_content_str),
        }) | border |
            color(Color::Cyan) | size(HEIGHT, GREATER_THAN, 3),
    });

    return vbox({
               text(" MODULO INTERACTIVE TESTER ") | bold | center,
               separator(),
               hbox({
                   text(" ACTIVE PATH: ") | dim,
                   text(active_file) | color(Color::Yellow),
               }),
               separator(),

               vbox({
                   filename_box,
                   btn_create->Render() | align_right,
                   separatorLight(),
                   write_box,
                   btn_write->Render() | align_right,
                   separatorLight(),
                   read_box,
                   btn_read->Render() | align_right,
               }) | flex |
                   border,

               vbox({
                   text(" SYSTEM LOG ") | dim,
                   separatorLight(),
                   text("> " + log_message) | color(Color::Green),
               }) | border |
                   size(HEIGHT, EQUAL, 5),

               btn_quit->Render() | center,
           }) |
           border | bgcolor(Color::Black) | color(Color::White);
  });

  screen.Loop(component);
}
