// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeapon.h"

#include "CBullet.h"
#include "CCharacter.h"
#include "CMagazine.h"
#include "CPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CCameraComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/DecalComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"
#include "Camera/CameraShakeBase.h"
#include "Engine/DamageEvents.h"

void FWeaponAimData::SetData(class ACCharacter* InOwner)
{
	USpringArmComponent* springArm = Cast<USpringArmComponent>(InOwner->GetComponentByClass(USpringArmComponent::StaticClass()));
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;
	springArm->bEnableCameraLag = bEnableCameraLag;
	UCameraComponent* camera = Cast<UCameraComponent>(InOwner->GetComponentByClass(UCameraComponent::StaticClass()));
	camera->FieldOfView = FieldOfView;
}

void FWeaponAimData::SetDataByNoneCurve(class ACCharacter* InOwner)
{
	USpringArmComponent* springArm = Cast<USpringArmComponent>(InOwner->GetComponentByClass(USpringArmComponent::StaticClass()));
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;
	springArm->bEnableCameraLag = bEnableCameraLag;

	UCameraComponent* camera = Cast<UCameraComponent>(InOwner->GetComponentByClass(UCameraComponent::StaticClass()));
	camera->FieldOfView = FieldOfView;
}

/////////////////////////
ACWeapon::ACWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(Scene);
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Time Line"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Script/Engine.CurveFloat'/Game/Blueprints/Weapon/AK/Curve_Aim.Curve_Aim'"));
	if (curve.Object != nullptr)
		AimCurve = curve.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> decal(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Material/M_Decal_Inst.M_Decal_Inst'"));
	if (decal.Succeeded() == true)
		HitDecal = decal.Object;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> hitParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/Effects/P_Impact_Default.P_Impact_Default'"));
	if (hitParticle.Succeeded() == true)
		HitParticle = hitParticle.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> flashParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/Effects/P_Muzzleflash.P_Muzzleflash'"));
	if (flashParticle.Succeeded() == true)
		FlashParticle = flashParticle.Object;
	static  ConstructorHelpers::FObjectFinder<UParticleSystem> ejectParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/Effects/P_Eject_bullet.P_Eject_bullet'"));
	if (ejectParticle.Succeeded() == true)
		EjectParticle = ejectParticle.Object;
	static  ConstructorHelpers::FObjectFinder<USoundWave> fireSound(TEXT("/Script/Engine.SoundWave'/Game/Assets/Sounds/S_RifleShoot.S_RifleShoot'"));
	if (fireSound.Succeeded() == true)
		FireSound = fireSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundWave> breth(TEXT("/Script/Engine.SoundWave'/Game/Sound/Sniper_Breath.Sniper_Breath'"));
	if (breth.Succeeded() == true)
		BreathSound = breth.Object;
	AutoFireHandle = FTimerHandle();
}

// Called when the game starts or when spawned
void ACWeapon::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
		return;

	if (HolsterSocketName.IsValid() == true)
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);


	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player != NULL && bIsCustom == false)
	{
		//Aim BaseData Setting
		USpringArmComponent* SpringArm = Cast<USpringArmComponent>(player->GetComponentByClass(USpringArmComponent::StaticClass()));
		UCameraComponent* camera = Cast<UCameraComponent>(player->GetComponentByClass(UCameraComponent::StaticClass()));
		if (SpringArm != nullptr && camera != nullptr)
		{
			BaseData.TargetArmLength = SpringArm->TargetArmLength;
			BaseData.SocketOffset = SpringArm->SocketOffset;
			BaseData.bEnableCameraLag = SpringArm->bEnableCameraLag;
			BaseData.FieldOfView = camera->FieldOfView;
		}
		else
			BaseData.SetDataByNoneCurve(OwnerCharacter);
	}
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	Camera = Cast<UCCameraComponent>(OwnerCharacter->GetComponentByClass(UCCameraComponent::StaticClass()));
	if (AimCurve != nullptr)
	{
		FOnTimelineFloat timeline;
		timeline.BindUFunction(this, "OnAiming");
		Timeline->AddInterpFloat(AimCurve, timeline);
		Timeline->SetLooping(false);
		Timeline->SetPlayRate(AimSpeed);
	}

	CurrentMagazineCount = MaxMagazineCount;
}

void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("%d"), CurrentMagazineCount);

}

bool ACWeapon::CanEquip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;
	b |= State->IsInventoryMode() == true;

	return !b;
}

void ACWeapon::Equip()
{
	bEquipping = true;
	if (State == nullptr)
		return;
	if (Camera != nullptr)
		Camera->EnableControlRotation();


	State->SetEquipMode();
	if (EquipSound != nullptr)
		UGameplayStatics::SpawnSoundAtLocation(OwnerCharacter->GetWorld(), EquipSound, FVector::ZeroVector, FRotator::ZeroRotator);

	if (EquipMontage == nullptr)
	{
		BeginEquip();
		EndEquip();
		return;
	}

	OwnerCharacter->PlayAnimMontage(EquipMontage, Equip_PlayRate);
}

void ACWeapon::BeginEquip()
{
	if (RightHandSokcetName.IsValid())
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightHandSokcetName);
}

void ACWeapon::EndEquip()
{
	bEquipping = false;
}

bool ACWeapon::CanUnequip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;
	b |= State->IsInventoryMode() == true;

	return !b;
}

void ACWeapon::Unequip()
{
	if (UnequipSound != nullptr)
		UGameplayStatics::SpawnSoundAtLocation(OwnerCharacter->GetWorld(), UnequipSound, FVector::ZeroVector, FRotator::ZeroRotator);
	if (HolsterSocketName.IsValid() == true)
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);
	if (Camera != nullptr)
		Camera->DisableControlRoation();
}

bool ACWeapon::CanFire()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;
	//b |= State->IsInventoryMode() == true;
	return !b;
}

void ACWeapon::BeginFire()
{
	bFiring = true;
	if (bAutoFire == true)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &ACWeapon::OnFireing, AutoFireInterval, true, 0);
		return;
	}

	OnFireing();
}

void ACWeapon::EndFire()
{
	if (bFiring == false)
		return;
	if (GetWorld()->GetTimerManager().IsTimerActive(AutoFireHandle))
		GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);

	bFiring = false;
}

void ACWeapon::OnFireing()
{
	if (FireMontage != nullptr)
		OwnerCharacter->PlayAnimMontage(FireMontage, FireRate);
	UCameraComponent* camera = Cast<UCameraComponent>(OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
	FTransform transform{};
	FVector direction{};
	if (camera == nullptr)
	{
		transform = Mesh->GetSocketTransform("Muzzle_Bullet");//camera->GetComponentToWorld();
		direction = transform.GetRotation().GetUpVector();//camera->GetForwardVector();
	}
	else
	{
		direction = camera->GetForwardVector();
		transform = camera->GetComponentToWorld();
	}


	FVector start = transform.GetLocation() + direction;

	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, RecoilAngle);

	FVector end = transform.GetLocation() + direction * HitDistance;

	TArray<AActor*> ignores;
	FHitResult hitResult;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignores, Debug, hitResult, true, DebugColor);
	if (hitResult.bBlockingHit == true)
	{
		if (HitDecal != nullptr)
		{
			FRotator rotator = hitResult.ImpactNormal.Rotation();
			UDecalComponent* decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, HitDecalSize, hitResult.Location, rotator, HitDecalLifeTime);
			decal->SetFadeScreenSize(0);
		}
		if (HitParticle != nullptr)
		{
			FRotator rotator = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, hitResult.Location, rotator);
		}
	}

	if (FlashParticle != nullptr)
		UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	if (EjectParticle != nullptr)
		UGameplayStatics::SpawnEmitterAttached(EjectParticle, Mesh, "Eject", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	FVector muzzleLocation = Mesh->GetSocketLocation("Muzzle");
	if (FireSound != nullptr)
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, muzzleLocation);
	if (CameraShak != nullptr)
	{

		APlayerController* controller = Cast<APlayerController>(OwnerCharacter->GetController());
		if (controller != nullptr)
		{
			if (bInAim == true && AimCameraShak != nullptr)
				controller->PlayerCameraManager->StartCameraShake(AimCameraShak);
			else
				controller->PlayerCameraManager->StartCameraShake(CameraShak);
		}
	}

	OwnerCharacter->AddControllerPitchInput(-RecoilRate * UKismetMathLibrary::RandomFloatInRange(0.8f, 1.2f));

	if (BulletClass != nullptr)
	{
		FVector location = Mesh->GetSocketLocation("Muzzle_Bullet");
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACBullet* bullet = GetWorld()->SpawnActor<ACBullet>(BulletClass, location, direction.Rotation(), param);
		bullet->OnHit.AddDynamic(this, &ACWeapon::OnBullet);
		if (bullet != nullptr)
			bullet->Shoot(direction);
	}

	if (CurrentMagazineCount > 1)
		CurrentMagazineCount--;
	else
		if (CanReload() == true)
			Reload();
}

void ACWeapon::ToggleAutoFire()
{
	bAutoFire = !bAutoFire;
}

bool ACWeapon::CanReload()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= CurrentMagazineCount == MaxMagazineCount;
	//b |= State->IsInventoryMode() == true;
	return !b;
}

void ACWeapon::Reload()
{
	bReload = true;
	EndAim();
	EndFire();

	if (ReloadMontage != nullptr)
		OwnerCharacter->PlayAnimMontage(ReloadMontage, ReloadPlayRate);

	// ������ �Ϸ� �� CurrentMagazineCount�� �ִ� ź������ �缳��
	CurrentMagazineCount = MaxMagazineCount;
	UGameplayStatics::PlaySoundAtLocation(OwnerCharacter->GetWorld(), ReloadSound, FVector::ZeroVector, FRotator::ZeroRotator);
}

void ACWeapon::Eject_Magazine()
{
	if (MagazineBoneName.IsValid() == true)
		Mesh->HideBoneByName(MagazineBoneName, PBO_None);
	if (MagazineClass == nullptr)
		return;

	FTransform transform = Mesh->GetSocketTransform(MagazineBoneName);
	ACMagazine* magazie = GetWorld()->SpawnActorDeferred<ACMagazine>(MagazineClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	magazie->SetEject();
	magazie->SetLifeSpan(5.0f);
	magazie->FinishSpawning(transform);
}

void ACWeapon::Spawn_Magazine()
{
	if (MagazineClass == nullptr)
		return;
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Magazine = GetWorld()->SpawnActor<ACMagazine>(MagazineClass, param);
	Magazine->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), MagazinSocketName);
}

void ACWeapon::Load_Magazine()
{
	CurrentMagazineCount = MaxMagazineCount;
	if (MagazineBoneName.IsValid() == true)
		Mesh->UnHideBoneByName(MagazineBoneName);

	if (Magazine != nullptr)
		Magazine->Destroy();
}

void ACWeapon::End_Magazine()
{
	bReload = false;
}

bool ACWeapon::CanAim()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;
	//b |= State->IsInventoryMode() == true;
	return !b;
}

void ACWeapon::BeginAim()
{
	ACPlayer* Player = Cast<ACPlayer>(OwnerCharacter);
	if (!Player)
		return;
	bInAim = true;
	if (BreathSound != nullptr)
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BreathSound, OwnerCharacter->GetActorLocation());
	if (AimCurve != nullptr)
	{
		Timeline->PlayFromStart();
		AimData.SetData(OwnerCharacter);
		return;
	}
	AimData.SetDataByNoneCurve(OwnerCharacter);
}

void ACWeapon::EndAim()
{
	if (bInAim == false)
		return;
	bInAim = false;

	if (AimCurve != nullptr)
	{
		Timeline->PlayFromStart();
		BaseData.SetData(OwnerCharacter);
		return;
	}
	BaseData.SetDataByNoneCurve(OwnerCharacter);
}

void ACWeapon::OnAiming(float Output)
{
	UCameraComponent* camera = Cast<UCameraComponent>(OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
	camera->FieldOfView = FMath::Lerp(AimData.FieldOfView, BaseData.FieldOfView, Output);
}

void ACWeapon::OnBullet(AActor* InCauser, ACharacter* InOtherCharacter)
{
	FDamageEvent e;
	UGameplayStatics::ApplyDamage(InOtherCharacter, 10, OwnerCharacter->GetController(), this,UDamageType::StaticClass());
//	TakeDamage(10, e, OwnerCharacter->GetController(), this);
}
