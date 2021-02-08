// All rights reserved Howler Games LLC


#include "WPlayerState.h"

#include "Engine/DemoNetDriver.h"

AWPlayerState::AWPlayerState() : Super()
{
	// I could add the gameplay ability system here
	PlayerColor = EWPlayerColor::None;
}

void AWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWPlayerState, PlayerColor);
}

EWPlayerColor AWPlayerState::GetPlayerColor()
{
	return PlayerColor;
}

void AWPlayerState::SetPlayerColor(EWPlayerColor NewPlayerColor)
{
	PlayerColor = NewPlayerColor;
}

void AWPlayerState::OnRep_PlayerColor_Implementation()
{
	// This should be implemented in blueprint - this is just here because it has to be
}

void AWPlayerState::BeginPlay()
{
	Super::BeginPlay();
}
