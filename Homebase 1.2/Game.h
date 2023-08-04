#pragma once
#include <Windows.h>
#include "SDK.hpp"
#include "Globals.h"
#include <detours.h>
#include <fstream>
#include <format>
#include "Utils.h"
using namespace SDK;

namespace Game {

	// DON'T CHANGE ANY OF THESE
	static bool isDev = true;
	static bool hasDumped = false;
	static bool inGame = false;
	static void* FortniteHook(UObject* Object, UFunction* Function, void* Params) {
		if (!Object || !Function)
			return Globals::ProcessEvent(Object, Function, Params);

		auto FuncName = Function->GetName();

		// Object Dump
		if (GetAsyncKeyState(VK_F1) & 0x1) {
			if (isDev == true && hasDumped == false) {
				hasDumped = true;

				std::ofstream Objects("Objects.log");

				Objects << "This was dumped using Homebase 1.2\n";

				for(int i = 0; i < UObject::GObjects->Num(); i++) {
					auto Object = UObject::GObjects->GetByIndex(i);

					if (Object) {
						Objects << std::format("[{}] {}\n", i, Object->GetFullName());
					}
				}

				printf("[Homebase Dev] Successfully dumped objects\n");
			}
		}

		if (FuncName.find("StartButton") != std::string::npos) {
			// Zone_Onboarding_FarmsteadFort
					// Zone_Outpost_Stonewood
					// Zone_Outpost_TwinePeaks
					// Zone_Outpost_CannyValley
					// Zone_Survival_Diners
					// Zone_Survival_ThunderRoute99
					// Zone_Temperate_Grassland
					// Zone_Outpost_Plankerton_AD
					// Outpost_FlingerIntro_Cine (SEQ)
					// Zone_Temperate_Industrial
					// Zone_Onboarding_Forest_a
					// Zone_Onboarding_Suburban_a
			Globals::map = "Zone_Onboarding_FarmsteadFort";
			Globals::LocalController->SwitchLevel(L"Zone_Onboarding_FarmsteadFort");
		}

		if (FuncName == "ReadyToStartMatch") {
			if (inGame == false) {
				inGame = true; // do it as soon as possible

				printf("[Homebase Dev] Globals::World\n");
				Globals::World = Globals::Engine->GameViewport->World;
				printf("[Homebase Dev] GameMode\n");
				auto GameMode = reinterpret_cast<AFortGameMode*>(Object);
				printf("[Homebase Dev] GameState\n");
				auto GameState = (AFortGameStateZone*)GameMode->GameState;
				printf("[Homebase Dev] Controller\n");
				auto Controller = reinterpret_cast<AFortPlayerController*>(Globals::Engine->GameInstance->LocalPlayers[0]->PlayerController);
				printf("[Homebase Dev] PlayerState\n");
				auto PlayerState = (AFortPlayerState*)Controller->PlayerState;

				printf("[Homebase Dev] GameController\n");
				Globals::GameController = Controller;
				
				printf("[Homebase Dev] GameState\n");
				Globals::GameState = GameState;

				
				//Globals::GameState->MissionManager = reinterpret_cast<AFortMissionManager*>(Globals::GameplayStatics->SpawnObject(AFortMissionManager::StaticClass(), Globals::GameState));

				//AFortObjectiveBase* ObjectiveTest = reinterpret_cast<AFortObjectiveBase*>(Globals::GameplayStatics->SpawnObject(AFortObjectiveBase::StaticClass(), nullptr));

				//std::cout << ("PlayerController Name: ") << Globals::Engine->GameInstance->LocalPlayers[0]->PlayerController->StaticClass()->GetName() << std::endl;


				//auto HandlerName = Globals::GameState->MissionManager->GetName();

				//std::cout << ("[HomebaseX Definition] MissionManager Name: ") << HandlerName << "\n\n";

				printf("[Homebase Dev] SpawnPos\n");
				FTransform SpawnPos = Globals::BaseTransform;

				//SpawnPos.Scale3D = { 1,1,1 };

				//auto PlayerStarts = GameMode->K2_FindPlayerStart(Globals::GameController, L"");

				//FVector SpawnLoc = PlayerStarts->K2_GetActorLocation();

				printf("[Homebase Dev] Define Player Start\n");
				if (Globals::map == "Zone_Outpost_Stonewood" ||
					Globals::map == "Zone_Outpost_TwinePeaks" ||
					Globals::map == "Zone_Outpost_CannyValley" ||
					Globals::map == "Zone_Survival_Diners" ||
					Globals::map == "Zone_Survival_ThunderRoute99" ||
					Globals::map == "Zone_Temperate_Grassland" ||
					Globals::map == "Zone_Outpost_Plankerton_AD" ||
					Globals::map == "Outpost_FlingerIntro_Cine" ||
					Globals::map == "Zone_Temperate_Industrial" ||
					Globals::map == "Zone_Onboarding_Forest_a" ||
					Globals::map == "Zone_Onboarding_Suburban_a" ||
					Globals::map == "Zone_Temperate_Urban_Blockbuster" ||
					Globals::map == "Zone_Arid_Stormquest_Desert") {
					SpawnPos.Translation = { 0, 0, 5500 };
				}
				else {
					SpawnPos.Translation = { -3600, -300, -1522 };
				}

				printf("[Homebase Dev] PawnSpawnActor\n");
				auto Pawn = Utils::SpawnActor<APlayerPawn_Generic_C>(nullptr, SpawnPos.Translation);

				printf("[Homebase Dev] Globals::Pawn\n");
				Globals::Pawn = Pawn;

				printf("[Homebase Dev] Globals::GameController Possess\n");
				Globals::GameController->Possess(Globals::Pawn);

				//printf("[Homebase Dev] OverriddenBackpackSize\n");
				//Globals::GameController->OverriddenBackpackSize = 10000;
				//printf("[Homebase Dev] bInfiniteAmmo\n");
				//Globals::GameController->bInfiniteAmmo = true;
				//printf("[Homebase Dev] bCraftFree\n");
				//Globals::GameController->bCraftFree = true;

				//Inventory::SetupInventory();
				//Inventory::GiveMaterials(); // give all material (soon have to make this based on portion of Mission)

				//Globals::GameController->bCraftFree = true;
				//reinterpret_cast<UFortCheatManager*>(Globals::GameController->CheatManager)->CraftFree();

				printf("[Homebase Dev] God\n");
				Globals::GameController->CheatManager->God();
				printf("[Homebase Dev] ServerSetClientHasFinishedLoading\n");
				Globals::GameController->ServerSetClientHasFinishedLoading(true);
				printf("[Homebase Dev] bHasServerFinishedLoading\n");
				Globals::GameController->bHasServerFinishedLoading = true;
				printf("[Homebase Dev] OnRep_bHasServerFinishedLoading\n");
				Globals::GameController->OnRep_bHasServerFinishedLoading();

				printf("[Homebase Dev] ServerReadyToStartMatch\n");
				//Globals::GameController->ServerReadyToStartMatch(); //magical Loading screen drop

				printf("[Homebase Dev] bIsGameSessionOwner\n");
				PlayerState->bIsGameSessionOwner = true;
				printf("[Homebase Dev] OnRep_SessionOwner\n");
				PlayerState->OnRep_SessionOwner();

				printf("[Homebase Dev] OnRep_CharacterParts\n");
				PlayerState->OnRep_CharacterParts();
				printf("[Homebase Dev] OnCharacterPartsReinitialized\n");
				Globals::Pawn->OnCharacterPartsReinitialized();

				

				printf("[Homebase Dev] StartMatch\n");
				GameMode->StartMatch();
				printf("[Homebase Dev] StartPlay\n");
				GameMode->StartPlay();
			}
		}

		if (FuncName == "ServerLoadingScreenDropped") {
			// TO IMPLEMENT

			// (THIS IS TO PATCH THE FIRST ISSUE, BUT WILL BE FIXED WHEN I LOAD ALL OF THE CINEMATICS IN
			Globals::GameController->SetCinematicMode(false, false, false, false, false); // Bring Player out of cinematic mode
			Globals::GameController->ClientSetCinematicMode(false, false, false, false); // incase SetCinematicMode doesn't work

			// just incase it doesn't listen
			Globals::GameController->Pawn->bHidden = false;
			Globals::GameController->Pawn->SetActorHiddenInGame(false);
		}


		return Globals::ProcessEvent(Object, Function, Params);
	}
}