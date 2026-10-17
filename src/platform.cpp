// Native SDL host services for the recovered Allegro 3 game.
#include "native.h"
#include "map_io.h"
#include "jailed.h"
#include "replay_clock.h"
#include <SDL.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <cstdarg>
#include <cctype>
extern "C" {
#include "allegro.h"
#include "jgmod.h"
}
#include "asset_types.h"
#include "trig_tables.h"
namespace fs=std::filesystem;
static fs::path &resources=cell::resource_dir, &saves=cell::save_dir;
static bool editor_mode;
static std::deque<SDL_Event> editor_events;
static SDL_Window *window;
SDL_Window* native_window(){return window;}
bool native_editor_event(SDL_Event& e){if(editor_events.empty())return false;e=editor_events.front();editor_events.pop_front();return true;}
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static SDL_AudioDeviceID audio_device;
static bool quitting, pumping, initialized;
static unsigned char key_state[128];
static std::deque<int> key_buffer;
static uint64_t start_us;
static uint64_t random_state=1;
struct InputStep {uint64_t at; int scan,down;};
static std::deque<InputStep> test_input;
static uint64_t test_duration;
static fs::path test_capture;
static bool scripted_replay;
static ReplayClock replay_clock;
static bool native_input_step_valid(uint64_t ms,int scan,int down){
 return ms<=UINT64_MAX/1000 && scan>=0 && scan<128 && (down==0 || down==1);
}
uint64_t native_counter_micros(uint64_t counter,uint64_t frequency){
 // Divide the whole seconds first: multiplying an absolute nanosecond counter
 // by 1,000,000 overflows after only about five hours of system uptime.
 return (counter/frequency)*1000000+(counter%frequency)*1000000/frequency;
}
static uint64_t micros(){return native_counter_micros(SDL_GetPerformanceCounter(),SDL_GetPerformanceFrequency());}
static std::string upper(std::string s){for(auto &c:s)c=std::toupper((unsigned char)c);return s;}
static fs::path original_path(const char *name){return cell::resolve_original(name);}
static std::vector<uint8_t> read_file(const fs::path &p){if(!fs::is_regular_file(p))return {};std::ifstream f(p,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
static uint16_t be16(const uint8_t *p){return p[0]*256+p[1];}
static uint32_t be32(const uint8_t *p){return (uint32_t(be16(p))<<16)|be16(p+2);}
static uint16_t le16(const uint8_t *p){return p[0]+p[1]*256;}
struct Timer {void (*fn)(); uint64_t period,next;};
static std::vector<Timer> timers;
static int scan(SDL_Scancode k){
 switch(k){
 case SDL_SCANCODE_ESCAPE:return 1;case SDL_SCANCODE_Q:return 16;case SDL_SCANCODE_W:return 17;
 case SDL_SCANCODE_A:return 30;case SDL_SCANCODE_S:return 31;case SDL_SCANCODE_D:return 32;
 case SDL_SCANCODE_RETURN:return 28;case SDL_SCANCODE_KP_ENTER:return 28;case SDL_SCANCODE_SPACE:return 57;
 case SDL_SCANCODE_UP:return 72;case SDL_SCANCODE_DOWN:return 80;case SDL_SCANCODE_LEFT:return 75;case SDL_SCANCODE_RIGHT:return 77;
 case SDL_SCANCODE_LCTRL:case SDL_SCANCODE_RCTRL:return 120;
 case SDL_SCANCODE_NUMLOCKCLEAR:return 69;case SDL_SCANCODE_KP_DIVIDE:return 122;case SDL_SCANCODE_KP_MULTIPLY:return 55;
 case SDL_SCANCODE_KP_MINUS:return 74;case SDL_SCANCODE_KP_PLUS:return 78;
 case SDL_SCANCODE_F9:return 67;case SDL_SCANCODE_F10:return 68;case SDL_SCANCODE_F11:return 87;case SDL_SCANCODE_F12:return 88;
 case SDL_SCANCODE_PRINTSCREEN:return 84;
 case SDL_SCANCODE_X:return 45; default:return 0;
 }
}
void native_pump(){
 if(pumping||!initialized)return;
 pumping=true;
 SDL_Event e;
 while(SDL_PollEvent(&e)){
  if(editor_mode){editor_events.push_back(e);continue;}
  if(e.type==SDL_QUIT){quitting=true;_playing=0;_going=0;}
  if(e.type==SDL_WINDOWEVENT&&e.window.event==SDL_WINDOWEVENT_FOCUS_LOST)memset(key_state,0,sizeof key_state);
  if(e.type==SDL_KEYDOWN||e.type==SDL_KEYUP){int s=scan(e.key.keysym.scancode);if(s)key_state[s]=e.type==SDL_KEYDOWN;if(e.type==SDL_KEYDOWN)key_buffer.push_back((s<<8)|(e.key.keysym.sym&255));}
 }
 if(quitting){pumping=false;native_shutdown();exit(0);}
 uint64_t now=micros();
 uint64_t replay_time=scripted_replay?replay_clock.elapsed:now-start_us;
 while(!test_input.empty() && replay_time>=test_input.front().at){
  auto input=test_input.front();test_input.pop_front();key_state[input.scan]=input.down;
  if(input.down)key_buffer.push_back(input.scan<<8);
 }
 if(test_duration && replay_time>=test_duration){
  if(_screen && !test_capture.empty()){
   auto surface=SDL_CreateRGBSurfaceWithFormat(0,_screen->w,_screen->h,32,SDL_PIXELFORMAT_ARGB8888);
   auto pixels=(uint32_t*)surface->pixels;
   for(int i=0;i<_screen->w*_screen->h;++i){auto c=__current_pallete[_screen->pixels[i]];pixels[i]=0xff000000u|((c.r*255/63)<<16)|((c.g*255/63)<<8)|(c.b*255/63);}
   SDL_SaveBMP(surface,test_capture.u8string().c_str());SDL_FreeSurface(surface);
  }
  fprintf(stderr,"Native replay complete: p1=(%d,%d) hp=%d state=%d; p2=(%d,%d) hp=%d state=%d; map=%s; ticks=%.0f\n",_player[0].x,_player[0].y,_player[0].health,_player[0].state,_player[1].x,_player[1].y,_player[1].health,_player[1].state,_cur_map?_cur_map:"(none)",_game_time);
  native_shutdown();exit(0);
 }
 while(true){
  auto it=std::min_element(timers.begin(),timers.end(),[](const Timer&a,const Timer&b){return a.next<b.next;});
  if(it==timers.end()||it->next>now)break;
  auto fn=it->fn;it->next+=it->period;fn();
 }
 pumping=false;
}
static void replay_poll(){if(scripted_replay)replay_clock.poll(micros());}
unsigned char *native_keys(){replay_poll();native_pump();return key_state;}
void native_present(){
 if(!renderer||!_screen)return;
 std::vector<uint32_t> rgba(_screen->w*_screen->h);
 for(size_t i=0;i<rgba.size();++i){auto c=__current_pallete[_screen->pixels[i]];rgba[i]=0xff000000u|((c.r*255/63)<<16)|((c.g*255/63)<<8)|(c.b*255/63);}
 SDL_UpdateTexture(texture,nullptr,rgba.data(),_screen->w*4);
 SDL_RenderClear(renderer);SDL_RenderCopy(renderer,texture,nullptr,nullptr);SDL_RenderPresent(renderer);
 native_pump();
}
int _allegro_init(){if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER))return -1;start_us=micros();initialized=true;return 0;}
void _allegro_exit(){native_shutdown();}
int _install_keyboard(){return 0;}
int _install_timer(){return 0;}
void _reserve_voices(int,int){}
int __go32_dpmi_lock_data(void*,int){return 0;}
int _set_gfx_mode(int,int w,int h,int,int){
 if(!w||!h)return 0;
 window=SDL_CreateWindow(editor_mode?"Jail-ed 1.1b":"Cell Block A",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,w*2,h*2,SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI);
 if(!window)return -1;
 // SDL2 enables text input by default. These canvases use physical keys, not
 // composed text; on macOS a text-input responder also enables IME overlays.
 // Native editor file dialogs manage their own text fields independently.
 SDL_StopTextInput();
 SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"0");
 renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
 if(!renderer)renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
 if(!renderer)return -1;
 SDL_RenderSetLogicalSize(renderer,w,h);SDL_RenderSetIntegerScale(renderer,SDL_TRUE);
 texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,w,h);
 _screen=_create_bitmap(w,h);static GFX_DRIVER gfx={};gfx.w=w;gfx.h=h;_gfx_driver=&gfx;return texture?0:-1;
}
BITMAP *_create_bitmap(int w,int h){auto b=(BITMAP*)calloc(1,sizeof(BITMAP));b->w=w;b->h=h;b->pixels=(uint8_t*)calloc(w*h,1);return b;}
void _destroy_bitmap(BITMAP*b){if(b){free(b->pixels);free(b);}}
void _clear(BITMAP*b){if(b)memset(b->pixels,0,b->w*b->h);}
void _putpixel(BITMAP*b,int x,int y,int c){if(b&&x>=0&&y>=0&&x<b->w&&y<b->h)b->pixels[y*b->w+x]=c;}
void _rectfill(BITMAP*b,int x1,int y1,int x2,int y2,int c){for(int y=std::max(0,y1);y<=std::min(y2,b->h-1);++y)for(int x=std::max(0,x1);x<=std::min(x2,b->w-1);++x)b->pixels[y*b->w+x]=c;}
void _blit(BITMAP*s,BITMAP*d,int sx,int sy,int dx,int dy,int w,int h){
 if(!s||!d)return;
 for(int y=0;y<h;++y)for(int x=0;x<w;++x)if(sx+x>=0&&sy+y>=0&&sx+x<s->w&&sy+y<s->h&&dx+x>=0&&dy+y>=0&&dx+x<d->w&&dy+y<d->h)d->pixels[(dy+y)*d->w+dx+x]=s->pixels[(sy+y)*s->w+sx+x];
 if(d==_screen)native_present();
}
static void sprite(BITMAP*d,BITMAP*s,int x,int y,bool flip){if(!s||!d)return;for(int j=0;j<s->h;++j)for(int i=0;i<s->w;++i){int c=s->pixels[j*s->w+(flip?s->w-1-i:i)];if(c)_putpixel(d,x+i,y+j,c);}}
void _draw_sprite(BITMAP*d,BITMAP*s,int x,int y){sprite(d,s,x,y,false);}
void _draw_sprite_h_flip(BITMAP*d,BITMAP*s,int x,int y){sprite(d,s,x,y,true);}
void _set_pallete(RGB*p){if(p)memmove(__current_pallete,p,sizeof __current_pallete);native_present();}
void _vsync(){native_pump();SDL_Delay(1000/60);}
// Allegro 3 fade_from_range uses (63-position), divides by 64, and
// programs the palette on two retraces per step (original 0x17e34).
static void fade(RGB*from,RGB*to,int speed){RGB p[256];for(int a=0;a<64;a+=speed){for(int i=0;i<256;++i){p[i].r=(from[i].r*(63-a)+to[i].r*a)/64;p[i].g=(from[i].g*(63-a)+to[i].g*a)/64;p[i].b=(from[i].b*(63-a)+to[i].b*a)/64;} _set_pallete(p);_vsync();_vsync();}_set_pallete(to);}
void _fade_out(int speed){RGB from[256];memcpy(from,__current_pallete,sizeof from);fade(from,_black_pallete,speed);}
void _fade_in(RGB*p,int speed){fade(_black_pallete,p,speed);}
static int text_width(FONT*f,const char*t){int w=0;for(;*t;++t){int i=(uint8_t)*t-32;if(i>=0&&i<224&&f->glyph[i])w+=f->glyph[i]->w;}return w;}
void _textout(BITMAP*d,FONT*f,char*t,int x,int y,int c){if(!f||!t)return;for(;*t;++t){int i=(uint8_t)*t-32;if(i<0||i>=224||!f->glyph[i])continue;BITMAP*g=f->glyph[i];for(int yy=0;yy<g->h;++yy)for(int xx=0;xx<g->w;++xx){int p=g->pixels[yy*g->w+xx];if(p)_putpixel(d,x+xx,y+yy,c<0?p:c);}x+=g->w;}}
void _textprintf(BITMAP*d,FONT*f,int x,int y,int c,char*fmt,...){char buf[1024];va_list a;va_start(a,fmt);vsnprintf(buf,sizeof buf,fmt,a);va_end(a);_textout(d,f,buf,x,y,c);}
void _textprintf_centre(BITMAP*d,FONT*f,int x,int y,int c,char*fmt,...){char buf[1024];va_list a;va_start(a,fmt);vsnprintf(buf,sizeof buf,fmt,a);va_end(a);_textout(d,f,buf,x-text_width(f,buf)/2,y,c);}
static BITMAP *bitmap_bytes(const uint8_t*p,bool depth){if(depth)p+=2;int w=be16(p),h=be16(p+2);auto b=_create_bitmap(w,h);memcpy(b->pixels,p+4,w*h);return b;}
DATAFILE *_load_datafile(char*name){
 bool game=upper(name)=="GAME.DAT";int count=game?int(std::size(game_types)):int(std::size(menu_types));auto types=game?game_types:menu_types;
 auto data=(DATAFILE*)calloc(count+1,sizeof(DATAFILE));
 for(int i=0;i<count;++i){char number[16];snprintf(number,sizeof number,"%03d.bin",i);auto b=read_file(resources/"assets"/(game?"GAME":"MENU")/number);if(b.empty()){fprintf(stderr,"Missing asset %s/%s\n",name,number);exit(1);}data[i].size=b.size();data[i].type=be32((const uint8_t*)types[i]);
  if(!strcmp(types[i],"BMP "))data[i].dat=bitmap_bytes(b.data(),true);
  else if(!strcmp(types[i],"FONT")){
   auto f=(FONT*)calloc(1,sizeof(FONT));int height=(int16_t)be16(b.data());size_t pos=2;
   for(int c=0;c<224;++c){if(height>0){auto g=_create_bitmap(8,height);for(int y=0;y<height;++y)for(int x=0;x<8;++x)g->pixels[y*8+x]=(b[pos+y]&(128>>x))?1:0;pos+=height;f->glyph[c]=g;}else{f->glyph[c]=bitmap_bytes(b.data()+pos,false);pos+=4+f->glyph[c]->w*f->glyph[c]->h;}f->height=std::max(f->height,f->glyph[c]->h);}data[i].dat=f;
  }else if(!strcmp(types[i],"SAMP")){
   auto s=(SAMPLE*)calloc(1,sizeof(SAMPLE));s->bits=be16(b.data());s->freq=be16(b.data()+2);s->len=be32(b.data()+4);s->loop_end=s->len;s->priority=128;s->data=malloc(b.size()-8);memcpy(s->data,b.data()+8,b.size()-8);data[i].dat=s;
  }else {data[i].dat=malloc(b.size());memcpy(data[i].dat,b.data(),b.size());}
 }data[count].type=-1;return data;
}
void _unload_datafile(DATAFILE*d){if(!d)return;for(int i=0;d[i].type!=-1;++i){switch(d[i].type){case 0x424d5020:_destroy_bitmap((BITMAP*)d[i].dat);break;case 0x464f4e54:{auto f=(FONT*)d[i].dat;for(auto g:f->glyph)_destroy_bitmap(g);free(f);break;}case 0x53414d50:{auto s=(SAMPLE*)d[i].dat;_stop_sample(s);free(s->data);free(s);break;}default:free(d[i].dat);}}free(d);}
BITMAP *_load_pcx(char*name,RGB*pal){
 auto v=read_file(original_path(name));
 if(v.size()<128||v[0]!=10||v[2]!=1||v[3]!=8||v[65]!=1)return nullptr;
 int w=le16(v.data()+8)-le16(v.data()+4)+1,h=le16(v.data()+10)-le16(v.data()+6)+1,stride=le16(v.data()+66);
 if(w<=0||h<=0||w>4096||h>4096||stride<w||stride>4096)return nullptr;
 auto b=_create_bitmap(w,h);size_t p=128;
 size_t end=v.size()>=897&&v[v.size()-769]==12?v.size()-769:v.size();
 for(int y=0;y<h;++y){int x=0;while(x<stride){
  if(p>=end){_destroy_bitmap(b);return nullptr;}int c=v[p++],n=1;
  if((c&0xc0)==0xc0){n=c&63;if(!n||p>=end){_destroy_bitmap(b);return nullptr;}c=v[p++];}
  if(n>stride-x){_destroy_bitmap(b);return nullptr;}
  while(n--){if(x<w)b->pixels[y*w+x]=c;++x;}
 }}
 if(pal&&end==v.size()-769){auto p=v.data()+v.size()-768;for(int i=0;i<256;++i)pal[i]={(uint8_t)(p[i*3]/4),(uint8_t)(p[i*3+1]/4),(uint8_t)(p[i*3+2]/4),0};}return b;
}
int _save_pcx(char*name,BITMAP*b,RGB*pal){auto v=cell::encode_pcx(b,pal);std::ofstream f(saves/fs::u8path(name),std::ios::binary);f.write((char*)v.data(),v.size());return f?0:-1;}
PACKFILE *_pack_fopen(char*name,char*mode){
 std::vector<uint8_t>b;
 if(strchr(mode,'p')){std::string error;b=cell::read_map(original_path(name),error);if(b.empty())fprintf(stderr,"%s\n",error.c_str());}
 else b=read_file(original_path(name));
 if(b.empty())return nullptr;
 auto f=(PACKFILE*)calloc(1,sizeof(PACKFILE));f->data=(uint8_t*)malloc(b.size());memcpy(f->data,b.data(),b.size());f->size=b.size();return f;
}
void _stretch_blit(BITMAP*s,BITMAP*d,int sx,int sy,int sw,int sh,int dx,int dy,int dw,int dh){
 if(!s||!d||dw<=0||dh<=0)return;
 for(int y=0;y<dh;++y)for(int x=0;x<dw;++x){int xx=sx+x*sw/dw,yy=sy+y*sh/dh;if(xx>=0&&xx<s->w&&yy>=0&&yy<s->h)_putpixel(d,dx+x,dy+y,s->pixels[yy*s->w+xx]);}
}
int _pack_getc(PACKFILE*f){return f&&f->pos<f->size?f->data[f->pos++]:EOF;}
int _pack_fclose(PACKFILE*f){if(f){free(f->data);free(f);}return 0;}
long pack_fread(void*p,long n,PACKFILE*f){n=std::min<unsigned long>(n,f->size-f->pos);memcpy(p,f->data+f->pos,n);f->pos+=n;return n;}
int pack_fseek(PACKFILE*f,int n){f->pos=std::min<unsigned long>(f->pos+n,f->size);return 0;}
long pack_fwrite(const void*,long,PACKFILE*){return 0;}
int pack_putc(int,PACKFILE*){return EOF;}
int _exists(char*p){return fs::exists(original_path(p))||fs::exists(saves/p);}
char *_get_filename(char*p){char*q=strrchr(p,'/');return q?q+1:p;}
char *cell_strupr(char*p){for(char*q=p;*q;++q)*q=std::toupper((uint8_t)*q);return p;}
void _free_map_filenames(){auto p=_maphead.next;while(p){auto n=p->next;free(p);p=n;}_maphead={};_num_maps=0;}
void _get_map_filenames(){
 _free_map_filenames();FileNode*tail=&_maphead;std::map<std::string,fs::path> files;
 for(auto dir:{resources/"original"/"MAPS",cell::map_directory()})if(fs::is_directory(dir))
  for(auto&e:fs::directory_iterator(dir))if(e.is_regular_file()&&upper(e.path().extension().u8string())==".CEL")files[upper(e.path().filename().u8string())]=e.path();
 for(auto&entry:files){auto name=entry.second.filename().u8string();std::string error;
  if(name.size()>70||cell::read_map(entry.second,error).empty())continue;
  auto n=(FileNode*)calloc(1,sizeof(FileNode));snprintf(n->name,sizeof n->name,"maps/%s",name.c_str());tail->next=n;tail=n;_num_maps++;
 }
}
void _free_mod_filenames(){auto p=_modhead.next;while(p&&p!=&_modhead){auto n=p->next;free(p);p=n;}_modhead={};_modhead.next=_modhead.prev=&_modhead;_cursong=&_modhead;_num_mods=0;}
void _get_mod_filenames(){_free_mod_filenames();for(auto&e:fs::directory_iterator(resources/"original"/"MUSIC"))if(upper(e.path().extension().string())==".MOD"){auto n=(ModNode*)calloc(1,sizeof(ModNode));snprintf(n->name,sizeof n->name,"music/%s",e.path().filename().u8string().c_str());n->prev=_modhead.prev;n->next=&_modhead;_modhead.prev->next=n;_modhead.prev=n;_num_mods++;}}
int _install_int(void(*fn)(),int ms){_remove_int(fn);timers.push_back({fn,(uint64_t)ms*1000,micros()+ms*1000});return 0;}
int _install_int_ex(void(*fn)(),int ticks){_remove_int(fn);uint64_t p=uint64_t(ticks)*1000000/1193181;timers.push_back({fn,std::max<uint64_t>(p,1),micros()+p});return 0;}
void _remove_int(void(*fn)()){timers.erase(std::remove_if(timers.begin(),timers.end(),[=](const Timer&t){return t.fn==fn;}),timers.end());}
int _keypressed(){replay_poll();native_present();SDL_Delay(1);return !key_buffer.empty();}
int _readkey(){while(!_keypressed()){}int k=key_buffer.front();key_buffer.pop_front();return k;}
void _clear_keybuf(){key_buffer.clear();}
int _clock(){return int((micros()-start_us)*91/1000000);}
uint _rawclock(){return (uint)std::chrono::system_clock::now().time_since_epoch().count();}
void _srand(uint s){random_state=s;}
int _rand(){random_state=random_state*UINT64_C(0x5851f42d4c957f2d)+1;return int((random_state>>21)&0x7fffffff);}
int _itofix(int x){return (int)(uint32_t(x)<<16);}
int _fixtoi(int x){return (x>>16)+((x&0x8000)>>15);}
int _ftofix(double x){if(x>32767.0)return INT32_MAX;if(x< -32767.0)return INT32_MIN;return int(x*65536.0+(x<0?-0.5:0.5));}
int _fcos(int x){return cos_table[((x>>15)+((x&0x4000)?1:0))&511];}
int _fsin(int x){return cos_table[((x>>15)-128+((x&0x4000)?1:0))&511];}
int _fatan2(int y,int x){if(!x)return y?(y<0?-0x400000:0x400000):0;int64_t div=(int64_t(y)*65536)/x;int q=std::clamp<int64_t>(div,-INT32_MAX,INT32_MAX),lo=q<0?128:0,hi=q<0?255:127,m=0;do{m=(lo+hi)>>1;int d=int(uint32_t(q)-uint32_t(tan_table[m]));if(d<0)hi=m-1;else if(d>0)lo=m+1;}while(lo<=hi&&q!=tan_table[m]);int a=m*32768+(q<0?-0x800000:0);if(x<0)a+=y<0?-0x800000:0x800000;return a;}
double _sqrt(double x){return sqrt(x);}
static std::map<std::string,int> config;
void _set_config_file(char*){fs::path file=fs::exists(saves/"CELL.CFG")?saves/"CELL.CFG":resources/"original"/"CELL.CFG";std::ifstream f(file);std::string s,section;while(std::getline(f,s)){if(s.empty())continue;if(s[0]=='['){section=s.substr(1,s.find(']')-1);continue;}auto p=s.find('=');if(p!=std::string::npos){std::string k=s.substr(0,p);k.erase(std::remove_if(k.begin(),k.end(),[](char c){return std::isspace((uint8_t)c);}),k.end());config[section+"/"+k]=atoi(s.c_str()+p+1);}}}
int _get_config_int(char*s,char*k,int d){auto i=config.find(std::string(s)+"/"+k);return i==config.end()?d:i->second;}
void _set_config_int(char*s,char*k,int v){config[std::string(s)+"/"+k]=v;std::ofstream f(saves/"CELL.CFG");f<<"[volume]\nsound = "<<config["volume/sound"]<<"\nmusic = "<<config["volume/music"]<<"\n\n[misc]\nmaxkills = "<<config["misc/maxkills"]<<"\nmaxtime = "<<config["misc/maxtime"]<<"\n";}

struct Voice{SAMPLE*s=nullptr;double pos=0;int freq=0,volume=255,pan=128,mode=0,direction=1;bool playing=false,temporary=false;};
static std::array<Voice,64> voices;
static std::recursive_mutex audio_mutex;
static void mix(void*,Uint8*stream,int length){std::lock_guard<std::recursive_mutex> lock(audio_mutex);auto out=(float*)stream;std::fill(out,out+length/4,0.f);for(int n=0;n<length/8;++n){float left=0,right=0;for(auto&v:voices){if(!v.playing||!v.s||!v.s->data||!v.s->len)continue;auto*s=v.s;int pos=int(v.pos);unsigned end=(v.mode&PLAYMODE_LOOP)?s->loop_end:s->len;if(pos<0||pos>=int(end)){if(v.mode&PLAYMODE_LOOP){if(v.mode&PLAYMODE_BIDIR){v.direction=-v.direction;v.pos=std::clamp(v.pos,0.0,double(end-1));}else v.pos=s->loop_start;pos=int(v.pos);}else{v.playing=false;if(v.temporary)v.s=nullptr;continue;}}float sample=s->bits==16?(int(((uint16_t*)s->data)[pos])-32768)/32768.f:(int(((uint8_t*)s->data)[pos])-128)/128.f;float gain=v.volume/255.f;left+=sample*gain*(255-v.pan)/255.f;right+=sample*gain*v.pan/255.f;v.pos+=v.direction*double(v.freq)/44100;if(v.direction<0&&v.pos<s->loop_start){v.direction=1;v.pos=s->loop_start;}}out[n*2]=std::clamp(left/4.f,-1.f,1.f);out[n*2+1]=std::clamp(right/4.f,-1.f,1.f);}}
int _install_sound(int,int,char*){SDL_AudioSpec want={},got={};want.freq=44100;want.format=AUDIO_F32SYS;want.channels=2;want.samples=512;want.callback=mix;audio_device=SDL_OpenAudioDevice(nullptr,0,&want,&got,0);if(!audio_device)return -1;SDL_PauseAudioDevice(audio_device,0);return 0;}
int allocate_voice(SAMPLE*s){std::lock_guard<std::recursive_mutex>l(audio_mutex);for(int i=0;i<64;++i)if(!voices[i].s){voices[i]=Voice{};voices[i].s=s;voices[i].freq=s->freq;return i;}return -1;}
void deallocate_voice(int v){std::lock_guard<std::recursive_mutex>l(audio_mutex);if(v>=0&&v<64)voices[v]=Voice{};}
void reallocate_voice(int v,SAMPLE*s){std::lock_guard<std::recursive_mutex>l(audio_mutex);if(v>=0&&v<64){voices[v].s=s;voices[v].pos=0;voices[v].direction=1;voices[v].playing=false;}}
#define VOICE_SET(name,field) void name(int v,int x){std::lock_guard<std::recursive_mutex>l(audio_mutex);if(v>=0&&v<64)voices[v].field=x;}
VOICE_SET(voice_set_frequency,freq)
VOICE_SET(voice_set_volume,volume)
VOICE_SET(voice_set_pan,pan)
VOICE_SET(voice_set_playmode,mode)
VOICE_SET(voice_set_position,pos)
void voice_start(int v){std::lock_guard<std::recursive_mutex>l(audio_mutex);if(v>=0&&v<64)voices[v].playing=true;}
void voice_stop(int v){std::lock_guard<std::recursive_mutex>l(audio_mutex);if(v>=0&&v<64)voices[v].playing=false;}
int voice_get_position(int v){std::lock_guard<std::recursive_mutex>l(audio_mutex);return v>=0&&v<64&&voices[v].playing?int(voices[v].pos):-1;}
void lock_sample(SAMPLE*){}
void _unlock_dpmi_data(void*,int){}
void register_datafile_object(int,void*(*)(PACKFILE*,long),void(*)(void*)){}
int _play_sample(SAMPLE*s,int vol,int pan,int freq,int loop){std::lock_guard<std::recursive_mutex>l(audio_mutex);int v=allocate_voice(s);if(v<0)return -1;auto&a=voices[v];a.freq=s->freq*freq/1000;a.volume=std::clamp(vol,0,255);a.pan=std::clamp(pan,0,255);a.mode=loop?1:0;a.temporary=true;a.playing=true;return v;}
void _stop_sample(SAMPLE*s){std::lock_guard<std::recursive_mutex>l(audio_mutex);for(auto&v:voices)if(v.s==s)v=Voice{};}
int _install_mod(int n){return install_mod(n);}
void _set_mod_volume(int n){set_mod_volume(n);}
MOD *_load_mod(char*p){return load_mod(p);}
void _destroy_mod(MOD*m){std::lock_guard<std::recursive_mutex>l(audio_mutex);if(m)destroy_mod(m);}
void _play_mod(MOD*m,int loop){if(m)play_mod(m,loop);}
void _stop_mod(){stop_mod();}
int _is_mod_playing(){return is_mod_playing();}
int _is_mod_paused(){return is_mod_paused();}
void _pause_mod(){pause_mod();}
void _resume_mod(){resume_mod();}
void native_shutdown(){if(audio_device){SDL_CloseAudioDevice(audio_device);audio_device=0;}initialized=false;SDL_Quit();}
int main(int argc,char**argv){
#ifdef CELL_TEST_BUILD
 // Preserve the last completed check even when a subprocess crashes.
 setvbuf(stdout,nullptr,_IONBF,0);
#endif
 char *base_path=SDL_GetBasePath();
 if(!base_path){fprintf(stderr,"Cannot locate application: %s\n",SDL_GetError());return 1;}
 auto base=fs::u8path(base_path);SDL_free(base_path);
 if(base.filename().empty())base=base.parent_path();
 // SDL returns Contents/Resources for Mac bundles, and the EXE directory on Windows.
 if(const char*p=getenv("CELL_RESOURCE_DIR"))resources=fs::u8path(p);
 else if(fs::exists(base/"original"))resources=base;
 else if(fs::exists(base/"resources"/"original"))resources=base/"resources";
 else resources=base.parent_path()/"analysis";
 if(const char*p=getenv("CELL_SAVE_DIR"))saves=fs::u8path(p);
 else{char*pref=SDL_GetPrefPath("KevlarX","CellBlockA");if(!pref){fprintf(stderr,"Cannot locate save directory: %s\n",SDL_GetError());return 1;}saves=fs::u8path(pref);SDL_free(pref);}
 fs::create_directories(saves);
 if(!fs::exists(resources/"original")){fprintf(stderr,"Missing original resources in %s\n",resources.u8string().c_str());return 1;}
#ifdef CELL_TEST_BUILD
 if(getenv("CELL_VERIFY")){extern int run_core_tests();return run_core_tests();}
#endif
 if(const char*p=getenv("CELL_INPUT_SCRIPT")){
  scripted_replay=true;
  std::ifstream f(fs::u8path(p));
  if(!f){fprintf(stderr,"Cannot open replay input file\n");return 1;}
  while(f>>std::ws && !f.eof()){
   uint64_t ms;int scan,down;
   if(!(f>>ms>>scan>>down) || !native_input_step_valid(ms,scan,down) ||
      (!test_input.empty() && ms*1000<test_input.back().at)){
    fprintf(stderr,"Invalid replay input: expected ordered milliseconds, scan code 0-127, and state 0 or 1\n");return 1;
   }
   test_input.push_back({ms*1000,scan,down});
  }
 }
 if(const char*p=getenv("CELL_TEST_SECONDS"))test_duration=uint64_t(atof(p)*1000000);
 if(const char*p=getenv("CELL_TEST_CAPTURE"))test_capture=fs::u8path(p);
#ifdef JAILED_APP
 editor_mode=true;
 return jailed_main(argc,argv);
#else
 return _main(argc,argv);
#endif
}
