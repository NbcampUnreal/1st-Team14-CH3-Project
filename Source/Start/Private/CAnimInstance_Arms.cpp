// Fill out your "

#include "CAnimInstance_Arms.h"

#include "GameFramework/Character.h"

void UCAnimInstance_Arms::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter == nullptr)
		return;

	Weapon = Cast<UCWeaponComponent>(OwnerCharacter->GetComponentByClass(UCWeaponComponent::StaticClass()));
	Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance_Arms::OnWeaponTypeChanged);
}

void UCAnimInstance_Arms::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter == nullptr)
		return;
}

void UCAnimInstance_Arms::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}
