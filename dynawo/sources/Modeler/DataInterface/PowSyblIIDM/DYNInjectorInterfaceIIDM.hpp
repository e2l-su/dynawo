//
// Copyright (c) 2015-2020, RTE (http://www.rte-france.com)
// See AUTHORS.txt
// All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, you can obtain one at http://mozilla.org/MPL/2.0/.
// SPDX-License-Identifier: MPL-2.0
//
// This file is part of Dynawo, an hybrid C++/Modelica open source time domain simulation tool for power systems.
//

//======================================================================
/**
 * @file  DYNInjectorInterfaceIIDM.hpp
 *
 * @brief Injector interface : implementation file for IIDM interface
 *
 */
//======================================================================

#ifndef MODELER_DATAINTERFACE_POWSYBLIIDM_DYNINJECTORINTERFACEIIDM_HPP_
#define MODELER_DATAINTERFACE_POWSYBLIIDM_DYNINJECTORINTERFACEIIDM_HPP_

// included in DYNInjectorInterfaceIIDM.h
#include "DYNInjectorInterfaceIIDM.h"

#include "DYNBusInterface.h"
#include "DYNVoltageLevelInterface.h"

#include "DYNModelConstants.h"
#include "DYNTrace.h"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

namespace DYN {

template <class T>
InjectorInterfaceIIDM<T>::~InjectorInterfaceIIDM() {}

template <class T>
InjectorInterfaceIIDM<T>::InjectorInterfaceIIDM(powsybl::iidm::Injection& injector, std::string id) : injectorIIDM_(injector),
                                                                                                      injectorId_(id),
                                                                                                      initialConnected_(boost::none) {}

template <class T>
void
InjectorInterfaceIIDM<T>::setBusInterface(const boost::shared_ptr<BusInterface>& busInterface) {
  busInterface_ = busInterface;
}

template <class T>
boost::shared_ptr<BusInterface>
InjectorInterfaceIIDM<T>::getBusInterface() const {
  return busInterface_;
}

template <class T>
void
InjectorInterfaceIIDM<T>::setVoltageLevelInterface(const boost::shared_ptr<VoltageLevelInterface>& voltageLevelInterface) {
  voltageLevelInterface_ = voltageLevelInterface;
}

template <class T>
boost::shared_ptr<VoltageLevelInterface>
InjectorInterfaceIIDM<T>::getVoltageLevelInterface() const {
  boost::shared_ptr<VoltageLevelInterface> voltageLevel = voltageLevelInterface_.lock();
  assert(voltageLevel && "shared_ptr for voltage level is empty");
  return voltageLevel;
}

template <class T>
bool
InjectorInterfaceIIDM<T>::getInitialConnected() {
  if (initialConnected_ == boost::none) {
    initialConnected_ = injectorIIDM_.getTerminal().isConnected();
  }
  return initialConnected_.value();
}

template <class T>
double
InjectorInterfaceIIDM<T>::getVNom() const {
  return getVoltageLevelInterface()->getVNom();
}

template <class T>
bool
InjectorInterfaceIIDM<T>::hasP() {
  return !std::isnan(injectorIIDM_.getTerminal().getP());
}

template <class T>
bool
InjectorInterfaceIIDM<T>::hasQ() {
  return !std::isnan(injectorIIDM_.getTerminal().getQ());
}

template <class T>
double
InjectorInterfaceIIDM<T>::getP() {
  if (getInitialConnected()) {
    if (!hasP()) {
      Trace::warn("DATAINTERFACE") << DYNLog(VariableNotSet, "Injection", getID(), "P") << Trace::endline;
      return 0.;
    }
    return injectorIIDM_.getTerminal().getP();
  } else {
    return 0.;
  }
}

template <class T>
double
InjectorInterfaceIIDM<T>::getQ() {
  if (getInitialConnected()) {
    if (!hasQ()) {
      Trace::warn("DATAINTERFACE") << DYNLog(VariableNotSet, "Injection", getID(), "Q") << Trace::endline;
      return 0.;
    }
    return injectorIIDM_.getTerminal().getQ();
  } else {
    return 0.;
  }
}

template <class T>
std::string
InjectorInterfaceIIDM<T>::getID() const {
  return injectorId_;
}

}  // namespace DYN

#endif  // MODELER_DATAINTERFACE_POWSYBLIIDM_DYNINJECTORINTERFACEIIDM_HPP_
