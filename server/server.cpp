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
#include "server.h"
#include "server_api_impl.h"
#include "server_menu.h"

const char* STANDARD_HEADERS = "Server: MusicSquareServer-" D_VERSION;
const char* CONTENT_TYPE_HEADER = "Content-Type: application/json";
const char* CONTENT_LENGTH_HEADER = "Content-Length: ";
const char* CONNECTION_CLOSE_HEADER = "Connection: close";

/* Server */
// ----------------------------------------------------------------------------
Server::Server(int port)
  : m_is_stopped(false)
  , m_api_impl(nullptr)
  , m_socket(socket(PF_INET, SOCK_STREAM, 0)) {  // create an endpoint for communication

  if (m_socket < 0) {
    ERR("Failed to open socket");
    throw ServerException();
  }
  memset(&m_address_structure, 0, sizeof(sockaddr_in));
  m_address_structure.sin_family = AF_INET;
  m_address_structure.sin_addr.s_addr = htonl(INADDR_ANY);
  m_address_structure.sin_port = htons(port);

  if (bind(m_socket, reinterpret_cast<sockaddr*>(&m_address_structure), sizeof(m_address_structure)) < 0) {
    ERR("Failed to bind socket to the address");
    throw ServerException();
  }

  // set the "LINGER" timeout to zero, to close the listen socket
  // immediately at program termination.
  linger linger_opt = { 1, 0 };  // Linger active, timeout 0
  setsockopt(m_socket, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

  listen(m_socket, 20);  // listen for incoming connections, up to 20 pending connection in a queue

  // utility table
  m_methods["GET"]    = Method::GET;
  m_methods["POST"]   = Method::POST;
  m_methods["PUT"]    = Method::PUT;
  m_methods["DELETE"] = Method::DELETE;

  m_paths[PATH_ALL]    = Path::ALL;
  m_paths[PATH_GENRES] = Path::GENRES;
  m_paths[PATH_SINGLE] = Path::SINGLE;

  m_api_impl = new ServerApiImpl();
}

Server::~Server() {
  if (!m_is_stopped) {
    stop();
  }

  delete m_api_impl;  m_api_impl = nullptr;
}

void Server::run() {
  std::thread t(&Server::runListener, this);
  t.detach();

  menu::printHelp();

  // evaluate user commands
  std::string command;
  do {
    menu::printPrompt();
    std::getline(std::cin, command);
  } while (menu::evaluate(this, command));
}

void Server::stop() {
  m_is_stopped = true;
  close(m_socket);
}

// ----------------------------------------------------------------------------
void Server::runListener() {
  while (!m_is_stopped) {  // server loop
    sockaddr_in client_address_structure;
    socklen_t client_address_structure_size = sizeof(client_address_structure);

    // accept one pending connection, waits until a new connection comes
    int peer_socket = accept(m_socket, reinterpret_cast<sockaddr*>(&client_address_structure), &client_address_structure_size);
    if (peer_socket < 0) {
      ERR("Failed to open new socket for data transfer");
      continue;  // skip failed connection
    }

    printClientInfo(client_address_structure);

    // get incoming message
    std::thread t(&Server::handleRequest, this, peer_socket);
    t.detach();
  }
}

/* Process request */
// ----------------------------------------------
Request Server::getRequest(int socket, bool* is_closed) {
  char buffer[MESSAGE_SIZE];
  memset(buffer, 0, MESSAGE_SIZE);
  int read_bytes = recv(socket, buffer, MESSAGE_SIZE, 0);
  if (read_bytes <= 0) {
    if (read_bytes == -1) {
      ERR("getRequest() error: %s", strerror(errno));
    }
    DBG("Connection closed");
    *is_closed = true;
    return Request::EMPTY;
  }
  try {
    DBG("Raw request[%i bytes]: %.*s", read_bytes, (int) read_bytes, buffer);
    return m_parser.parseRequest(buffer, read_bytes);
  } catch (ParseException exception) {
    FAT("ParseException on raw request[%i bytes]: %.*s", read_bytes, (int) read_bytes, buffer);
    return Request::EMPTY;
  }
}

Method Server::getMethod(const std::string& method) const {
  auto it = m_methods.find(method);
  if (it != m_methods.end()) {
    return it->second;
  }
  return Method::UNKNOWN;
}

Path Server::getPath(const std::string& path) const {
  int i1 = path.find_first_of('?');
  std::string path_no_params = path.substr(0, i1);
  auto it = m_paths.find(path_no_params);
  if (it != m_paths.end()) {
    return it->second;
  }
  return Path::UNKNOWN;
}

void Server::handleRequest(int socket) {
  while (!m_is_stopped) {
    bool is_closed = false;
    Request request = getRequest(socket, &is_closed);
    if (is_closed) {
      DBG("Stopping peer thread...");
      close(socket);
      return;
    }

    if (request == Request::EMPTY) {
      ERR("Empty request - ignored");
      continue;
    }

    Method method = getMethod(request.startline.method);
    if (method == Method::UNKNOWN) {
      ERR("Invalid method: %s", request.startline.method.c_str());
      continue;
    }

    Path path = getPath(request.startline.path);
    if (path == Path::UNKNOWN) {
      ERR("Invalid path: %s", request.startline.path.c_str());
      continue;
    }
    VER("Method: %i, Path: %i", static_cast<int>(method), static_cast<int>(path));

    switch (path) {
      case Path::ALL:
        switch (method) {
          case Method::GET:
            {
              INF("Get all");
              std::vector<SmallModel> models;
              m_api_impl->getModels(&models);
              sendModels(socket, models);
            }
            break;
        }
        break;
      case Path::GENRES:
        switch (method) {
          case Method::GET:
            {
              INF("Get genres");
              std::vector<std::string> genres;
              m_api_impl->getTitles(&genres);
              sendTitles(socket, genres);
            }
            break;
        }
        break;
      case Path::SINGLE:
        switch (method) {
          case Method::GET:
            {
              INF("Get single");
              int64_t id = parseId(request.startline.path.c_str());
              Model model = m_api_impl->getModel(id);
              if (model.isEmpty()) {
                sendError(socket, 404, "Model not found");
              } else {
                sendModel(socket, model);
              }
            }
            break;
        }
        break;
    }
  }
}

/* Send response */
// ----------------------------------------------
void Server::sendToSocket(int socket, const std::string& data) const {
  std::ostringstream oss;
  oss << "HTTP/1.1 200 OK\r\n"
      << STANDARD_HEADERS << "\r\n"
      << CONTENT_TYPE_HEADER << "\r\n"
      << CONTENT_LENGTH_HEADER << data.length() << "\r\n\r\n"
      << data << "\0";
  MSG("Response: %s", oss.str().c_str());
  send(socket, oss.str().c_str(), oss.str().length(), 0);
}

void Server::sendError(int socket, int error_code, const std::string& message) const {
  std::ostringstream oss;
  oss << "HTTP/1.1 " << error_code << " " << message <<"\r\n"
      << STANDARD_HEADERS << "\r\n\r\n\0";
  MSG("Response: %s", oss.str().c_str());
  send(socket, oss.str().c_str(), oss.str().length(), 0);
}

void Server::sendModels(int socket, const std::vector<SmallModel>& models) const {
  std::vector<const common::Jsonable*> ptrs;
  ptrs.reserve(models.size());
  for (auto& item : models) {
    ptrs.push_back(&item);
    VER("Send small model: %s", item.toString().c_str());
  }
  std::string json = common::arrayToJson(ptrs);
  sendToSocket(socket, json);
}

void Server::sendModel(int socket, const Model& model) const {
  std::string json = model.toJson();
  VER("Send model: %s", model.toString().c_str());
  sendToSocket(socket, json);
}

void Server::sendTitles(int socket, const std::vector<std::string>& titles) const {
  const char* delim = "";
  std::ostringstream json;
  json << "[";
  for (auto& item : titles) {
    json << delim << item;
    delim = ",";
  }
  json << "]";
  VER("Send titles: %s", json.str().c_str());
  sendToSocket(socket, json.str());
}

/* Utility */
// ----------------------------------------------
int64_t Server::parseId(const std::string& path) {
  std::vector<Query> params;
  m_parser.parsePath(path, &params);
  for (auto& query : params) {
    DBG("Query: %s: %s", query.key.c_str(), query.value.c_str());
  }
  if (params.size() < 1 || params[0].key.compare(ITEM_ID) != 0) {
    ERR("Wrong query params: %s", path.c_str());
    return 0;
  }
  return std::stoll(params[0].value.c_str());
}

void Server::printClientInfo(sockaddr_in& peeraddr) {
  INF("Connection from IP %d.%d.%d.%d, port %d",
        (ntohl(peeraddr.sin_addr.s_addr) >> 24) & 0xff, // High byte of address
        (ntohl(peeraddr.sin_addr.s_addr) >> 16) & 0xff, // . . .
        (ntohl(peeraddr.sin_addr.s_addr) >> 8) & 0xff,  // . . .
        ntohl(peeraddr.sin_addr.s_addr) & 0xff,         // Low byte of address
        ntohs(peeraddr.sin_port));
}

/* Main */
// ----------------------------------------------------------------------------
int main(int argc, char** argv) {
  int port = 9123;
  if (argc > 1) {
    port = std::atoi(argv[1]);
  }
  Server server(port);
  server.run();
  return 0;
}

