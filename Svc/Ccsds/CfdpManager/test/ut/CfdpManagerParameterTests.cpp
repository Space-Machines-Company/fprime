// ======================================================================
// \title  CfdpManagerParameterTests.cpp
// \author Auto-generated
// \brief  cpp file for CfdpManager component parameter unit tests
//
// ======================================================================

#include "CfdpManagerTester.hpp"

#include <Os/FileSystem.hpp>

namespace Svc {
namespace Ccsds {
namespace Cfdp {

// ----------------------------------------------------------------------
// Parameter Tests
// ----------------------------------------------------------------------
//
// Each parameter has two tests:
//   *Default : after loadParameters() (called in the tester constructor),
//              component.paramGet_<Name>() returns the FPP-declared default and
//              flags ParamValid::DEFAULT.
//   *SetGet  : paramSet_<Name>() stages a value, paramSend_<Name>() commits it,
//              verified via component.paramGet_<Name>() which then flags VALID.
//
// The tester is a friend of CfdpManager, so component.paramGet_<Name>() is
// accessible directly. Parameter SET opcodes are handled synchronously inside
// the generated cmdIn dispatch (unlike async user commands), so paramSend_*
// applies the value immediately and no doDispatch() is needed afterward.

// ---- LocalEid ----

void CfdpManagerTester::testParamLocalEidDefault() {
    Fw::ParamValid valid;
    const EntityId value = this->component.paramGet_LocalEid(valid);
    ASSERT_EQ(Fw::ParamValid::DEFAULT, valid.e) << "Loaded default should flag DEFAULT";
    ASSERT_EQ(42u, value);
}

void CfdpManagerTester::testParamLocalEidSetGet() {
    const EntityId newValue = 77;
    this->paramSet_LocalEid(newValue, Fw::ParamValid::VALID);
    this->paramSend_LocalEid(0, 0);

    Fw::ParamValid valid;
    ASSERT_EQ(newValue, this->component.paramGet_LocalEid(valid));
    ASSERT_EQ(Fw::ParamValid::VALID, valid.e);
}

// ---- OutgoingFileChunkSize ----

void CfdpManagerTester::testParamOutgoingFileChunkSizeDefault() {
    Fw::ParamValid valid;
    const U32 value = this->component.paramGet_OutgoingFileChunkSize(valid);
    ASSERT_EQ(Fw::ParamValid::DEFAULT, valid.e) << "Loaded default should flag DEFAULT";
    ASSERT_EQ(992u, value);
}

void CfdpManagerTester::testParamOutgoingFileChunkSizeSetGet() {
    const U32 newValue = 512;
    this->paramSet_OutgoingFileChunkSize(newValue, Fw::ParamValid::VALID);
    this->paramSend_OutgoingFileChunkSize(0, 0);

    Fw::ParamValid valid;
    ASSERT_EQ(newValue, this->component.paramGet_OutgoingFileChunkSize(valid));
    ASSERT_EQ(Fw::ParamValid::VALID, valid.e);
}

// ---- RxCrcCalcBytesPerCycle ----

void CfdpManagerTester::testParamRxCrcCalcBytesPerCycleDefault() {
    Fw::ParamValid valid;
    const U32 value = this->component.paramGet_RxCrcCalcBytesPerCycle(valid);
    ASSERT_EQ(Fw::ParamValid::DEFAULT, valid.e) << "Loaded default should flag DEFAULT";
    ASSERT_EQ(65536u, value);
}

void CfdpManagerTester::testParamRxCrcCalcBytesPerCycleSetGet() {
    const U32 newValue = 32768;
    this->paramSet_RxCrcCalcBytesPerCycle(newValue, Fw::ParamValid::VALID);
    this->paramSend_RxCrcCalcBytesPerCycle(0, 0);

    Fw::ParamValid valid;
    ASSERT_EQ(newValue, this->component.paramGet_RxCrcCalcBytesPerCycle(valid));
    ASSERT_EQ(Fw::ParamValid::VALID, valid.e);
}

// ---- FileInDefaultChannel ----

void CfdpManagerTester::testParamFileInDefaultChannelDefault() {
    Fw::ParamValid valid;
    const U8 value = this->component.paramGet_FileInDefaultChannel(valid);
    ASSERT_EQ(Fw::ParamValid::DEFAULT, valid.e) << "Loaded default should flag DEFAULT";
    ASSERT_EQ(0u, value);
}

void CfdpManagerTester::testParamFileInDefaultChannelSetGet() {
    const U8 newValue = 1;
    this->paramSet_FileInDefaultChannel(newValue, Fw::ParamValid::VALID);
    this->paramSend_FileInDefaultChannel(0, 0);

    Fw::ParamValid valid;
    ASSERT_EQ(newValue, this->component.paramGet_FileInDefaultChannel(valid));
    ASSERT_EQ(Fw::ParamValid::VALID, valid.e);
}

// ---- FileInDefaultDestEntityId ----

void CfdpManagerTester::testParamFileInDefaultDestEntityIdDefault() {
    Fw::ParamValid valid;
    const EntityId value = this->component.paramGet_FileInDefaultDestEntityId(valid);
    ASSERT_EQ(Fw::ParamValid::DEFAULT, valid.e) << "Loaded default should flag DEFAULT";
    ASSERT_EQ(100u, value);
}

void CfdpManagerTester::testParamFileInDefaultDestEntityIdSetGet() {
    const EntityId newValue = 200;
    this->paramSet_FileInDefaultDestEntityId(newValue, Fw::ParamValid::VALID);
    this->paramSend_FileInDefaultDestEntityId(0, 0);

    Fw::ParamValid valid;
    ASSERT_EQ(newValue, this->component.paramGet_FileInDefaultDestEntityId(valid));
    ASSERT_EQ(Fw::ParamValid::VALID, valid.e);
}

// ---- FileInDefaultClass ----

void CfdpManagerTester::testParamFileInDefaultClassDefault() {
    Fw::ParamValid valid;
    const Cfdp::Class value = this->component.paramGet_FileInDefaultClass(valid);
    ASSERT_EQ(Fw::ParamValid::DEFAULT, valid.e) << "Loaded default should flag DEFAULT";
    ASSERT_EQ(Cfdp::Class::CLASS_2, value);
}

void CfdpManagerTester::testParamFileInDefaultClassSetGet() {
    const Cfdp::Class newValue = Cfdp::Class::CLASS_1;
    this->paramSet_FileInDefaultClass(newValue, Fw::ParamValid::VALID);
    this->paramSend_FileInDefaultClass(0, 0);

    Fw::ParamValid valid;
    ASSERT_EQ(Cfdp::Class::CLASS_1, this->component.paramGet_FileInDefaultClass(valid));
    ASSERT_EQ(Fw::ParamValid::VALID, valid.e);
}

// ---- FileInDefaultKeep ----

void CfdpManagerTester::testParamFileInDefaultKeepDefault() {
    Fw::ParamValid valid;
    const Cfdp::Keep value = this->component.paramGet_FileInDefaultKeep(valid);
    ASSERT_EQ(Fw::ParamValid::DEFAULT, valid.e) << "Loaded default should flag DEFAULT";
    ASSERT_EQ(Cfdp::Keep::DELETE, value);
}

void CfdpManagerTester::testParamFileInDefaultKeepSetGet() {
    const Cfdp::Keep newValue = Cfdp::Keep::KEEP;
    this->paramSet_FileInDefaultKeep(newValue, Fw::ParamValid::VALID);
    this->paramSend_FileInDefaultKeep(0, 0);

    Fw::ParamValid valid;
    ASSERT_EQ(Cfdp::Keep::KEEP, this->component.paramGet_FileInDefaultKeep(valid));
    ASSERT_EQ(Fw::ParamValid::VALID, valid.e);
}

// ---- FileInDefaultPriority ----

void CfdpManagerTester::testParamFileInDefaultPriorityDefault() {
    Fw::ParamValid valid;
    const U8 value = this->component.paramGet_FileInDefaultPriority(valid);
    ASSERT_EQ(Fw::ParamValid::DEFAULT, valid.e) << "Loaded default should flag DEFAULT";
    ASSERT_EQ(0u, value);
}

void CfdpManagerTester::testParamFileInDefaultPrioritySetGet() {
    const U8 newValue = 5;
    this->paramSet_FileInDefaultPriority(newValue, Fw::ParamValid::VALID);
    this->paramSend_FileInDefaultPriority(0, 0);

    Fw::ParamValid valid;
    ASSERT_EQ(newValue, this->component.paramGet_FileInDefaultPriority(valid));
    ASSERT_EQ(Fw::ParamValid::VALID, valid.e);
}

// ---- SequenceFile ----

namespace {
const char* const SEQUENCE_PATH = "CfdpManagerSequenceFileUt.txt";
const char* const UNWRITABLE_SEQUENCE_PATH = "/nonexistent-cfdp-directory/sequence";
}  // namespace

void CfdpManagerTester::useSequenceFile(const char* path) {
    this->paramSet_SequenceFile(Fw::ParamString(path), Fw::ParamValid::VALID);
    this->paramSend_SequenceFile(0, 0);
}

void CfdpManagerTester::testParamSequenceFileEmptyNumbersFromOne() {
    ASSERT_EQ(1u, this->component.m_engine->nextSequence());
    ASSERT_EQ(2u, this->component.m_engine->nextSequence());
    ASSERT_EVENTS_SequenceFileUnavailable_SIZE(0);
}

void CfdpManagerTester::testParamSequenceFileSurvivesARestart() {
    (void)Os::FileSystem::removeFile(SEQUENCE_PATH);
    this->useSequenceFile(SEQUENCE_PATH);
    const TransactionSeq first = this->component.m_engine->nextSequence();
    ASSERT_EQ(static_cast<TransactionSeq>(first + 1), this->component.m_engine->nextSequence());

    CfdpManagerTester rebooted;
    rebooted.useSequenceFile(SEQUENCE_PATH);
    ASSERT_EQ(static_cast<TransactionSeq>(first + Engine::SequenceBlock), rebooted.component.m_engine->nextSequence())
        << "A restart must not reuse a number from the block reserved before it";
    ASSERT_EVENTS_SequenceFileUnavailable_SIZE(0);
    (void)Os::FileSystem::removeFile(SEQUENCE_PATH);
}

void CfdpManagerTester::testParamSequenceFileReservesTheNextBlock() {
    (void)Os::FileSystem::removeFile(SEQUENCE_PATH);
    this->useSequenceFile(SEQUENCE_PATH);
    const TransactionSeq first = this->component.m_engine->nextSequence();
    for (TransactionSeq offset = 1; offset <= Engine::SequenceBlock; ++offset) {
        ASSERT_EQ(static_cast<TransactionSeq>(first + offset), this->component.m_engine->nextSequence());
    }

    CfdpManagerTester rebooted;
    rebooted.useSequenceFile(SEQUENCE_PATH);
    ASSERT_EQ(static_cast<TransactionSeq>(first + 2 * Engine::SequenceBlock),
              rebooted.component.m_engine->nextSequence())
        << "Crossing a block must reserve the next one before using it";
    (void)Os::FileSystem::removeFile(SEQUENCE_PATH);
}

void CfdpManagerTester::testParamSequenceFileUnwritableWarnsAndStillNumbers() {
    this->useSequenceFile(UNWRITABLE_SEQUENCE_PATH);
    const TransactionSeq first = this->component.m_engine->nextSequence();
    ASSERT_EQ(static_cast<TransactionSeq>(first + 1), this->component.m_engine->nextSequence());
    ASSERT_EVENTS_SequenceFileUnavailable_SIZE(1);
}

}  // namespace Cfdp
}  // namespace Ccsds
}  // namespace Svc
