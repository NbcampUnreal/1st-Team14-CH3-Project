#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CEnemyAIController.generated.h"

struct FAIStimulus;

UCLASS()
class START_API ACEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACEnemyAIController();

protected:
	UPROPERTY(VisibleAnywhere, Category="AI")
	class UAIPerceptionComponent* AIPerception;
	UPROPERTY(VisibleAnywhere, Category="AI")
	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
