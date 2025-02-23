// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeapon.h"
#include "CCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CCameraComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"

void FWeaponAimData::SetData(class ACCharacter* InOwner)
{
	USpringArmComponent* springArm = Cast<USpringArmComponent>(InOwner->GetComponentByClass(USpringArmComponent::StaticClass()));
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;
}

void FWeaponAimData::SetDataByNoneCurve(class ACCharacter* InOwner)
{
	USpringArmComponent* springArm = Cast<USpringArmComponent>(InOwner->GetComponentByClass(USpringArmComponent::StaticClass()));
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;

	UCameraComponent* camera = Cast<UCameraComponent>(InOwner->GetComponentByClass(UCameraComponent::StaticClass()));
	camera->FieldOfView = FieldOfView;
}

/////////////////////////
ACWeapon::ACWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);
	
	
	/*Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Time Line"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Script/Engine.CurveFloat'/Game/Blueprints/Weapon/Curve_Aim.Curve_Aim'"));
	if (curve.Object != nullptr)
		AimCurve = curve.Object;*/
}

// Called when the game starts or when spawned
void ACWeapon::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
		return;
	
	if (HolsterSocketName.IsValid() == true)
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative,true), HolsterSocketName);


	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	Camera = Cast<UCCameraComponent>(OwnerCharacter->GetComponentByClass(UCCameraComponent::StaticClass()));
	//BaseData.SetDataByNoneCurve(OwnerCharacter);
	/*if (AimCurve != nullptr)
	{
		FOnTimelineFloat timeline;
		timeline.BindUFunction(this,"OnAiming");
		Timeline->AddInterpFloat(AimCurve,timeline);
		Timeline->SetLooping(false);
		Timeline->SetPlayRate(AimSpeed);
	}*/
}

// Called every frame
void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACWeapon::CanEquip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;
	
	return !b;
}

void ACWeapon::Equip()
{
	bEquipping = true;
	if(State == nullptr)
		return;
	if(Camera == nullptr)
		return;

	State->SetEquipMode();
	Camera->EnableControlRotation();

	if (EquipMontage == nullptr)
	{
		BeginEquip();
		EndEquip();
		return;
	}
	
	OwnerCharacter->PlayAnimMontage(EquipMontage,Equip_PlayRate);
}

void ACWeapon::BeginEquip()
{
	if (RightHandSokcetName.IsValid())
		AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true), RightHandSokcetName);
}

void ACWeapon::EndEquip()
{
	bEquipping =false;

	State->SetIdleMode();
}

bool ACWeapon::CanUnequip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;
	
	return !b;
}

void ACWeapon::Unequip()
{
	if (HolsterSocketName.IsValid() == true)
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative,true), HolsterSocketName);
	Camera->DisableControlRoation();
}

bool ACWeapon::CanFire()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;

	return !b;
}

void ACWeapon::BeginFire()
{
	bFiring = true;

	OnFireing();
}

void ACWeapon::EndFire()
{
	bFiring = false;
}

void ACWeapon::OnFireing()
{
	UCameraComponent* camera = Cast<UCameraComponent>(OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
	if (camera == nullptr)
		return;

	FVector direction = camera->GetForwardVector();
	FTransform transform = camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;
	FVector end = transform.GetLocation() + direction*HitDistance;
	if (Debug == true)	
		DrawDebugLine(GetWorld(),start,end,DebugColor,true,LifeTime);
}

//bool ACWeapon::CanAim()
//{
//	bool b = false;
//	b |= bEquipping;
//	b |= bReload;
//	b |= bFiring;
//
//	return !b;
//}

//void ACWeapon::BeginAim()
//{
//	bInAim = true;
//	if (AimCurve != nullptr)
//	{
//		Timeline->PlayFromStart();
//		AimData.SetData(OwnerCharacter);
//		return;
//	}
//	AimData.SetDataByNoneCurve(OwnerCharacter);
//}

//void ACWeapon::EndAim()
//{
//	if  (bInAim == false)
//		return;
//	bInAim = false;
//
//	if (AimCurve != nullptr)
//	{
//		Timeline->PlayFromStart();
//		BaseData.SetData(OwnerCharacter);
//		return;
//	}
//	BaseData.SetDataByNoneCurve(OwnerCharacter);
//}

//void ACWeapon::OnAiming(float Output)
//{
//	UCameraComponent* camera = Cast<UCameraComponent>(OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
//	UE_LOG(LogTemp, Error, TEXT("Test"));
//	camera->FieldOfView = FMath::Lerp(AimData.FieldOfView,BaseData.FieldOfView,Output);
//}