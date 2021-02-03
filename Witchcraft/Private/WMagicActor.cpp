// All rights reserved Howler Games LLC

#include "WMagicActor.h"
#include "Abilities/GameplayAbility.h"

// Sets default values
AWMagicActor::AWMagicActor() :
	MagicType(EWMagicType::None),
	Speed(0),
	BouncesMax(0),
	DistanceMax(0),
	DamageMin(0),
	DamageMax(0),
	BlastRadius(0)
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWMagicActor::BeginPlay()
{
	Super::BeginPlay();

	if (CreatedByAbility != nullptr )
	{
		for (FWEffectSpecWrapper& EffectWrapper : AppliesGameplaySpecs)
		{
			EffectWrapper.GameplayEffectSpec = CreatedByAbility->MakeOutgoingGameplayEffectSpec(EffectWrapper.GameplayEffectClass, 1);
		}
	}
}

// Called every frame
void AWMagicActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

