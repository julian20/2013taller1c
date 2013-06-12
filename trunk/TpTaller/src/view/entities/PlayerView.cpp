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
#include <view/sound/SoundEffectHandler.h>
#include <model/Logs/Logs.h>
#include <cmath>
#include <string>

//Posicion de los pies del personaje respecto de la base de la imagen
#define OFFSET_Y	15
#define ANIMATION_CHANGE_DELAY 2
#define STANDING_ANIMATION_LOCATION_IN_IMAGE_FILE 16
#define DEFAULT_CHARACTER_ID	"characterDefault"
#define NUMBER_OF_STANDING_FRAMES 3
#define STEP_SOUND "resources/sound/player/steps.ogg"
#define ATTACK_SOUND "resources/sound/player/sword.ogg"
#define DIE1_SOUND "resources/sound/player/die1.ogg"
#define DIE2_SOUND "resources/sound/player/die2.ogg"
#define QUAKE_SOUND "resources/sound/spells/quake.ogg"

#define HP_BAR_WIDTH 100
#define HP_BAR_HEIGHT 10
#define EMPTY_BAR_IMG "resources/misc/HPBarEmpty.png"
#define FULL_BAR_IMG "resources/misc/HPBarFull.png"
#define FULL_MP_BAR_IMG "resources/misc/MPBarFull.png"

#define WEAPON_MARGIN 800
#define WEAPONSHEIGHT 60
#define WEAPONSWIDTH 60
#define WEAPONWINDOW "resources/fonts/chat-window.png"
#define SWORDIMAGE "resources/weapons/sword.png"
#define FROSTWANDIMAGE "resources/weapons/magicWand.png"

double uniform(double a, double b) {
	return rand() / (RAND_MAX + 1.0) * (b - a) + a;
}

PlayerView::PlayerView()
//Llamamos al constructor de la superclase
:
		MobileEntityView() {
	camPos = new Position(0, 0);
	marco = 0;
	animationChangeRate = 0;
	numberOfClips = 0;
	movable = true;
	direction = DOWN;
	wasStanding  = true;
	attacking = attacked = loaded = spellBeingCast = spellHasEnded = false;
	player = NULL;
	nameImage = NULL;
	currentSprite = DOWN;
	lastDirection = M_PI * 1 / 2;
	chatView = NULL;
	previousLife = 0;

	loadWeaponsHudSurfaces();
}

PlayerView::PlayerView(PlayerView* otherPlayer) :
		MobileEntityView(otherPlayer) {
	imageHeight = otherPlayer->getImageHeight();
	imageWidth = otherPlayer->getImageWidth();
	delay = otherPlayer->getDelay();
	fps = otherPlayer->getFps();
	camPos = new Position(0, 0);
	marco = 0;
	animationChangeRate = 0;
	numberOfClips = otherPlayer->getNClips();
	setNumberOfRepeats(otherPlayer->getNumberOfRepeats());
	movable = true;
	direction = DOWN;
	wasStanding =  true;
	attacking = attacked = loaded = spellBeingCast = spellHasEnded =false;
	player = NULL;
	nameImage = NULL;
	currentSprite = DOWN;
	lastDirection = M_PI * 1 / 2;
	textureHolder = otherPlayer->getTextureHolder();
	this->setName(otherPlayer->getName());
	chatView = NULL;
	previousLife = 0;

	loadWeaponsHudSurfaces();
}

list<PlayerEvent*> PlayerView::getPlayerViewEvents() {
	return this->events;
}

void PlayerView::showFrame(SDL_Surface* screen, SDL_Rect* clip, bool drawFog) {
	SDL_Rect offset, offsetFog, offsetColor;// Se tiene que hacer esto porque al
											// blitear se cagan algunos valores
	if (drawFog)
		return;

	Vector3* position = player->getCurrentPos();
	float x = position->getX();
	float y = position->getY();
	offset.x = offsetFog.x = offsetColor.x = (int) x + camPos->getX()
			- this->anchorPixel->getX();
	int h = Tile::computePositionTile(0, 0).h;
	offset.y = offsetFog.y = offsetColor.y = (int) y + camPos->getY()
			- this->anchorPixel->getY() - h / 2;
	offset.w = offsetFog.w = offsetColor.w = clip->w;
	offset.h = offsetFog.h = offsetColor.h = clip->h;
	blitSpellEffect(screen, x, y);
	SDL_BlitSurface(this->image, clip, screen, &offset);
	SDL_BlitSurface(this->teamColorImage, clip, screen, &offsetColor);

	if (player->playerIsActive() == false) {
		SDL_BlitSurface(fogImage, clip, screen, &offsetFog);
	}

	blitName(screen, x, y);

	blitHPBar(screen, x, y);
	blitIceSpellEffect(screen,x,y);
}

void PlayerView::blitSpellEffect(SDL_Surface* screen, int x, int y) {
	if (!player->getUsingShieldSpell()){
		currentSpellClip = 0;
		return;
	}
	SDL_Surface* spellSprite = spellMap[QUAKE_IMG_ID].image;
	int numberOfClips = spellMap[QUAKE_IMG_ID].numberOfClips;
	if (currentSpellClip >= numberOfClips) {
		currentSpellClip = 0;
	}
	SDL_Rect offsetSpell, currentClip;
	int frameWidth = spellSprite->w / numberOfClips;
	offsetSpell.x = (int) x + camPos->getX() - frameWidth / 2;
	offsetSpell.y = (int) y + camPos->getY() - this->anchorPixel->getY() - 20;

	currentClip.x = currentSpellClip * frameWidth;
	currentClip.y = 0;
	currentClip.w = frameWidth;
	currentClip.h = spellSprite->h;
	SDL_BlitSurface(spellSprite, &currentClip, screen, &offsetSpell);
	currentSpellClip++;

}

void PlayerView::blitName(SDL_Surface* screen, int x, int y) {
	SDL_Rect offsetNombre;
	int h = Tile::computePositionTile(0, 0).h;
	offsetNombre.x = (int) x + camPos->getX() - nameImage->w / 2;
	offsetNombre.y = (int) y + camPos->getY() - this->anchorPixel->getY()
			- h / 2 - 20;
	offsetNombre.w = nameImage->w;
	offsetNombre.h = nameImage->h;
	SDL_BlitSurface(nameImage, NULL, screen, &offsetNombre);

}

void PlayerView::blitHPBar(SDL_Surface* screen, int x, int y) {
	//Empty bar
	SDL_Surface* emptyBarTemp = IMG_Load(EMPTY_BAR_IMG);
	float xScale = (HP_BAR_WIDTH * 1.005) / emptyBarTemp->w;
	SDL_Surface* scaledBar = rotozoomSurfaceXY(emptyBarTemp, 0, xScale, 1, 0);
	SDL_Surface* emptyBar = SDL_DisplayFormatAlpha(scaledBar);
	SDL_FreeSurface(emptyBarTemp);

	SDL_Rect offsetEmpty;
	int h = Tile::computePositionTile(0, 0).h;
	offsetEmpty.x = (int) x + camPos->getX() - nameImage->w / 2 + 14;
	offsetEmpty.y = (int) y + camPos->getY() - this->anchorPixel->getY() - h / 2
			- 20 + nameImage->h;
	offsetEmpty.w = emptyBar->w;
	offsetEmpty.h = emptyBar->h;
	SDL_BlitSurface(emptyBar, NULL, screen, &offsetEmpty);
	SDL_FreeSurface(emptyBar);

	int life = player->getLife();
	if (life <= 0)
		life = 0;

	SDL_Surface* fullBarTemp = IMG_Load(FULL_BAR_IMG);
	float xScale2 = (float) HP_BAR_WIDTH / fullBarTemp->w;
	SDL_Surface* scaledFullBar = rotozoomSurfaceXY(fullBarTemp, 0, xScale2, 1,
			0);
	SDL_Surface* bar = SDL_DisplayFormatAlpha(scaledFullBar);
	SDL_FreeSurface(fullBarTemp);
	SDL_FreeSurface(scaledFullBar);

	SDL_Rect size;
	size.x = 0;
	size.y = 0;
	size.h = bar->h;
	float hpSize = (float) life / 100 * HP_BAR_WIDTH;
	size.w = (int) hpSize;

	SDL_Rect offsetHP;
	offsetHP.x = (int) x + camPos->getX() - nameImage->w / 2 + 14;
	offsetHP.y = (int) y + camPos->getY() - this->anchorPixel->getY() - h / 2
			- 20 + nameImage->h;

	SDL_BlitSurface(bar, &size, screen, &offsetHP);
	SDL_FreeSurface(bar);

	//Mp bar

	float yScale = 0.5;
	SDL_Surface* emptyMPBar = rotozoomSurfaceXY(scaledBar, 0, 1, yScale, 0);
	offsetEmpty.x = (int) x + camPos->getX() - nameImage->w / 2 + 14;
	offsetEmpty.y = (int) y + camPos->getY() - this->anchorPixel->getY() - h / 2
			- 20 + nameImage->h + emptyMPBar->h / yScale;

	SDL_BlitSurface(emptyMPBar, NULL, screen, &offsetEmpty);
	SDL_FreeSurface(scaledBar);

	SDL_Surface* fullMPBarTemp = IMG_Load(FULL_MP_BAR_IMG);
	SDL_Surface* scaledFullMPBar = rotozoomSurfaceXY(fullMPBarTemp, 0, xScale2,
			yScale, 0);
	SDL_Surface* MPbar = SDL_DisplayFormatAlpha(scaledFullMPBar);
	SDL_FreeSurface(fullMPBarTemp);
	SDL_FreeSurface(scaledFullMPBar);

	SDL_Rect MPsize;
	MPsize.x = 0;
	MPsize.y = 0;
	MPsize.h = MPbar->h;
	float mpSize = (float) player->getMagic() / 100 * HP_BAR_WIDTH;
	MPsize.w = (int) mpSize;

	SDL_Rect offsetMP;
	offsetMP.x = (int) x + camPos->getX() - nameImage->w / 2 + 14;
	offsetMP.y = (int) y + camPos->getY() - this->anchorPixel->getY() - h / 2
			- 20 + nameImage->h + emptyMPBar->h / yScale;

	SDL_BlitSurface(MPbar, &MPsize, screen, &offsetMP);
	SDL_FreeSurface(emptyMPBar);
	SDL_FreeSurface(MPbar);
}
void PlayerView::draw(SDL_Surface* screen, Position* cam, bool drawFog) {

	UpdateCameraPos(cam);
	Show(screen, drawFog);
	//showWeapon(screen);
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

void initSounds() {
	SoundEffectHandler::loadSound(string("walk"), STEP_SOUND);
	SoundEffectHandler::loadSound(string("attack"), ATTACK_SOUND);
	SoundEffectHandler::loadSound(string(DIE1_SOUND), DIE1_SOUND);
	SoundEffectHandler::loadSound(string(DIE2_SOUND), DIE2_SOUND);
	SoundEffectHandler::loadSound(string(QUAKE_SOUND), QUAKE_SOUND);

}

FoggedSprite PlayerView::loadFoggedSprite(const char* modifier) {
	string id = string(modifier);
	FoggedSprite sprite;
	sprite.image = textureHolder->getTexture(name + id);
	sprite.foggedImage = textureHolder->getFogTexture(name + id);
	sprite.teamColorImage = FogCreator::getFog(sprite.image,
			teamColors[player->getTeam()]);
	sprite.numberOfClips = computeNumberOfClips(sprite.image);
	return sprite;

}

FoggedSprite PlayerView::loadFoggedSpellSprite(const char* modifier) {
	string id = string(modifier);
	FoggedSprite sprite;
	sprite.image = textureHolder->getTexture(id);
	sprite.foggedImage = textureHolder->getFogTexture(id);
	sprite.teamColorImage = FogCreator::getFog(sprite.image,
			teamColors[player->getTeam()]);
	sprite.numberOfClips = 4;
	return sprite;

}

map<string, FoggedSprite> PlayerView::loadSwordImages() {
	map<string, FoggedSprite> swordMap;
	FoggedSprite walkingSprite, blockingSprite, standingSprite, attackSprite,
			castSprite, hitSprite, dieSprite;

	walkingSprite = loadFoggedSprite(WALKING_MODIFIER);
	swordMap[string("walking")] = walkingSprite;

	standingSprite = loadFoggedSprite(IDLE_MODIFIER);
	swordMap[string("standing")] = standingSprite;

	attackSprite = loadFoggedSprite(ATTACK_MODIFIER);
	swordMap[string("attacking")] = attackSprite;

	blockingSprite = loadFoggedSprite(IDLE_BLOCKING_MODIFIER);
	swordMap[string("blocking")] = blockingSprite;

	castSprite = loadFoggedSprite(CAST_SPELL_MODIFIER);
	swordMap[string("cast")] = castSprite;

	hitSprite = loadFoggedSprite(HIT_MODIFIER);
	swordMap[string("hit")] = hitSprite;

	dieSprite = loadFoggedSprite(DIE_MODIFIER);
	swordMap[string("die")] = dieSprite;

	return swordMap;

}

map<string, FoggedSprite> PlayerView::loadBowImages() {
	map<string, FoggedSprite> bowMap;
	FoggedSprite walkingSprite, blockingSprite, standingSprite, attackSprite,
			castSprite, hitSprite, dieSprite;

	walkingSprite = loadFoggedSprite(BOW_WALKING_MODIFIER);
	bowMap[string("walking")] = walkingSprite;

	standingSprite = loadFoggedSprite(BOW_IDLE_MODIFIER);
	bowMap[string("standing")] = standingSprite;

	attackSprite = loadFoggedSprite(BOW_ATTACK_MODIFIER);
	bowMap[string("attacking")] = attackSprite;

	blockingSprite = loadFoggedSprite(IDLE_BLOCKING_MODIFIER);
	bowMap[string("blocking")] = blockingSprite;

	castSprite = loadFoggedSprite(BOW_CAST_SPELL_MODIFIER);
	bowMap[string("cast")] = castSprite;

	hitSprite = loadFoggedSprite(BOW_HIT_MODIFIER);
	bowMap[string("hit")] = hitSprite;

	dieSprite = loadFoggedSprite(BOW_DIE_MODIFIER);
	bowMap[string("die")] = dieSprite;

	return bowMap;

}

map<string, FoggedSprite> PlayerView::loadSpellImages() {
	map<string, FoggedSprite> spellMap;
	FoggedSprite quakeSprite;

	quakeSprite = loadFoggedSpellSprite(QUAKE_IMG_ID);
	spellMap[QUAKE_IMG_ID] = quakeSprite;

	return spellMap;

}

void PlayerView::loadPlayerImage() {
	previousLife = player->getLife();
	spellMap = loadSpellImages();
	weaponViewMap[string("sword")] = loadSwordImages();

	weaponViewMap[string("bow")] = loadBowImages();

	initSounds();
}

void PlayerView::setEntity(Entity* entity) {
	//TODO: Error check (si no es un personaje)
	Player* aux = (Player*) entity;
	player = aux;
}

void PlayerView::showStandingAnimation(SpriteType sprite, SDL_Surface* fondo,
		bool drawFog) {

	SDL_Rect clipToDraw;
	clipToDraw.x = imageWidth * currentClip * scaleWidth;

	clipToDraw.y = imageHeight * sprite;
	clipToDraw.w = imageWidth * scaleWidth;

	clipToDraw.h = imageHeight * scaleHeight;

	showFrame(fondo, &clipToDraw, drawFog);

	timeSinceLastAnimation = timer.getTimeSinceLastAnimation();

	if (!player->isFrozen()) {
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
}

void PlayerView::showCorpse(SDL_Surface* fondo, bool drawFog,
		SpriteType sprite) {
	marco = spriteMap[string("die")].numberOfClips - 1;
	wasStanding = false;
	currentSprite = sprite;
	image = spriteMap[string("die")].image;
	fogImage = spriteMap[string("die")].foggedImage;
	teamColorImage = spriteMap[string("die")].teamColorImage;
	numberOfClips = spriteMap[string("die")].numberOfClips;
	lastDirection = direction;
	playAnimation(sprite, fondo, drawFog);
	marco += ANIMATION_CHANGE_DELAY;
}

SpriteType computeDirection(float direction) {
	const float step = M_PI * 1 / 8;
	SpriteType sprite = DOWN;
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

	return sprite;
}

void PlayerView::Show(SDL_Surface* fondo, bool drawFog) {
	//cosas del sound
	string walkID = string("walk");
	string attackID = string("attack");
	if (!loaded) {
		loaded = true;
		loadPlayerImage();
		damageReceivedTimer.start();
	}
	if (player->getMagic()<=0)
		spellHasEnded = true;
	player->updateDamageTaken();
	Vector2* movementDirection = this->player->getMovementDirection();
	float direction;

	if (movementDirection->getNorm() == 0) {
		if (player->isAttacking())
			direction = player->getLastAttackingDirecton();
		else
			direction = lastDirection;
	} else {
		direction = movementDirection->getAngle();
	}

	SpriteType sprite = computeDirection(direction);

	int life = player->getLife();

	if (marco >= numberOfClips) {
		previousLife = life;
		if (attacked)
			attacked = false;
		if (player->isDead()) {
			showCorpse(fondo, drawFog, sprite);
			return;
		}
		marco = 0;
		if (spellBeingCast) {
			spellBeingCast = false;
			currentSpellClip = 0;
			player->addEvent(new PlayerEvent(EVENT_CANCEL_CAST));
		}
		if (player->isAttacking()) {
			attacking = false;
			player->addEvent(new PlayerEvent(EVENT_CANCEL_ATTACK));
			SoundEffectHandler::stopSound(attackID);
		}
		if (player->isBlocking())
			marco = spriteMap[string("blocking")].numberOfClips - 1;
	}

	//TODO - Aca se elige el map que corresponda con el arma equipada
	/*
	 *	spriteMap=selectWeaponView(this->player);
	 *
	 */
	spriteMap = weaponViewMap[string("sword")];

	FoggedSprite spriteToBeShown;

	if (previousLife != life) {
		attacked = true;
		if (marco >= spriteMap[string("hit")].numberOfClips)
			//marco = 0;
			player->stop();
		spriteToBeShown = spriteMap[string("hit")];
	}
	if (player->isDead()) {
		spriteToBeShown = spriteMap[string("die")];
		double rnd = uniform(0.0, 1.0);
		if ((marco >= spriteToBeShown.numberOfClips - 1)
				&& (!SoundEffectHandler::isSoundPlaying(string(DIE1_SOUND)))
				&& (!SoundEffectHandler::isSoundPlaying(string(DIE2_SOUND)))) {
			if (rnd > 0.5)
				SoundEffectHandler::playSound(string(DIE1_SOUND));
			else
				SoundEffectHandler::playSound(string(DIE2_SOUND));
		}
	}

	if (player->isCastingSpell()) {
		spriteToBeShown = spriteMap[string("cast")];
		spellBeingCast = true;
		spellHasEnded = false;
		if (!SoundEffectHandler::isSoundPlaying(QUAKE_SOUND))
					SoundEffectHandler::playSound(QUAKE_SOUND);
	}

	if (player->isAttacking()) {
		spriteToBeShown = spriteMap[string("attacking")];
		//Si se estaba moviendo, reseteamos el marco para que no quede un # de clip invalido
		if (player->IsMoving()) {
			if (marco >= spriteToBeShown.numberOfClips)
				marco = 0;
			player->stop();
		}
		if (!SoundEffectHandler::isSoundPlaying(attackID))
			SoundEffectHandler::playSound(attackID);
	}

	if (player->isBlocking()) {
		SoundEffectHandler::stopSound(attackID);
		marco = 0;
		player->stop();
		spriteToBeShown = spriteMap[string("blocking")];
	}
	if (player->IsMoving()) {
		SoundEffectHandler::stopSound(attackID);
		if (!SoundEffectHandler::isSoundPlaying(walkID))
			SoundEffectHandler::playSound(walkID);
		spriteToBeShown = spriteMap[string("walking")];
	} else
		SoundEffectHandler::stopSound(walkID);

	if (!player->IsMoving() && !player->isAttacking() && !player->isBlocking()
			&& previousLife == life && !player->isDead()
			&& !player->isCastingSpell()) {
		if (!wasStanding) {
			timer.start();
			wasStanding = true;
		}
		spriteToBeShown = spriteMap[string("standing")];
		image = spriteToBeShown.image;
		fogImage = spriteToBeShown.foggedImage;
		numberOfClips = spriteToBeShown.numberOfClips;
		teamColorImage = spriteToBeShown.teamColorImage;
		lastDirection = direction;
		showStandingAnimation(sprite, fondo, drawFog);
		return;
	}

	wasStanding = false;
	currentSprite = sprite;
	image = spriteToBeShown.image;
	fogImage = spriteToBeShown.foggedImage;
	teamColorImage = spriteToBeShown.teamColorImage;
	numberOfClips = spriteToBeShown.numberOfClips;
	lastDirection = direction;
	playAnimation((SpriteType) currentSprite, fondo, drawFog);

}

PlayerView::~PlayerView() {
	//libera la memoria que pide para La imagen
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
	if (nameImage)
		SDL_FreeSurface(nameImage);
	nameImage = TTF_RenderText_Solid(font, name.c_str(), color);
	if (!nameImage || !font)
		Logs::logErrorMessage(
				"Error al cargar la fuente para el nombre del personaje");

}

void PlayerView::setShowableName(string name) {
	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	TTF_Font* font = TTF_OpenFont("resources/fonts/Baramond.ttf", 28);
	if (nameImage)
		SDL_FreeSurface(nameImage);
	nameImage = TTF_RenderText_Solid(font, name.c_str(), color);
	if (!nameImage || !font)
		Logs::logErrorMessage(
				"Error al cargar la fuente para el nombre del personaje");
}

void PlayerView::playAnimation(SpriteType sprite, SDL_Surface* screen,
		bool drawFog) {
	SDL_Rect clipToDraw;
	clipToDraw.x = imageWidth * marco * scaleWidth;
	clipToDraw.y = imageHeight * sprite;
	clipToDraw.w = imageWidth * scaleWidth;
	clipToDraw.h = imageHeight * scaleHeight;

	showFrame(screen, &clipToDraw, drawFog);
	if (animationChangeRate >= ANIMATION_CHANGE_DELAY) {
		if (!player->isFrozen()) this->marco++;
		animationChangeRate = 0; // Move to the next marco in the animation
	} else
		animationChangeRate++;

}

void PlayerView::blitIceSpellEffect(SDL_Surface* screen, int x, int y) {
	if (!player->isFrozen()){
		currentIceSpellClip = 0;
		return;
	}
	SDL_Surface* spellSprite = textureHolder->getTexture(string(ICE_IMG));
	int numberOfClips = 4;
	if (currentIceSpellClip >= numberOfClips) {
		currentIceSpellClip = numberOfClips-1;
	}
	SDL_Rect offsetSpell, currentClip;
	int frameWidth = spellSprite->w / numberOfClips;
	offsetSpell.x = (int) x + camPos->getX() - frameWidth / 2 + 10;
	offsetSpell.y = (int) y + camPos->getY() - this->anchorPixel->getY() + frameWidth/10;

	currentClip.x = currentIceSpellClip * frameWidth;
	currentClip.y = 0;
	currentClip.w = frameWidth;
	currentClip.h = spellSprite->h;
	SDL_BlitSurface(spellSprite, &currentClip, screen, &offsetSpell);
	currentIceSpellClip++;

}

void PlayerView::showWeaponsHud(SDL_Surface* fondo) {
	SDL_Rect clip;
	clip.x = fondo->w - WEAPONSWIDTH;
	clip.y = fondo->h - WEAPONSHEIGHT;
	clip.h = WEAPONSHEIGHT;
	clip.w = WEAPONSWIDTH;
	SDL_Surface* img = weaponsHud[player->getCurrentWeapon()->getName()];

//	float scaleX = (float) clip.w / img->w;
//	float scaleY = (float) clip.h / img->h;

//	SDL_Surface* resizeImg = rotozoomSurfaceXY(img,0,scaleX,scaleY,0);
	SDL_Flip(img);
	SDL_BlitSurface(img,NULL,fondo,&clip);

}

void PlayerView::loadWeaponsHudSurfaces() {
	weaponsHud["Sword"] = IMG_Load(SWORDIMAGE);
	weaponsHud["FrostWand"] = IMG_Load(FROSTWANDIMAGE);

}

