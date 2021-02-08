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
	AWCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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


	// -------------------------------------------
	// --- Gameplay Attribute OnChange Handlers --
	// -------------------------------------------
	
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

	/** Function to bind to Attribute Changed delegate. Calls OnMoveSpeedChanged. */
	void OnMoveSpeedChangedInternal(const FOnAttributeChangeData& Data);

	/** Blueprint-implementable On Health Changed event. Used to send information to the UI about current health. */
	UFUNCTION(BlueprintImplementableEvent, Category="Abilities|Attributes")
    void OnMoveSpeedChanged(float OldValue, float NewValue);

	
	/** Grants an ability at the given level */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
    void GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputCode);


	// -------------------------------------------
	// ------------ Damage calculation -----------
	// -------------------------------------------

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

	/** Character Attribute Set. UPROPERTY macro required for reflection. */
	UPROPERTY()
	const class UWAttributeSetBase* AttributeSetBase;
	

	// -------------------------------------------
	// ------------ Initial Abilities ------------
	// -------------------------------------------
	
	/** Passive gameplay effects applied on creation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;

	/** Passive gameplay effects applied on creation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> InitialGameplayAbilities;

	/** Apply the startup gameplay abilities and effects */
	void InitializeAbilitiesAndEffects();

	/** If true we have initialized our abilities */
	int32 AbilitiesInitialized;


	// -------------------------------------------
	// --------------- PlayerColor ---------------
	// -------------------------------------------

	/** The Players Skin Color */
	//UPROPERTY(BlueprintReadOnly, Category = "PlayerColor", ReplicatedUsing = OnRep_PlayerColor)
	//EWPlayerColor PlayerColor;
	
	/** Notify function if the players color changed */
	///UFUNCTION(BlueprintNativeEvent, Category="PlayerColor")
    //void OnRep_PlayerColor();

	/** RPC to set the color of a player */
	//UFUNCTION( Server, Reliable )
	//void ServerSetUniqueWitchColor();

	
	// -------------------------------------------
	// ---------- Cache Gameplay Tags ------------
	// -------------------------------------------

	/**  We cache some tags so we can refer to them at will */
	static uint8 TagsCached;								// Whether we cached the tags or not
	static void CacheGameplayTags();						// Setup the cache data
	static FGameplayTag TagSetEquippedSkills;				// The ability to switch equipped spells
	static FGameplayTag TagSpellUnknown;					// General error tag
	static TMap<uint16, FGameplayTagContainer> SpellTags;	// Abilities representing spells for the player to cast


	// -------------------------------------------
	// --------------- Statistics ----------------
	// -------------------------------------------

	// Some statistics we track while a level is playing
	float StatDamageToEnemies = 0;
	float StatDamageToFriends = 0;
	int32 StatKilledEnemies = 0;
	int32 StatKilledFriends = 0;
	int32 StatDeaths = 0;

	/** Increase the damage to enemies by the given value */
	UFUNCTION(BlueprintCallable, Category="Statistics")
	void StatIncreaseDamageToEnemies(float Damage);

	/** Increase the damage to friends by the given value */
	UFUNCTION(BlueprintCallable, Category="Statistics")
	void StatIncreaseDamageToFriends(float Damage);

	/** Increase the killed enemies count by one */
	UFUNCTION(BlueprintCallable, Category="Statistics")
	void StatIncrementKilledEnemies();

	/** Increase the killed friends count by one */
	UFUNCTION(BlueprintCallable, Category="Statistics")
	void StatIncrementKilledFriends();

	/** Increase the death count by one */
	UFUNCTION(BlueprintCallable, Category="Statistics")
	void StatIncrementDeaths();

	/** Return all available statistics */
	UFUNCTION(BlueprintCallable, Category="Statistics")
	void StatGetAll(float& DamageToEnemies, float& DamageToFriends, int32& KilledEnemies, int32& KilledFriends, int32& Deaths);
};