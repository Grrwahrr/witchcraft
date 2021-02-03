// All rights reserved Howler Games LLC


#include "WCharacter.h"

#include "AbilitySystemComponent.h"
#include "GeneratedCodeHelpers.h"
#include "WAttributeSetBase.h"

uint8 AWCharacter::TagsCached = 0;
FGameplayTag AWCharacter::TagSetEquippedSkills;
FGameplayTag AWCharacter::TagSpellUnknown;
TMap<uint16, FGameplayTagContainer> AWCharacter::SpellTags;

// Sets default values
AWCharacter::AWCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add an Ability System Component to this character
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability System Component"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	// Adding it as a sub-object of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	//AttributeSetBase = CreateDefaultSubobject<UWAttributeSetBase>(TEXT("AttributeSetBase"));
	//NOTE: This didnt actually work - GETTER would alwaays be 0
	// I Use AttributeSetBase = AbilitySystemComponent->GetSet<UWAttributeSetBase>(); instead



	AbilitiesInitialized = false;
}

/** Called when the game starts or when spawned */
void AWCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initializes the attribute set using a data table.
	if (IsValid(AbilitySystemComponent))
	{
		//this or AttributeSetBase = CreateDefaultSubobject<UWAttributeSetBase>(TEXT("AttributeSetBase"));
		AttributeSetBase = AbilitySystemComponent->GetSet<UWAttributeSetBase>();
		// Apparently I might have to reset values on respawn
		//AttributeSet->InitHealth(AttributeSet->GetHealthMaximum());

		// GetGameplayAttributeValueChangedDelegate will enable you to bind delegates without programming them manually
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AWCharacter::OnHealthChangedInternal);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetEquippedSpellsAttribute()).AddUObject(this, &AWCharacter::OnEquippedSpellsChangedInternal);

		// Cache the tag we use for equipping skills
		CacheGameplayTags();
	}
}

void AWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initializes the attribute set using a data table.
	if (IsValid(AbilitySystemComponent))
	{
		// Set actor info
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// Grant initial abilities and apply effects
		InitializeAbilitiesAndEffects();
	}

	// ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
	SetOwner(NewController);
}

void AWCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Our controller changed, must update ActorInfo on AbilitySystemComponent
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void AWCharacter::StatIncreaseDamageToEnemies(float Damage)
{
	StatDamageToEnemies += Damage;
}

void AWCharacter::StatIncreaseDamageToFriends(float Damage)
{
	StatDamageToFriends += Damage;
}

void AWCharacter::StatIncrementKilledEnemies()
{
	StatKilledEnemies++;
}

void AWCharacter::StatIncrementKilledFriends()
{
	StatKilledFriends++;
}

void AWCharacter::StatIncrementDeaths()
{
	StatDeaths++;
}

void AWCharacter::StatGetAll(float& DamageToEnemies, float& DamageToFriends, int32& KilledEnemies, int32& KilledFriends, int32& Deaths)
{
	DamageToEnemies = StatDamageToEnemies;
	DamageToFriends = StatDamageToFriends;
	KilledEnemies = StatKilledEnemies;
	KilledFriends = StatKilledFriends;
	Deaths = StatDeaths;
}



// Called every frame
void AWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



// --------------------------------------------

void AWCharacter::InitializeAbilitiesAndEffects()
{
	check(AbilitySystemComponent);
	
	// Grant abilities, but only on the server	
	if (GetLocalRole() == ROLE_Authority && !AbilitiesInitialized )
	{
		for (TSubclassOf<UGameplayAbility>& StartupAbility : InitialGameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
		}

		// Now apply passives
		for (TSubclassOf<UGameplayEffect>& GameplayEffect : InitialGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}

		AbilitiesInitialized = true;
	}
}


//Create an instance of an Ability and add it to the Ability System Component.
void AWCharacter::GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputCode)
{
	if (GetLocalRole() == ROLE_Authority && IsValid(AbilitySystemComponent) && IsValid(AbilityClass))
	{
		UGameplayAbility* Ability = AbilityClass->GetDefaultObject<UGameplayAbility>();

		if (IsValid(Ability))
		{
			// create the new ability spec struct. Ability specs contain metadata about the ability, like what level they're set to, as well as a reference to the ability.
			const FGameplayAbilitySpec AbilitySpec(
                Ability,
                Level,
                InputCode
            );

			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

void AWCharacter::CalculateIncomingSpellDamage(AWMagicActor* WMagicActor, float& ActualDamage) const
{
	ActualDamage = 0;
	
	if (!IsValid(WMagicActor))
	{
		return;
	}
	
	ActualDamage = FMath::FRandRange(WMagicActor->DamageMin, WMagicActor->DamageMax);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Float Min %f"), WMagicActor->DamageMin));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Float Max %f"), WMagicActor->DamageMax));
	
	if (IsValid(AttributeSetBase))
	{
		switch(WMagicActor->MagicElement)
		{
			case EWMagicElement::Fire:		ActualDamage *= AttributeSetBase->GetFireDamageMultiplier();
			case EWMagicElement::Water:		ActualDamage *= AttributeSetBase->GetWaterDamageMultiplier();
			case EWMagicElement::Ice:		ActualDamage *= AttributeSetBase->GetIceDamageMultiplier();
			case EWMagicElement::Wind:		ActualDamage *= AttributeSetBase->GetWindDamageMultiplier();
			case EWMagicElement::Electric:	ActualDamage *= AttributeSetBase->GetElectricDamageMultiplier();
			case EWMagicElement::Life:		ActualDamage *= AttributeSetBase->GetLifeDamageMultiplier();
			case EWMagicElement::Death:		ActualDamage *= AttributeSetBase->GetDeathDamageMultiplier();
			default:						break;
		}
	}
}

void AWCharacter::CalculateIncomingMeleeDamage(AWMagicActor* WMagicActor, float& ActualDamage) const
{
	ActualDamage = 1;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Call to unimplemented CalculateIncomingMeleeDamage %f"), 0.0f));
}

void AWCharacter::CalculateIncomingRangedDamage(AWMagicActor* WMagicActor, float& ActualDamage) const
{
	ActualDamage = 1;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Call to unimplemented CalculateIncomingRangedDamage %f"), 0.0f));
}

void AWCharacter::OnHealthChangedInternal(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

void AWCharacter::OnEquippedSpellsChangedInternal(const FOnAttributeChangeData& Data)
{
	FFloat2Bytes F2BOld, F2BNew;
	F2BOld.F = Data.OldValue;
	F2BNew.F = Data.NewValue;

	// LeftMagicType,LeftMagicElement,RightMagicType,RightMagicElement
	OnEquippedSpellsChanged(
		static_cast<EWMagicType>(F2BOld.B[0]),
		static_cast<EWMagicElement>(F2BOld.B[1]),
		static_cast<EWMagicType>(F2BOld.B[2]),
		static_cast<EWMagicElement>(F2BOld.B[3]),
		static_cast<EWMagicType>(F2BNew.B[0]),
        static_cast<EWMagicElement>(F2BNew.B[1]),
        static_cast<EWMagicType>(F2BNew.B[2]),
        static_cast<EWMagicElement>(F2BNew.B[3])
    );
}

//Getters for Health
float AWCharacter::GetHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetHealth();
	}

	return -1.0f;
}

void AWCharacter::GetEquippedMagic(EWMagicType& LeftMagicType, EWMagicElement& LeftMagicElement, EWMagicType& RightMagicType, EWMagicElement& RightMagicElement) const
{
	if (IsValid(AttributeSetBase))
	{
		FFloat2Bytes F2B;
		F2B.F = AttributeSetBase->GetEquippedSpells(); // LeftMagicType,LeftMagicElement,RightMagicType,RightMagicElement
		LeftMagicType		= static_cast<EWMagicType>(F2B.B[0]);
		LeftMagicElement	= static_cast<EWMagicElement>(F2B.B[1]);
		RightMagicType		= static_cast<EWMagicType>(F2B.B[2]);
		RightMagicElement	= static_cast<EWMagicElement>(F2B.B[3]);
	}

	LeftMagicType		= EWMagicType::None;
	LeftMagicElement	= EWMagicElement::None;
	RightMagicType		= EWMagicType::None;
	RightMagicElement	= EWMagicElement::None;
}

EWMagicElement AWCharacter::GetMagicElementOnHand(const EWHand Hand) const
{
	if (IsValid(AttributeSetBase))
	{
		FFloat2Bytes F2B;
		F2B.F = AttributeSetBase->GetEquippedSpells(); // LeftMagicType,LeftMagicElement,RightMagicType,RightMagicElement
		switch (Hand)
		{
			case EWHand::Left:
				return static_cast<EWMagicElement>(F2B.B[1]);
			case EWHand::Right:
				return static_cast<EWMagicElement>(F2B.B[3]);
		}
	}
	return EWMagicElement::None;
}

FGameplayTagContainer AWCharacter::GetTagForGameplayAbilityOnHand(EWHand Hand) const
{
	if (IsValid(AttributeSetBase))
	{
		FFloat2Bytes F2B;
		F2B.F = AttributeSetBase->GetEquippedSpells(); // LeftMagicType,LeftMagicElement,RightMagicType,RightMagicElement
		uint16 FindByKey = 0;

		switch (Hand)
		{
			case EWHand::Left:
				FindByKey = (F2B.B[0] << 8) + static_cast<uint8>(Hand);
				break;
			case EWHand::Right:
				FindByKey = (F2B.B[2] << 8) + static_cast<uint8>(Hand);
				break;
		}

		// Lookup the correct tag for this hand and element
		const FGameplayTagContainer* FoundItem = SpellTags.Find(FindByKey);

		// Return a copy of that tag
		if (FoundItem != nullptr)
		{
			return *SpellTags.Find(FindByKey);
		}
	}
	
	return TagSpellUnknown.GetSingleTagContainer();
}

void AWCharacter::CacheGameplayTags()
{
	// We only need to do this once
	if ( TagsCached == 1 )
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Already cached %f"), 0.f));
		return;
	}
	TagsCached = 1;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Caching tags %f"), 0.f));
	
	TagSetEquippedSkills	= FGameplayTag::RequestGameplayTag(FName("Event.SetEquippedSpells"));
	TagSpellUnknown			= FGameplayTag::RequestGameplayTag(FName("Spell.Unknown"));
	const auto TagLeftHand	= FGameplayTag::RequestGameplayTag(FName("Hand.Left"));
	const auto TagRightHand	= FGameplayTag::RequestGameplayTag(FName("Hand.Right"));

	TArray<FGameplayTag> LeftBeam;
	LeftBeam.Add(TagLeftHand);
	LeftBeam.Add(FGameplayTag::RequestGameplayTag(FName("Spell.Beam")));
	
	TArray<FGameplayTag> LeftNone;
	LeftNone.Add(TagLeftHand);
	LeftNone.Add(FGameplayTag::RequestGameplayTag(FName("Spell.None")));
	
	TArray<FGameplayTag> LeftNova;
	LeftNova.Add(TagLeftHand);
	LeftNova.Add(FGameplayTag::RequestGameplayTag(FName("Spell.Nova")));
	
	TArray<FGameplayTag> LeftProjectile;
	LeftProjectile.Add(TagLeftHand);
	LeftProjectile.Add(FGameplayTag::RequestGameplayTag(FName("Spell.Projectile")));

	TArray<FGameplayTag> LeftShield;
	LeftShield.Add(TagLeftHand);
	LeftShield.Add(FGameplayTag::RequestGameplayTag(FName("Spell.Shield")));
	
	TArray<FGameplayTag> LeftUltra;
	LeftUltra.Add(TagLeftHand);
	LeftUltra.Add(FGameplayTag::RequestGameplayTag(FName("Spell.Ultra")));
	
	
	SpellTags.Add((static_cast<uint8>(EWMagicType::Beam) << 8) + static_cast<uint8>(EWHand::Left), FGameplayTagContainer::CreateFromArray(LeftBeam));
	SpellTags.Add((static_cast<uint8>(EWMagicType::None) << 8) + static_cast<uint8>(EWHand::Left), FGameplayTagContainer::CreateFromArray(LeftNone));
	SpellTags.Add((static_cast<uint8>(EWMagicType::Nova) << 8) + static_cast<uint8>(EWHand::Left), FGameplayTagContainer::CreateFromArray(LeftNova));
	SpellTags.Add((static_cast<uint8>(EWMagicType::Projectile) << 8) + static_cast<uint8>(EWHand::Left), FGameplayTagContainer::CreateFromArray(LeftProjectile));
	SpellTags.Add((static_cast<uint8>(EWMagicType::Shield) << 8) + static_cast<uint8>(EWHand::Left), FGameplayTagContainer::CreateFromArray(LeftShield));
	SpellTags.Add((static_cast<uint8>(EWMagicType::Ultra) << 8) + static_cast<uint8>(EWHand::Left), FGameplayTagContainer::CreateFromArray(LeftUltra));


	TArray<FGameplayTag> RightBeam;
	RightBeam.Add(TagRightHand);
	RightBeam.Add(FGameplayTag::RequestGameplayTag(FName("Spell.Beam")));
	
	TArray<FGameplayTag> RightNone;
	RightNone.Add(TagRightHand);
	RightNone.Add(FGameplayTag::RequestGameplayTag(FName("Spell.None")));
	
	TArray<FGameplayTag> RightNova;
	RightNova.Add(TagRightHand);
	RightNova.Add(FGameplayTag::RequestGameplayTag(FName("Spell.Nova")));
	
	TArray<FGameplayTag> RightProjectile;
	RightProjectile.Add(TagRightHand);
	RightProjectile.Add(FGameplayTag::RequestGameplayTag(FName("Spell.Projectile")));

	TArray<FGameplayTag> RightShield;
	RightShield.Add(TagRightHand);
	RightShield.Add(FGameplayTag::RequestGameplayTag(FName("Spell.Shield")));
	
	TArray<FGameplayTag> RightUltra;
	RightUltra.Add(TagRightHand);
	RightUltra.Add(FGameplayTag::RequestGameplayTag(FName("Spell.Ultra")));
	
	SpellTags.Add((static_cast<uint8>(EWMagicType::Beam) << 8) + static_cast<uint8>(EWHand::Right), FGameplayTagContainer::CreateFromArray(RightBeam));
	SpellTags.Add((static_cast<uint8>(EWMagicType::None) << 8) + static_cast<uint8>(EWHand::Right), FGameplayTagContainer::CreateFromArray(RightNone));
	SpellTags.Add((static_cast<uint8>(EWMagicType::Nova) << 8) + static_cast<uint8>(EWHand::Right), FGameplayTagContainer::CreateFromArray(RightNova));
	SpellTags.Add((static_cast<uint8>(EWMagicType::Projectile) << 8) + static_cast<uint8>(EWHand::Right), FGameplayTagContainer::CreateFromArray(RightProjectile));
	SpellTags.Add((static_cast<uint8>(EWMagicType::Shield) << 8) + static_cast<uint8>(EWHand::Right), FGameplayTagContainer::CreateFromArray(RightShield));
	SpellTags.Add((static_cast<uint8>(EWMagicType::Ultra) << 8) + static_cast<uint8>(EWHand::Right), FGameplayTagContainer::CreateFromArray(RightUltra));
}

void AWCharacter::SetLeftHandMagicType(EWMagicType MagicType) const
{
	if (IsValid(AttributeSetBase))
	{
		FFloat2Bytes F2B;
		F2B.F		= AttributeSetBase->GetEquippedSpells(); // LeftMagicType,LeftMagicElement,RightMagicType,RightMagicElement
		F2B.B[0]	= static_cast<uint8>(MagicType);
		CallSetEquippedSpellsByEvent(F2B.F);
	}
}

void AWCharacter::SetLeftHandMagicElement(EWMagicElement MagicElement) const
{
	if (IsValid(AttributeSetBase))
	{
		FFloat2Bytes F2B;
		F2B.F		= AttributeSetBase->GetEquippedSpells(); // LeftMagicType,LeftMagicElement,RightMagicType,RightMagicElement
		F2B.B[1]	= static_cast<uint8>(MagicElement);
		CallSetEquippedSpellsByEvent(F2B.F);
	}
}

void AWCharacter::SetRightHandMagicType(EWMagicType MagicType) const
{
	if (IsValid(AttributeSetBase))
	{
		FFloat2Bytes F2B;
		F2B.F		= AttributeSetBase->GetEquippedSpells(); // LeftMagicType,LeftMagicElement,RightMagicType,RightMagicElement
		F2B.B[2]	= static_cast<uint8>(MagicType);
		CallSetEquippedSpellsByEvent(F2B.F);
	}
}

void AWCharacter::SetRightHandMagicElement(EWMagicElement MagicElement) const
{
	if (IsValid(AttributeSetBase))
	{
		FFloat2Bytes F2B;
		F2B.F		= AttributeSetBase->GetEquippedSpells(); // LeftMagicType,LeftMagicElement,RightMagicType,RightMagicElement
		F2B.B[3]	= static_cast<uint8>(MagicElement);
		CallSetEquippedSpellsByEvent(F2B.F);
	}
}

void AWCharacter::CallSetEquippedSpellsByEvent(float EncodedValue) const
{
	// Create the event data with our custom magnitude
	FGameplayEventData EventData;
	EventData.EventMagnitude = EncodedValue;

	// Call the SetEquippedSkills event
	AbilitySystemComponent->HandleGameplayEvent(TagSetEquippedSkills, &EventData);
}