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

#ifndef MSQ_SERVER_API__H__
#define MSQ_SERVER_API__H__

#include <string>
#include <vector>
#include <cstdint>
#include "model.h"

class IServerApi {
public:
  virtual ~IServerApi() {}
  virtual int getTotalGenres() = 0;
  virtual int getTotalModels() = 0;
  virtual int getTotalModels(const std::vector<std::string>& titles) = 0;
  virtual Genre getGenre(const std::string& name) = 0;
  virtual Model getModel(int64_t id) = 0;
  virtual void getGenres(std::vector<Genre>* const output) = 0;
  virtual void getModels(std::vector<SmallModel>* const output, int limit, int offset) = 0;
  virtual void getModels(std::vector<SmallModel>* const output, int limit, int offset, const std::vector<std::string>& titles) = 0;
};

#endif  // MSQ_SERVER_API__H__

