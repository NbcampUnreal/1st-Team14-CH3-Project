
#include "CInteractable.h"
#include "Components/InputComponent.h"
#include "CPlayerController.h"
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
}

void ACInteractable::OnInteractableOverlap(UPrimitiveComponent* OverlapPrimitiveComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player") || OtherComp && OtherComp->ComponentHasTag("Player")) //ĳ���ͷ� ĳ��Ʈ�Ǹ�
	{
		//���� Ȱ��ȭ
		OverlapActivate(OtherActor);

		//Ű�� ������
		KeyPressedActivate(OtherActor); 
	}
}

void ACInteractable::OnInteractableEndOverlap(UPrimitiveComponent* OverlapPrimitiveComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//���� ��Ȱ��ȭ
}

void ACInteractable::OverlapActivate(AActor* Activator)
{
	
}

void ACInteractable::KeyPressedActivate(AActor* Activator)
{
}

void ACInteractable::DestroyInteractable()
{
	Destroy();
}

