#ifdef NDEBUG
#error "Regression tests must be compiled with assertions enabled"
#endif
#include "native.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <climits>
#include "rendering.h"
#include "replay_clock.h"

static void test_map_bounds(){
  // In-range classification is unchanged, including every original tile byte.
  for(int kind=0;kind<256;++kind){
    int expected=kind==4?2:(kind>=6&&kind<=17?1:0);
    for(int x=0;x<20;++x)for(int y=0;y<13;++y){
      _map[x][y].kind=kind;
      assert(_hit(x*32,y*32)==expected);
      assert(_hit(x*32+31,y*32+31)==expected);
    }
  }
  // Solid tiles and player bytes must never affect an outside-map query.
  for(auto &column:_map)for(auto &tile:column)tile.kind=7;
  memset(_player,4,sizeof _player);
  const int xs[]={INT_MIN,-640,-33,-32,-1,0,31,32,639,640,641,INT_MAX};
  const int ys[]={INT_MIN,-416,-33,-32,-1,0,31,32,415,416,417,INT_MAX};
  for(int x:xs)for(int y:ys)
    assert(_hit(x,y)==(x>=0&&x<640&&y>=0&&y<416?1:0));
  Player players_before[2];memcpy(players_before,_player,sizeof _player);
  auto maphead_before=_maphead;
  const int rectangles[][4]={
    {-48,-33,16,20}, {-100,-100,-1,-1}, {640,416,700,500},
    {-32,32,-1,63}, {0,-32,31,-1}, {640,0,671,415}, {0,416,639,447},
    {624,400,680,460}, {31,31,32,32}, {0,0,639,415},
    {10,10,9,20}, {10,10,20,9}, {639,415,639,415},
    {INT_MIN,INT_MIN,INT_MAX,INT_MAX}, {INT_MIN,0,INT_MIN,415},
    {INT_MAX,0,INT_MAX,415}
  };
  for(const auto &r:rectangles){
    for(auto &column:_map)for(auto &tile:column)tile.dirty=0;
    _make_dirty(r[0],r[1],r[2],r[3]);
    for(int x=0;x<20;++x)for(int y=0;y<13;++y){
      // Independent overlap oracle; no clipping or coordinate division.
      bool overlap=r[0]<=r[2]&&r[1]<=r[3]&&
        x*32<=r[2]&&x*32+31>=r[0]&&y*32<=r[3]&&y*32+31>=r[1];
      assert(_map[x][y].dirty==int(overlap));
      assert(_map[x][y].kind==7);
    }
    assert(memcmp(players_before,_player,sizeof _player)==0);
    assert(memcmp(&maphead_before,&_maphead,sizeof _maphead)==0);
  }
  memset(_map,0,sizeof _map);memset(_player,0,sizeof _player);
  puts("PASS bounded collision and clipped redraw rectangles, including extreme coordinates");
}

int run_core_tests(){
  ReplayClock replay;
  uint64_t wall=60000000; // Resource loading must not consume scripted inputs.
  assert(replay.poll(wall)==0);
  for(int i=0;i<90;++i)replay.poll(wall+=50000);
  assert(replay.elapsed==4500000); // First scripted Enter is now pressed.
  auto after_stall=replay.poll(wall+=10000000);
  assert(after_stall>=4500000&&after_stall<4600000); // Release must not collapse into the same poll.
  assert(replay.poll(wall+=50000)==4600000);
  puts("PASS replay excludes startup and preserves a key press across a long stall");
  assert(native_counter_micros(UINT64_C(1234567890123456789),UINT64_C(1000000000))==UINT64_C(1234567890123456));
  assert(native_counter_micros(UINT64_MAX,UINT64_C(24000000))==UINT64_C(768614336404564650));
  assert(native_counter_micros(UINT64_C(24000001),UINT64_C(24000000))==UINT64_C(1000000));
  puts("PASS timer conversion at long system uptimes");
  static_assert(sizeof(Player)==88);
  static_assert(sizeof(Point)==28);
  static_assert(sizeof(Tile)==2);
  assert(_allegro_init()==0);
  assert(_set_gfx_mode(0,640,480,0,0)==0);
  _game_data=_load_datafile((char*)"game.dat");
  _menu_data=_load_datafile((char*)"menu.dat");
  assert(_game_data[469].type==-1&&_menu_data[25].type==-1);
  int sixteen_bit_samples=0;
  for(int i=316;i<=445;++i){
    assert(_game_data[i].type==0x53414d50);
    auto sample=(SAMPLE*)_game_data[i].dat;
    assert(sample->len>0&&sample->data!=nullptr);
    assert(sample->bits==8||sample->bits==16);
    assert(sample->len*unsigned(sample->bits/8)==unsigned(_game_data[i].size-8));
    if(sample->bits==16)++sixteen_bit_samples;
  }
  assert(sixteen_bit_samples==119);
  auto sprite=_create_bitmap(2,2), flipped=_create_bitmap(2,2);
  sprite->pixels[0]=1;sprite->pixels[1]=2;sprite->pixels[2]=0;sprite->pixels[3]=4;
  memset(flipped->pixels,9,4);
  _draw_sprite_h_flip(flipped,sprite,0,0);
  const unsigned char expected_flip[]={2,1,4,9};
  assert(memcmp(flipped->pixels,expected_flip,4)==0);
  _destroy_bitmap(sprite);_destroy_bitmap(flipped);
  puts("PASS 1.3b: 469 game assets, 25 menu assets, 130 sound samples, horizontal sprite flip");
  _buffer=_create_bitmap(640,480);
  native_arena_background=_create_bitmap(640,480);
  _blit((BITMAP*)_game_data[0].dat,native_arena_background,0,0,0,0,640,480);
  test_rendering();
  _nosound=1;
  _srand(1);
  uint64_t state=1;
  for(int i=0;i<1000;++i){state=state*UINT64_C(0x5851f42d4c957f2d)+1;assert(_rand()==int((state>>21)&0x7fffffff));}
  assert(_fixtoi(32768)==1);
  assert(_fixtoi(-32768)==0);
  assert(_fixtoi(-32769)==-1);
  assert(_itofix(-1)==-65536);
  assert(_ftofix(0.15)==9830);
  assert(_fcos(0)==65536);
  assert(_fsin(0)==0);
  assert(_fsin(64*65536)==65536);
  assert(_distance(0,0,3,4)==5);
  assert(_collide(0,0,10,10,5,5,10,10)==1);
  assert(_collide(0,0,10,10,20,20,10,10)==0);
  assert(_collide(0,0,10,10,10,10,10,10)==1); // touching edges collide
  test_map_bounds();
  _get_map_filenames();
  assert(_num_maps==12);
  _reset_points();
  for(const auto &p:_point){assert(p.life==-1&&p.color==255&&p.type==0);}
  _match_start=_clock()-91*35;
  _item_timer();
  int items=0;
  for(const auto &p:_point)if(p.life>0&&p.type>0)++items;
  assert(items>0);
  puts("PASS original particle initialization and timed item spawning");
  int maps=0;
  for(auto n=_maphead.next;n;n=n->next){
    _load_map(n->name);
    _reset_points();
    _lastspot=-1;
    _srand(17);
    _spawn_player(0);_spawn_player(1);
    assert(_player[0].health==99&&_player[1].health==99);
    assert(_player[0].height==25&&_player[1].height==25);
    assert(_player[0].state==0&&_player[1].state==0);
    assert(_player[0].frame==315&&_player[1].frame==315);
    _draw_map();
    for(int tick=0;tick<500;++tick){
      _player[0].left=tick%200<70;
      _player[0].right=tick%200>=100&&tick%200<170;
      _player[0].up=tick%80<10;
      _player[0].down=tick%90>80;
      _player[1].left=tick%160<50;
      _player[1].right=tick%160>=80&&tick%160<130;
      _player[1].up=tick%100<10;
      _player[1].down=tick%70>60;
      for(int p=0;p<2;++p){
        if(_player[p].health==0&&_endmatch<=0)_spawn_player(p);
        if(tick%80==0){_player[p].weapon=(tick/80)%4;_player[p].ammo=6;}
        if(tick%15==0)_player_attack(p);
      }
      _game_tick();_translate_points();_translate_points();
      if(tick%50==0)_item_timer();
      _clean_map();_draw_player(0);_draw_player(1);_draw_points();
      assert(_player[0].health>=0&&_player[1].health>=0);
    }
    printf("PASS map %s: 500 player ticks, 1000 particle ticks, all weapon selections\n",n->name);
    maps++;
  }
  assert(maps==12);
  extern void run_jailed_tests();
  run_jailed_tests();
  _reset_points();
  _player[0].health=99;
  _damage_player(0,5,100,100,2);
  assert(_player[0].health==94);
  _damage_player(0,99,100,100,2);
  assert(_player[0].health==0&&_endmatch==75);
  _BLOODBATH=1;
  _player[0].health=99;
  _damage_player(0,99,100,100,2);
  assert(_player[0].health>=94&&_player[0].health<=98);
  _BLOODBATH=0;
  assert(_install_sound(-1,0,nullptr)==0);
  assert(_install_mod(16)>=0);
  for(int i=316;i<=445;++i){
    auto sample=(SAMPLE*)_game_data[i].dat;
    assert(_play_sample(sample,255,128,1000,0)>=0);
    _vsync();
    _stop_sample(sample);
  }
  puts("PASS playback of all 130 original 1.3b sound samples through native mixer");
  _get_mod_filenames();
  assert(_num_mods==8);
  _nosound=0;
  for(int i=0;i<18;++i){_next_song();assert(_music!=nullptr);}
  for(int i=0;i<18;++i){_prev_song();assert(_music!=nullptr);}
  _pause_mod();assert(_is_mod_paused());
  _resume_mod();assert(!_is_mod_paused());
  _stop_mod();_destroy_mod(_music);_music=nullptr;
  _free_mod_filenames();
  puts("PASS bloodbath damage and all 8 MOD files with next/previous wraparound");
  _free_map_filenames();
  _destroy_bitmap(native_arena_background);_destroy_bitmap(_buffer);_unload_datafile(_game_data);_unload_datafile(_menu_data);
  native_shutdown();
  puts("PASS recovered layouts, RNG, fixed-point, collision, damage, assets, maps and simulation");
  return 0;
}
