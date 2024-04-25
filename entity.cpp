#include "entity.h"
#include "question.h"
#include <SDL.h>
#include <SDL_image.h>

using namespace std;
//---------------------------------------- Entity ----------------------------------------
//any object with a position and an image
Entity::Entity(int x, int y, SDL_Texture *tex, int w, int h, int imageNo)
: deleted(false), x(x), y(y), tex(tex), currentClip({0, imageNo * h, w, h})
{}

float Entity::getX() { return x; }
float Entity::getY() { return y; }
void Entity::setX(float x) { this->x = x; }
void Entity::setY(float y) { this->y = y; }
void Entity::setPos(float x, float y)
{
    this->x = x;
    this->y = y;
}

//render entity tex to screen
void Entity::render(int offsetX, int offsetY, SDL_Renderer *renderer)
{
    SDL_Rect dest;
    dest.x = x + offsetX;
    dest.y = y + offsetY;
    dest.w = currentClip.w;
    dest.h = currentClip.h;
    SDL_RenderCopy(renderer, tex, &currentClip, &dest);
}

//virtual methods used by derived classes, for updating all entities iteratively
void Entity::updateClips() {}
void Entity::updatePos() {}
void Entity::updateGravity(float gravity) {}
void Entity::updateCollisions(Map *map) {}
void Entity::updateKeyDoor(Map *map) {}
void Entity::updateChest(Map *map) {}
bool Entity::checkChest(Map *map) {}
bool Entity::checkQuai(Map *map) {}


//---------------------------------------- AnimEntity ----------------------------------------
//objects with animations
AnimEntity::AnimEntity(int x, int y, SDL_Texture *tex, int w, int h, std::vector<std::vector<SDL_Point>> *clips)
: Entity(x, y, tex, w, h, 0), clips(clips), animNo(0), frameNo(0), animDelay(1), animStart(0), animType(cnst::ANIM_END)
{}

//update clipping area for animation
void AnimEntity::updateClips()
{
    int noOfFrames = clips->at(animNo).size();
    frameNo = ((float)SDL_GetTicks() - animStart) / animDelay;

    if (animType == cnst::ANIM_DEFAULT && frameNo >= noOfFrames)
    {
        animType = cnst::ANIM_REPEAT;
        animNo = 0;
    }
    if (animType == cnst::ANIM_END && frameNo >= noOfFrames) frameNo = noOfFrames - 1;
    if (animType == cnst::ANIM_REPEAT) frameNo %= noOfFrames;
    //update clips
    currentClip.x = clips->at(animNo).at(frameNo).x;
    currentClip.y = clips->at(animNo).at(frameNo).y;
}

//set and begin animation
void AnimEntity::setAnim(int animNo, int animType, int animDelay)
{
    this->animNo = animNo;
    this->animType = animType;
    this->animDelay = animDelay;
    animStart = SDL_GetTicks();
}

int AnimEntity::getAnim() { return animNo; }


//---------------------------------------- ActiveEntity ----------------------------------------
//objects with velocities
ActiveEntity::ActiveEntity(int x, int y, SDL_Texture *tex, int w, int h, std::vector<std::vector<SDL_Point>> *clips, bool gravity, bool collisions)
: AnimEntity(x, y, tex, w, h, clips), xVel(0), yVel(0), gravity(gravity), collisions(collisions), prevX(x), prevY(y)
{}

void ActiveEntity::updatePos()
{
    if (fabs(xVel) < 0.01) xVel = 0;
    if (fabs(yVel) < 0.01) yVel = 0;
    x += xVel;
    y += yVel;
}

void ActiveEntity::updateGravity(float gravity)
{
    if (gravity)
    {
        yVel += gravity;
        if (yVel >= cnst::TERMINAL_VEL) yVel = cnst::TERMINAL_VEL;
    }
}

void ActiveEntity::updateCollisions(Map *map)
{
    if (collisions)
    {
        if (xVel >= 0)
        {
            if (map->checkCollision('r', this, 1))
            {
                xVel = 0;
                if ((int)prevX % 50 > 0) x = (int)prevX - (int)prevX % 50 + 50;
                else x = (int)prevX;
            }
        }
        else
        {
            if (map->checkCollision('l', this, 1))
            {
                xVel = 0;
                if ((int)prevX % 50 > 0) x = (int)prevX - (int)prevX % 50;
                else x = (int)prevX;
            }
        }
        if (yVel >= 0)
        {
            if (map->checkCollision('d', this, 1))
            {
                yVel = 0;
                if ((int)prevY % 50 > 0) y = (int)prevY - (int)prevY % 50 + 50;
                else y = (int)prevY;
            }
        }
        else
        {
            if (map->checkCollision('u', this, 1))
            {
                yVel = 0;
                if ((int)prevY % 50 > 0) y = (int)prevY - (int)prevY % 50;
                else y = (int)prevY;
            }
        }
        prevX = x;
        prevY = y;
    }
}

float ActiveEntity::getXVel() { return xVel; }
float ActiveEntity::getYVel() { return yVel; }
void ActiveEntity::setXVel(float xVel) { this->xVel = xVel; }
void ActiveEntity::setYVel(float yVel) { this->yVel = yVel; }
void ActiveEntity::addXVel(float xVel) { this->xVel += xVel; }
void ActiveEntity::addYVel(float yVel) { this->yVel += yVel; }


//---------------------------------------- Player ----------------------------------------
//player has additional members for movement and updating camera
Player::Player(int x, int y, SDL_Texture *tex, std::vector<std::vector<SDL_Point>> *clips,
Mix_Chunk *jumpSfx, Mix_Chunk *dashSfx, Mix_Chunk *thudSfx,
AnimEntity *dust1, AnimEntity *dust2)
: ActiveEntity(x, y, tex, 70, 70, clips, true, true), onGround(true), canDoubleJump(false), dashTime(0), stunTime(0), keys(0),
  jumpSfx(jumpSfx), dashSfx(dashSfx), thudSfx(thudSfx), dust1(dust1), dust2(dust2)
{}

void Player::render(int offsetX, int offsetY, SDL_Renderer *renderer)
{
    SDL_Rect dest;
    dest.x = x + offsetX - 10;
    dest.y = y + offsetY - 10;
    dest.w = currentClip.w;
    dest.h = currentClip.h;
    SDL_RenderCopy(renderer, tex, &currentClip, &dest);
}

void Player::moveLeft()
{
    if (!isStunned())
    {
        xVel -= 0.5;
        if (getAnim() != 2) setAnim(2, cnst::ANIM_REPEAT, 80);
    }
}

void Player::moveRight()
{
    if (!isStunned())
    {
        xVel += 0.5;
        if (getAnim() != 1) setAnim(1, cnst::ANIM_REPEAT, 80);
    }
}

bool Player::isStunned()
{
    return (SDL_GetTicks() < stunTime) ? true : false;
}
void Player::stun(int duration)
{
    stunTime = SDL_GetTicks() + duration;
}

void Player::dash()
{
    if (abs(xVel != 0) && !isStunned() && SDL_GetTicks() > dashTime)
    {
        yVel = 0;
        if (xVel > 0) xVel = 22;
        else xVel = -22;
        dashTime = SDL_GetTicks() + 800;
        dust1->setPos(x - 20, y - 30);
        dust1->setAnim(4, cnst::ANIM_END, 30);
        dust2->setPos(x - 20, y + 30);
        dust2->setAnim(5, cnst::ANIM_END, 30);
        Mix_PlayChannel(-1, dashSfx, 0);
    }
}
void Player::jump()
{
    if (!isStunned())
    {
        yVel = -12;
        if (!onGround)
        {
            canDoubleJump = false;
            dust1->setPos(x - 30, y + 20);
            dust1->setAnim(2, cnst::ANIM_END, 30);
            dust2->setPos(x + 30, y + 20);
            dust2->setAnim(3, cnst::ANIM_END, 30);
            Mix_PlayChannel(-1, jumpSfx, 0);
        }
        onGround = false;
    }
}

bool Player::hasKey() { return (keys > 0); }
void Player::addKey() { keys++; }
void Player::useKey() { if (keys > 0) keys--; }

void Player::drag(float drag) { xVel *= drag; }

//player collision
void Player::updateCollisions(Map *map)
{
    onGround = false;
    if (xVel >= 0)
    {
        if (map->checkCollision('r', this, 1))
        {
            xVel = 0;
            if ((int)prevX % 50 > 0) x = (int)prevX - (int)prevX % 50 + 50;
            else x = (int)prevX;
        }
    }
    else
    {
        if (map->checkCollision('l', this, 1) || x + xVel < 0)
        {
            xVel = 0;
            if ((int)prevX % 50 > 0) x = (int)prevX - (int)prevX % 50;
            else x = (int)prevX;
        }
    }
    if (yVel >= 0)
    {
        if (map->checkCollision('d', this, 1))
        {
            float oldYVel = yVel;
            yVel = 0;
            if ((int)prevY % 50 > 0) y = (int)prevY - (int)prevY % 50 + 50;
            else y = (int)prevY;

            if (oldYVel >= cnst::TERMINAL_VEL)
            {
                xVel = 0;
                setAnim(3, cnst::ANIM_DEFAULT, 100);
                stun(400);
                map->shakeCamera(12);
                dust1->setPos(x - 60, y);
                dust1->setAnim(0, cnst::ANIM_END, 100);
                dust2->setPos(x + 60, y);
                dust2->setAnim(1, cnst::ANIM_END, 100);
                Mix_PlayChannel(-1, thudSfx, 0);
            }
            onGround = true;
            canDoubleJump = true;
        }
    }
    else
    {
        if (map->checkCollision('u', this, 1))
        {
            yVel = 0;
            if ((int)prevY % 50 > 0) y = (int)prevY - (int)prevY % 50;
            else y = (int)prevY;
        }
    }
    prevX = x;
    prevY = y;
}


//---------------------------------------- Door ----------------------------------------
Door::Door(int x, int y, SDL_Texture *tex, std::vector<std::vector<SDL_Point>> *clips, Mix_Chunk *doorSfx, Player *player)
: AnimEntity(x, y, tex, 50, 200, clips), open(false), doorSfx(doorSfx), player(player)
{}

void Door::updateKeyDoor(Map *map)
{
    //if player touches closed door while in possesion of a key
    if (!open && player->hasKey()
    && player->getX() + 50 >= x && player->getX() - currentClip.w <= x
    && player->getY() + 50 >  y && player->getY() - currentClip.h <  y)
    {
        open = true;
        player->useKey();
        setAnim(1, cnst::ANIM_END, 50);
        player->stun(500);
        map->setTile(x/cnst::TILE_SIZE, y/cnst::TILE_SIZE, 2);
        map->setTile(x/cnst::TILE_SIZE, y/cnst::TILE_SIZE + 1, 2);
        map->setTile(x/cnst::TILE_SIZE, y/cnst::TILE_SIZE + 2, 2);
        map->setTile(x/cnst::TILE_SIZE, y/cnst::TILE_SIZE + 3, 2);
        Mix_PlayChannel(-1, doorSfx, 0);
    }
}

//---------------------------------------- Key ----------------------------------------
Key::Key(int x, int y, SDL_Texture *tex, std::vector<std::vector<SDL_Point>> *clips, Mix_Chunk *keySfx, Player *player)
: AnimEntity(x, y, tex, 50, 50, clips), collected(0), keySfx(keySfx), player(player)
{}

void Key::updateKeyDoor(Map *map)
{
    //if player touches key
    if (!collected
    && player->getX() + 50 > x && player->getX() - currentClip.w < x
    && player->getY() + 50 > y && player->getY() - currentClip.h < y)
    {
        std::cout << "true";
        collected = true;
        player->addKey();
        setAnim(1, cnst::ANIM_END, 1);
        Mix_PlayChannel(-1, keySfx, 0);
    }
}

void Key::render(int offsetX, int offsetY, SDL_Renderer *renderer)
{
    SDL_Rect dest;
    dest.x = x + offsetX;
    dest.y = y + offsetY + 5*sin(SDL_GetTicks()/200.0f);
    dest.w = currentClip.w;
    dest.h = currentClip.h;
    SDL_RenderCopy(renderer, tex, &currentClip, &dest);
}

//---------------------------------------- Chest ----------------------------------------
Chest::Chest(int x, int y, SDL_Texture *tex, std::vector<std::vector<SDL_Point>> *clips, Mix_Chunk *chestSfx, Player *player)
: AnimEntity(x, y, tex, 100, 100, clips), collected(0), chestSfx(chestSfx), player(player)
{}

void Chest::updateChest(Map *map)
{
    if (!collected
    && player->getX() + 100 > x && player->getX() - currentClip.w < x
    && player->getY() + 100 > y && player->getY() - currentClip.h < y)
    {
        //std::cout << "true";
        //collected = true;
        setAnim(1, cnst::ANIM_END, 1);
        Mix_PlayChannel(-1, chestSfx, 0);

        //std::cout << x << " " << y;

    }
}

bool Chest::checkChest(Map *map)
{
    if (!collected
    && player->getX() + 100 > x && player->getX() - currentClip.w < x
    && player->getY() + 100 > y && player->getY() - currentClip.h < y)
    {

    return true;
    }
    return false;
}


void Chest::render(int offsetX, int offsetY, SDL_Renderer *renderer)
{
    SDL_Rect dest;
    dest.x = x + offsetX;
    dest.y = y + offsetY + 5*sin(SDL_GetTicks()/200.0f);
    dest.w = currentClip.w;
    dest.h = currentClip.h;
    SDL_RenderCopy(renderer, tex, &currentClip, &dest);
}

//---------------------------------------- Quai ----------------------------------------
Quai::Quai(int x, int y, SDL_Texture *tex, std::vector<std::vector<SDL_Point>> *clips, Player *player)
: AnimEntity(x, y, tex, 100, 100, clips), player(player)
{}

bool Quai::checkQuai(Map *map)
{
    if (player->getX() + 100 > x && player->getX() - currentClip.w < x
        && player->getY() + 100 > y && player->getY() - currentClip.h < y)
    {

        return true;
    }
    return false;
}
void Quai::render(int offsetX, int offsetY, SDL_Renderer *renderer)
{
    SDL_Rect dest;
    dest.x = x + offsetX;
    dest.y = y + offsetY;
    dest.w = currentClip.w;
    dest.h = currentClip.h;
    SDL_RenderCopy(renderer, tex, &currentClip, &dest);
}
