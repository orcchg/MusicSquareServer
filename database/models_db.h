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

#ifndef MSQ_MODELS_DB__H__
#define MSQ_MODELS_DB__H__

#include "database.h"
#include "model.h"

#define TABLE_NAME "models"

#define D_COLUMN_NAME_ROW_ID            "__rowid__"
#define D_COLUMN_NAME_MODEL_ID          "id"
#define D_COLUMN_NAME_MODEL_NAME        "name"
#define D_COLUMN_NAME_MODEL_GENRES      "genres"
#define D_COLUMN_NAME_MODEL_TRACKS      "tracks"
#define D_COLUMN_NAME_MODEL_ALBUMS      "albums"
#define D_COLUMN_NAME_MODEL_LINK        "link"
#define D_COLUMN_NAME_MODEL_DESCRIPTION "description"
#define D_COLUMN_NAME_MODEL_COVER_BIG   "cover_big"
#define D_COLUMN_NAME_MODEL_COVER_SMALL "cover_small"

extern const char* COLUMN_NAME_ROW_ID;
extern const char* COLUMN_NAME_MODEL_ID;
extern const char* COLUMN_NAME_MODEL_NAME;
extern const char* COLUMN_NAME_MODEL_GENRES;
extern const char* COLUMN_NAME_MODEL_TRACKS;
extern const char* COLUMN_NAME_MODEL_ALBUMS;
extern const char* COLUMN_NAME_MODEL_LINK;
extern const char* COLUMN_NAME_MODEL_DESCRIPTION;
extern const char* COLUMN_NAME_MODEL_COVER_BIG;
extern const char* COLUMN_NAME_MODEL_COVER_SMALL;

namespace db {

class ModelsDatabase : private Database {
public:
  ModelsDatabase(const std::string& database_name = DATABASE_NAME);
  ModelsDatabase(ModelsDatabase&& rval_obj);
  virtual ~ModelsDatabase();

  int64_t addModel(const Model& model);
  void removeModel(int64_t id);
  Model getModel(int64_t id);
  void getModels(std::vector<Model>* const output);
  SmallModel getSmallModel(int64_t id);
  void getSmallModels(std::vector<SmallModel>* const output);
  void getSmallModels(std::vector<SmallModel>* const output, int limit, int offset);
  void getSmallModels(std::vector<SmallModel>* const output, int limit, int offset, const std::vector<std::string>& titles);
  int getTotalModels();
  int getTotalModels(const std::vector<std::string>& titles);

private:
  void __init__() override final;
  void __create_table__() override final;

  Model getModelFromStatement();
  SmallModel getSmallModelFromStatement();
  void getSmallModelsFromStatement(std::vector<SmallModel>* const output, const std::string& select_statement);
  int getTotalModelsFromStatement(const std::string& count_statement);

  ModelsDatabase(const ModelsDatabase& obj) = delete;
  ModelsDatabase& operator = (const ModelsDatabase& rhs) = delete;
  ModelsDatabase& operator = (ModelsDatabase&& rval_rhs) = delete;
};

}  // namespace db

#endif  // MSQ_MODELS_DB__H__

