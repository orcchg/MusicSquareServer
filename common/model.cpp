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

#include "model.h"
#include "rapidjson/document.h"

const char* ITEM_ID          = D_ITEM_ID;
const char* ITEM_NAME        = D_ITEM_NAME;
const char* ITEM_GENRES      = D_ITEM_GENRES;
const char* ITEM_TRACKS      = D_ITEM_TRACKS;
const char* ITEM_ALBUMS      = D_ITEM_ALBUMS;
const char* ITEM_LINK        = D_ITEM_LINK;
const char* ITEM_DESCRIPTION = D_ITEM_DESCRIPTION;
const char* ITEM_COVER       = D_ITEM_COVER;

const char* ITEM_LIMIT       = D_ITEM_LIMIT;
const char* ITEM_OFFSET      = D_ITEM_OFFSET;
const char* ITEM_VALUE       = D_ITEM_VALUE;

const char* COVER_BIG   = D_COVER_BIG;
const char* COVER_SMALL = D_COVER_SMALL;

// ----------------------------------------------
SmallModel::SmallModel(const SmallModel::Builder& builder)
  : id(builder.id)
  , name(builder.name)
  , cover_small(builder.cover_small) {
}

std::string SmallModel::toString() const {
  return toJson();
}

SmallModel SmallModel::fromJson(const std::string& json) {
  rapidjson::Document document;
  document.Parse(json.c_str());

  SmallModel model;

  if (document.IsObject() &&
      document.HasMember(ITEM_ID) && document[ITEM_ID].IsInt64() &&
      document.HasMember(ITEM_NAME) && document[ITEM_NAME].IsString() &&
      document.HasMember(ITEM_COVER) && document[ITEM_COVER].IsString()) {
    model.id = document[ITEM_ID].GetInt();
    model.name = document[ITEM_NAME].GetString();
    model.cover_small = document[ITEM_COVER].GetString();
  } else {
    ERR("Failed to parse small model from json: %s", json.c_str());
  }

  return model;
}

/**
 * {"id":1080505,
 *  "name":"Tove Lo",
 *  "cover":"http://avatars.yandex.net/get-music-content/dfc531f5.p.1080505/300x300"
 * }
 */
std::string SmallModel::toJson() const {
  std::ostringstream oss;
  oss << "{\"" D_ITEM_ID "\":" << id
      << ",\"" D_ITEM_NAME "\":\"" << name
      << "\",\"" D_ITEM_COVER "\":\"" << cover_small
      << "\"}";
  return oss.str();
}

// ----------------------------------------------
Model::Model(const Model::Builder& builder)
  : id(builder.id)
  , name(builder.name)
  , genres(builder.genres)
  , tracks_count(builder.tracks_count)
  , albums_count(builder.albums_count)
  , web_link(builder.web_link)
  , description(builder.description)
  , covers(builder.covers) {
}

std::string Model::toString() const {
  return toJson();
}

Model Model::fromJson(const std::string& json) {
  rapidjson::Document document;
  document.Parse(json.c_str());

  Model model;

  if (document.IsObject() &&
      document.HasMember(ITEM_ID) && document[ITEM_ID].IsInt64() &&
      document.HasMember(ITEM_NAME) && document[ITEM_NAME].IsString() &&
      document.HasMember(ITEM_GENRES) && document[ITEM_GENRES].IsArray() &&
      document.HasMember(ITEM_TRACKS) && document[ITEM_TRACKS].IsInt() &&
      document.HasMember(ITEM_ALBUMS) && document[ITEM_ALBUMS].IsInt() &&
//      document.HasMember(ITEM_LINK) && document[ITEM_LINK].IsString() &&
//      document.HasMember(ITEM_DESCRIPTION) && document[ITEM_DESCRIPTION].IsString() &&
      document.HasMember(ITEM_COVER) && document[ITEM_COVER].IsObject()) {
    model.id = document[ITEM_ID].GetInt();
    model.name = document[ITEM_NAME].GetString();

    // genres
    const rapidjson::Value& genres = document[ITEM_GENRES];
    for (rapidjson::SizeType i = 0; i < genres.Size(); ++i) {
      std::string genre = genres[i].GetString();
      model.genres.push_back(genre);
    }

    model.tracks_count = document[ITEM_TRACKS].GetInt();
    model.albums_count = document[ITEM_ALBUMS].GetInt();
    model.web_link = document.HasMember(ITEM_LINK) && document[ITEM_LINK].IsString() ? document[ITEM_LINK].GetString() : "";
    model.description = document.HasMember(ITEM_DESCRIPTION) && document[ITEM_DESCRIPTION].IsString() ? document[ITEM_DESCRIPTION].GetString() : "";

    // covers
    const char* kTypeNames[] = { COVER_BIG, COVER_SMALL };
    model.covers[COVER_BIG] = document[ITEM_COVER][COVER_BIG].GetString();
    model.covers[COVER_SMALL] = document[ITEM_COVER][COVER_SMALL].GetString();
    //for (rapidjson::Value::ConstMemberIterator it = document[ITEM_COVER].MemberBegin(); it != document[ITEM_COVER].MemberEnd(); ++it) {}
  } else {
    ERR("Failed to parse model from json: %s", json.c_str());
  }

  return model;
}

/**
 * {"id":1080505,
 *  "name":"Tove Lo",
 *  "genres":["pop","dance","electronics"],
 *  "tracks":81,
 *  "albums":22,
 *  "link":"http://www.tove-lo.com/",
 *  "description":"Hello World.",
 *  "cover":{"small":"http://avatars.yandex.net/get-music-content/dfc531f5.p.1080505/300x300",
 *           "big"  :"http://avatars.yandex.net/get-music-content/dfc531f5.p.1080505/1000x1000"
 *          }
 * }
 */
std::string Model::toJson() const {
  std::ostringstream oss;
  oss << "{\"" D_ITEM_ID     "\":" << id
      << ",\"" D_ITEM_NAME   "\":\"" << name
      << "\",\"" D_ITEM_GENRES "\":[";
  const char* delim = "";
  for (auto& item : genres) {
    oss << delim << "\"" << item << "\"";
    delim = ",";
  }
  bool small = false;
  oss << "],\"" D_ITEM_TRACKS "\":" << tracks_count
      << ",\""  D_ITEM_ALBUMS "\":" << albums_count
      << ",\""  D_ITEM_LINK   "\":\"" << web_link
      << "\",\"" D_ITEM_DESCRIPTION "\":\"" << description
      << "\",\"" D_ITEM_COVER "\":{";
  if (covers.find(COVER_SMALL) != covers.end()) {
    oss << "\"" D_COVER_SMALL "\":\"" << covers.at(COVER_SMALL) << "\"";
    small = true;
  }
  if (covers.find(COVER_BIG) != covers.end()) {
    if (small) oss << ",";
    oss << "\"" D_COVER_BIG "\":\"" << covers.at(COVER_BIG) << "\"";
  }
  oss << "}}";
  return oss.str();
}

std::string Model::getGenresStr() const {
  return common::whitespaceSeparated(genres);
}

// ----------------------------------------------
Genre::Genre(const Genre::Builder& builder)
  : name(builder.name)
  , genres(builder.genres) {
}

std::string Genre::toString() const {
  return toJson();
}

Genre Genre::fromJson(const std::string& json) {
  rapidjson::Document document;
  document.Parse(json.c_str());

  Genre model;

  if (document.IsObject() &&
      document.HasMember(ITEM_NAME) && document[ITEM_NAME].IsString() &&
      document.HasMember(ITEM_GENRES) && document[ITEM_GENRES].IsArray()) {
    model.name = document[ITEM_NAME].GetString();
    const rapidjson::Value& genres = document[ITEM_GENRES];
    for (rapidjson::SizeType i = 0; i < genres.Size(); ++i) {
      std::string genre = genres[i].GetString();
      model.genres.push_back(genre);
    }
  } else {
    ERR("Failed to parse genre from json: %s", json.c_str());
  }

  return model;
}

/**
 * {"name":"rock","genres":["rock","rusrock","ukrrock"]}
 */
std::string Genre::toJson() const {
  std::ostringstream oss;
  oss << "{\"" D_ITEM_NAME "\":\"" << name
      << "\",\"" D_ITEM_GENRES "\":[";
  const char* delim = "";
  for (auto& item : genres) {
    oss << delim << "\"" << item << "\"";
    delim = ",";
  }
  oss << "]}";
  return oss.str();
}

std::string Genre::getGenresStr() const {
  return common::whitespaceSeparated(genres);
}

