#include "multisprite.h"
#include "gameData.h"
// #include "imageFactory.h"
#include "renderContext.h"


void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Vector2f MultiSprite::makeVelocity(int vx, int vy) const {
    float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
    float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
    newvx *= [] (){ if(rand()%2) return -1; else return 1; } ();
    newvy *= [] (){ if(rand()%2) return -1; else return 1; } ();

    return Vector2f(newvx, newvy);
}

Vector2f MultiSprite::randomVec(int vx, int vy, int range) const {
    float newvx = Gamedata::getInstance().getRandFloat(vx-range,vx+range);;
    float newvy = Gamedata::getInstance().getRandFloat(vy-range,vy+range);;
    newvx *= [] (){ if(rand()%2) return 1; else return -1; } ();
    newvy *= [] (){ if(rand()%2) return 1; else return -1; } ();

    return Vector2f(newvx, newvy);
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  // images( RenderContext::getInstance()->getImages(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

MultiSprite::MultiSprite( const std::string& name,
                          const Vector2f& pos,
                          const Vector2f& vel) :
    Drawable(name, pos, vel),
    images( RenderContext::getInstance()->getImages(name) ),

    currentFrame(0),
    numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
    frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
    timeSinceLastFrame( 0 ),
    worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
    worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{
}

MultiSprite::MultiSprite( const std::string& name, int randRange ) :
    Drawable(name,
             randomVec(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                       Gamedata::getInstance().getXmlInt(name+"/startLoc/y"),
                       randRange),
             randomVec(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                       Gamedata::getInstance().getXmlInt(name+"/speedY"),
                       randRange)
             ),
    images( RenderContext::getInstance()->getImages(name) ),

    currentFrame(0),
    numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
    frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
    timeSinceLastFrame( 0 ),
    worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
    worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{
    //std::cout << "speed: " << getVelocity() << std::endl;
    if(getX() < 0) {
        setX(0);
    }
    else if(getX() > worldWidth - getScaledWidth()) {
        setX(worldWidth - getScaledWidth());
    }
    if(getY() < 0) {
        setY(0);
    }
    else if(getY() > worldHeight - getScaledHeight() - 200) {
        setY(worldHeight - getScaledHeight() - 200);
    }
}

MultiSprite::MultiSprite(const MultiSprite& s) :
    Drawable(s),
    images(s.images),
    currentFrame(s.currentFrame),
    numberOfFrames( s.numberOfFrames ),
    frameInterval( s.frameInterval ),
    timeSinceLastFrame( s.timeSinceLastFrame ),
    worldWidth( s.worldWidth ),
    worldHeight( s.worldHeight )
{
}

MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
    Drawable::operator=(s);
    images = (s.images);
    currentFrame = (s.currentFrame);
    numberOfFrames = ( s.numberOfFrames );
    frameInterval = ( s.frameInterval );
    timeSinceLastFrame = ( s.timeSinceLastFrame );
    worldWidth = ( s.worldWidth );
    worldHeight = ( s.worldHeight );
    return *this;
}

void MultiSprite::draw() const {
    if(-getVelocityX() >= 0) {
        images[currentFrame]->draw(getX(), getY(), getScale());
    }
    else {
        SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
        images[currentFrame]->draw(getX(), getY(), getScale(), flip);
    }
}

// void MultiSprite::draw() const { 
//   images[currentFrame]->draw(getX(), getY(), getScale());
// }

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  // Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  Vector2f incr;
  incr[0] = getVelocityX() * static_cast<float>(ticks) * 0.001;
  incr[1] = 0;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() + getScaledHeight() > worldHeight) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() + getScaledWidth() > worldWidth) {
    // std::cout << "getName() is " << getName() << std::endl;
    // std::cout << "worldWidth() is " << worldWidth << std::endl;
    // std::cout << "getX() is " << getX() << std::endl;
    // std::cout << "getScaledWidth() is " << getScaledWidth() << std::endl;
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
