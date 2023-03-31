// Copyright Epic Games, Inc. All Rights Reserved.

#include "Test1GameMode.h"
#include "Test1Character.h"
#include "UObject/ConstructorHelpers.h"

ATest1GameMode::ATest1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
