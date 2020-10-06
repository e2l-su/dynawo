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

#include "DYNBusInterfaceIIDM.h"
#include "DYNSwitchInterfaceIIDM.h"

#include "gtest_dynawo.h"

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>

using powsybl::iidm::Network;
using powsybl::iidm::Substation;
using powsybl::iidm::TopologyKind;
using powsybl::iidm::VoltageLevel;

namespace DYN {

TEST(DataInterfaceTest, testSwitchInterface_1) {
  Network network("test", "test");

  Substation& s = network.newSubstation()
                       .setId("S")
                       .add();

  VoltageLevel& vl1 = s.newVoltageLevel()
                       .setId("VL1")
                       .setNominalVoltage(400.)
                       .setTopologyKind(TopologyKind::BUS_BREAKER)
                       .setHighVoltageLimit(420.)
                       .setLowVoltageLimit(380.)
                       .add();

  auto swAdder = vl1.getBusBreakerView().newSwitch()
                       .setId("Sw")
                       .setName("SwName")
                       .setFictitious(false)
                       .setOpen(false);

  powsybl::iidm::Bus& b1 = vl1.getBusBreakerView().newBus().setId("BUS1").add();
  powsybl::iidm::Bus& b2 = vl1.getBusBreakerView().newBus().setId("BUS2").add();
  swAdder.setBus1("BUS1");
  swAdder.setBus2("BUS2");

  powsybl::iidm::Switch& aSwitch = swAdder.add();
  ASSERT_EQ(aSwitch.getId(), "Sw");

  SwitchInterfaceIIDM sw(aSwitch);
  ASSERT_EQ(sw.getID(), "Sw");
  ASSERT_EQ(SwitchInterfaceIIDM::VAR_STATE, 0);
  ASSERT_FALSE(sw.isOpen());

  const boost::shared_ptr<BusInterface> x_b1(new BusInterfaceIIDM(b1));
  const boost::shared_ptr<BusInterface> x_b2(new BusInterfaceIIDM(b2));
  sw.setBusInterface1(x_b1);
  sw.setBusInterface2(x_b2);
  ASSERT_EQ(sw.getBusInterface1()->getID(), "BUS1");
  ASSERT_EQ(sw.getBusInterface2()->getID(), "BUS2");

  sw.close();
  ASSERT_FALSE(sw.isOpen());
  sw.open();
  ASSERT_TRUE(sw.isOpen());
  sw.close();
  ASSERT_FALSE(sw.isOpen());

  ASSERT_EQ(sw.getComponentVarIndex("state"), 0);
  ASSERT_EQ(sw.getComponentVarIndex("others"), -1);

  sw.importStaticParameters();
  sw.exportStateVariablesUnitComponent();

}  // TEST(DataInterfaceTest, testSwitchInterface_1)

TEST(DataInterfaceTest, testSwitchInterface_2) {
  Network network("test", "test");

  Substation& s = network.newSubstation()
                       .setId("S")
                       .add();

  VoltageLevel& vl1 = s.newVoltageLevel()
                       .setId("VL1")
                       .setNominalVoltage(400.)
                       .setTopologyKind(TopologyKind::NODE_BREAKER)
                       .setNominalVoltage(225.0)
                       .setLowVoltageLimit(0.0)
                       .setHighVoltageLimit(250.0)
                       .add();

  auto swAdder1 = vl1.getNodeBreakerView().newSwitch()
                       .setId("BREAKER4")
                       .setName("NameOfOneSwitch")
                       .setNode1(0)
                       .setNode2(1)
                       .setKind(powsybl::iidm::SwitchKind::BREAKER);

  powsybl::iidm::Switch& aSwitch = swAdder1.add();
  ASSERT_EQ(aSwitch.getId(), "BREAKER4");

  auto swAdder2 = vl1.getNodeBreakerView().newSwitch()
                       .setId("DISCONNECTOR7")
                       .setNode1(6)
                       .setNode2(7)
                       .setKind(powsybl::iidm::SwitchKind::DISCONNECTOR);
  swAdder2.add();

  const VoltageLevel& cVl = vl1;
  ASSERT_EQ(2UL, cVl.getSwitchCount());

}  // TEST(DataInterfaceTest, testSwitchInterface_2)

}  // namespace DYN
