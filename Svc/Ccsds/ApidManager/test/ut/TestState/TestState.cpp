// ======================================================================
// \title  ApidManagerTestState.cpp
// \author thomas-bc
// \brief  Shadow helper implementations for ApidManager test state
// ======================================================================

#include "Svc/Ccsds/ApidManager/test/ut/TestState/TestState.hpp"

#include <iterator>
#include "STest/Random/Random.hpp"
#include "Svc/Ccsds/ApidManager/ApidManager.hpp"
#include "Svc/Ccsds/Types/FppConstantsAc.hpp"

namespace Svc {

namespace Ccsds {

// ----------------------------------------------------------------------
// Shadow helpers
// ----------------------------------------------------------------------

U16 ApidManagerTestState::shadow_getAndIncrementSeqCount(ComCfg::Apid::T apid) {
    auto it = this->shadow_downlinkCounts.find(apid);
    if (it != this->shadow_downlinkCounts.end()) {
        // APID already tracked: return current count and advance to the next
        U16 current = it->second;
        it->second = static_cast<U16>((current + 1) % (1 << SpacePacketSubfields::SeqCountWidth));
        return current;
    }
    // APID not yet tracked: register it, starting at count 0
    this->shadow_downlinkCounts[apid] = static_cast<U16>(1);  // next expected is 1
    return 0;                                                 // first count returned is 0
}

void ApidManagerTestState::shadow_validateApidSeqCount(ComCfg::Apid::T apid, U16 receivedSeqCount) {
    this->shadow_uplinkCounts[apid] =
        static_cast<U16>((receivedSeqCount + 1) % (1 << SpacePacketSubfields::SeqCountWidth));
}

ComCfg::Apid::T ApidManagerTestState::shadow_getRandomTrackedApid(const std::map<ComCfg::Apid::T, U16>& counts) {
    FW_ASSERT(!counts.empty());
    U32 idx = STest::Random::lowerUpper(0, static_cast<U32>(counts.size()) - 1);
    return std::next(counts.begin(), idx)->first;
}

}  // namespace Ccsds

}  // namespace Svc
