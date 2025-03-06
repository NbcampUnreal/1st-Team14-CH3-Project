
#include "CPatrolPath.h"

ACPatrolPath::ACPatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

}

AActor* ACPatrolPath::GetWaypoint(int32 index) const
{
	if (Waypoints.Num() > 0 && Waypoints.IsValidIndex(index))
	{
		return Waypoints[index];
	}

	return nullptr;
}

int32 ACPatrolPath::Num() const
{
	return Waypoints.Num();
}
