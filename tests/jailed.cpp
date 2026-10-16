#include "jailed.h"
#include "map_io.h"
#include <cassert>
#include <fstream>
#include <climits>
namespace fs=std::filesystem;
static void paint(int tool,int x,int y,int button=1){jailed_tool=tool;jailed_mouse_x=x;jailed_mouse_y=y;jailed_mouse_b=button;jailed_screen_click();}
static void play_saved(const fs::path& path){
 auto name=path.u8string();_load_map(name.data());_reset_points();_lastspot=-1;_srand(17);_endmatch=0;
 _spawn_player(0);_spawn_player(1);
 assert(_player[0].health==99&&_player[1].health==99);
 assert(_player[0].x>=0&&_player[0].x<640&&_player[1].x>=0&&_player[1].x<640);
 _draw_map();
 for(int tick=0;tick<1000;++tick){
  for(int p=0;p<2;++p){
   _player[p].left=tick%200<80;_player[p].right=tick%200>=100;_player[p].up=tick%70<10;
   if(_player[p].health==0){_endmatch=0;_spawn_player(p);}
   if(tick%80==0){_player[p].weapon=(tick/80)%4;_player[p].ammo=6;_player_attack(p);}
  }
  _game_tick();_translate_points();_translate_points();_clean_map();_draw_player(0);_draw_player(1);_draw_points();
 }
 printf("PASS editor map in game: %s, 1000 gameplay ticks\n",path.filename().u8string().c_str());
}
void run_jailed_tests(){
 std::string error;
 jailed_reset_map();
 assert((_start[0].packed&0x7fff)==(1|(2<<7)));
 assert((_start[1].packed&0x7fff)==(18|(2<<7)|(1<<14)));
 for(int i=2;i<4;++i)assert((_start[i].packed&0x7fff)==0x3fff);
 for(auto& col:_map)for(auto& t:col)assert(t.kind==0&&t.dirty==0);
 _srand(1);
 for(int tool=1;tool<=3;++tool){
  bool variants[4]={};for(int j=0;j<200;++j){paint(tool,96,160);int k=_map[3][5].kind;assert(k>=6+4*(tool-1)&&k<=9+4*(tool-1));variants[k-6-4*(tool-1)]=true;}
  for(bool seen:variants)assert(seen);
  paint(tool,96,160,2);assert(_map[3][5].kind==0);
 }
 paint(4,96,160);assert(_map[3][5].kind==4);
 paint(1,96,160,2);assert(_map[3][5].kind==4); // Platform eraser leaves ladders.
 paint(4,96,160,2);assert(_map[3][5].kind==0);
 paint(1,96,160);paint(4,96,160,2);assert(_map[3][5].kind>=6);
 paint(5,160,192);assert((_start[2].packed&0x7fff)==(5|(6<<7)));
 paint(5,160,192);assert((_start[2].packed&(1<<14))!=0);
 paint(5,160,192,2);assert((_start[2].packed&0x3fff)==0x3fff);
 auto before=cell::encode_map(error);
 for(auto xy:{std::pair<int,int>{-1,0},{640,0},{0,416},{INT_MIN,INT_MAX}})paint(4,xy.first,xy.second);
 assert(before==cell::encode_map(error));
 // Every original map passes native decoding and lossless original-format output.
 auto directory=cell::save_dir/"editor-verification";fs::create_directories(directory);
 int count=0;
 for(auto& entry:fs::directory_iterator(cell::resource_dir/"original/MAPS"))if(entry.path().extension()==".CEL"){
  auto bytes=cell::read_map(entry.path(),error);assert(bytes.size()==272);
  assert(cell::load_map(entry.path(),error));assert(cell::encode_map(error)==bytes);
  auto pic=jailed_mappic();auto path=directory/entry.path().filename();assert(cell::save_map(path,pic,error));
  assert(cell::read_map(path,error)==bytes);_destroy_bitmap(pic);++count;
 }
 assert(count==12);
 jailed_reset_map();
 for(int x=0;x<20;++x)paint(2,x*32,96); // Walkable floor beneath original starts.
 paint(1,0,96);paint(3,608,96);paint(4,320,64);
 jailed_draw_map();auto pic=jailed_mappic();assert(pic->w==200&&pic->h==150);
 for(int y=0;y<150;++y)for(int x=0;x<200;++x)assert(pic->pixels[y*200+x]==_buffer->pixels[(y*480/150)*640+x*640/200]);
 auto created=directory/"NEW.CEL";assert(cell::save_map(created,pic,error));_destroy_bitmap(pic);
 assert(cell::load_map(created,error));paint(4,128,64);paint(5,192,64);
 pic=jailed_mappic();auto edited=directory/"EDITED.CEL";auto expected=cell::encode_map(error);
 assert(cell::save_map(edited,pic,error));_destroy_bitmap(pic);
 jailed_reset_map();assert(cell::load_map(edited,error));assert(cell::encode_map(error)==expected);
 auto pcx=edited;pcx.replace_extension(".pcx");auto filename=pcx.u8string();auto loaded_pic=_load_pcx(filename.data(),nullptr);
 pic=jailed_mappic();assert(loaded_pic&&loaded_pic->w==200&&loaded_pic->h==150);assert(memcmp(loaded_pic->pixels,pic->pixels,30000)==0);_destroy_bitmap(loaded_pic);_destroy_bitmap(pic);
 // Rejected input must preserve the current document and never hang spawning.
 auto invalid=directory/"BAD.CEL";{std::ofstream f(invalid,std::ios::binary);f<<"slh!"<<char(0)<<char(1);}
 assert(!cell::load_map(invalid,error));assert(cell::encode_map(error)==expected);
 auto invalid_bytes=expected;invalid_bytes[260]=20;assert(!cell::validate_map(invalid_bytes,error));
 for(int i=0;i<4;++i)_start[i].packed=0x3fff;
 pic=jailed_mappic();assert(!cell::save_map(edited,pic,error));_destroy_bitmap(pic);
 assert(cell::read_map(edited,error)==expected);
 // A wrong extension must not make map and preview resolve to the same file.
 assert(cell::load_map(edited,error));pic=jailed_mappic();
 auto old_preview=cell::encode_pcx(pic,__current_pallete);
 auto wrong=edited;wrong.replace_extension(".pcx");
 assert(!cell::save_map(wrong,pic,error));_destroy_bitmap(pic);
 {std::ifstream f(wrong,std::ios::binary);std::vector<unsigned char> contents((std::istreambuf_iterator<char>(f)),{});assert(contents==old_preview);}
 assert(cell::load_map(edited,error));pic=jailed_mappic();assert(!cell::save_map(directory/"missing"/"FAIL.CEL",pic,error));_destroy_bitmap(pic);
 play_saved(created);play_saved(edited);
 // The normal game menu finds user maps and reads their corresponding previews.
 assert(cell::load_map(edited,error));pic=jailed_mappic();auto user=cell::map_directory()/"JAILEDTEST.CEL";assert(cell::save_map(user,pic,error));_destroy_bitmap(pic);
 _get_map_filenames();bool found=false;for(auto n=_maphead.next;n;n=n->next)if(std::string(n->name)=="maps/JAILEDTEST.CEL")found=true;assert(found);
 assert(_exists((char*)"maps/JAILEDTEST.CEL"));assert(_exists((char*)"maps/JAILEDTEST.PCX"));
 loaded_pic=_load_pcx((char*)"maps/JAILEDTEST.PCX",nullptr);assert(loaded_pic);_destroy_bitmap(loaded_pic);
 fs::remove(user);user.replace_extension(".pcx");fs::remove(user);_get_map_filenames();assert(_num_maps==12);
 puts("PASS Jail-ed defaults, all tools/variants, starts, clipping, 12 map roundtrips, preview, failures and game discovery");
}
