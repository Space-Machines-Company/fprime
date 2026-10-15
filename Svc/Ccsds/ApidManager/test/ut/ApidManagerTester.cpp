// ======================================================================
// \title  ApidManagerTester.cpp
// \author thomas-bc
// \brief  cpp file for ApidManager component test harness implementation class
// ======================================================================

#include "ApidManagerTester.hpp"

namespace Svc {

namespace Ccsds {

// ----------------------------------------------------------------------
// Construction and destruction
// ----------------------------------------------------------------------

ApidManagerTester::ApidManagerTester()
    : ApidManagerGTestBase("ApidManagerTester", ApidManagerTester::MAX_HISTORY_SIZE), component("ApidManager") {
    this->initComponents();
    this->connectPorts();
}

ApidManagerTester::~ApidManagerTester() {}

void ApidManagerTester::checkBothDirectionsCountApart() {
    const ComCfg::Apid::T apid = ComCfg::Apid::FW_PACKET_FILE;
    this->clearHistory();
    (void)this->invoke_to_validateApidSeqCountIn(0, apid, 0);
    ASSERT_EQ(this->invoke_to_getApidSeqCountIn(0, apid, 0), 0);
    ASSERT_EQ(this->invoke_to_getApidSeqCountIn(0, apid, 0), 1);
    (void)this->invoke_to_validateApidSeqCountIn(0, apid, 1);
    ASSERT_EVENTS_UnexpectedSequenceCount_SIZE(0);
    (void)this->invoke_to_validateApidSeqCountIn(0, apid, 7);
    ASSERT_EVENTS_UnexpectedSequenceCount_SIZE(1);
    ASSERT_EVENTS_UnexpectedSequenceCount(0, 7, 2);
    ASSERT_EQ(this->invoke_to_getApidSeqCountIn(0, apid, 0), 2);
}

}  // namespace Ccsds

}  // namespace Svc
