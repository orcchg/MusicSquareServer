/** 
 *   HTTP Chat server with authentication and multi-channeling.
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

#include <gtest/gtest.h>
#include "model.h"
#include "models_raw.h"

namespace test {

TEST(ObtainModel_raw, FromJson) {
  std::vector<std::string> genres;
  genres.push_back("pop");
  genres.push_back("dance");
  genres.push_back("electronics");

  Model model = Model::fromJson(user_1);
  Model etalon = Model::Builder(1080505, "Tove Lo")
      .setGenres(genres)
      .setTracksCount(81)
      .setAlbumsCount(22)
      .setWebLink("http://www.tove-lo.com/")
      .setDescription("one")
      .setCoverBig("http://avatars.yandex.net/get-music-content/dfc531f5.p.1080505/1000x1000")
      .setCoverSmall("http://avatars.yandex.net/get-music-content/dfc531f5.p.1080505/300x300")
      .build();

  EXPECT_EQ(etalon.getId(), model.getId());
  EXPECT_STREQ(etalon.getName().c_str(), model.getName().c_str());
  for (size_t i = 0; i < genres.size(); ++i) {
    EXPECT_STREQ(etalon.getGenres()[i].c_str(), model.getGenres()[i].c_str());
  }
  EXPECT_STREQ(etalon.getGenresStr().c_str(), model.getGenresStr().c_str());
  EXPECT_EQ(etalon.getTracksCount(), model.getTracksCount());
  EXPECT_EQ(etalon.getAlbumsCount(), model.getAlbumsCount());
  EXPECT_STREQ(etalon.getWebLink().c_str(), model.getWebLink().c_str());
  EXPECT_STREQ(etalon.getDescription().c_str(), model.getDescription().c_str());
  EXPECT_STREQ(etalon.getCoverBig().c_str(), model.getCoverBig().c_str());
  EXPECT_STREQ(etalon.getCoverSmall().c_str(), model.getCoverSmall().c_str());
}

TEST(ObtainModel, FromJson) {
  std::vector<std::string> genres;
  genres.push_back("pop");
  genres.push_back("dance");
  genres.push_back("electronics");

  Model model = Model::fromJson(user_1);
  Model etalon = Model::Builder(1080505, "Tove Lo")
      .setGenres(genres)
      .setTracksCount(81)
      .setAlbumsCount(22)
      .setWebLink("http://www.tove-lo.com/")
      .setDescription("one")
      .setCoverBig("http://avatars.yandex.net/get-music-content/dfc531f5.p.1080505/1000x1000")
      .setCoverSmall("http://avatars.yandex.net/get-music-content/dfc531f5.p.1080505/300x300")
      .build();
  EXPECT_TRUE(etalon == model);
  EXPECT_EQ(etalon, model);
}

TEST(ObtainSmallModel, FromJson) {
  SmallModel model = SmallModel::fromJson(user_0);
  SmallModel etalon = SmallModel::Builder(1080505, "Tove Lo")
      .setCoverSmall("http://avatars.yandex.net/get-music-content/dfc531f5.p.1080505/300x300")
      .build();
  EXPECT_EQ(etalon, model);
}

}  // namespace test

