// All rights reserved Howler Games LLC


#include "WMagicBeamSegment.h"

void AWMagicBeamSegment::BeamSegmentDeactivate()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AWMagicBeamSegment::BeamSegmentActivate()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}
