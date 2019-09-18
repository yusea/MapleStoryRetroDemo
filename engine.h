#include <vector>
#include <SDL.h>
// #include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "infoHUD.h"
#include "sound.h"

class CollisionStrategy;
class SmartSprite;
class SmartSprite;
class SmartMultiSprite;
class SubjectSprite;
class Player;
class Bullet;


class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  // void switchSprite();

  // void printScales() const = delete;

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World world;
  World background1;
  Viewport& viewport;

  Player * player;
  // std::std::vector<SmartSprite*> enemies;
  std::vector<SmartMultiSprite*> enemies;
  // std::vector<Drawable*> enemies;

  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  bool collision;
  bool makeVideo;
  bool hud_on;
  bool GODmode_on;
  HUD& hud;
  InfoHUD& infoHUD; //note: &
  SDLSound& sound;



  // std::vector<Drawable*> sprites_vec;

  // int currentSprite;

  void draw() const;
  void update(Uint32);

  void checkForCollisions();  
};
