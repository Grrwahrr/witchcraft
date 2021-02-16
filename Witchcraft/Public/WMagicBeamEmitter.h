// All rights reserved Howler Games LLC

#pragma once

#include "CoreMinimal.h"
#include "WMagicActor.h"
#include "WMagicBeamSegment.h"

#include "WMagicBeamEmitter.generated.h"

/**
 * 
 */
UCLASS()
class WITCHCRAFT_API AWMagicBeamEmitter : public AWMagicActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = Beam)
	TMap<int32,AWMagicBeamSegment*> BeamSegments;

	UFUNCTION(BlueprintCallable, Category = Beam)
	void BeamContinueTrace(uint8 Num, const FVector& EmitterLocation, const FVector& ForwardVector, float MaxDistance, ECollisionChannel TraceChannel, FHitResult& OutHit);
};
