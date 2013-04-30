/* 
 * File:   PersonajeVista.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:25
 */

#include <view/entities/PlayerView.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_ttf.h>
#include <model/Logs/Logs.h>

#include <cmath>
#include <string>

//Posicion de los pies del personaje respecto de la base de la imagen
#define OFFSET_Y	15
#define ANIMATION_CHANGE_DELAY 1
#define STANDING_ANIMATION_LOCATION_IN_IMAGE_FILE 16
#define DEFAULT_CHARACTER_ID	"characterDefault"
#define NUMBER_OF_STANDING_FRAMES 3

PlayerView::PlayerView()
//Llamamos al constructor de la superclase
:
		EntityView() {
	camPos = new Position(0, 0);
	marco = 0;
	animationChangeRate = 0;
	imageHeight = 0;
	imageWidth = 0;
	numberOfClips = 0;
	movable = true;
	direction = DOWN;
	wasStanding = true;
	player = NULL;
	nameImage = NULL;
	runningImage = NULL;
	walkingImage = NULL;
	attackImage = NULL;
	idleImage = NULL;
	numberOfRunningClips = 0;
	numberOfWalkingClips = 0;
	numberOfIdleClips = 0;
	numberOfAttackClips = 0;
	currentSprite = DOWN;
	lastDirection = M_PI * 1 / 2;

}

void PlayerView::showFrame(SDL_Surface* source, SDL_Surface* screen, SDL_Rect* clip) {
	SDL_Rect offset;

	Vector3* position = player->getCurrentPos();
	float x = position->getX();
	float y = position->getY();
	offset.x = (int) x + camPos->getX() - this->anchorPixel->getX();
	int h = Tile::computePositionTile(0, 0).h;
	offset.y = (int) y + camPos->getY() - this->anchorPixel->getY() - h / 2;
	offset.w = clip->w;
	offset.h = clip->h;

	SDL_BlitSurface(source, clip, screen, &offset);

	SDL_Rect offsetNombre;
	offsetNombre.x = (int) x + camPos->getX() - nameImage->w / 2;
	offsetNombre.y = (int) y + camPos->getY() - this->anchorPixel->getY() - h / 2 - 20;
	offsetNombre.w = nameImage->w;
	offsetNombre.h = nameImage->h;
	SDL_BlitSurface(nameImage, NULL, screen, &offsetNombre);
}

void PlayerView::draw(SDL_Surface* screen, Position* cam, bool drawFog) {
	UpdateCameraPos(cam);
	Show(screen);
}

void PlayerView::UpdateCameraPos(Position* _camPos) {
	delete camPos;
	camPos = new Position(_camPos->getX(), _camPos->getY());
}

void PlayerView::setPersonaje(Player* personaje) {
	this->player = personaje;
	Vector2* anchorPixel = new Vector2(clip.w / 2, OFFSET_Y);
	player->getBase()->setAnchorPixel(anchorPixel);
}

void PlayerView::loadPlayerImage() {
	walkingImage = textureHolder->getTexture(name + string(WALKING_MODIFIER));
	idleImage = textureHolder->getTexture(name + string(IDLE_MODIFIER));
	attackImage = textureHolder->getTexture(name + string(ATTACK_MODIFIER));
	runningImage = textureHolder->getTexture(name + string(RUNNING_MODIFIER));
	idleBlockImage = textureHolder->getTexture(name + string(IDLE_BLOCKING_MODIFIER));

	//If there was a problem loading the sprite
	if (!walkingImage) {
		Logs::logErrorMessage("Unable to load walking image");
		//TODO: cargo una alternativa
		walkingImage = textureHolder->getTexture(DEFAULT_CHARACTER_ID);
	}

	if (!idleImage) {
		Logs::logErrorMessage("Unable to load idle image");
		//TODO: cargo una alternativa
		walkingImage = textureHolder->getTexture(DEFAULT_CHARACTER_ID);
	}

	if (!attackImage) {
		Logs::logErrorMessage("Unable to load attack image");
		//TODO: cargo una alternativa
		attackImage = textureHolder->getTexture(DEFAULT_CHARACTER_ID);
	}
	if (!runningImage) {
		Logs::logErrorMessage("Unable to load running image");
		//TODO: cargo una alternativa
		attackImage = textureHolder->getTexture(DEFAULT_CHARACTER_ID);
	}
	if (!idleBlockImage) {
		Logs::logErrorMessage("Unable to load idle blocking image");
		//TODO: cargo una alternativa
		attackImage = textureHolder->getTexture(DEFAULT_CHARACTER_ID);
	}

	numberOfWalkingClips = computeNumberOfClips(walkingImage);
	numberOfIdleClips = computeNumberOfClips(idleImage);
	numberOfRunningClips = computeNumberOfClips(runningImage);
	numberOfAttackClips = computeNumberOfClips(attackImage);
	numberOfIdleBlockClips = computeNumberOfClips(idleBlockImage);
}

void PlayerView::setEntity(Entity* entity) {
	//TODO: Error check (si no es un personaje)
	Player* aux = (Player*) entity;
	player = aux;
}

void PlayerView::showStandingAnimation(SpriteType sprite, SDL_Surface* fondo) {

	SDL_Rect clipToDraw;
	clipToDraw.x = imageWidth * currentClip * scaleWidth;

	clipToDraw.y = imageHeight * sprite;
	clipToDraw.w = imageWidth * scaleWidth;

	clipToDraw.h = imageHeight * scaleHeight;

	showFrame(this->image, fondo, &clipToDraw);

	timeSinceLastAnimation = timer.getTimeSinceLastAnimation();

	//TODO - deberia ser numberOfClips-1 pero parece q esta mal la imagen ?¿

	//Apply delay
	if (currentClip < (numberOfClips - 1) && timeSinceLastAnimation >= delay * 1000) {
		//Apply FPS cap
		if (animationRateTimer.getTimeSinceLastAnimation() >= 1000 / fps) {
			currentClip++;
			animationRateTimer.start();
		}

	} else {
		if (timeSinceLastAnimation >= delay * 1000)
			timer.start();
		currentClip = 0;
	}

}

void PlayerView::Show(SDL_Surface* fondo) {

	if (this->image == NULL)
		loadPlayerImage();

	if (marco >= numberOfClips) {
		marco = 0;
		if (player->isAttacking())
			player->cancelAttack();
		if (player->isBlocking())
			marco = numberOfIdleBlockClips-1;
	}

	Vector2* movementDirection = this->player->getMovementDirection();
	float direction;

	Vector2* v = new Vector2(0, 0);
	if (movementDirection->isEqual(v))
		direction = lastDirection;
	else
		direction = movementDirection->getAngle();
	delete v;

	SpriteType sprite = DOWN;
	image = walkingImage;
	numberOfClips = numberOfWalkingClips;

	const float step = M_PI * 1 / 8;

	if (step * 15 < direction || direction < step)
		sprite = RIGHT;
	else if (step < direction && direction < step * 3)
		sprite = DOWN_RIGHT;
	else if (step * 3 < direction && direction < step * 5)
		sprite = DOWN;
	else if (step * 5 < direction && direction < step * 7)
		sprite = DOWN_LEFT;
	else if (step * 7 < direction && direction < step * 9)
		sprite = LEFT;
	else if (step * 9 < direction && direction < step * 11)
		sprite = UP_LEFT;
	else if (step * 11 < direction && direction < step * 13)
		sprite = UP;
	else if (step * 13 < direction && direction < step * 15)
		sprite = UP_RIGHT;

	if (player->isRunning()) {
		image = runningImage;
		numberOfClips = numberOfRunningClips;
	}

	if (!player->IsMoving() && !player->isAttacking() && !player->isBlocking()) {
		if (!wasStanding) {
			timer.start();
			wasStanding = true;
		}
		image = idleImage;
		numberOfClips = numberOfIdleClips;
		lastDirection = direction;
		showStandingAnimation(sprite, fondo);
		return;
	}

	if (player->isAttacking()) {
		player->stop();
		image = attackImage;
		numberOfClips = numberOfAttackClips;
	}
	if (player->isBlocking()) {
		player->stop();
		image = idleBlockImage;
		numberOfClips = numberOfIdleBlockClips;
	}

	//lucas TODO- Pensar como meter el cambio de standing animation?
	wasStanding = false;
	currentSprite = sprite;
//	if (marco >= numberOfClips)
//		marco = 0;    // Loop the animation

	lastDirection = direction;
	playAnimation(currentSprite, fondo);

}

PlayerView::~PlayerView() {
	//libera la memoria que pide para La imagen
}

int PlayerView::getImageHeight() {
	return this->imageHeight;
}

void PlayerView::setImageHeight(int height) {
	this->imageHeight = height;
}

int PlayerView::getImageWidth() {
	return this->imageWidth;
}

void PlayerView::setImageWidth(int width) {
	this->imageWidth = width;
}

int PlayerView::getNClips() {
	return this->numberOfClips;
}

void PlayerView::setNClips(int clips) {
	this->numberOfClips = clips;
}

Player* PlayerView::getPersonaje() {
	return this->player;
}

Player* PlayerView::getEntity() {
	return player;
}

void PlayerView::setName(std::string name) {
	this->name = name;
	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	TTF_Font* font = TTF_OpenFont("resources/fonts/Baramond.ttf", 28);
	nameImage = TTF_RenderText_Solid(font, name.c_str(), color);
	if (!nameImage || !font)
		Logs::logErrorMessage("Error al cargar la fuente para el nombre del personaje");

}

void PlayerView::playAnimation(SpriteType sprite, SDL_Surface* screen) {
	SDL_Rect clipToDraw;
	clipToDraw.x = imageWidth * marco * scaleWidth;
	clipToDraw.y = imageHeight * sprite;
	clipToDraw.w = imageWidth * scaleWidth;
	clipToDraw.h = imageHeight * scaleHeight;

	showFrame(this->image, screen, &clipToDraw);

	if (animationChangeRate == ANIMATION_CHANGE_DELAY) {
		this->marco++;
		animationChangeRate = 0; // Move to the next marco in the animation
	} else {
		animationChangeRate++;
	}

}

int PlayerView::computeNumberOfClips(SDL_Surface* img) {
	return img->w / imageWidth;
}
