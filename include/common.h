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

#ifndef MSQ_COMMON__H__
#define MSQ_COMMON__H__

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

/**
 *  GET /all?limit=20&offset=25&genres=rap,rnb,rock
 */

/**
 *  GET /genres
 */

/**
 *  GET /genre?name=rock
 */

/**
 *  GET /single?id=100500
 */

/**
 *  GET /total?genres=rap,rnb,rock
 */

/**
 *  GET /total_genres
 */

#define D_PATH_ALL    "/all"
#define D_PATH_GENRES "/genres"
#define D_PATH_GENRE  "/genre"
#define D_PATH_SINGLE "/single"
#define D_PATH_TOTAL  "/total"
#define D_PATH_TOTAL_GENRES "/total_genres"

extern const char* PATH_ALL;
extern const char* PATH_GENRES;
extern const char* PATH_GENRE;
extern const char* PATH_SINGLE;
extern const char* PATH_TOTAL;
extern const char* PATH_TOTAL_GENRES;

enum class Method : int {
  UNKNOWN = -1, GET = 0, POST = 1, PUT = 2, DELETE = 3
};

enum class Path : int {
  UNKNOWN = -1, ALL = 0, GENRES = 1, GENRE = 2, SINGLE = 3, TOTAL = 4, TOTAL_GENRES = 5
};

namespace common {

struct Jsonable {
  virtual std::string toJson() const = 0;
};

std::string arrayToJson(const std::vector<const Jsonable*>& array);
std::string whitespaceSeparated(const std::vector<std::string>& array);

template <typename Iter, typename T>
Iter binary_find(Iter begin, Iter end, T value) {
  Iter it = std::lower_bound(begin, end, value);
  if (it != end && !(value < *it)) return it;
  return end;
}

}  // namespace common

#endif  // MSQ_COMMON__H__

