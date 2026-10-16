#ifndef CELL_MAP_IO_H
#define CELL_MAP_IO_H
#include "native.h"
#include <filesystem>
#include <string>
#include <vector>
namespace cell {
namespace fs = std::filesystem;
extern fs::path resource_dir, save_dir;
fs::path map_directory();
fs::path resolve_original(const char* name);
std::vector<unsigned char> read_map(const fs::path&, std::string& error);
bool load_map(const fs::path&, std::string& error);
bool save_map(const fs::path&, BITMAP* preview, std::string& error);
std::vector<unsigned char> encode_map(std::string& error);
bool validate_map(const std::vector<unsigned char>&, std::string& error);
std::vector<unsigned char> encode_pcx(BITMAP*, const RGB*);
}
#endif
