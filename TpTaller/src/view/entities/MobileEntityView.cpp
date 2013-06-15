/* 
 * File:   PersonajeVista.cpp
 * Author: damian
 * 
 * Created on 23 de marzo de 2013, 12:25
 */

#include <view/entities/MobileEntityView.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_ttf.h>
#include <model/Logs/Logs.h>

#include <cmath>
#include <string>
#include <sstream>

//Posicion de los pies del personaje respecto de la base de la imagen
#define OFFSET_Y	15
#define ANIMATION_CHANGE_DELAY 2
#define STANDING_ANIMATION_LOCATION_IN_IMAGE_FILE 16
#define DEFAULT_CHARACTER_ID	"characterDefault"
#define NUMBER_OF_STANDING_FRAMES 3

MobileEntityView::MobileEntityView()
//Llamamos al constructor de la superclase
:
		EntityView() {
	camPos = new Position(0, 0);
	marco = 0;
	animationChangeRate = 0;
	numberOfClips = currentSpellClip = 0;
	movable = true;
	direction = DOWN;
	wasStanding = true;
	mobileEntity = NULL;
	nameImage = runningImage = walkingImage = NULL;
	idleImage = attackImage = NULL;
	numberOfRunningClips = numberOfWalkingClips = 0;
	numberOfIdleClips = 0;
	numberOfAttackClips = 0;
	currentSprite = DOWN;
	lastDirection = M_PI * 1 / 2;
	wasFrozen = false;
}

MobileEntityView::MobileEntityView(MobileEntityView* othermobileEntity) :
		EntityView(othermobileEntity) {

	imageHeight = othermobileEntity->getImageHeight();
	imageWidth = othermobileEntity->getImageWidth();
	delay = othermobileEntity->getDelay();
	fps = othermobileEntity->getFps();
	camPos = new Position(0, 0);
	marco = 0;
	animationChangeRate = currentSpellClip = 0;
	numberOfClips = othermobileEntity->getNClips();
	setNumberOfRepeats(othermobileEntity->getNumberOfRepeats());
	movable = true;
	direction = DOWN;
	wasStanding = true;
	mobileEntity = NULL;
	nameImage = runningImage = walkingImage = NULL;
	idleImage = attackImage = NULL;
	numberOfRunningClips = numberOfWalkingClips = 0;
	numberOfIdleClips = 0;
	numberOfAttackClips = 0;
	currentSprite = DOWN;
	lastDirection = M_PI * 1 / 2;
	textureHolder = othermobileEntity->getTextureHolder();
	//chatView = new ChatWindowsView();
	this->setName(othermobileEntity->getName());
	wasFrozen = false;
}

void MobileEntityView::showFrame(SDL_Surface* screen, SDL_Rect* clip,
		bool drawFog) {
	SDL_Rect offset, offsetFog;

	if (drawFog)
		return;

	Vector3* position = mobileEntity->getCurrentPos();
	float x = position->getX();
	float y = position->getY();
	offset.x = offsetFog.x = (int) x + camPos->getX()
			- this->anchorPixel->getX();
	int h = Tile::computePositionTile(0, 0).h;
	offset.y = offsetFog.y = (int) y + camPos->getY()
			- this->anchorPixel->getY() - h / 2;
	offset.w = offsetFog.w = clip->w;
	offset.h = offsetFog.h = clip->h;

	SDL_BlitSurface(this->image, clip, screen, &offset);
	blitIceSpellEffect(screen, x, y);
	//blitName(screen, x, y);
}


void MobileEntityView::blitName(SDL_Surface* screen, int x, int y) {
	SDL_Rect offsetNombre;
	this->setShowableName(mobileEntity->getName());
	int h = Tile::computePositionTile(0, 0).h;
	offsetNombre.x = (int) x + camPos->getX() - nameImage->w / 2;
	offsetNombre.y = (int) y + camPos->getY() - this->anchorPixel->getY()
			- h / 2 - 20;
	offsetNombre.w = nameImage->w;
	offsetNombre.h = nameImage->h;
	SDL_BlitSurface(nameImage, NULL, screen, &offsetNombre);

}
void MobileEntityView::blitIceSpellEffect(SDL_Surface* screen, int x, int y) {
	if (mobileEntity->isFrozen() && !wasFrozen) {
		if (!SoundEffectHandler::isSoundPlaying(ICE_SOUND)) {
			SoundEffectHandler::playSound(ICE_SOUND);
		}
		wasFrozen = true;
	}

	if (!mobileEntity->isFrozen()) {
		currentSpellClip = 0;
		wasFrozen = false;
		return;
	}
	SDL_Surface* spellSprite = textureHolder->getTexture(string(ICE_IMG));
	int numberOfClips = 4;
	if (currentSpellClip >= numberOfClips) {
		currentSpellClip = numberOfClips - 1;
	}
	SDL_Rect offsetSpell, currentClip;
	int frameWidth = spellSprite->w / numberOfClips;
	offsetSpell.x = (int) x + camPos->getX() - frameWidth / 2 + 10;
	offsetSpell.y = (int) y + camPos->getY() - this->anchorPixel->getY()
			+ frameWidth / 10;

	currentClip.x = currentSpellClip * frameWidth;
	currentClip.y = 0;
	currentClip.w = frameWidth;
	currentClip.h = spellSprite->h;
	SDL_BlitSurface(spellSprite, &currentClip, screen, &offsetSpell);
	currentSpellClip++;

}

void MobileEntityView::draw(SDL_Surface* screen, Position* cam, bool drawFog) {

	UpdateCameraPos(cam);
	Show(screen, drawFog);
	//chatView->drawChatView(screen);
}

void MobileEntityView::UpdateCameraPos(Position* _camPos) {
	delete camPos;
	camPos = new Position(_camPos->getX(), _camPos->getY());
}

void MobileEntityView::setMobileEntity(MobileEntity* entity) {
	this->mobileEntity = entity;
	Vector2* anchorPixel = new Vector2(clip.w / 2, OFFSET_Y);
	mobileEntity->getBase()->setAnchorPixel(anchorPixel);

}

void MobileEntityView::loadMobileEntityImage() {
	walkingImage = textureHolder->getTexture(name + string(WALKING_MODIFIER));
	idleImage = textureHolder->getTexture(name + string(IDLE_MODIFIER));
	attackImage = textureHolder->getTexture(name + string(ATTACK_MODIFIER));
	runningImage = textureHolder->getTexture(name + string(RUNNING_MODIFIER));

	// Fogs

	//If there was a problem loading the sprite
	if (!walkingImage) {
		Logs::logErrorMessage("Unable to load walking image");
		walkingImage = textureHolder->getTexture(DEFAULT_CHARACTER_ID);
	}

	if (!idleImage) {
		Logs::logErrorMessage("Unable to load idle image");
		idleImage = textureHolder->getTexture(DEFAULT_CHARACTER_ID);
	}

	if (!attackImage) {
		Logs::logErrorMessage("Unable to load attack image");
		attackImage = textureHolder->getTexture(DEFAULT_CHARACTER_ID);
	}
	if (!runningImage) {
		Logs::logErrorMessage("Unable to load running image");
		runningImage = textureHolder->getTexture(DEFAULT_CHARACTER_ID);
	}

	numberOfWalkingClips = computeNumberOfClips(walkingImage);
	numberOfIdleClips = computeNumberOfClips(idleImage);
	numberOfRunningClips = computeNumberOfClips(runningImage);
	numberOfAttackClips = computeNumberOfClips(attackImage);
}

void MobileEntityView::setEntity(Entity* entity) {
	MobileEntity* aux = (MobileEntity*) entity;
	mobileEntity = aux;
}

void MobileEntityView::showStandingAnimation(SpriteType sprite,
		SDL_Surface* fondo, bool drawFog) {

	SDL_Rect clipToDraw;
	clipToDraw.x = imageWidth * currentClip * scaleWidth;

	clipToDraw.y = imageHeight * sprite;
	clipToDraw.w = imageWidth * scaleWidth;

	clipToDraw.h = imageHeight * scaleHeight;

	showFrame(fondo, &clipToDraw, drawFog);

	timeSinceLastAnimation = timer.getTimeSinceLastAnimation();

	//Apply delay
	if (currentClip < (numberOfClips - 1)
			&& timeSinceLastAnimation >= delay * 1000) {
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

void MobileEntityView::Show(SDL_Surface* fondo, bool drawFog) {

	if (this->image == NULL)
		loadMobileEntityImage();

	if (marco >= numberOfClips) {
		marco = 0;
		if (mobileEntity->isAttacking())
			mobileEntity->cancelAttack();
	}

	Vector2* movementDirection = this->mobileEntity->getMovementDirection();
	float direction;

	if (movementDirection->getNorm() == 0) {
		if (mobileEntity->isAttacking())
			direction = mobileEntity->getLastAttackingDirecton();
		else
			direction = lastDirection;
	} else {
		direction = movementDirection->getAngle();
	}

	SpriteType sprite = DOWN;

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

	if (mobileEntity->isAttacking()) {
		//Si se estaba moviendo, reseteamos el marco para que no quede un # de clip invalido
		if (mobileEntity->IsMoving()) {
			marco = 0;
			mobileEntity->stop();
		}
		image = attackImage;
		numberOfClips = numberOfAttackClips;
	}

	if (mobileEntity->IsMoving()) {
		image = walkingImage;
		numberOfClips = numberOfWalkingClips;
	}

	if (!mobileEntity->IsMoving() && !mobileEntity->isAttacking()) {
		if (!wasStanding) {
			timer.start();
			wasStanding = true;
		}
		image = idleImage;
		numberOfClips = numberOfIdleClips;
		lastDirection = direction;
		showStandingAnimation(sprite, fondo, drawFog);
		return;
	}

	wasStanding = false;
	currentSprite = sprite;

	lastDirection = direction;
	playAnimation(currentSprite, fondo, drawFog);

}

MobileEntityView::~MobileEntityView() {
	//libera la memoria que pide para La imagen
}

int MobileEntityView::getNClips() {
	return this->numberOfClips;
}

void MobileEntityView::setNClips(int clips) {
	this->numberOfClips = clips;
}

MobileEntity* MobileEntityView::getEntity() {
	return mobileEntity;
}

void MobileEntityView::setName(std::string name) {
	this->name = name;
	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	//TTF_Font* font = TTF_OpenFont("resources/fonts/Baramond.ttf", 28);
	/*if (nameImage)
		SDL_FreeSurface(nameImage);
	nameImage = TTF_RenderText_Solid(font, name.c_str(), color);*/
	//if (!nameImage || !font)
	//	Logs::logErrorMessage(
		//		"Error al cargar la fuente para el nombre del personaje");

}

void MobileEntityView::setShowableName(string name) {
	if (!nameImage){
		int id = this->mobileEntity->getId();
		std::stringstream sstm;
		sstm <<  id;
		SDL_Color color;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		TTF_Font* font = TTF_OpenFont("resources/fonts/Baramond.ttf", 28);

		//	SDL_FreeSurface(nameImage);
		nameImage = TTF_RenderText_Solid(font, sstm.str().c_str(), color);
		if (!nameImage || !font)
			Logs::logErrorMessage(
					"Error al cargar la fuente para el nombre del personaje");
	}
}

void MobileEntityView::playAnimation(SpriteType sprite, SDL_Surface* screen,
		bool drawFog) {
	SDL_Rect clipToDraw;
	clipToDraw.x = imageWidth * marco * scaleWidth;
	clipToDraw.y = imageHeight * sprite;
	clipToDraw.w = imageWidth * scaleWidth;
	clipToDraw.h = imageHeight * scaleHeight;

	showFrame(screen, &clipToDraw, drawFog);

	if (animationChangeRate == ANIMATION_CHANGE_DELAY) {
		if (!mobileEntity->isFrozen())
			this->marco++;
		animationChangeRate = 0; // Move to the next marco in the animation
	} else {
		animationChangeRate++;
	}

}

int MobileEntityView::computeNumberOfClips(SDL_Surface* img) {
	float number = (float) img->w / imageWidth;
	float aux = round(number);
	return (int) aux;
}
