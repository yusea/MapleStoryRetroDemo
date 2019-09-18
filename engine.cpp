#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>

#include "sprite.h"
#include "multisprite.h"
#include "twowayMultisprite.h"

#include "smartSprite.h"
#include "smartMultiSprite.h"

#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "bullet.h"

#include "collisionStrategy.h"


Engine::~Engine() { 
  delete player;
  for(auto enemy : enemies) {
    delete enemy;
  }
  // for(auto sprite : sprites_vec) {
  //       if(sprite != nullptr) { delete sprite; }
  //       else {
  //         // std::string("null sprite pointer");
  //       }
  //     }
  for(CollisionStrategy* strategy : strategies) {
    delete strategy;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  //background
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  background1("cloud", Gamedata::getInstance().getXmlInt("cloud/factor") ),
  viewport( Viewport::getInstance() ),
  // star(new Sprite("YellowStar")),
  // spinningStar(new MultiSprite("SpinningStar")),
  // spinningyStar(new MultiSprite("YellowStar")),
  player( new Player("mychar","magic_cross")),
  enemies(),
  strategies(),
  currentStrategy(1),
  collision(false),
  makeVideo(false),
  hud_on(true),
  GODmode_on(false),
  hud(HUD::getInstance()),
  infoHUD( InfoHUD::getInstance() ),
  sound(SDLSound::getInstance())
  // sprites_vec(),
  // currentSprite(0)
{
  // int n = Gamedata::getInstance().getXmlInt("numberOfStars");

  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();

  for(int i = 0;
    i < Gamedata::getInstance().getXmlInt("yellow_mushroom/number");
    i++) {
    enemies.push_back(new SmartMultiSprite("yellow_mushroom", pos, w, h));
    player->attach(enemies.back());
  }

  for(int i = 0;
    i < Gamedata::getInstance().getXmlInt("RedSnail/number");
    i++) {
    enemies.push_back(new SmartMultiSprite("RedSnail", pos, w, h));
    player->attach(enemies.back());
  }
   
  for(int i = 0;
    i < Gamedata::getInstance().getXmlInt("zombie_mushroom/number");
    i++) {
    enemies.push_back(new SmartMultiSprite("zombie_mushroom", pos, w, h));
    player->attach(enemies.back());
  } 
  
  // for(int i = 0;
  //     i < Gamedata::getInstance().getXmlInt("mychar/number");
  //     i++) {
  //     sprites_vec.emplace_back(new MultiSprite("mychar"));
  // }
  strategies.push_back(new RectangularCollisionStrategy);
  strategies.push_back(new PerPixelCollisionStrategy);
  strategies.push_back(new MidPointCollisionStrategy);
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;

  // Viewport::getInstance().setObjectToTrack(sprites_vec[sprites_vec.size() - 1]);
  // std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  infoHUD.addLine("Smart Enemies Remaining: " + std::to_string(enemies.size()));
  infoHUD.addLine("GOD mode is: " + ( GODmode_on ? std::string("ON") : std::string("OFF")));
  world.draw();
  background1.draw();

  for(auto enemy : enemies) {
      enemy->draw();
  }
  player->draw();
  viewport.draw();

  strategies[currentStrategy]->draw();

  // std::stringstream strm;
  // // strm << enemies.size() << " Smart Enemies Remaining";
  // hud.addLine(strm.str());
  
  if(hud_on){
    hud.draw();
    infoHUD.draw();
  }

  // if ( collision ) {
  //   IoMod::getInstance().writeText("Oops: Collision", 500, 90);
  // }
  if ( enemies.size() == 0 ) {
      SDL_Rect rect = {200, 200, 220, 60};
      SDL_SetRenderDrawColor( renderer, 255, 255, 255, 200 );
      SDL_RenderFillRect( renderer, &rect );
      io.writeText("Congratulation ! YOU WIN !", 210,215,
                   {0, 128, 128, 255});
      io.writeText("Press R to Restart the game", 210, 230,
                   {0, 128, 128, 255});
      clock.pause();
  }
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {

    // Check enemies attack the player
    auto it = enemies.begin();
    while ( it != enemies.end() ) {
        if ( strategies[currentStrategy]->execute(*player, **it) ) {
          if(GODmode_on) break;
          if(!(player->isCrouch()) && !((*it)->isExplode())) {
            player->explode();
            break;
          }
        }
        else it++;
    }

    // Check the bullets attack the enemies
    std::list<Bullet*>& bulletList = player->getBullets().getBulletList();
    auto it1 = enemies.begin();
    bool destroyed = false;
    while ( it1 != enemies.end() ) {
        //std::cout << "- it1: " << static_cast<void*>(*it1) << '\n';
        destroyed = false;
        auto it2 = bulletList.begin();
        while ( it2 != bulletList.end() ) {
            //std::cout << "it2: " << static_cast<void*>(*it2) << '\n';
            if ( strategies[currentStrategy]->execute(**it1, **it2) ) {
                if(!((*it1)->isExplode()) && !((*it2)->isExplode())) {
                    (*it2)->explode();
                    (*it1)->explode();
                    player->detach(*it1);
                    delete (*it1);
                    it1 = enemies.erase(it1);
                    destroyed = true;
                    break;
                }
                else it2++;
            }
            else it2++;
        }
        if(!destroyed) it1++;
    }
}

void Engine::update(Uint32 ticks) {
  // star->update(ticks);
  // spinningStar->update(ticks);
  // spinningyStar->update(ticks);
  collision = false;
  checkForCollisions();
  player->update(ticks);

  for(auto enemy : enemies){
    enemy->update(ticks);
  }
  hud.update();
  infoHUD.update();
  world.update();
  viewport.update(); // always update viewport last
}

// void Engine::switchSprite(){
//   ++currentSprite;
//   // currentSprite = currentSprite % 2;
//   if(currentSprite >= int(sprites_vec.size())) {
//      currentSprite = 0;
//   }
//   Viewport::getInstance().setObjectToTrack(sprites_vec[currentSprite]);

// }

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        // if ( keystate[SDL_SCANCODE_T] ) {
        //   switchSprite();
        // }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if ( keystate[SDL_SCANCODE_I] ) {
          hud_on = !hud_on;
        }
        if (keystate[SDL_SCANCODE_G]) {
          GODmode_on = !GODmode_on;
          // player->crouch();
        }
        if (keystate[SDL_SCANCODE_SPACE]) {
          player->shoot();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();

      // movement control
      if (keystate[SDL_SCANCODE_A]) {
          static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
          static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
          // static_cast<Player*>(player)->up();
          static_cast<Player*>(player)->jump();
      }
      if (keystate[SDL_SCANCODE_S]) {
          // static_cast<Player*>(player)->down();
          static_cast<Player*>(player)->crouch();
      }
      if (keystate[SDL_SCANCODE_E]) {
          // static_cast<Player*>(player)->down();
          static_cast<Player*>(player)->explode();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
