// All rights reserved Howler Games LLC

#pragma once

#include "GameplayEffectTypes.h"
#include "WTypes.generated.h"

class UGameplayEffect;


/** What type of magical element? */
UENUM(BlueprintType)
enum class EWMagicElement : uint8
{
	None		= 0	UMETA(DisplayName = "None"),
    Fire		= 1	UMETA(DisplayName = "Fire"),
    Water		= 2	UMETA(DisplayName = "Water"),
    Ice			= 3	UMETA(DisplayName = "Ice"),
    Wind		= 4	UMETA(DisplayName = "Wind"),
    Electric	= 5	UMETA(DisplayName = "Electric"),
    Life		= 6	UMETA(DisplayName = "Life"),
    Death		= 7	UMETA(DisplayName = "Death"),
};

/** What type of magic? */
UENUM(BlueprintType)
enum class EWMagicType : uint8
{
	None		= 0	UMETA(DisplayName = "None"),
    Projectile	= 1	UMETA(DisplayName = "Projectile"),
    Beam		= 2	UMETA(DisplayName = "Beam"),
    Nova		= 3	UMETA(DisplayName = "Nova"),
    Shield		= 4	UMETA(DisplayName = "Shield"),
    Ultra		= 5	UMETA(DisplayName = "Ultra"),
};

/** What hand? */
UENUM(BlueprintType)
enum class EWHand : uint8
{
	Left	= 0		UMETA(DisplayName = "Left"),
    Right	= 1		UMETA(DisplayName = "Right"),
};

/** What input action is the player modifying? */
UENUM(BlueprintType)
enum class EWInputSwitchingAction : uint8
{
	None			= 0		UMETA(DisplayName = "None"),
    MagicElement	= 1		UMETA(DisplayName = "MagicElement"),
    MagicType		= 2		UMETA(DisplayName = "MagicType"),
};

/** What input action is the player modifying? */
UENUM(BlueprintType)
enum class EWEffectSpecWrapperType : uint8
{
	ModNothing	= 0		UMETA(DisplayName = "ModNothing"),
    ModHealth	= 1		UMETA(DisplayName = "ModHealth"),
};

/** These are the possible player colors */
UENUM(BlueprintType)
enum class EWPlayerColor : uint8
{
	None	= 0		UMETA(DisplayName = "None"),
	Blue	= 1		UMETA(DisplayName = "Blue"),
    Red		= 2		UMETA(DisplayName = "Red"),
	Purple	= 3		UMETA(DisplayName = "Purple"),
	Green	= 4		UMETA(DisplayName = "Green"),
};

/** Struct representing a wrapped gameplay effect spec */
USTRUCT(BlueprintType)
struct WITCHCRAFT_API FWEffectSpecWrapper
{
	GENERATED_BODY()

	/** What type of wrapper is this? We will use this to call AssignTagSetByCallerMagnitude */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectWrapper)
	EWEffectSpecWrapperType WrapperType;

	/** The Gameplay Effect class that the spec will be constructed for */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectWrapper)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	/** The Gameplay Effect spec is constructed on BeginPlay in c++ from the provided class */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayEffectWrapper)
	FGameplayEffectSpecHandle GameplayEffectSpec;
};