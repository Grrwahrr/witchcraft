// All rights reserved Howler Games LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "WMagicActor.h"

#include "WCharacter.generated.h"

class UGameplayEffect;

/** This is a union used to access the 4 bytes of a float and modify those individually */
union FFloat2Bytes { float F; uint8 B[sizeof(float)]; };

UCLASS()
class WITCHCRAFT_API AWCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;

	/** Ability System Component. Required to use Gameplay Attributes and Gameplay Abilities. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	class UAbilitySystemComponent* AbilitySystemComponent;

	/** Ability System Interface getter. */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

	/** Blueprint-accessible getter for GAS attributes */
	UFUNCTION(BlueprintPure, Category = "Abilities|Attributes")
    float GetHealth() const;

	/** The magic element and type currently assigned to the left and right hand */
	UFUNCTION(BlueprintPure, Category = "Abilities|Attributes")
	void GetEquippedMagic(EWMagicType& LeftMagicType, EWMagicElement& LeftMagicElement, EWMagicType& RightMagicType, EWMagicElement& RightMagicElement) const;

	/** The magic element assigned to the given hand */
	UFUNCTION(BlueprintPure, Category = "Abilities|Attributes")
    EWMagicElement GetMagicElementOnHand(EWHand Hand) const;

	/** The tag for the ability cast from the given hand */
	UFUNCTION(BlueprintPure, Category = "Abilities|Attributes")
    FGameplayTagContainer GetTagForGameplayAbilityOnHand(EWHand Hand) const;




	/** Creates an event that activates the Gameplay Ability to update the equipped magic */
	UFUNCTION(BlueprintCallable, Category = "Abilities|Attributes")
    void SetLeftHandMagicType(EWMagicType MagicType) const;
	
	/** Creates an event that activates the Gameplay Ability to update the equipped magic */
	UFUNCTION(BlueprintCallable, Category = "Abilities|Attributes")
	void SetLeftHandMagicElement(EWMagicElement MagicElement) const;
	
	/** Creates an event that activates the Gameplay Ability to update the equipped magic */
	UFUNCTION(BlueprintCallable, Category = "Abilities|Attributes")
    void SetRightHandMagicType(EWMagicType MagicType) const;

	/** Creates an event that activates the Gameplay Ability to update the equipped magic */
	UFUNCTION(BlueprintCallable, Category = "Abilities|Attributes")
    void SetRightHandMagicElement(EWMagicElement MagicElement) const;

	/** Internal function that is called to execute the SetEquippedSpells Gameplay Ability by event */
	void CallSetEquippedSpellsByEvent(float EncodedValue) const;


	
	/** Function to bind to Attribute Changed delegate. Calls On Health Changed. */
	void OnHealthChangedInternal(const FOnAttributeChangeData& Data);

	/** Blueprint-implementable On Health Changed event. Used to send information to the UI about current health. */
	UFUNCTION(BlueprintImplementableEvent, Category="Abilities|Attributes")
    void OnHealthChanged(float OldValue, float NewValue);

	/** Function to bind to Attribute Changed delegate. Calls On Equipped Spells Changed. */
	void OnEquippedSpellsChangedInternal(const FOnAttributeChangeData& Data);

	/** Blueprint-implementable On Equipped Spell Changed event. Used to update FX on players hands. */
	UFUNCTION(BlueprintImplementableEvent, Category="Abilities|Attributes")
    void OnEquippedSpellsChanged(EWMagicType OldLeftMagicType, EWMagicElement OldLeftMagicElement, EWMagicType OldRightMagicType, EWMagicElement OldRightMagicElement, EWMagicType NewLeftMagicType, EWMagicElement NewLeftMagicElement, EWMagicType NewRightMagicType, EWMagicElement NewRightMagicElement);

	/** Grants an ability at the given level */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
    void GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputCode);

	/** Blueprint callable function to calculate the damaged inflicted by a spell (WMagicActor) */
	UFUNCTION(BlueprintPure, Category="Abilities|Attributes")
    void CalculateIncomingSpellDamage(AWMagicActor* WMagicActor, float& ActualDamage) const;

	/** Blueprint callable function to calculate the damaged inflicted by a melee attack */
	UFUNCTION(BlueprintPure, Category="Abilities|Attributes")
    void CalculateIncomingMeleeDamage(AWMagicActor* WMagicActor, float& ActualDamage) const;

	/** Blueprint callable function to calculate the damaged inflicted by a ranged attack */
	UFUNCTION(BlueprintPure, Category="Abilities|Attributes")
    void CalculateIncomingRangedDamage(AWMagicActor* WMagicActor, float& ActualDamage) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Apply the startup gameplay abilities and effects */
	void InitializeAbilitiesAndEffects();

	/** If true we have initialized our abilities */
	int32 AbilitiesInitialized;

	/**  Cache this tag so we can refer to it when required */
	static FGameplayTag TagSetEquippedSkills;

	static FGameplayTag TagSpellUnknown;

	static uint8 TagsCached;

	/** Cache the tags for each spell the player can cast */
	static TMap<uint16, FGameplayTagContainer> SpellTags;

	/** Helper function used to create a tag map */
	static void CacheGameplayTags();

	/** Character Attribute Set. UPROPERTY macro required for reflection. */
	UPROPERTY()
	const class UWAttributeSetBase* AttributeSetBase;

	/** Passive gameplay effects applied on creation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;

	/** Passive gameplay effects applied on creation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> InitialGameplayAbilities;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
