#include <view/FogCreator.h>

FogCreator::FogCreator() {

}

FogCreator::~FogCreator() {

}

SDL_Surface* FogCreator::getFog(SDL_Surface* image) {
	SDL_LockSurface(image);
	SDL_Surface* fog = createFogSurface(image->w, image->h);

	Uint32 *pixels = (Uint32 *) image->pixels;
	Uint32 pixelValue;
	Uint8 red, green, blue, alpha;	// channels

	for (int x = 0; x < image->w; x++) {
		for (int y = 0; y < image->h; y++) {
			pixelValue = pixels[(y * image->w) + x];
			SDL_GetRGBA(pixelValue, image->format, &red, &green, &blue, &alpha);

			if (alpha < 100)
				setPixelInvisible(fog, x, y);
		}
	}

	SDL_UnlockSurface(image);

	return fog;
}

void FogCreator::setPixelInvisible(SDL_Surface * surface, int x, int y) {
	Uint32 mask = 0x00000000;

	Uint8 * pixel = (Uint8*) surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint32));
	*((Uint32*) pixel) = mask;
}

SDL_Surface* FogCreator::createFogSurface(int width, int heigth) {
	Uint32 rmask, gmask, bmask, amask;
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;

	SDL_Surface* retval = SDL_CreateRGBSurface(SDL_SWSURFACE, width, heigth, 32,
			rmask, gmask, bmask, amask);

	SDL_FillRect(retval, NULL, 0x90000000);	// Si quieren tocar que tan oscuro es el fog cambien
											// los dos primeros valores del hexa ese.

	if (retval == NULL) {
		Logs::logErrorMessage("Fog surface cannot be loaded");
	}

	return retval;
}
