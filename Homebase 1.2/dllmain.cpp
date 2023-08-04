// Homebase 1.2 - Singleplayer
#include <Windows.h>
#include "MinHook.h"
#include "SDK.hpp"
#include "Globals.h"
#include <detours.h>
#include "Game.h"
using namespace SDK;


void InitOthers() {
    Globals::Engine = UObject::FindObjectFast<UFortEngine>("FortEngine_0");

    if (Globals::Engine)
        printf("[Homebase Defs] Engine is not NULL! *this is good*\n");

    Globals::World = Globals::Engine->GameViewport->World;

    if (Globals::World)
        printf("[Homebase Defs] World is not NULL! *this is good*\n");

    Globals::GameplayStatics = UObject::FindObjectFast<UGameplayStatics>("Default__GameplayStatics");

    if (Globals::GameplayStatics)
        printf("[Homebase Defs] GameplayStatics is not NULL! *this is good*\n");

    Globals::LocalController = Globals::Engine->GameInstance->LocalPlayers[0]->PlayerController;

    if (Globals::LocalController)
        printf("[Homebase Defs] LocalController is not NULL! *this is good*\n");

    uintptr_t ProcessEventAddr = (uintptr_t(GetModuleHandle(0)) + Offsets::ProcessEvent);
    Globals::ProcessEvent = decltype(Globals::ProcessEvent)(ProcessEventAddr);

    if (Globals::ProcessEvent)
        printf("[Homebase Defs] ProcessEvent is not NULL! *this is REALLY good*\n");

    Globals::Engine->GameViewport->ViewportConsole = reinterpret_cast<UConsole*>(Globals::GameplayStatics->SpawnObject(UConsole::StaticClass(), Globals::Engine->GameViewport));

    Globals::BaseTransform.Scale3D = { 1,1,1 };

    printf("[Homebase Console] successfully created Console!");
}

DWORD WINAPI Main(LPVOID) {
    FILE* K;
    AllocConsole();
    freopen_s(&K, "CONOUT$", "w", stdout);

    MH_Initialize(); // Initialize MinHook

    InitGObjects(); // Initialize SDK

    InitOthers(); // Initialize the Global Variables


    auto welcome = R"(
  _    _                      ____                __   __
 | |  | |                    |  _ \               \ \ / /
 | |__| | ___  _ __ ___   ___| |_) | __ _ ___  ___ \ V / 
 |  __  |/ _ \| '_ ` _ \ / _ \  _ < / _` / __|/ _ \ > <  
 | |  | | (_) | | | | | |  __/ |_) | (_| \__ \  __// . \ 
 |_|  |_|\___/|_| |_| |_|\___|____/ \__,_|___/\___/_/ \_\
                                                         
    
Welcome to HomebaseX!
Created by @whisperends on Discord.

Discord: https://discord.gg/hMw32VDPwt

                                               
)";

    printf(welcome);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)Globals::ProcessEvent, Game::FortniteHook);
    DetourTransactionCommit();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, Main, 0, 0, 0);
        break;
    }
    return TRUE;
}

