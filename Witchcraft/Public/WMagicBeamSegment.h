// All rights reserved Howler Games LLC

#pragma once

#include "CoreMinimal.h"
#include "WMagicActor.h"
#include "WMagicBeamSegment.generated.h"

class AWMagicBeamEmitter;

/**
 * 
 */
UCLASS()
class WITCHCRAFT_API AWMagicBeamSegment : public AWMagicActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = BeamSegment, Meta = (ExposeOnSpawn="true"))
	AWMagicBeamEmitter* Emitter;
	
	UPROPERTY(BlueprintReadWrite, Category = BeamSegment, Meta = (ExposeOnSpawn="true"))
	int32 Index;
	
	UPROPERTY(BlueprintReadWrite, Category = BeamSegment, Meta = (ExposeOnSpawn="true"))
	FVector LocationFrom;

	UPROPERTY(BlueprintReadWrite, Category = BeamSegment, Meta = (ExposeOnSpawn="true"))
	FVector LocationTo;

	UPROPERTY(BlueprintReadWrite, Category = BeamSegment)
	float Length;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = BeamSegment)
	void BeamSegmentSetup(const FVector& InFrom, const FVector& InTo);

	UFUNCTION(BlueprintCallable, Category = BeamSegment)
	void BeamSegmentDeactivate();

	UFUNCTION(BlueprintCallable, Category = BeamSegment)
	void BeamSegmentActivate();
};
