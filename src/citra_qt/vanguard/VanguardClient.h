
#pragma once
#include <string>

#include "../../../externals/dynarmic/src/common/common_types.h"

class VanguardClientUnmanaged
{
public:
    static void CORE_STEP();
    static void LOAD_GAME_START(std::string romPath);
    static void LOAD_GAME_DONE();
    static void GAME_CLOSED();
    static void CLOSE_GAME();
    static int GAME_NAME;
    static void InvokeEmuThread();
    static bool RTC_OSD_ENABLED();
    static void RunCallback([[maybe_unused]] u64 userdata, int cycles_late);
};
