// All rights reserved Howler Games LLC

#pragma once

#include "CoreMinimal.h"

#include "WCharacter.h"
#include "GameFramework/PlayerState.h"
#include "WPlayerState.generated.h"

/**
 * Player state is replicated to all players. We use it to assign colors to each players.
 */
UCLASS()
class WITCHCRAFT_API AWPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AWPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerColor")
	EWPlayerColor GetPlayerColor();

	void SetPlayerColor(EWPlayerColor NewPlayerColor);
	
protected:
	UPROPERTY(ReplicatedUsing=OnRep_PlayerColor)
	EWPlayerColor PlayerColor;

	/** Notify function if the players color changed */
	UFUNCTION(BlueprintNativeEvent, Category="PlayerColor")
	void OnRep_PlayerColor();

	virtual void BeginPlay() override;
};
