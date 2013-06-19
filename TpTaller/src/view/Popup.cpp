/*
 * Button.cpp
 *
 *  Created on: 25/03/2013
 *      Author: tomas
 */

#include <view/Popup.h>
#include <SDL/SDL_image.h>
#include <model/Logs/Logs.h>
#include <unistd.h>


#define ENVIRONMENT_WINDOW "SDL_VIDEO_CENTERED=1"
#define WIDTH 300
#define HEIGHT 100

using namespace std;

Popup::Popup() {

}

void alertBox(string s) {
	char cmd[1024];
	sprintf(cmd,
			"xmessage -font \"xft:Dejavu Sans Mono:pixelsize=40.0\" -timeout 5 -buttons OK -title 'Alert' -center \"%s\"",
			s.c_str());
	pid_t pid = fork();
	if (pid == 0) {
		close(1);
		close(2);
		system(cmd);
		exit(0);
	}

}

void msgBox(string s) {
	char cmd[1024];
	sprintf(cmd,
			"xmessage -font \"xft:Dejavu Sans Mono:pixelsize=40.0\" -timeout 5 -buttons OK -title 'Error' -center \"%s\"",
			s.c_str());
	if (fork() == 0) {
		close(1);
		close(2);
		system(cmd);
		exit(0);
	}
}

void Popup::popupWindow(string msg) {

	msgBox(msg);

}

void Popup::alertWindow(string msg) {

	alertBox(msg);

}
