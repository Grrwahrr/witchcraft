// All rights reserved Howler Games LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WTypes.h"
#include "WitchcraftGameModeBase.generated.h"

/**
 * The game mode is instanced every time a level is loaded. It is not replicated to anyone.
 * We can handle connection requests, and the way a player spawns.
 *
 * https://docs.unrealengine.com/en-US/InteractiveExperiences/Framework/GameMode/index.html
 */
UCLASS()
class WITCHCRAFT_API AWitchcraftGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AWitchcraftGameModeBase();

protected:
	virtual void BeginPlay() override;

	TArray<EWPlayerColor> PossiblePlayerColors;
	TArray<EWPlayerColor> InitializePossiblePlayerColors() const;

private:
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
