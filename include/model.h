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

#ifndef MSQ_MODEL__H__
#define MSQ_MODEL__H__

#include <unordered_map>
#include <cstdint>
#include "common.h"
#include "logger.h"

#define D_ITEM_ID           "id"
#define D_ITEM_NAME         "name"
#define D_ITEM_GENRES       "genres"
#define D_ITEM_TRACKS       "tracks"
#define D_ITEM_ALBUMS       "albums"
#define D_ITEM_LINK         "link"
#define D_ITEM_DESCRIPTION  "description"
#define D_ITEM_COVER        "cover"

#define D_ITEM_LIMIT        "limit"
#define D_ITEM_OFFSET       "offset"
#define D_ITEM_VALUE        "value"

#define D_COVER_BIG   "big"
#define D_COVER_SMALL "small"

extern const char* ITEM_ID;
extern const char* ITEM_NAME;
extern const char* ITEM_GENRES;
extern const char* ITEM_TRACKS;
extern const char* ITEM_ALBUMS;
extern const char* ITEM_LINK;
extern const char* ITEM_DESCRIPTION;
extern const char* ITEM_COVER;

extern const char* ITEM_LIMIT;
extern const char* ITEM_OFFSET;
extern const char* ITEM_VALUE;

extern const char* COVER_BIG;
extern const char* COVER_SMALL;

// ----------------------------------------------
class SmallModel : public common::Jsonable {
  int64_t id;
  std::string name;
  std::string cover_small;

public:
  SmallModel() : id(0) {}
  virtual ~SmallModel() {}

  class Builder {
    friend class SmallModel;
  public:
    Builder(int64_t id, const std::string& name) : id(id), name(name) {}
    Builder& setCoverSmall(const std::string& cover) { cover_small = cover;  return *this; }
    SmallModel build() { return SmallModel(*this); }

  private:
    int64_t id;
    std::string name;
    std::string cover_small;
  };

  explicit SmallModel(const SmallModel::Builder& builder);

  std::string toString() const;
  std::string toJson() const override final;
  static SmallModel fromJson(const std::string& json);

  inline int64_t getId() const { return id; }
  inline const std::string& getName() const { return name; }
  inline const std::string& getCoverSmall() const { return cover_small; }

  bool operator == (const SmallModel& rhs) const {
    return id == rhs.id && name.compare(rhs.name) == 0 && cover_small.compare(rhs.cover_small) == 0;
  }

  bool operator != (const SmallModel& rhs) const { return !(*this == rhs); }

  inline bool isEmpty() const { return id == 0; }
};

// ----------------------------------------------
class Model : public common::Jsonable {
  int64_t id;
  std::string name;
  std::vector<std::string> genres;
  int tracks_count;
  int albums_count;
  std::string web_link;
  std::string description;
  std::unordered_map<std::string, std::string> covers;

public:
  Model() : id(0) {}
  virtual ~Model() {}

  class Builder {
    friend class Model;
  public:
    Builder(int64_t id, const std::string& name) : id(id), name(name) {}
    Builder& setGenres(const std::vector<std::string>& genres) { this->genres = genres;  return *this; }
    Builder& addGenre(const std::string& genre)                { this->genres.push_back(genre);  return *this; }
    Builder& setTracksCount(int count)                         { tracks_count = count;   return *this; }
    Builder& setAlbumsCount(int count)                         { albums_count = count;   return *this; }
    Builder& setWebLink(const std::string& link)               { web_link = link;  return *this; }
    Builder& setDescription(const std::string& description)    { this->description = description;  return *this; }
    Builder& setCoverBig(const std::string& cover)             { covers[COVER_BIG] = cover;  return *this; }
    Builder& setCoverSmall(const std::string& cover)           { covers[COVER_SMALL] = cover;  return *this; }
    Model build() { return Model(*this); }

  private:
    int64_t id;
    std::string name;
    std::vector<std::string> genres;
    int tracks_count;
    int albums_count;
    std::string web_link;
    std::string description;
    std::unordered_map<std::string, std::string> covers;
  };

  explicit Model(const Model::Builder& builder);

  std::string toString() const;
  std::string toJson() const override final;
  static Model fromJson(const std::string& json);

  inline int64_t getId() const { return id; }
  inline const std::string& getName() const { return name; }
  inline const std::vector<std::string>& getGenres() const { return genres; }
  inline int getTracksCount() const { return tracks_count; }
  inline int getAlbumsCount() const { return albums_count; }
  inline const std::string& getWebLink() const { return web_link; }
  inline const std::string& getDescription() const { return description; }
  inline const std::string& getCoverBig() const { return covers.at(COVER_BIG); }
  inline const std::string& getCoverSmall() const { return covers.at(COVER_SMALL); }

  std::string getGenresStr() const;

  bool operator == (const Model& rhs) const {
    bool accumulate = genres.size() == rhs.genres.size();
    if (accumulate) {
      for (size_t i = 0; i < genres.size(); ++i) {
        accumulate = accumulate && (genres[i].compare(rhs.genres[i]) == 0);
      }
    }
    return id == rhs.id && name.compare(rhs.name) == 0 && accumulate &&
        tracks_count == rhs.tracks_count && albums_count == rhs.albums_count &&
        web_link.compare(rhs.web_link) == 0 && description.compare(rhs.description) == 0 &&
        covers.at(COVER_BIG).compare(rhs.covers.at(COVER_BIG)) == 0 &&
        covers.at(COVER_SMALL).compare(rhs.covers.at(COVER_SMALL)) == 0;
  }

  bool operator != (const Model& rhs) const { return !(*this == rhs); }

  inline bool isEmpty() const { return id == 0; }
};

// ----------------------------------------------
class Genre : public common::Jsonable {
  std::string name;
  std::vector<std::string> genres;

public:
  Genre() {}
  explicit Genre(const std::string& name) : name(name) { this->genres.push_back(name); }
  virtual ~Genre() {}

  class Builder {
    friend class Genre;
  public:
    Builder(const std::string& name) : name(name) { this->genres.push_back(name); }
    Builder& setGenres(const std::vector<std::string>& genres) { this->genres = genres;  return *this; }
    Builder& addGenre(const std::string& genre) { this->genres.push_back(genre);  return *this; }
    Genre build() { return Genre(*this); }

  private:
    std::string name;
    std::vector<std::string> genres;
  };

  explicit Genre(const Genre::Builder& builder);

  std::string toString() const;
  std::string toJson() const override final;
  static Genre fromJson(const std::string& json);

  inline const std::string& getName() const { return name; }
  inline const std::vector<std::string>& getGenres() const { return genres; }

  std::string getGenresStr() const;

  bool operator == (const Genre& rhs) const { return name.compare(rhs.name) == 0; }
  bool operator != (const Genre& rhs) const { return name.compare(rhs.name) != 0; }
  bool operator <  (const Genre& rhs) const { return name.compare(rhs.name) <  0; }
  bool operator <= (const Genre& rhs) const { return name.compare(rhs.name) <= 0; }
  bool operator >  (const Genre& rhs) const { return name.compare(rhs.name) >  0; }
  bool operator >= (const Genre& rhs) const { return name.compare(rhs.name) >= 0; }

  inline bool isEmpty() { return genres.empty(); }
};

#endif  // MSQ_MODEL__H__

