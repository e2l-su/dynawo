//
// Copyright (c) 2015-2020, RTE (http://www.rte-france.com)
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

#include "DYNInjectorInterfaceIIDM.h"

#include "DYNBusInterfaceIIDM.h"
#include "DYNLoadInterfaceIIDM.h"
#include "DYNVoltageLevelInterfaceIIDM.h"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/LoadAdder.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/Substation.hpp>

#include "gtest_dynawo.h"

namespace DYN {

TEST(DataInterfaceTest, Injector) {
  using powsybl::iidm::Bus;
  using powsybl::iidm::Load;
  using powsybl::iidm::LoadType;
  using powsybl::iidm::Network;
  using powsybl::iidm::Substation;
  using powsybl::iidm::TopologyKind;
  using powsybl::iidm::VoltageLevel;

  Network network("test", "test");
  Substation& substation = network.newSubstation()
                               .setId("S1")
                               .setName("S1_NAME")
                               .setCountry(powsybl::iidm::Country::FR)
                               .setTso("TSO")
                               .add();

  VoltageLevel& vl1 = substation.newVoltageLevel()
                          .setId("VL1")
                          .setName("VL1_NAME")
                          .setTopologyKind(TopologyKind::BUS_BREAKER)
                          .setNominalVoltage(380.0)
                          .setLowVoltageLimit(340.0)
                          .setHighVoltageLimit(420.0)
                          .add();

  Bus& bus1 = vl1.getBusBreakerView().newBus().setId("VL1_BUS1").add();

  vl1.newLoad()
      .setId("LOAD1")
      .setBus("VL1_BUS1")
      .setConnectableBus("VL1_BUS1")
      .setName("LOAD1_NAME")
      .setLoadType(LoadType::UNDEFINED)
      .setP0(50.0)
      .setQ0(40.0)
      .add();

  substation.newVoltageLevel()
      .setId("VL2")
      .setName("VL2_NAME")
      .setTopologyKind(TopologyKind::NODE_BREAKER)
      .setNominalVoltage(225.0)
      .setLowVoltageLimit(200.0)
      .setHighVoltageLimit(260.0)
      .add();

  Load& load = network.getLoad("LOAD1");
  ASSERT_EQ("LOAD1", load.getId());
  ASSERT_EQ("LOAD1_NAME", load.getOptionalName());
  ASSERT_EQ(powsybl::iidm::ConnectableType::LOAD, load.getType());
  ASSERT_EQ(LoadType::UNDEFINED, load.getLoadType());

  LoadInterfaceIIDM loadIfce(load);

  ASSERT_EQ(loadIfce.getComponentVarIndex(std::string("p")), LoadInterfaceIIDM::VAR_P);
  ASSERT_EQ(loadIfce.getComponentVarIndex(std::string("P1")), -1);
  ASSERT_EQ(loadIfce.getComponentVarIndex(std::string("q")), LoadInterfaceIIDM::VAR_Q);
  ASSERT_EQ(loadIfce.getComponentVarIndex(std::string("state")), LoadInterfaceIIDM::VAR_STATE);

  ASSERT_EQ(loadIfce.getID(), load.getId());
  ASSERT_DOUBLE_EQ(loadIfce.getPUnderVoltage(), 0.0);

  ASSERT_TRUE(loadIfce.getInitialConnected());
  load.getTerminal().disconnect();
  ASSERT_TRUE(loadIfce.getInitialConnected());
  {
    LoadInterfaceIIDM connectedLoadIfce(load);
    ASSERT_FALSE(connectedLoadIfce.getInitialConnected());
    load.getTerminal().connect();
    ASSERT_FALSE(connectedLoadIfce.getInitialConnected());
    ASSERT_DOUBLE_EQ(connectedLoadIfce.getP(), 0.0);
    ASSERT_DOUBLE_EQ(connectedLoadIfce.getQ(), 0.0);
  }

  ASSERT_EQ(loadIfce.getBusInterface().get(), nullptr);
  const boost::shared_ptr<BusInterface> busIfce(new BusInterfaceIIDM(bus1));
  loadIfce.setBusInterface(busIfce);
  ASSERT_EQ(loadIfce.getBusInterface().get()->getID(), "VL1_BUS1");

  ASSERT_DOUBLE_EQ(loadIfce.getP0(), 50.0);
  ASSERT_DOUBLE_EQ(loadIfce.getP(), 0.0);
  load.getTerminal().setP(1000.0);
  ASSERT_DOUBLE_EQ(loadIfce.getP(), 1000.0);

  ASSERT_DOUBLE_EQ(loadIfce.getQ0(), 40.0);
  ASSERT_DOUBLE_EQ(loadIfce.getQ(), 0.0);
  load.getTerminal().setQ(499.0);
  ASSERT_DOUBLE_EQ(loadIfce.getQ(), 499.0);

  // Manque le test de setVoltageLevelInterface DG - FAIRE
  // loadIfce.getVoltageLevelInterface() ;

}  // TEST(DataInterfaceTest, Injector)
}  // namespace DYN
