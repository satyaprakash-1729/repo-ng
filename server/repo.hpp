/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014,  Regents of the University of California.
 *
 * This file is part of NDN repo-ng (Next generation of NDN repository).
 * See AUTHORS.md for complete list of repo-ng authors and contributors.
 *
 * repo-ng is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * repo-ng is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * repo-ng, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef REPO_SERVER_REPO_HPP
#define REPO_SERVER_REPO_HPP

#include "../storage/storage-handle.hpp"
#include "../storage/sqlite/sqlite-handle.hpp"
#include "../ndn-handle/read-handle.hpp"
#include "../ndn-handle/write-handle.hpp"
#include "../ndn-handle/delete-handle.hpp"
#include "../ndn-handle/tcp-bulk-insert-handle.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/util/command-interest-validator.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

namespace repo {

using std::string;
using std::vector;
using std::pair;

struct RepoConfig
{
  //StorageMethod storageMethod; This will be implemtented if there is other method.
  std::string dbPath;
  vector<ndn::Name> dataPrefixes;
  vector<ndn::Name> repoPrefixes;
  vector<pair<string, string> > tcpBulkInsertEndpoints;

  //@todo validator should be configured in config file
  boost::property_tree::ptree validatorNode;
};

RepoConfig
parseConfig(const std::string& confPath);

class Repo : noncopyable
{

public:
  class Error : public std::runtime_error
  {
  public:
    explicit
    Error(const std::string& what)
      : std::runtime_error(what)
    {
    }
  };

public:
  Repo(boost::asio::io_service& ioService, const RepoConfig& config);

  void
  enableListening();

private:
  static shared_ptr<StorageHandle>
  openStorage(const RepoConfig& config);

private:
  RepoConfig m_config;
  ndn::Scheduler m_scheduler;
  ndn::Face m_face;
  shared_ptr<StorageHandle> m_storageHandle;
  KeyChain m_keyChain;
  CommandInterestValidator m_validator;
  ReadHandle m_readHandle;
  WriteHandle m_writeHandle;
  DeleteHandle m_deleteHandle;
  TcpBulkInsertHandle m_tcpBulkInsertHandle;
};

} // namespace repo

#endif // REPO_SERVER_REPO_HPP