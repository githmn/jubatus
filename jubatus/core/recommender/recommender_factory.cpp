// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include <string>
#include <pficommon/data/string/utility.h>
#include <pficommon/lang/shared_ptr.h>
#include <pficommon/text/json.h>
#include "../common/exception.hpp"
#include "../common/jsonconfig.hpp"
#include "../nearest_neighbor/nearest_neighbor_factory.hpp"
#include "../storage/norm_factory.hpp"
#include "../table/column/column_table.hpp"
#include "recommender_factory.hpp"
#include "recommender.hpp"

using std::string;
using pfi::text::json::json;
using jubatus::core::jsonconfig::config;
using jubatus::core::jsonconfig::config_cast_check;

namespace jubatus {
namespace core {
namespace recommender {
namespace {

const std::string NEAREST_NEIGHBOR_PREFIX("nearest_neighbor_recommender:");

}  // namespace

recommender_base* recommender_factory::create_recommender(
    const string& name,
    const config& param,
    const string& id) {
  if (name == "inverted_index") {
    // inverted_index doesn't have parameter
    return new inverted_index;
  } else if (name == "minhash") {
    return new minhash(config_cast_check<minhash::config>(param));
  } else if (name == "lsh") {
    return new lsh(config_cast_check<lsh::config>(param));
  } else if (name == "euclid_lsh") {
    return new euclid_lsh(config_cast_check<euclid_lsh::config>(param));
  } else if (pfi::data::string::starts_with(name, NEAREST_NEIGHBOR_PREFIX)) {
    const std::string nearest_neighbor_method =
        name.substr(NEAREST_NEIGHBOR_PREFIX.size());
    pfi::lang::shared_ptr<table::column_table> table(new table::column_table);
    pfi::lang::shared_ptr<nearest_neighbor::nearest_neighbor_base>
        nearest_neighbor_engine(nearest_neighbor::create_nearest_neighbor(
            nearest_neighbor_method, param, table, id));
    return new nearest_neighbor_recommender(nearest_neighbor_engine);
  } else {
    throw JUBATUS_EXCEPTION(unsupported_method(name));
  }
}

}  // namespace recommender
}  // namespace core
}  // namespace jubatus
