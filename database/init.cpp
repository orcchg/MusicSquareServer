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

#include <fstream>
#include <sys/stat.h>
#include "models_db.h"

#define FILE_STEP 4096

bool isFileAccessible(const std::string& filename) {
  struct stat buffer;
  return (stat(filename.c_str(), &buffer) == 0);
}

std::string readFileToString(const std::string& filename) {
  if (!isFileAccessible(filename)) {
    ERR("File is not accessible: %s", filename.c_str());
    return "";
  }
  std::ifstream fin(filename, std::fstream::in);
  fin.seekg(0, std::ios::end);
  size_t size = fin.tellg();
  std::string buffer(size, ' ');
  fin.seekg(0);
  fin.read(&buffer[0], size);
  fin.close();
  return buffer;
}

int main(int argc, char** argv) {
  db::ModelsDatabase db;

  std::string filename = "../database/raw_data.json";
  if (isFileAccessible(filename)) {
    // read file object-by-object
    std::ifstream fin(filename, std::fstream::in);
    fin.seekg(0, std::ios::end);
    size_t size = fin.tellg();
    fin.seekg(1);  // omit '['

    int index = 0;

    size_t position = 1;
    while (position < size) {  // omit ']'
      size_t eat_size = position + FILE_STEP < size ? FILE_STEP : size - position;
      if (eat_size <= 0) {
        SYS("Stop 1");
        break;
      }

      std::string buffer(eat_size, ' ');
      fin.read(&buffer[0], eat_size);
      int i1 = buffer.find("}}");
      TRC("buffer[%i]: %s", i1, buffer.c_str());
      if (i1 < 0) {
        SYS("Stop 2");
        break;
      }

      std::string json = buffer.substr(0, i1 + 2);
      VER("model[%i]: %s", index, json.c_str());
      Model model = Model::fromJson(json);
      db.addModel(model);

      ++index;
      position += i1 + 3;
      fin.seekg(position);
    }

    INF("Total: %i", index);
  } else {
    ERR("File %s does not exists!", filename.c_str());
    throw 1;
  }

  return 0;
}

