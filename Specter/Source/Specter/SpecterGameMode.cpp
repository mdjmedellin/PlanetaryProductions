// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Specter.h"
#include "SpecterGameMode.h"
#include "SpecterCharacter.h"

ASpecterGameMode::ASpecterGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScroller/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
