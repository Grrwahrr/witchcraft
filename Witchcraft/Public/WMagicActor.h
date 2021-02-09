// All rights reserved Howler Games LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WTypes.h"
#include "WMagicActor.generated.h"

class UGameplayEffect;

UCLASS()
class WITCHCRAFT_API AWMagicActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWMagicActor();

	/** User-visible short name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	FText SpellName;

	/** User-visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	FText SpellDescription;

	/** Type of spell */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	EWMagicType MagicType;

	/** Magic element */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	EWMagicElement MagicElement;
	
	/** Speed of the projectile, if any */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	float Speed;

	/** Maximum number of bounces before simulation terminates */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	int32 BouncesMax;

	/** Maximum distance traversed before simulation terminates */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	float DistanceMax;

	/** Minimum amount of damage for this spell */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	float DamageMin;

	/** Maximum amount of damage for this spell */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	float DamageMax;

	/** Impact force used in hit reactions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	float ImpactForce;

	/** Blast radius on detonation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	float BlastRadius;

	/** Need a reference to the GameplayAbility that created this actor in order to create GameplayEffectSpecs */
	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = Magic)
	UGameplayAbility* CreatedByAbility;

	/** These GameplayEffects will be applied, constructed from their class. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	TArray<TSubclassOf<UGameplayEffect>> AppliesGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic)
	TArray<FWEffectSpecWrapper> AppliesGameplaySpecs;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
