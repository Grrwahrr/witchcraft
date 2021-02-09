// All rights reserved Howler Games LLC

#include "WAttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"


void UWAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMoveSpeedAttribute())
	{
		// Clamp the movement speed
		NewValue = FMath::Clamp(NewValue, 150.f, 1000.f);
	}
}

//The PostGameplayEffectExecute function handles any logic that should happen after a GameplayEffect is applied to the Ability System Component that owns this AttributeSet. In effect it "listens" for changes to happen.
void UWAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//Clamping Health based on Max Health.
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetHealthMaximum()));
	}
}

void UWAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UWAttributeSetBase, EquippedSpells, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWAttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
	/*
	 * REPTNOTIFY_Always tells the OnRep function to trigger if the local value is already equal
	 * to the value being repped down from the Server (due to prediction). By default it won't
	 * trigger the OnRep function if the local value is the same as the value being repped down from the Server.
	 **/
}

void UWAttributeSetBase::OnRep_EquippedSpells(const FGameplayAttributeData& OldEquippedSpells)
{
	// WCharacter can listen to changes to this attribute and do additional things
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWAttributeSetBase, EquippedSpells, OldEquippedSpells);
}

void UWAttributeSetBase::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	// WCharacter can listen to changes to this attribute and do additional things
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWAttributeSetBase, MoveSpeed, OldMoveSpeed);
}