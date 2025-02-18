
#include "CInteractable.h"
#include "Components/BoxComponent.h"

ACInteractable::ACInteractable()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	InteractableCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractableCollision"));
	InteractableCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	InteractableCollision->SetupAttachment(Scene);

	InteractableCollision->OnComponentBeginOverlap.AddDynamic(this, &ACInteractable::OnInteractableOverlap);
	InteractableCollision->OnComponentEndOverlap.AddDynamic(this, &ACInteractable::OnInteractableEndOverlap);

}

void ACInteractable::OnInteractableOverlap(UPrimitiveComponent* OverlapPrimitiveComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor && OtherActor->ActorHasTag("Player")) //캐릭터로 캐스트되면
	{
		
		OverlapActivate(OtherActor);
		//키를 누르면
		KeyPressedActivate(OtherActor);
		DestroyInteractable();
	}
}

void ACInteractable::OnInteractableEndOverlap(UPrimitiveComponent* OverlapPrimitiveComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void ACInteractable::OverlapActivate(AActor* Activator)
{
	GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, FString::Printf(TEXT("CInteractableOverlap")));
	// F키 위젯 띄우기
}

void ACInteractable::KeyPressedActivate(AActor* Activator)
{
	GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, FString::Printf(TEXT("CInteractableKeyPressed")));
	//상호작용
}

void ACInteractable::DestroyInteractable()
{
	Destroy();
}

