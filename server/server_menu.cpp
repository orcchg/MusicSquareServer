/** 
 *   HTTP Server
 *
 *   Copyright (C) 2016  Maxim Alov
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software Foundation,
 *   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 *   This program and text files composing it, and/or compiled binary files
 *   (object files, shared objects, binary executables) obtained from text
 *   files of this program using compiler, as well as other files (text, images, etc.)
 *   composing this program as a software project, or any part of it,
 *   cannot be used by 3rd-parties in any commercial way (selling for money or for free,
 *   advertising, commercial distribution, promotion, marketing, publishing in media, etc.).
 *   Only the original author - Maxim Alov - has right to do any of the above actions.
 */

#include <string>
#include <cstdio>
#include <cstring>
#include "logger.h"
#include "server_menu.h"

namespace menu {

const char* HELP = "help";
const char* STOP = "stop";

bool evaluate(Server* server, const std::string& command) {
  int64_t id = 0;
  char* message = nullptr;
  if (strcmp(HELP, command.c_str()) == 0) {
    printHelp();
  } else if (strcmp(STOP, command.c_str()) == 0) {
    server->stop();
    return false;
  } else {
    WRN("Undefined command: %s", command.c_str());
  }
  return true;
}

void printHelp() {
  printf("\e[5;00;33m\t***    HTTP Server " D_VERSION "    ***\t\e[m\n");
  printf("Commands:\n\t%s - print this help \
                   \n\t%s - send terminate signal to all peers and stop server\n", HELP, STOP);
}

void printPrompt() {
  printf("server@server:");
}

}

