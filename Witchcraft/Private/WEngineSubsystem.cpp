// All rights reserved Howler Games LLC


#include "WEngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void UWEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}
