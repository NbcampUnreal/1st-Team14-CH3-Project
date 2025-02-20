
#include "PatrolPath.h"

APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

}

AActor* APatrolPath::GetWaypoint(int32 index) const
{
	if (Waypoints.IsValidIndex(index))
	{
		return Waypoints[index];
	}

	return nullptr;
}

int32 APatrolPath::Num() const
{
	return Waypoints.Num();
}
