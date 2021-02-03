// All rights reserved Howler Games LLC

#include "WAttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "Engine/DemoNetDriver.h"

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
	/*
	 * REPTNOTIFY_Always tells the OnRep function to trigger if the local value is already equal
	 * to the value being repped down from the Server (due to prediction). By default it won't
	 * trigger the OnRep function if the local value is the same as the value being repped down from the Server.
	 **/
}

void UWAttributeSetBase::OnRep_EquippedSpells(const FGameplayAttributeData& OldEquippedSpells)
{
	// How can i do stuff in blueprint from this?
	//const float current = EquippedSpells.GetCurrentValue();
	//const float old = OldEquippedSpells.GetCurrentValue();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("OnRep_EquippedSpells %f, %f"), current, old));

	
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWAttributeSetBase, EquippedSpells, OldEquippedSpells);
}
