//
// Copyright (c) 2015-2019, RTE (http://www.rte-france.com)
// See AUTHORS.txt
// All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, you can obtain one at http://mozilla.org/MPL/2.0/.
// SPDX-License-Identifier: MPL-2.0
//
// This file is part of Dynawo, an hybrid C++/Modelica open source time domain
// simulation tool for power systems.
//

/**
 * @file JOBLineariseEntry.h
 * @brief Linearise entries description : interface file
 *
 */

#ifndef API_JOB_JOBLINEARISEENTRY_H_
#define API_JOB_JOBLINEARISEENTRY_H_

#include <string>

#include "JOBExport.h"

namespace job {

/**
 * @class LineariseEntry
 * @brief Linearise entries container class
 */
class __DYNAWO_JOB_EXPORT LineariseEntry {
 public:
  /**
   * @brief Destructor
   */
  virtual ~LineariseEntry() {}

  /**
   * @brief Start time setter
   * @param startTime : Start time of the linearization
   */
  virtual void setLineariseTime(const double & lineariseTime) = 0;

  /**
   * @brief Start time getter
   * @return to retrieve time of the linearisation
   */
  virtual double getLineariseTime() const = 0;

  /**
   * @brief Output file attribute setter
   * @param outputFile: Output file for Linearise
   */
  virtual void setOutputFile(const std::string & outputFile) = 0;

  /**
   * @brief Export Mode attribute setter
   * @param exportMode: Export mode for Linearise
   */
  // virtual void setExportMode(const std::string & exportMode) = 0;

  /**
   * @brief Output file attribute getter
   * @return Output file for Linearise
   */
  virtual std::string getOutputFile() const = 0;

  /**
   * @brief Export mode attribute getter
   * @return Export mode for Linearise
   */
  // virtual std::string getExportMode() const = 0;

  class Impl;  ///< implemented class
};

}  // namespace job

#endif  // API_JOB_JOBLINEARISEENTRY_H_
