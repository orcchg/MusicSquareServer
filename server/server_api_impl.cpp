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

#include <algorithm>
#include "server_api_impl.h"

ServerApiImpl::ServerApiImpl() {
  /**
   * {"alternative", "african", "bard", "blues", "classical", "conjazz", "country",
   *  "dance", "disco", "dnb", "dubstep", "dub", "electronics", "estrada", "folk",
   *  "house", "indie", "industrial", "jazz", "latinfolk", "local-indie", "lounge",
   *  "metal", "newwave", "pop", "prog", "punk", "rap", "reggae", "relax", "rnb",
   *  "rock", "rusfolk", "rusrap", "rusrock", "soul", "soundtrack", "trance", "ukrrock",
   *  "urban", "videogame"}
   */
  m_genres.emplace_back("alternative");
  m_genres.emplace_back(Genre::Builder("african").addGenre("soul"));
  m_genres.emplace_back("bard");
  m_genres.emplace_back(Genre::Builder("jazz").addGenre("blues").addGenre("conjazz").addGenre("country"));
  m_genres.emplace_back("classical");
  m_genres.emplace_back(Genre::Builder("dance").addGenre("disco").addGenre("trance"));
  m_genres.emplace_back(Genre::Builder("dubstep").addGenre("dub"));
  m_genres.emplace_back("dnb");
  m_genres.emplace_back(Genre::Builder("electronics").addGenre("house"));
  m_genres.emplace_back(Genre::Builder("pop").addGenre("estrada"));
  m_genres.emplace_back(Genre::Builder("folk").addGenre("latinfolk").addGenre("rusfolk"));
  m_genres.emplace_back(Genre::Builder("indie").addGenre("local-indie"));
  m_genres.emplace_back("industrial");
  m_genres.emplace_back("lounge");
  m_genres.emplace_back("metal");
  m_genres.emplace_back("newwave");
  m_genres.emplace_back("prog");
  m_genres.emplace_back("punk");
  m_genres.emplace_back(Genre::Builder("rap").addGenre("rusrap"));
  m_genres.emplace_back("reggae");
  m_genres.emplace_back("relax");
  m_genres.emplace_back(Genre::Builder("rnb").addGenre("urban"));
  m_genres.emplace_back(Genre::Builder("rock").addGenre("rusrock").addGenre("ukrrock"));
  m_genres.emplace_back(Genre::Builder("soundtrack").addGenre("videogame"));
}

ServerApiImpl::~ServerApiImpl() {
}

int ServerApiImpl::getTotalModels() {
  return m_db.getTotalModels();
}

int ServerApiImpl::getTotalModels(const std::vector<std::string>& titles) {
  if (titles.empty()) return m_db.getTotalModels();
  return m_db.getTotalModels(titles);
}

Model ServerApiImpl::getModel(int64_t id) {
  return m_db.getModel(id);
}

void ServerApiImpl::getModels(std::vector<SmallModel>* const output, int limit, int offset) {
  m_db.getSmallModels(output, limit, offset);
}

void ServerApiImpl::getModels(std::vector<SmallModel>* const output, int limit, int offset, const std::vector<std::string>& titles) {
  m_db.getSmallModels(output, limit, offset, titles);
}

void ServerApiImpl::getGenres(std::vector<Genre>* const output) {
  std::copy(m_genres.begin(), m_genres.end(), std::back_inserter(*output));
}

