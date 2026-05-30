#pragma once
#include "state/RunPersistentState.h"
#include "state/BlindRuntimeState.h"

// COMPOSITE ROOT
struct RunSessionState {
    RunPersistentState persistentState;
    BlindRuntimeState runtimeState;
};
