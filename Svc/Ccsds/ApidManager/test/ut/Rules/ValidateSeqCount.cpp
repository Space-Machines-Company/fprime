// ======================================================================
// \title  ValidateSeqCount.cpp
// \author thomas-bc
// \brief  Rule implementations for the ValidateSeqCount rule group
//
// These rules exercise the validateApidSeqCountIn port.
// ======================================================================

#include "Svc/Ccsds/ApidManager/test/ut/ApidManagerTester.hpp"
#include "Svc/Ccsds/Types/FppConstantsAc.hpp"

namespace Svc {

namespace Ccsds {

// ----------------------------------------------------------------------
// ValidateSeqCount.Ok
// ----------------------------------------------------------------------

bool ApidManagerTester::ValidateSeqCount__Ok__precondition() const {
    return !this->shadow.shadow_uplinkCounts.empty();
}

void ApidManagerTester::ValidateSeqCount__Ok__action() {
    this->clearHistory();

    ComCfg::Apid::T apid = ApidManagerTestState::shadow_getRandomTrackedApid(this->shadow.shadow_uplinkCounts);
    U16 expected = this->shadow.shadow_uplinkCounts.at(apid);
    this->invoke_to_validateApidSeqCountIn(0, apid, expected);
    this->shadow.shadow_validateApidSeqCount(apid, expected);

    ASSERT_EVENTS_UnexpectedSequenceCount_SIZE(0);
}

// ----------------------------------------------------------------------
// ValidateSeqCount.Failure
// ----------------------------------------------------------------------

bool ApidManagerTester::ValidateSeqCount__Failure__precondition() const {
    return !this->shadow.shadow_uplinkCounts.empty();
}

void ApidManagerTester::ValidateSeqCount__Failure__action() {
    this->clearHistory();

    ComCfg::Apid::T apid = ApidManagerTestState::shadow_getRandomTrackedApid(this->shadow.shadow_uplinkCounts);
    U16 correctCount = this->shadow.shadow_uplinkCounts.at(apid);

    // Increment by 1 (mod 14-bit counter width) to produce a provably wrong count
    U16 wrongCount = static_cast<U16>((correctCount + 1) % (1 << SpacePacketSubfields::SeqCountWidth));

    this->invoke_to_validateApidSeqCountIn(0, apid, wrongCount);
    this->shadow.shadow_validateApidSeqCount(apid, wrongCount);

    ASSERT_EVENTS_UnexpectedSequenceCount_SIZE(1);
    ASSERT_EVENTS_UnexpectedSequenceCount(0, wrongCount, correctCount);
}

// ----------------------------------------------------------------------
// ValidateSeqCount.NewOk
// ----------------------------------------------------------------------

bool ApidManagerTester::ValidateSeqCount__NewOk__precondition() const {
    constexpr U16 maxTrackedApids = ApidManager::MAX_TRACKED_APIDS;
    return this->shadow.shadow_uplinkCounts.size() < maxTrackedApids;
}

void ApidManagerTester::ValidateSeqCount__NewOk__action() {
    this->clearHistory();

    Fw::Optional<ComCfg::Apid::T> apidOption = CcsdsTestUtils::getRandomApid();
    if (!apidOption.has_value() || this->shadow.shadow_uplinkCounts.count(apidOption.value()) != 0) {
        return;
    }
    const auto apid = apidOption.value();
    this->invoke_to_validateApidSeqCountIn(0, apid, 0);
    this->shadow.shadow_validateApidSeqCount(apid, 0);

    ASSERT_EVENTS_UnexpectedSequenceCount_SIZE(0);
}

}  // namespace Ccsds

}  // namespace Svc
