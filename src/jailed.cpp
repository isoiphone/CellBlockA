// Native host for the recovered Jail-ed 1.1b editor. The canvas, tools, default
// starts, sprite variants and preview rendering are original recovered code.
#include "jailed.h"
#include "map_io.h"
#include "native_dialog.h"
#include <SDL.h>
#include <fstream>
#include <sstream>
#include <iostream>
extern SDL_Window* native_window();
extern bool native_editor_event(SDL_Event&);
namespace {
std::filesystem::path current_file;
bool changed=false;
int mouse_x=-32,mouse_y=-32;
int cursor_asset=2;
void title(){auto name=current_file.empty()?"Untitled":current_file.filename().u8string();SDL_SetWindowTitle(native_window(),("Jail-ed 1.1b - "+name+(changed?" *":"")).c_str());}
void error(const std::string& message){fprintf(stderr,"Jail-ed: %s\n",message.c_str());SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Jail-ed",message.c_str(),native_window());}
bool confirm(const char* message){
 SDL_MessageBoxButtonData buttons[]={{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,0,"No"},{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"Yes"}};
 SDL_MessageBoxData data={SDL_MESSAGEBOX_WARNING,native_window(),"Jail-ed",message,2,buttons,nullptr};int answer=0;
 return SDL_ShowMessageBox(&data,&answer)==0&&answer==1;
}
void reset(){jailed_reset_map();jailed_draw_map();current_file.clear();changed=false;title();}
void tool(char c){switch(c){case 'q':jailed_tool=4;cursor_asset=1;break;case 'w':jailed_tool=1;cursor_asset=2;break;case 'e':jailed_tool=2;cursor_asset=3;break;case 'r':jailed_tool=3;cursor_asset=4;break;case 't':jailed_tool=5;cursor_asset=5;break;}}
void click(int x,int y,int buttons){
 Tile before[20][13];Start starts[4];memcpy(before,_map,sizeof before);memcpy(starts,_start,sizeof starts);
 jailed_mouse_x=x;jailed_mouse_y=y;jailed_mouse_b=buttons;jailed_screen_click();
 if(memcmp(before,_map,sizeof before)||memcmp(starts,_start,sizeof starts)){changed=true;title();}
}
bool load(const std::filesystem::path& path,std::string& problem){
 if(!cell::load_map(path,problem))return false;current_file=path;changed=false;jailed_draw_map();title();return true;
}
bool save(std::filesystem::path path,std::string& problem){
 if(path.extension().empty())path.replace_extension(".cel");
 auto preview=jailed_mappic();bool ok=cell::save_map(path,preview,problem);_destroy_bitmap(preview);
 if(!ok)return false;current_file=path;changed=false;title();return true;
}
void present(){_blit(_buffer,_screen,0,0,0,0,640,480);_draw_sprite(_screen,(BITMAP*)_game_data[cursor_asset].dat,mouse_x,mouse_y);native_present();}
int script(const char* path){
 // Deterministic native integration driver; the same commands call the UI's
 // tool, click, load and save handlers. No alternate editor implementation.
 std::ifstream f(std::filesystem::u8path(path));if(!f){fprintf(stderr,"Cannot open editor script\n");return 1;}
 std::string line,problem;int number=0;
 while(std::getline(f,line)){++number;std::istringstream in(line);std::string command;in>>command;if(command.empty()||command[0]=='#')continue;
  bool ok=true;
  if(command=="new")reset();
  else if(command=="tool"){char c;ok=bool(in>>c);if(ok)tool(c);}
  else if(command=="click"){int x,y,b;ok=bool(in>>x>>y>>b);if(ok)click(x,y,b);}
  else if(command=="load"||command=="save"){std::string p;std::getline(in>>std::ws,p);ok=!p.empty()&&(command=="load"?load(std::filesystem::u8path(p),problem):save(std::filesystem::u8path(p),problem));}
  else if(command=="capture"){std::string p;std::getline(in>>std::ws,p);present();ok=_save_pcx(const_cast<char*>(p.c_str()),_screen,__current_pallete)==0;}
  else {ok=false;problem="Unknown command";}
  if(!ok){fprintf(stderr,"Editor script line %d: %s\n",number,problem.c_str());return 1;}present();
 }
 return 0;
}
}
int jailed_main(int argc,char**argv){
 if(_allegro_init()<0||_set_gfx_mode(0,640,480,0,0)<0){fprintf(stderr,"Jail-ed video initialization failed: %s\n",SDL_GetError());return 1;}
 _game_data=_load_datafile((char*)"game.dat");if(!_game_data)return 1;
 _set_pallete((RGB*)_game_data[10].dat);_buffer=_create_bitmap(640,480);_clear(_buffer);reset();
 SDL_ShowCursor(SDL_DISABLE);
 int result=0;
 if(argc==3&&std::string(argv[1])=="--script")result=script(argv[2]);
 else {
  if(argc>1){std::string problem;if(!load(std::filesystem::u8path(argv[1]),problem))error(problem);}
  bool done=false;
  while(!done){
   present();SDL_Event e;
   while(native_editor_event(e)){
    if(e.type==SDL_QUIT){done=confirm(changed?"Discard changes and quit?":"Do you really want to quit?");continue;}
    if(e.type==SDL_MOUSEMOTION){mouse_x=e.motion.x;mouse_y=e.motion.y;if(e.motion.state&(SDL_BUTTON_LMASK|SDL_BUTTON_RMASK))click(mouse_x,mouse_y,(e.motion.state&SDL_BUTTON_RMASK)?2:1);}
    if(e.type==SDL_MOUSEBUTTONDOWN&&(e.button.button==SDL_BUTTON_LEFT||e.button.button==SDL_BUTTON_RIGHT)){mouse_x=e.button.x;mouse_y=e.button.y;click(mouse_x,mouse_y,e.button.button==SDL_BUTTON_RIGHT?2:1);}
    if(e.type==SDL_WINDOWEVENT&&e.window.event==SDL_WINDOWEVENT_LEAVE){mouse_x=mouse_y=-32;}
    if(e.type!=SDL_KEYDOWN||e.key.repeat)continue;
    auto key=e.key.keysym.sym;
    if(key==SDLK_ESCAPE){done=confirm(changed?"Discard changes and quit?":"Do you really want to quit?");}
    else if(key==SDLK_n){if(!changed||confirm("Discard changes and create a new map?"))reset();}
    else if(key==SDLK_s||key==SDLK_l){
     bool saving=key==SDLK_s;
     if(!saving&&changed&&!confirm("Discard changes and load a map?"))continue;
     // Keep the native arrow visible inside an operating-system dialog.
     SDL_ShowCursor(SDL_ENABLE);
     auto initial=saving?cell::map_directory():(current_file.empty()?cell::resource_dir/"original/MAPS":current_file);
     if(saving&&!current_file.empty())initial=cell::map_directory()/current_file.filename();
     auto p=native_map_dialog(saving,initial);SDL_RaiseWindow(native_window());SDL_ShowCursor(SDL_DISABLE);
     if(!p.empty()){std::string problem;if(!(saving?save(std::filesystem::u8path(p),problem):load(std::filesystem::u8path(p),problem)))error(problem);}
    } else if(key>=0&&key<128)tool((char)key);
   }
   SDL_Delay(8);
  }
 }
 SDL_ShowCursor(SDL_ENABLE);_destroy_bitmap(_buffer);_buffer=nullptr;_unload_datafile(_game_data);_game_data=nullptr;native_shutdown();return result;
}
