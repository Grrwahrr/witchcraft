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

	// -------------------------------------------
	// --------------- Statistics ----------------
	// -------------------------------------------
	// 
	// Some statistics we track while a level is playing
	float StatDamageToEnemies = 0;
	float StatDamageToFriends = 0;
	int32 StatKilledEnemies = 0;
	int32 StatKilledFriends = 0;
	int32 StatDeaths = 0;

	/** Increase the damage to enemies by the given value */
	UFUNCTION(BlueprintCallable, Category="Statistics")
    void StatIncreaseDamageDealt(float InDamageToFriends, float InDamageToEnemies);

	/** Increase the killed enemies count by one */
	UFUNCTION(BlueprintCallable, Category="Statistics")
    void StatIncreaseKills(int32 InKilledFriends, int32 InKilledEnemies);

	/** Increase the death count by one */
	UFUNCTION(BlueprintCallable, Category="Statistics")
    void StatIncrementDeaths();

	/** Return all available statistics */
	UFUNCTION(BlueprintCallable, Category="Statistics")
    void StatGetAll(float& DamageToEnemies, float& DamageToFriends, int32& KilledEnemies, int32& KilledFriends, int32& Deaths);
};
