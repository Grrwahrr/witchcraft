// All rights reserved Howler Games LLC

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WAttributeSetBase.generated.h"

/**
 * 
 */
UCLASS()
class WITCHCRAFT_API UWAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/** Health GameplayAttribute */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData Health;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Health);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, Health);

	/** Health Maximum GameplayAttribute. A cap for the Health GameplayAttribute */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData HealthMaximum;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(HealthMaximum);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(HealthMaximum);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(HealthMaximum);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, HealthMaximum);

	/** Currently equipped magic spell and element for bot hands */
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_EquippedSpells)
	FGameplayAttributeData EquippedSpells;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(EquippedSpells);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(EquippedSpells);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(EquippedSpells);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, EquippedSpells);

	/** Fire Magic Damage Multiplier GameplayAttribute */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData FireDamageMultiplier;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(FireDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(FireDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(FireDamageMultiplier);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, FireDamageMultiplier);

	/** Water Magic Damage Multiplier GameplayAttribute */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData WaterDamageMultiplier;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(WaterDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(WaterDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(WaterDamageMultiplier);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, WaterDamageMultiplier);
	
	/** Ice Magic Damage Multiplier GameplayAttribute */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData IceDamageMultiplier;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(IceDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(IceDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(IceDamageMultiplier);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, IceDamageMultiplier);

	/** Wind Magic Damage Multiplier GameplayAttribute */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData WindDamageMultiplier;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(WindDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(WindDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(WindDamageMultiplier);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, WindDamageMultiplier);

	/** Electric Magic Damage Multiplier GameplayAttribute */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData ElectricDamageMultiplier;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(ElectricDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(ElectricDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(ElectricDamageMultiplier);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, ElectricDamageMultiplier);

	/** Life Magic Damage Multiplier GameplayAttribute */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData LifeDamageMultiplier;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(LifeDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(LifeDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(LifeDamageMultiplier);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, LifeDamageMultiplier);

	/** Death Magic Damage Multiplier GameplayAttribute */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData DeathDamageMultiplier;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(DeathDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(DeathDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(DeathDamageMultiplier);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, DeathDamageMultiplier);

	/** Melee Damage Multiplier GameplayAttribute */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData MeleeDamageMultiplier;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MeleeDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MeleeDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MeleeDamageMultiplier);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, MeleeDamageMultiplier);

	/** Ranged Damage Multiplier GameplayAttribute */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FGameplayAttributeData RangedDamageMultiplier;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(RangedDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(RangedDamageMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(RangedDamageMultiplier);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWAttributeSetBase, RangedDamageMultiplier);

protected:
	UFUNCTION()
	virtual void OnRep_EquippedSpells(const FGameplayAttributeData& OldEquippedSpells);
};
