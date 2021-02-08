// All rights reserved Howler Games LLC

#pragma once

#include "CoreMinimal.h"
#include "WMagicActor.h"
#include "WMagicBeamSegment.generated.h"

/**
 * 
 */
UCLASS()
class WITCHCRAFT_API AWMagicBeamSegment : public AWMagicActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = BeamSegment)
	FVector LocationFrom;

	UPROPERTY(BlueprintReadWrite, Category = BeamSegment)
	FVector LocationTo;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = BeamSegment)
	void BeamSegmentSetup(const FVector& InFrom, const FVector& InTo);
};
