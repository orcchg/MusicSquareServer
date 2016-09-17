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
#include "database/models_db.h"
#include "models_raw.h"

namespace test {

// ----------------------------------------------
class DatabaseFixture : public ::testing::Test {
protected:
  DatabaseFixture() {}
  void SetUp() override;
  void TearDown() override;

  db::ModelsDatabase* db;
};

void DatabaseFixture::SetUp() {
  db = new db::ModelsDatabase("test_ModelsDatabase.db");
  db->addModel(Model::fromJson(user_1));
  db->addModel(Model::fromJson(user_2));
  db->addModel(Model::fromJson(user_3));
  db->addModel(Model::fromJson(user_4));
  db->addModel(Model::fromJson(user_5));
  db->addModel(Model::fromJson(user_6));
}

void DatabaseFixture::TearDown() {
  delete db;  db = nullptr;
  remove("test_ModelsDatabase.db");
}

/* Tests */
// ----------------------------------------------------------------------------
TEST_F(DatabaseFixture, Add1) {
  std::vector<Model> models = db->getModels();
  EXPECT_EQ(6, models.size());
}

TEST_F(DatabaseFixture, Add2) {
  std::vector<SmallModel> models = db->getSmallModels();
  EXPECT_EQ(6, models.size());
}

TEST_F(DatabaseFixture, GetAllModels1) {
  std::vector<Model> models = db->getModels();
  EXPECT_EQ(Model::fromJson(user_1), models[0]);
  EXPECT_EQ(Model::fromJson(user_2), models[1]);
  EXPECT_EQ(Model::fromJson(user_3), models[2]);
  EXPECT_EQ(Model::fromJson(user_4), models[3]);
  EXPECT_EQ(Model::fromJson(user_5), models[4]);
  EXPECT_EQ(Model::fromJson(user_6), models[5]);
}

TEST_F(DatabaseFixture, GetAllModels2) {
  std::vector<SmallModel> models = db->getSmallModels();
  EXPECT_EQ(SmallModel::fromJson(user_0), models[0]);
}

}  // namespace test

