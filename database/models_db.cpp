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

#include <sstream>
#include "logger.h"
#include "models_db.h"

namespace db {

const char* COLUMN_NAME_ROW_ID            = D_COLUMN_NAME_ROW_ID;
const char* COLUMN_NAME_MODEL_ID          = D_COLUMN_NAME_MODEL_ID;
const char* COLUMN_NAME_MODEL_NAME        = D_COLUMN_NAME_MODEL_NAME;
const char* COLUMN_NAME_MODEL_GENRES      = D_COLUMN_NAME_MODEL_GENRES;
const char* COLUMN_NAME_MODEL_TRACKS      = D_COLUMN_NAME_MODEL_TRACKS;
const char* COLUMN_NAME_MODEL_ALBUMS      = D_COLUMN_NAME_MODEL_ALBUMS;
const char* COLUMN_NAME_MODEL_LINK        = D_COLUMN_NAME_MODEL_LINK;
const char* COLUMN_NAME_MODEL_DESCRIPTION = D_COLUMN_NAME_MODEL_DESCRIPTION;
const char* COLUMN_NAME_MODEL_COVER_BIG   = D_COLUMN_NAME_MODEL_COVER_BIG;
const char* COLUMN_NAME_MODEL_COVER_SMALL = D_COLUMN_NAME_MODEL_COVER_SMALL;

ModelsDatabase::ModelsDatabase(const std::string& database_name)
  : Database(database_name, TABLE_NAME) {
  INF("enter ModelsDatabase constructor.");
  this->__init__();
  INF("exit ModelsDatabase constructor.");
}

ModelsDatabase::ModelsDatabase(ModelsDatabase&& rval_obj)
  : Database(std::move(static_cast<Database&>(rval_obj))) {
}

ModelsDatabase::~ModelsDatabase() {
  INF("enter ModelsDatabase destructor.");
  this->__close_database__();
  INF("exit ModelsDatabase destructor.");
}

// ----------------------------------------------
int64_t ModelsDatabase::addModel(const Model& model) {
  INF("enter ModelsDatabase::addModel().");
  std::string insert_statement = "INSERT INTO '";
  insert_statement += this->m_table_name;
  insert_statement += "' VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10);";
  this->__prepare_statement__(insert_statement);

  bool accumulate = true;
  int64_t i_row_id = this->m_next_id++;
  accumulate = accumulate && (sqlite3_bind_int64(this->m_db_statement, 1, i_row_id) == SQLITE_OK);
  DBG("ID [%li] has been stored in table ["%s"], SQLite database ["%s"].",
      i_row_id, this->m_table_name.c_str(), this->m_db_name.c_str());

  int64_t i_model_id = model.getId();
  accumulate = accumulate && (sqlite3_bind_int64(this->m_db_statement, 2, i_model_id) == SQLITE_OK);
  DBG("Model ID [%li] has been stored in table ["%s"], SQLite database ["%s"].",
      i_model_id, this->m_table_name.c_str(), this->m_db_name.c_str());

  WrappedString i_name = WrappedString(model.getName());
  int name_n_bytes = i_name.n_bytes();
  accumulate = accumulate && (sqlite3_bind_text(this->m_db_statement, 3, i_name.c_str(), name_n_bytes, SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Model name ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
      i_name.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  WrappedString i_genres = WrappedString(model.getGenresStr());
  int genres_n_bytes = i_genres.n_bytes();
  accumulate = accumulate && (sqlite3_bind_text(this->m_db_statement, 4, i_genres.c_str(), genres_n_bytes, SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Model genres ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
      i_genres.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  int tracks_count = model.getTracksCount();
  accumulate = accumulate && (sqlite3_bind_int64(this->m_db_statement, 5, tracks_count) == SQLITE_OK);
  DBG("Model tracks count [%i] has been stored in table ["%s"], SQLite database ["%s"].",
      tracks_count, this->m_table_name.c_str(), this->m_db_name.c_str());

  int albums_count = model.getAlbumsCount();
  accumulate = accumulate && (sqlite3_bind_int64(this->m_db_statement, 6, albums_count) == SQLITE_OK);
  DBG("Model albums count [%i] has been stored in table ["%s"], SQLite database ["%s"].",
      albums_count, this->m_table_name.c_str(), this->m_db_name.c_str());

  WrappedString i_link = WrappedString(model.getWebLink());
  int link_n_bytes = i_link.n_bytes();
  accumulate = accumulate && (sqlite3_bind_text(this->m_db_statement, 7, i_link.c_str(), link_n_bytes, SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Model web link ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
      i_link.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  WrappedString i_description = WrappedString(model.getDescription());
  int description_n_bytes = i_description.n_bytes();
  accumulate = accumulate && (sqlite3_bind_text(this->m_db_statement, 8, i_description.c_str(), description_n_bytes, SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Model description ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
      i_description.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  WrappedString i_cover_big = WrappedString(model.getCoverBig());
  int cover_big_n_bytes = i_cover_big.n_bytes();
  accumulate = accumulate && (sqlite3_bind_text(this->m_db_statement, 9, i_cover_big.c_str(), cover_big_n_bytes, SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Model cover big ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
      i_cover_big.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  WrappedString i_cover_small = WrappedString(model.getCoverSmall());
  int cover_small_n_bytes = i_cover_small.n_bytes();
  accumulate = accumulate && (sqlite3_bind_text(this->m_db_statement, 10, i_cover_small.c_str(), cover_small_n_bytes, SQLITE_TRANSIENT) == SQLITE_OK);
  DBG("Model cover small ["%s"] has been stored in table ["%s"], SQLite database ["%s"].",
      i_cover_small.c_str(), this->m_table_name.c_str(), this->m_db_name.c_str());

  if (sqlite3_step(this->m_db_statement) != SQLITE_DONE) { ERR("Error addModel"); }
  if (!accumulate) {
    ERR("Error during saving data into table ["%s"], database ["%s"] by statement ["%s"]!",
        this->m_table_name.c_str(), this->m_db_name.c_str(), insert_statement.c_str());
    this->__finalize_and_throw__(insert_statement.c_str(), SQLITE_ACCUMULATED_PREPARE_ERROR);
  } else {
    DBG("All insertions have succeeded.");
  }

  this->__finalize__(insert_statement.c_str());
  this->__increment_rows__();
  INF("exit ModelsDatabase::addModel().");
  return i_row_id;
}

// ----------------------------------------------
void ModelsDatabase::removeModel(int64_t id) {
  INF("enter ModelsDatabase::removeModel(%li).", id);
  std::string delete_statement = "DELETE FROM '";
  delete_statement += this->m_table_name;
  delete_statement += "' WHERE ID == '";
  delete_statement += std::to_string(id);
  delete_statement += "';";
  this->__prepare_statement__(delete_statement);
  if (sqlite3_step(this->m_db_statement) != SQLITE_DONE) { ERR("Error removeModel"); }
  this->__finalize__(delete_statement.c_str());
  this->__decrement_rows__();
  if (id + 1 == this->m_next_id) {
    int64_t last_row_id = this->__read_last_id__(this->m_table_name);
    this->m_next_id = last_row_id + 1;
    DBG("Deleted model with largest ID. Next ID value is set to [%li].", this->m_next_id);
  }
  if (this->__empty__()) {
    DBG("Table ["%s"] has become empty. Next ID value is set to zero.", this->m_table_name.c_str());
    this->m_next_id = BASE_ID;
  }
  DBG("Deleted model [ID: %li] in table ["%s"].", id, this->m_table_name.c_str());
  INF("exit ModelsDatabase::removeModel(%li).", id);
}

// ----------------------------------------------
Model ModelsDatabase::getModel(int64_t i_model_id) {
  INF("enter ModelsDatabase::getModel(%li).", i_model_id);
  std::string select_statement = "SELECT * FROM '";
  select_statement += this->m_table_name;
  select_statement += "' WHERE " D_COLUMN_NAME_MODEL_ID " == '";
  select_statement += std::to_string(i_model_id);
  select_statement += "';";

  this->__prepare_statement__(select_statement);
  if (sqlite3_step(this->m_db_statement) != SQLITE_DONE) { ERR("Error getModel"); }
  Model model = getModelFromStatement();
  //TABLE_ASSERT("Input model ID does not equal to primary key value from database!" && model.getId() == i_model_id);
  this->__finalize__(select_statement.c_str());
  INF("exit ModelsDatabase::getModel(%li).", i_model_id);
  return (model);
}

void ModelsDatabase::getModels(std::vector<Model>* const output) {
  INF("enter ModelsDatabase::getModels().");
  std::string select_statement = "SELECT * FROM '";
  select_statement += this->m_table_name;
  select_statement += "';";

  this->__prepare_statement__(select_statement);
  while (sqlite3_step(this->m_db_statement) == SQLITE_ROW) {
    Model model = getModelFromStatement();
    output->push_back(model);
  }
  this->__finalize__(select_statement.c_str());
  INF("exit ModelsDatabase::getModels().");
}

// ----------------------------------------------
SmallModel ModelsDatabase::getSmallModel(int64_t i_model_id) {
  INF("enter ModelsDatabase::getSmallModel(%li).", i_model_id);
  std::string select_statement = "SELECT " D_COLUMN_NAME_ROW_ID "," D_COLUMN_NAME_MODEL_ID "," D_COLUMN_NAME_MODEL_NAME "," D_COLUMN_NAME_MODEL_COVER_SMALL " FROM '";
  select_statement += this->m_table_name;
  select_statement += "' WHERE " D_COLUMN_NAME_MODEL_ID " == '";
  select_statement += std::to_string(i_model_id);
  select_statement += "';";

  this->__prepare_statement__(select_statement);
  if (sqlite3_step(this->m_db_statement) != SQLITE_DONE) { ERR("Error getSmallModel"); }
  SmallModel model = getSmallModelFromStatement();
  //TABLE_ASSERT("Input model ID does not equal to primary key value from database!" && model.getId() == i_model_id);
  this->__finalize__(select_statement.c_str());
  INF("exit ModelsDatabase::getSmallModel(%li).", i_model_id);
  return (model);
}

void ModelsDatabase::getSmallModels(std::vector<SmallModel>* const output) {
  INF("enter ModelsDatabase::getSmallModels().");
  std::string select_statement = "SELECT " D_COLUMN_NAME_ROW_ID "," D_COLUMN_NAME_MODEL_ID "," D_COLUMN_NAME_MODEL_NAME "," D_COLUMN_NAME_MODEL_COVER_SMALL " FROM '";
  select_statement += this->m_table_name;
  select_statement += "';";

  getSmallModelsFromStatement(output, select_statement);
  INF("exit ModelsDatabase::getSmallModels().");
}

void ModelsDatabase::getSmallModels(std::vector<SmallModel>* const output, int limit, int offset) {
  INF("enter ModelsDatabase::getSmallModels(%i, %i).", limit, offset);
  std::string select_statement = "SELECT " D_COLUMN_NAME_ROW_ID "," D_COLUMN_NAME_MODEL_ID "," D_COLUMN_NAME_MODEL_NAME "," D_COLUMN_NAME_MODEL_COVER_SMALL " FROM '";
  select_statement += this->m_table_name;
  select_statement += "' LIMIT ";
  select_statement += std::to_string(limit);
  select_statement += " OFFSET ";
  select_statement += std::to_string(offset);
  select_statement += ";";

  getSmallModelsFromStatement(output, select_statement);
  INF("exit ModelsDatabase::getSmallModels(%i, %i).", limit, offset);
}

void ModelsDatabase::getSmallModels(std::vector<SmallModel>* const output, int limit, int offset, const std::vector<std::string>& titles) {
  INF("enter ModelsDatabase::getSmallModels(%i, %i, titles).", limit, offset);
  std::ostringstream oss;
  const char* delim = "";
  for (auto& item : titles) {
    oss << delim << COLUMN_NAME_MODEL_GENRES << " LIKE '%" << item << "%'";
    delim = " OR ";
  }

  std::string select_statement = "SELECT " D_COLUMN_NAME_ROW_ID "," D_COLUMN_NAME_MODEL_ID "," D_COLUMN_NAME_MODEL_NAME "," D_COLUMN_NAME_MODEL_COVER_SMALL " FROM '";
  select_statement += this->m_table_name;
  select_statement += "' WHERE ";
  select_statement += oss.str();
  select_statement += " LIMIT ";
  select_statement += std::to_string(limit);
  select_statement += " OFFSET ";
  select_statement += std::to_string(offset);
  select_statement += ";";

  getSmallModelsFromStatement(output, select_statement);
  INF("exit ModelsDatabase::getSmallModels(%i, %i, titles).", limit, offset);
}

int ModelsDatabase::getTotalModels() {
  INF("enter ModelsDatabase::getTotalModels().");
  std::string count_statement = "SELECT COUNT(*) FROM '";
  count_statement += this->m_table_name;
  count_statement += "';";

  return getTotalModelsFromStatement(count_statement);
  INF("exit ModelsDatabase::getTotalModels().");
}

int ModelsDatabase::getTotalModels(const std::vector<std::string>& titles) {
  INF("enter ModelsDatabase::getTotalModels(titles).");
  std::ostringstream oss;
  const char* delim = "";
  for (auto& item : titles) {
    oss << delim << COLUMN_NAME_MODEL_GENRES << " LIKE '%" << item << "%'";
    delim = " OR ";
  }

  std::string count_statement = "SELECT COUNT(*) FROM '";
  count_statement += this->m_table_name;
  count_statement += "' WHERE ";
  count_statement += oss.str();
  count_statement += ";";

  return getTotalModelsFromStatement(count_statement);
  INF("exit ModelsDatabase::getTotalModels(titles).");
}

/* Private members */
// ----------------------------------------------------------------------------
void ModelsDatabase::__init__() {
  DBG("enter ModelsDatabase::__init__().");
  Database::__init__();
  int64_t last_row_id = this->__read_last_id__(this->m_table_name);
  this->m_next_id = last_row_id == 0 ? BASE_ID : last_row_id + 1;
  TRC("Initialization has completed: total rows [%i], last row id [%li], next_id [%li].",
      this->m_rows, last_row_id, this->m_next_id);
  DBG("exit ModelsDatabase::__init__().");
}

void ModelsDatabase::__create_table__() {
  DBG("enter ModelsDatabase::__create_table__().");
  std::string statement = "CREATE TABLE IF NOT EXISTS ";
  statement += this->m_table_name;
  statement += "('" D_COLUMN_NAME_ROW_ID "' INTEGER PRIMARY KEY UNIQUE DEFAULT 0, "
      "'" D_COLUMN_NAME_MODEL_ID "' INTEGER, "
      "'" D_COLUMN_NAME_MODEL_NAME "' TEXT, "
      "'" D_COLUMN_NAME_MODEL_GENRES "' TEXT, "
      "'" D_COLUMN_NAME_MODEL_TRACKS "' INTEGER, "
      "'" D_COLUMN_NAME_MODEL_ALBUMS "' INTEGER, "
      "'" D_COLUMN_NAME_MODEL_LINK "' TEXT, "
      "'" D_COLUMN_NAME_MODEL_DESCRIPTION "' TEXT, "
      "'" D_COLUMN_NAME_MODEL_COVER_BIG "' TEXT,"
      "'" D_COLUMN_NAME_MODEL_COVER_SMALL "' TEXT);";
  this->__prepare_statement__(statement);
  if (sqlite3_step(this->m_db_statement) != SQLITE_DONE) { ERR("Error during creating table!"); }
  DBG("Table ["%s"] has been successfully created.", this->m_table_name.c_str());
  this->__finalize__(statement.c_str());
  DBG("exit ModelsDatabase::__create_table__().");
}

Model ModelsDatabase::getModelFromStatement() {
  DBG("enter ModelsDatabase::getModelFromStatement().");
  int64_t row_id = sqlite3_column_int64(this->m_db_statement, 0);
  int64_t model_id = sqlite3_column_int64(this->m_db_statement, 1);

  Model model;
  if (row_id != 0) {
    const void* raw_name   = reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 2));  WrappedString name(raw_name);
    const void* raw_genres = reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 3));  WrappedString genres(raw_genres);
    int tracks_count = sqlite3_column_int(this->m_db_statement, 4);
    int albums_count = sqlite3_column_int(this->m_db_statement, 5);
    const void* raw_link        = reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 6));  WrappedString link(raw_link);
    const void* raw_description = reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 7));  WrappedString description(raw_description);
    const void* raw_cover_big   = reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 8));  WrappedString cover_big(raw_cover_big);
    const void* raw_cover_small = reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 9));  WrappedString cover_small(raw_cover_small);
    DBG("Loaded column data: " D_COLUMN_NAME_ROW_ID " [%li]; " D_COLUMN_NAME_MODEL_ID " [%li]; " D_COLUMN_NAME_MODEL_NAME " ["%s"]; " D_COLUMN_NAME_MODEL_GENRES " ["%s"]; " D_COLUMN_NAME_MODEL_TRACKS " [%i]; " D_COLUMN_NAME_MODEL_ALBUMS " [%i]; " D_COLUMN_NAME_MODEL_LINK " ["%s"]; " D_COLUMN_NAME_MODEL_DESCRIPTION " ["%s"]; " D_COLUMN_NAME_MODEL_COVER_BIG " ["%s"]; " D_COLUMN_NAME_MODEL_COVER_SMALL " ["%s"].",
         row_id, model_id, name.c_str(), genres.c_str(), tracks_count, albums_count, link.c_str(), description.c_str(), cover_big.c_str(), cover_small.c_str());

    // parse genres
    std::vector<std::string> genres_array;
    std::stringstream ss(genres.get());
    std::string item;
    while (std::getline(ss, item, ' ')) {
      genres_array.push_back(item);
      TRC("Parsed genre: %s", item.c_str());
    }

    model = Model::Builder(model_id, name.get())
        .setGenres(genres_array)
        .setTracksCount(tracks_count)
        .setAlbumsCount(albums_count)
        .setWebLink(link.get())
        .setDescription(description.get())
        .setCoverBig(cover_big.get())
        .setCoverSmall(cover_small.get())
        .build();
    DBG("Proper model instance has been constructed.");
  } else {
    WRN("Model ID [%li] is missing in table ["%s"] of database %p!",
        model_id, this->m_table_name.c_str(), this->m_db_handler);
  }

  DBG("exit ModelsDatabase::getModelFromStatement().");
  return (model);
}

SmallModel ModelsDatabase::getSmallModelFromStatement() {
  DBG("enter ModelsDatabase::getSmallModelFromStatement().");
  int64_t row_id = sqlite3_column_int64(this->m_db_statement, 0);
  int64_t model_id = sqlite3_column_int64(this->m_db_statement, 1);

  SmallModel model;
  if (row_id != 0) {
    const void* raw_name = reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 2));  WrappedString name(raw_name);
    const void* raw_cover_small = reinterpret_cast<const char*>(sqlite3_column_text(this->m_db_statement, 3));  WrappedString cover_small(raw_cover_small);
    DBG("Loaded column data: " D_COLUMN_NAME_ROW_ID " [%li]; " D_COLUMN_NAME_MODEL_ID " [%li]; " D_COLUMN_NAME_MODEL_NAME " ["%s"]; " D_COLUMN_NAME_MODEL_COVER_SMALL " ["%s"].",
        row_id, model_id, name.c_str(), cover_small.c_str());

    model = SmallModel::Builder(model_id, name.get()).setCoverSmall(cover_small.get()).build();
    DBG("Proper model instance has been constructed.");
  } else {
    WRN("Model ID [%li] is missing in table ["%s"] of database %p!",
        model_id, this->m_table_name.c_str(), this->m_db_handler);
  }

  DBG("exit ModelsDatabase::getSmallModelFromStatement().");
  return (model);
}

void ModelsDatabase::getSmallModelsFromStatement(std::vector<SmallModel>* const output, const std::string& select_statement) {
  DBG("enter ModelsDatabase::getSmallModelsFromStatement().");
  this->__prepare_statement__(select_statement);
  while (sqlite3_step(this->m_db_statement) == SQLITE_ROW) {
    SmallModel model = getSmallModelFromStatement();
    output->push_back(model);
  }
  this->__finalize__(select_statement.c_str());
  DBG("exit ModelsDatabase::getSmallModelsFromStatement().");
}

int ModelsDatabase::getTotalModelsFromStatement(const std::string& count_statement) {
  DBG("enter ModelsDatabase::getTotalModelsFromStatement().");
  int total = -1;
  this->__prepare_statement__(count_statement);
  if (sqlite3_step(this->m_db_statement) == SQLITE_ROW) {
    total = sqlite3_column_int(this->m_db_statement, 0);
    DBG("Total models: %i", total);
  } else {
    ERR("Error counting total models!");
  }
  DBG("exit ModelsDatabase::getTotalModelsFromStatement().");
  return (total);
}

}  // namespace db

