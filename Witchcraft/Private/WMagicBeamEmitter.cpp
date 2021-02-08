// All rights reserved Howler Games LLC


#include "WMagicBeamEmitter.h"

void AWMagicBeamEmitter::BeamContinueTrace(uint8 Num, const FVector& EmitterLocation, const FVector& ForwardVector, float MaxDistance, ECollisionChannel TraceChannel, FHitResult& OutHit)
{
	// Where do we want to trace to?
	FVector TraceEnd = EmitterLocation + (ForwardVector * MaxDistance);

	// We cannot collide with our own beam
	TArray<AWMagicBeamSegment*> IgnoreActors;
	BeamSegments.GenerateValueArray(IgnoreActors);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(static_cast<TArray<AActor*>>(IgnoreActors));
	QueryParams.bDebugQuery = true;

	FHitResult TraceResult;
	ActorLineTraceSingle(TraceResult, EmitterLocation, TraceEnd, TraceChannel, QueryParams);
}