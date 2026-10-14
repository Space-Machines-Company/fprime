#include <Os/File.hpp>
#include <Os/FileSystem.hpp>
#include "CfdpManagerTester.hpp"

namespace Svc {
namespace Ccsds {
namespace Cfdp {

namespace {
constexpr EntityId GROUND = 10;
constexpr U32 INTERVAL = 2;
constexpr U32 INACTIVITY = 120;
constexpr U32 ROOMY = 64;
constexpr U32 NARROW = 2;
const char* const OUT = "test/ut/output";
const char* const FAIL_DIR = "test/ut/output/downlink_fail";

Fw::String path(const char* dir, const char* name) {
    Fw::String full(dir);
    full += "/";
    full += name;
    return full;
}
}  // namespace

ChannelArrayParams CfdpManagerTester::channels(Fw::Enabled dequeue, U32 pdusPerCycle) {
    return ChannelArrayParams(ChannelParams(10, 10, 2, INACTIVITY, dequeue, Fw::String(""), pdusPerCycle,
                                            Fw::String(OUT), Fw::String(FAIL_DIR)));
}

PollArrayParams CfdpManagerTester::polling(U8 channel, const char* srcDir, U32 interval) {
    PollArrayParams polls;
    ChannelPollParams entries = polls[channel];
    entries[0] = PollParams(Fw::Enabled::ENABLED, interval, 0, Class::CLASS_1, GROUND, Fw::String(srcDir),
                            Fw::String("/downlink"));
    polls[channel] = entries;
    return polls;
}

void CfdpManagerTester::boot(const PollArrayParams& polls, Fw::Enabled dequeue, U32 pdusPerCycle) {
    (void)Os::FileSystem::createDirectory(FAIL_DIR);
    this->paramSet_PollConfig(polls, Fw::ParamValid::VALID);
    this->paramSet_ChannelConfig(this->channels(dequeue, pdusPerCycle), Fw::ParamValid::VALID);
    this->clearHistory();
    this->m_pduCopyCount = 0;
    this->component.loadParameters();
}

void CfdpManagerTester::cycles(U32 count) {
    for (U32 i = 0; i < count; i++) {
        this->invoke_to_run1Hz(0, 0);
        this->component.doDispatch();
    }
}

void CfdpManagerTester::putFile(const char* dir, const char* name, FwSizeType size) {
    (void)Os::FileSystem::createDirectory(dir);
    Os::File file;
    ASSERT_EQ(Os::File::OP_OK, file.open(path(dir, name).toChar(), Os::File::OPEN_CREATE, Os::File::OVERWRITE));
    U8 byte = 0x5A;
    for (FwSizeType i = 0; i < size; i++) {
        FwSizeType one = 1;
        ASSERT_EQ(Os::File::OP_OK, file.write(&byte, one));
    }
    file.close();
}

void CfdpManagerTester::testBootPollSendsWithNoCommand() {
    const char* dir = "test/ut/output/boot_poll";
    this->putFile(dir, "a.bin", 512);
    this->boot(this->polling(0, dir, INTERVAL), Fw::Enabled::ENABLED, ROOMY);
    ASSERT_EVENTS_PollDirInitiated_SIZE(1);
    this->cycles(INTERVAL + 3);
    ASSERT_GT(this->fromPortHistory_dataOut->size(), 0U);
    ASSERT_FALSE(Os::FileSystem::exists(path(dir, "a.bin").toChar()));
}

void CfdpManagerTester::testDefaultPollConfigPollsNothing() {
    const char* dir = "test/ut/output/no_poll";
    this->putFile(dir, "a.bin", 512);
    this->boot(PollArrayParams(), Fw::Enabled::ENABLED, ROOMY);
    this->cycles(INTERVAL + 10);
    ASSERT_EQ(0U, this->fromPortHistory_dataOut->size());
    ASSERT_TRUE(Os::FileSystem::exists(path(dir, "a.bin").toChar()));
    (void)Os::FileSystem::removeFile(path(dir, "a.bin").toChar());
}

void CfdpManagerTester::testPollConfigRefusesAZeroInterval() {
    const char* dir = "test/ut/output/zero_poll";
    this->putFile(dir, "a.bin", 512);
    this->boot(this->polling(0, dir, 0), Fw::Enabled::ENABLED, ROOMY);
    ASSERT_EVENTS_InvalidPollInterval_SIZE(1);
    ASSERT_EVENTS_PollDirInitiated_SIZE(0);
    this->cycles(5);
    ASSERT_EQ(0U, this->fromPortHistory_dataOut->size());
    (void)Os::FileSystem::removeFile(path(dir, "a.bin").toChar());
}

void CfdpManagerTester::testPollConfigSkipsAnUnwiredChannel() {
    const char* dir = "test/ut/output/unwired_poll";
    this->putFile(dir, "a.bin", 512);
    this->boot(this->polling(1, dir, INTERVAL), Fw::Enabled::ENABLED, ROOMY);
    ASSERT_EVENTS_PollDirUnwired_SIZE(1);
    ASSERT_EVENTS_PollDirUnwired(0, 1, 0);
    this->cycles(INTERVAL + 3);
    ASSERT_TRUE(Os::FileSystem::exists(path(dir, "a.bin").toChar()));
    (void)Os::FileSystem::removeFile(path(dir, "a.bin").toChar());
}

void CfdpManagerTester::testDequeueStateFollowsBootAndCommands() {
    this->boot(PollArrayParams(), Fw::Enabled::DISABLED, ROOMY);
    this->cycles(1);
    DequeueTlmArray expected(DequeueTlm(DequeueState::DISABLED_AT_BOOT, 0));
    ASSERT_TLM_Dequeue(this->tlmHistory_Dequeue->size() - 1, expected);

    this->sendCmd_DisableDequeue(0, 0, 0);
    this->component.doDispatch();
    ASSERT_CMD_RESPONSE(0, CfdpManagerComponentBase::OPCODE_DISABLEDEQUEUE, 0, Fw::CmdResponse::OK);
    ASSERT_EVENTS_DequeueDisabled_SIZE(1);
    this->cycles(1);
    expected[0] = DequeueTlm(DequeueState::DISABLED_BY_COMMAND, 1);
    ASSERT_TLM_Dequeue(this->tlmHistory_Dequeue->size() - 1, expected);

    this->sendCmd_ResetCounters(0, 0, 0xFF);
    this->component.doDispatch();
    this->sendCmd_EnableDequeue(0, 0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_DequeueEnabled_SIZE(1);
    this->cycles(1);
    expected[0] = DequeueTlm(DequeueState::ENABLED, 2);
    ASSERT_TLM_Dequeue(this->tlmHistory_Dequeue->size() - 1, expected);

    this->clearHistory();
    this->sendCmd_EnableDequeue(0, 0, Cfdp::NumChannels);
    this->component.doDispatch();
    ASSERT_CMD_RESPONSE(0, CfdpManagerComponentBase::OPCODE_ENABLEDEQUEUE, 0, Fw::CmdResponse::VALIDATION_ERROR);
}

void CfdpManagerTester::testDisabledDequeueHoldsAPolledFile() {
    const char* dir = "test/ut/output/held_poll";
    this->putFile(dir, "held.bin", 512);
    this->boot(this->polling(0, dir, INTERVAL), Fw::Enabled::DISABLED, ROOMY);
    this->cycles(INACTIVITY + INTERVAL + 10);
    ASSERT_EQ(0U, this->fromPortHistory_dataOut->size());
    ASSERT_TRUE(Os::FileSystem::exists(path(dir, "held.bin").toChar()));
    ASSERT_FALSE(Os::FileSystem::exists(path(FAIL_DIR, "held.bin").toChar()));
    ASSERT_EVENTS_TxInactivityTimeout_SIZE(0);

    this->sendCmd_EnableDequeue(0, 0, 0);
    this->component.doDispatch();
    this->cycles(3);
    ASSERT_GT(this->fromPortHistory_dataOut->size(), 0U);
    ASSERT_FALSE(Os::FileSystem::exists(path(dir, "held.bin").toChar()));
}

void CfdpManagerTester::testDisableLetsTheActiveTransferFinish() {
    const char* dir = "test/ut/output/two_sends";
    this->putFile(dir, "first.bin", 4096);
    this->putFile(dir, "second.bin", 1024);
    this->boot(PollArrayParams(), Fw::Enabled::ENABLED, NARROW);
    this->sendCmd_SendFile(0, 0, 0, GROUND, Class::CLASS_1, Keep::KEEP, 0, path(dir, "first.bin"),
                           Fw::String("/downlink/first.bin"));
    this->component.doDispatch();
    this->sendCmd_SendFile(0, 0, 0, GROUND, Class::CLASS_1, Keep::KEEP, 0, path(dir, "second.bin"),
                           Fw::String("/downlink/second.bin"));
    this->component.doDispatch();
    this->cycles(1);
    ASSERT_GT(this->fromPortHistory_dataOut->size(), 0U);

    this->sendCmd_DisableDequeue(0, 0, 0);
    this->component.doDispatch();
    this->cycles(10);
    ASSERT_EVENTS_TxFileTransferCompleted_SIZE(1);
    const FwSizeType afterFirst = this->fromPortHistory_dataOut->size();
    this->cycles(5);
    ASSERT_EQ(afterFirst, this->fromPortHistory_dataOut->size());

    this->sendCmd_EnableDequeue(0, 0, 0);
    this->component.doDispatch();
    this->cycles(5);
    ASSERT_EVENTS_TxFileTransferCompleted_SIZE(2);
}

void CfdpManagerTester::testResetCountersKeepsQueueDepths() {
    const char* dir = "test/ut/output/reset_depths";
    this->putFile(dir, "long.bin", 8192);
    this->paramSet_ChannelConfig(this->channels(Fw::Enabled::ENABLED, NARROW), Fw::ParamValid::VALID);
    this->component.loadParameters();
    this->sendCmd_SendFile(0, 0, 0, GROUND, Class::CLASS_1, Keep::KEEP, 0, path(dir, "long.bin"),
                           Fw::String("/downlink/long.bin"));
    this->component.doDispatch();
    this->cycles(1);
    this->sendCmd_ResetCounters(0, 0, 0xFF);
    this->component.doDispatch();
    this->cycles(20);
    const ChannelTelemetry last =
        this->tlmHistory_ChannelTelemetry->at(this->tlmHistory_ChannelTelemetry->size() - 1).arg[0];
    ASSERT_EQ(0U, last.get_queueTxActive());
}

void CfdpManagerTester::testPrmSetChangesNoSlotAndNoState() {
    const char* dir = "test/ut/output/late_poll";
    this->putFile(dir, "late.bin", 512);
    this->boot(PollArrayParams(), Fw::Enabled::ENABLED, ROOMY);
    this->paramSet_PollConfig(this->polling(0, dir, INTERVAL), Fw::ParamValid::VALID);
    this->paramSend_PollConfig(0, 0);
    this->paramSet_ChannelConfig(this->channels(Fw::Enabled::DISABLED, ROOMY), Fw::ParamValid::VALID);
    this->paramSend_ChannelConfig(0, 0);
    this->cycles(INTERVAL + 3);
    ASSERT_EQ(0U, this->fromPortHistory_dataOut->size());
    ASSERT_EQ(DequeueState::ENABLED,
              this->tlmHistory_Dequeue->at(this->tlmHistory_Dequeue->size() - 1).arg[0].get_state());
    (void)Os::FileSystem::removeFile(path(dir, "late.bin").toChar());
}

}  // namespace Cfdp
}  // namespace Ccsds
}  // namespace Svc
