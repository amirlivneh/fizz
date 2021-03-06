/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <folly/ExceptionWrapper.h>
#include <folly/io/IOBuf.h>
#include <folly/io/async/AsyncTransport.h>

namespace fizz {

/**
 * Application data to deliver to the application.
 */
struct DeliverAppData {
  std::unique_ptr<folly::IOBuf> data;
};

/**
 * Raw data that must be written to the transport.
 */
struct WriteToSocket {
  folly::AsyncTransportWrapper::WriteCallback* callback{nullptr};
  std::unique_ptr<folly::IOBuf> data;
  folly::WriteFlags flags{folly::WriteFlags::NONE};
};

/**
 * Reports that a fatal error has occured on the connection.
 */
struct ReportError {
  folly::exception_wrapper error;

  explicit ReportError(const std::string& errorMsg)
      : error(folly::make_exception_wrapper<std::runtime_error>(errorMsg)) {}

  explicit ReportError(folly::exception_wrapper e) : error(std::move(e)) {}
};

/**
 * Reports that more data is needed to progress. waitForData() should be
 * called.
 */
struct WaitForData {};

namespace detail {

template <typename ActionsType>
void addAction(ActionsType& /*acts*/) {}

template <typename ActionsType, typename ActionType, typename... Args>
void addAction(ActionsType& acts, ActionType&& thisAct, Args&&... act) {
  acts.emplace_back(std::forward<ActionType>(thisAct));
  addAction(acts, std::forward<Args>(act)...);
}
} // namespace detail
} // namespace fizz
