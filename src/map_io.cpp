// Portable original Allegro map/PCX serialization. No DOS runtime is used.
#include "map_io.h"
#include <algorithm>
#include <array>
#include <fstream>
#include <cctype>
#include <system_error>
namespace cell {
fs::path resource_dir, save_dir;
static std::string upper(std::string s) { for(auto& c:s)c=std::toupper((unsigned char)c);return s; }
fs::path map_directory() { auto p=save_dir/"maps";fs::create_directories(p);return p; }
static fs::path lookup(const fs::path& dir,const fs::path& name) {
 if(fs::is_regular_file(dir/name))return dir/name;
 if(fs::is_directory(dir))for(auto& e:fs::directory_iterator(dir))
  if(e.is_regular_file()&&upper(e.path().filename().u8string())==upper(name.u8string()))return e.path();
 return {};
}
fs::path resolve_original(const char* name) {
 auto p=fs::u8path(name);
 if(p.is_absolute())return p;
 if(upper(p.parent_path().generic_u8string())=="MAPS") {
  auto user=lookup(map_directory(),p.filename());if(!user.empty())return user;
  auto bundled=lookup(resource_dir/"original/MAPS",p.filename());if(!bundled.empty())return bundled;
 }
 return resource_dir/"original"/upper(p.generic_u8string());
}
bool validate_map(const std::vector<unsigned char>& b,std::string& error) {
 error.clear();
 if(b.size()!=272){error="Map must contain exactly 272 bytes.";return false;}
 for(int i=0;i<260;++i)if(b[i]>17){error="Map contains an invalid tile.";return false;}
 int starts=0;
 for(int i=260;i<272;i+=3){
  int x=b[i],y=b[i+1];
  if(b[i+2]>1){error="Invalid starting direction.";return false;}
  if((x==255||x==127)&&(y==255||y==127))continue;
  if(x>=20||y>=13){error="Start point is outside the map.";return false;}
  ++starts;
 }
 if(starts<2){error="Not enough start points!";return false;}
 return true;
}
std::vector<unsigned char> read_map(const fs::path& path,std::string& error) {
 std::ifstream f(path,std::ios::binary);
 if(!f){error="Could not open map: "+path.u8string();return {};}
 std::vector<unsigned char> raw;
 char c;while(f.get(c)){raw.push_back((unsigned char)c);if(raw.size()>4096){error="Map file is too large.";return {};}}
 std::vector<unsigned char> b;
 if(raw.size()<4){error="Truncated map.";return {};}
 if(std::equal(raw.begin(),raw.begin()+4,"slh."))b.assign(raw.begin()+4,raw.end());
 else if(std::equal(raw.begin(),raw.begin()+4,"slh!")) {
  std::array<unsigned char,4096> ring{};size_t pos=4;unsigned write=4096-18;
  while(pos<raw.size()&&b.size()<=272){
   unsigned flags=raw[pos++];
   for(int bit=0;bit<8&&pos<raw.size();++bit){
    if(flags&(1<<bit)){auto v=raw[pos++];b.push_back(v);ring[write]=v;write=(write+1)&4095;}
    else {
     if(pos+2>raw.size()){error="Truncated compressed map.";return {};}
     unsigned lo=raw[pos++],hi=raw[pos++],read=lo|((hi&240)<<4);
     for(unsigned j=0;j<(hi&15)+3;++j){auto v=ring[(read+j)&4095];b.push_back(v);ring[write]=v;write=(write+1)&4095;}
    }
    if(b.size()>272)break;
   }
  }
  if(pos!=raw.size()){error="Excess compressed map data.";return {};}
 } else {error="Not an Allegro map packfile.";return {};}
 if(!validate_map(b,error))return {};
 return b;
}
bool load_map(const fs::path& path,std::string& error) {
 auto b=read_map(path,error);if(b.empty())return false;
 for(int x=0;x<20;++x)for(int y=0;y<13;++y)_map[x][y]={b[x*13+y],0};
 for(int i=0;i<4;++i){int n=260+i*3;_start[i].packed=(b[n]&127)|((b[n+1]&127)<<7)|(b[n+2]<<14);_start[i].unused=0;}
 return true;
}
std::vector<unsigned char> encode_map(std::string& error) {
 std::vector<unsigned char>b;
 for(auto& column:_map)for(auto& tile:column)b.push_back(tile.kind);
 for(auto& start:_start){int x=start.packed&127,y=(start.packed>>7)&127;b.push_back(x==127?255:x);b.push_back(y==127?255:y);b.push_back((start.packed>>14)&1);}
 if(!validate_map(b,error))return {};
 return b;
}
std::vector<unsigned char> encode_pcx(BITMAP* b,const RGB* pal) {
 std::vector<unsigned char> v(128);v[0]=10;v[1]=5;v[2]=1;v[3]=8;
 auto put=[&](int i,int n){v[i]=n&255;v[i+1]=n>>8;};
 int stride=(b->w+1)&~1;put(8,b->w-1);put(10,b->h-1);put(12,b->w);put(14,b->h);v[65]=1;put(66,stride);put(68,1);
 for(int y=0;y<b->h;++y)for(int x=0;x<stride;++x){unsigned char c=x<b->w?b->pixels[y*b->w+x]:0;if(c>=192)v.push_back(193);v.push_back(c);}
 v.push_back(12);for(int i=0;i<256;++i){v.push_back(pal[i].r*4);v.push_back(pal[i].g*4);v.push_back(pal[i].b*4);}return v;
}
static bool write(const fs::path& path,const std::vector<unsigned char>& bytes) {
 std::ofstream f(path,std::ios::binary|std::ios::trunc);f.write((const char*)bytes.data(),bytes.size());f.close();return bool(f);
}
bool save_map(const fs::path& path,BITMAP* preview,std::string& error) {
 if(upper(path.extension().u8string())!=".CEL"){error="Map filename must end in .cel.";return false;}
 auto b=encode_map(error);if(b.empty())return false;
 // Allegro's packed stream accepts literal groups as well as LZSS matches.
 std::vector<unsigned char> packed={'s','l','h','!'};
 for(size_t i=0;i<b.size();i+=8){packed.push_back(255);packed.insert(packed.end(),b.begin()+i,b.begin()+std::min(i+8,b.size()));}
 auto pic=path;pic.replace_extension(".pcx");
 auto temp=path;temp+=".tmp";auto ptemp=pic;ptemp+=".tmp";
 if(!write(temp,packed)||!write(ptemp,encode_pcx(preview,__current_pallete))){error="Could not write map and preview.";std::error_code ec;fs::remove(temp,ec);fs::remove(ptemp,ec);return false;}
 // Move complete files into place; readers never see a partially written map.
 std::error_code ec;
#ifdef _WIN32
 // std::filesystem::rename cannot replace a destination on Windows.
 // MoveFileExW is supplied below through the small platform helper.
 extern bool replace_file(const fs::path&,const fs::path&);
 if(!replace_file(ptemp,pic)||!replace_file(temp,path)){error="Could not replace map files.";return false;}
#else
 fs::rename(ptemp,pic,ec);if(!ec)fs::rename(temp,path,ec);
 if(ec){error="Could not replace map files: "+ec.message();return false;}
#endif
 return true;
}
}
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#include <windows.h>
namespace cell { bool replace_file(const fs::path& a,const fs::path& b){return MoveFileExW(a.c_str(),b.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)!=0;} }
#endif
