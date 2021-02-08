// All rights reserved Howler Games LLC


#include "WitchcraftGameModeBase.h"

#include "WPlayerState.h"

AWitchcraftGameModeBase::AWitchcraftGameModeBase() : Super()
{
	PossiblePlayerColors = InitializePossiblePlayerColors();
}

void AWitchcraftGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

TArray<EWPlayerColor> AWitchcraftGameModeBase::InitializePossiblePlayerColors() const
{
	TArray<EWPlayerColor> Temp;
	Temp.Add(EWPlayerColor::Blue);
	Temp.Add(EWPlayerColor::Red);
	Temp.Add(EWPlayerColor::Purple);
	Temp.Add(EWPlayerColor::Green);

	const int32 LastIndex = Temp.Num() - 1;
	for (int32 i=0; i<=LastIndex; ++i)
	{
		const int32 Index = FMath::RandRange(i, LastIndex);
		if (i != Index)
		{
			Temp.Swap(i, Index);
		}
	}
	
	return Temp;
}

void AWitchcraftGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	auto PlayerCount = GetNumPlayers();

	// TODO: we use this to prevent players from joining the game
}

void AWitchcraftGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Copy all possible colors
	TArray<EWPlayerColor> AvailableColors = PossiblePlayerColors;
	
	for ( FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator )
	{
		// B* b = dynamic_cast<B*>(a); - can do  that to cast to another class and check if ok
		//
		// if (b == nullptr)
		// {
		// 	// Cast failed
		// }
		APlayerController* ExistingPlayer = Iterator->Get();
		const auto ExistingPlayerState = ExistingPlayer->GetPlayerState<AWPlayerState>();
		if ( ExistingPlayerState != nullptr )
		{
			AvailableColors.Remove(ExistingPlayerState->GetPlayerColor());
		}
	}
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("START START START: %d"), static_cast<uint8>(0)));
	// We are left with colors that we have not yet used
	// for (auto AvailableColor : AvailableColors)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("ColorFree: %d"), static_cast<uint8>(AvailableColor)));
	// }
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("DONE DONE DONE: %d"), static_cast<uint8>(0)));

	auto NewPlayerState = NewPlayer->GetPlayerState<AWPlayerState>();
	if (NewPlayerState != nullptr)
	{
		NewPlayerState->SetPlayerColor(AvailableColors.Pop());
	}
}
