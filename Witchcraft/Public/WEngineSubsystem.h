// All rights reserved Howler Games LLC

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "WEngineSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class WITCHCRAFT_API UWEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};