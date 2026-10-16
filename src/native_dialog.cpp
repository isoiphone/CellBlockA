#include "native_dialog.h"
#if defined(__APPLE__)
#import <Cocoa/Cocoa.h>
std::string native_map_dialog(bool save,const std::filesystem::path& initial) {
 @autoreleasepool {
  NSSavePanel* panel=save?[NSSavePanel savePanel]:[NSOpenPanel openPanel];
  [panel setTitle:save?@"Save As...":@"Load..."];
  [panel setAllowedFileTypes:@[@"cel",@"CEL"]];
  [panel setCanCreateDirectories:YES];
  auto dir=std::filesystem::is_directory(initial)?initial:initial.parent_path();
  [panel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:dir.u8string().c_str()]]];
  if(save)[panel setNameFieldStringValue:[NSString stringWithUTF8String:(initial.has_extension()?initial.filename().u8string():"UNTITLED.CEL").c_str()]];
  else {[(NSOpenPanel*)panel setCanChooseDirectories:NO];[(NSOpenPanel*)panel setAllowsMultipleSelection:NO];}
  if([panel runModal]!=NSModalResponseOK)return {};
  return std::string([[[panel URL] path] UTF8String]);
 }
}
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
std::string native_map_dialog(bool save,const std::filesystem::path& initial) {
 wchar_t filename[32768]={};
 auto dir=std::filesystem::is_directory(initial)?initial:initial.parent_path();
 auto name=initial.has_extension()?initial.filename().wstring():L"UNTITLED.CEL";
 if(save)wcsncpy(filename,name.c_str(),32767);
 OPENFILENAMEW ofn{};ofn.lStructSize=sizeof ofn;ofn.lpstrFile=filename;ofn.nMaxFile=32768;
 ofn.lpstrFilter=L"Cell Block A maps (*.cel)\0*.cel\0\0";ofn.lpstrDefExt=L"cel";
 ofn.lpstrInitialDir=dir.c_str();ofn.lpstrTitle=save?L"Save As...":L"Load...";
 ofn.Flags=OFN_EXPLORER|OFN_NOCHANGEDIR|OFN_PATHMUSTEXIST|(save?OFN_OVERWRITEPROMPT:OFN_FILEMUSTEXIST);
 if(!(save?GetSaveFileNameW(&ofn):GetOpenFileNameW(&ofn)))return {};
 return std::filesystem::path(filename).u8string();
}
#else
// Linux is optional; use the system desktop chooser when present. No shell
// interpolation: filenames are passed as individual argv entries.
#include <unistd.h>
#include <sys/wait.h>
std::string native_map_dialog(bool save,const std::filesystem::path& initial) {
 int fd[2];if(pipe(fd))return {};pid_t child=fork();
 if(child==0){close(fd[0]);dup2(fd[1],STDOUT_FILENO);close(fd[1]);auto path=initial.u8string();
  if(save)execlp("zenity","zenity","--file-selection","--save","--confirm-overwrite","--filename",path.c_str(),"--file-filter=*.cel *.CEL",(char*)nullptr);
  else execlp("zenity","zenity","--file-selection","--filename",path.c_str(),"--file-filter=*.cel *.CEL",(char*)nullptr);
  _exit(127);
 }
 close(fd[1]);std::string result;char buf[1024];ssize_t n;
 if(child<0){close(fd[0]);return {};}
 while((n=read(fd[0],buf,sizeof buf))>0)result.append(buf,n);
 close(fd[0]);int status;waitpid(child,&status,0);
 if(!WIFEXITED(status)||WEXITSTATUS(status))return {};
 if(!result.empty()&&result.back()=='\n')result.pop_back();return result;
}
#endif
