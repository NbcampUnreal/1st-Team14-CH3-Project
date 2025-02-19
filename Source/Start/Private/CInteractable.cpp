
#include "CInteractable.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

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


	/*
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}
	if(InputComponent)
	{
		InputComponent->BindAction(EKeys::F, IE_Pressed, this, &ACInteractable::KeyPressedActivate);
	}
	*/

}

void ACInteractable::OnInteractableOverlap(UPrimitiveComponent* OverlapPrimitiveComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor && OtherActor->ActorHasTag("Player") || OtherComp && OtherComp->ComponentHasTag("Player")) //캐릭터로 캐스트되면
	{
		//위젯 활성화
		OverlapActivate(OtherActor);
		//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		//if (PlayerController)
		//{
		//	EnableInput(PlayerController);
		//}

		KeyPressedActivate(OtherActor);
	}
}

void ACInteractable::OnInteractableEndOverlap(UPrimitiveComponent* OverlapPrimitiveComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//위젯 비활성화
	//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//if (PlayerController)
	//{
	//	DisableInput(PlayerController);
	//}
}

void ACInteractable::OverlapActivate(AActor* Activator)
{
	//GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, FString::Printf(TEXT("CInteractableOverlap")));
	
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

