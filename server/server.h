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

#ifndef MSQ_SERVER__H__
#define MSQ_SERVER__H__

#include <unordered_map>
#include <vector>
#include "all.h"
#include "common.h"
#include "parser/my_parser.h"
#include "server_api.h"

#define MESSAGE_SIZE 256

extern const char* STANDARD_HEADERS;
extern const char* CONTENT_TYPE_HEADER;
extern const char* CONTENT_LENGTH_HEADER;
extern const char* CONNECTION_CLOSE_HEADER;

class Server {
public:
  Server(int port);
  ~Server();

  void run();
  void stop();

private:
  bool m_is_stopped;
  IServerApi* m_api_impl;
  int m_socket;  // file descriptor to refer to that endpoint
  sockaddr_in m_address_structure;  // structure to handle connection addresses
  MyParser m_parser;
  std::unordered_map<std::string, Method> m_methods;
  std::unordered_map<std::string, Path> m_paths;

  void runListener();
  Request getRequest(int socket, bool* is_closed, std::vector<Request>* requests);
  Method getMethod(const std::string& method) const;
  Path getPath(const std::string& path) const;
  void handleRequest(int socket);

  void sendToSocket(int socket, const std::string& data) const;
  void sendError(int socket, int error_code, const std::string& message) const;
  void sendIntValue(int socket, int value) const;
  void sendGenre(int socket, const Genre& genre) const;
  void sendModel(int socket, const Model& model) const;
  void sendGenres(int socket, const std::vector<Genre>& genres) const;
  void sendModels(int socket, const std::vector<SmallModel>& models) const;

  int64_t parseId(const std::string& path);
  std::string parseName(const std::string& path);
  void parseGenresFromQuery(const Query& query, std::vector<std::string>* genres);
  void parseParamsGenres(const std::string& path, std::vector<std::string>* genres);
  void parseParamsForAll(const std::string& path, int* limit, int* offset, std::vector<std::string>* genres);
  void printClientInfo(sockaddr_in& peeraddr);
};

#endif  // MSQ_SERVER__H__

