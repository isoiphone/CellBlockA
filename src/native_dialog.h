#ifndef CELL_NATIVE_DIALOG_H
#define CELL_NATIVE_DIALOG_H
#include <filesystem>
#include <string>
// OS file selection is a host service, like replacing Allegro's DOS video driver.
std::string native_map_dialog(bool save,const std::filesystem::path& initial);
#endif
