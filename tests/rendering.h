// Pixel oracles use the original asset pixels directly, not the host's drawing
// functions. The offsets/ranges come from the original player/tile routines.
#include <vector>
#include <algorithm>

static void reference_sprite(std::vector<unsigned char>& pixels, BITMAP* source,
                             int x, int y, bool mirror) {
  for(int row=0;row<source->h;++row)for(int column=0;column<source->w;++column){
    int dx=x+(mirror?source->w-1-column:column),dy=y+row;
    unsigned char color=source->pixels[row*source->w+column];
    if(color&&dx>=0&&dx<640&&dy>=0&&dy<480)pixels[dy*640+dx]=color;
  }
}

static void test_rendering(){
  constexpr int size=640*480;
  std::vector<unsigned char> expected(size);
  // Includes both orientations of every platform end/corner, plus empty tiles.
  const int tile_assets[]={-1,-1,-1,-1,446,-1,447,448,449,450,451,452,453,454,447,448,449,450};
  for(int kind=0;kind<256;++kind){
    memset(_buffer->pixels,251,size);
    std::fill(expected.begin(),expected.end(),251);
    for(int y=64;y<96;++y)for(int x=64;x<96;++x)
      expected[y*640+x]=native_arena_background->pixels[y*640+x];
    if(kind<18&&tile_assets[kind]>=0)
      reference_sprite(expected,(BITMAP*)_game_data[tile_assets[kind]].dat,64,64,kind>=14);
    _map[2][2].kind=kind;
    _draw_tile(_buffer,2,2);
    if(memcmp(expected.data(),_buffer->pixels,size)!=0){
      fprintf(stderr,"Tile pixel mismatch: kind=%d\n",kind);abort();
    }
  }
  // All player animation frames, both players and both directions. Climbing is
  // intentionally unmirrored; the extended extinguisher has asymmetric origins.
  struct Sequence {int state,weapon,first,last,left_x,right_x,y;bool mirror;};
  const Sequence sequences[]={
    {0,0,315,315,-24,-24,-20,true}, // standing
    {1,0,107,107,-24,-24,-20,true}, // crouching
    {2,0,210,239,-24,-24,-20,true}, // walking
    {3,0,195,209,-24,-24,-33,true}, // rolling
    {4,0,74,103,-30,-30,-20,false}, // climbing
    {5,0,130,159,-24,-24,-20,true}, // jumping/falling
    {6,0,160,174,-24,-24,-20,true}, // fists
    {6,1,175,194,-31,-31,-28,true}, // rebar
    {6,2,240,269,-24,-24,-20,true}, // shotgun
    {6,3,108,127,-24,-24,-24,true}, // extinguisher melee
    {6,3,290,314,-48,0,-24,true},   // extinguisher spray
    {7,0,44,73,-24,-24,-20,true},
    {8,0,14,43,-24,-24,-20,true},
    {9,0,270,289,-24,-24,0,true},
    {10,0,44,73,-24,-24,-20,true},
  };
  int cases=0;
  for(auto sequence:sequences)for(int frame=sequence.first;frame<=sequence.last;++frame)
    for(int index=0;index<2;++index)for(int facing=0;facing<2;++facing){
      auto &player=_player[index];player={};
      player.x=300;player.y=200;player.state=sequence.state;
      player.weapon=sequence.weapon;player.frame=frame;player.facing=facing;
      memset(_buffer->pixels,251,size);
      std::fill(expected.begin(),expected.end(),251);
      reference_sprite(expected,(BITMAP*)_game_data[frame].dat,
                       300+(facing?sequence.right_x:sequence.left_x),200+sequence.y,
                       sequence.mirror&&!facing);
      reference_sprite(expected,(BITMAP*)_game_data[128+index].dat,305,170,false);
      _draw_player(index);
      if(memcmp(expected.data(),_buffer->pixels,size)!=0){
        fprintf(stderr,"Player pixel mismatch: player=%d state=%d frame=%d facing=%d\n",
                index,sequence.state,frame,facing);abort();
      }
      ++cases;
    }
  memset(_map,0,sizeof _map);memset(_player,0,sizeof _player);
  printf("PASS exact tile pixels for all 256 kinds and %d player animation/direction cases\n",cases);
  memset(_point,0,sizeof _point);
  for(int frame=0;frame<6;++frame)for(int direction=-1;direction<=1;direction+=2){
    auto &point=_point[0];point={};
    point.life=frame+6;point.type=1;point.x=_itofix(300);point.y=_itofix(200);
    point.vx=_itofix(direction);point.vy=_itofix(1);
    memset(_buffer->pixels,251,size);std::fill(expected.begin(),expected.end(),251);
    reference_sprite(expected,(BITMAP*)_game_data[456+frame].dat,290,192,direction<0);
    _draw_points();
    assert(memcmp(expected.data(),_buffer->pixels,size)==0);
  }
  puts("PASS all 6 directional particle sprite frames, both directions");
  memset(_point,0,sizeof _point);memset(_map,0,sizeof _map);

  // A deterministic native-rendered contact sheet for visual review.
  _clear(_buffer);
  for(int kind=4;kind<=17;++kind){
    _map[kind][0].kind=kind;_draw_tile(_buffer,kind,0);
  }
  int cell=0;
  for(auto sequence:sequences){
    for(int facing=0;facing<2;++facing){
      auto &player=_player[facing];player={};
      player.x=32+(cell%4)*160+facing*64;player.y=100+(cell/4)*100;
      player.state=sequence.state;player.weapon=sequence.weapon;
      player.frame=(sequence.first+sequence.last)/2;player.facing=facing;
      _draw_player(facing);
    }
    ++cell;
  }
  assert(_save_pcx((char*)"rendering-regression.pcx",_buffer,(RGB*)_game_data[10].dat)==0);
  memset(_map,0,sizeof _map);memset(_player,0,sizeof _player);
}
