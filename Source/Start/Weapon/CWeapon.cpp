// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeapon.h"

#include "CWeaponStructures.h"
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
#include "Components/AudioComponent.h"
#include "Engine/DamageEvents.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"

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
	static  ConstructorHelpers::FObjectFinder<USoundWave> fireSound(TEXT("/Script/Engine.SoundWave'/Game/Assets/Mesh/MilitaryWeapSilver/Sound/Pistol/Wavs/PistolA_Fire_ST01.PistolA_Fire_ST01'"));
	if (fireSound.Succeeded() == true)
		FireSound = fireSound.Object;
	static ConstructorHelpers::FObjectFinder<USoundWave> breth(TEXT("/Script/Engine.SoundWave'/Game/Sound/Sniper_Breath_Start.Sniper_Breath_Start'"));
	if (breth.Succeeded() == true)
		BreathSound = breth.Object;
	static ConstructorHelpers::FObjectFinder<USoundWave> breth2(TEXT("/Script/Engine.SoundWave'/Game/Sound/Sniper_Breath_End.Sniper_Breath_End'"));
	if(breth2.Succeeded() == true)
		BreathSound2 = breth2.Object;
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
	if (player != nullptr && bIsCustom == false)
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
			AimData.SocketOffset = SpringArm->SocketOffset;
		}
		else
			BaseData.SetDataByNoneCurve(OwnerCharacter);
	}
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	Camera = Cast<UCCameraComponent>(OwnerCharacter->GetComponentByClass(UCCameraComponent::StaticClass()));
	Weapon = Cast<UCWeaponComponent>(OwnerCharacter->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (AimCurve != nullptr)
	{
		FOnTimelineFloat timeline;
		timeline.BindUFunction(this, "OnAiming");
		Timeline->AddInterpFloat(AimCurve, timeline);
		Timeline->SetLooping(false);
		Timeline->SetPlayRate(AimSpeed);
	}

	CurrentMagazineCount = MaxMagazineCount;
	if (OwnerCharacter)
	{
		InteractableCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

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
	State->SetIdleMode();
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

void ACWeapon::DonAction()
{
	State->SetActionMode();
}

void ACWeapon::BeginAction()
{

	bBeginAction = true;
}

void ACWeapon::EndAction()
{
	bBeginAction = false;
	State->SetIdleMode();
}

bool ACWeapon::CanFire()
{
	bool b = false;
	b |= bEquipping;
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Blue, FString::Printf(L"equip %d, %d", bEquipping, b));
	b |= bReload;
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Blue, FString::Printf(L"reload %d, %d", bReload, b));
	b |= bFiring;
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Blue, FString::Printf(L"fire %d, %d", bFiring, b));
	b |= CurrentMagazineCount == 0;
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Blue, FString::Printf(L"count %d, %d", CurrentMagazineCount == 0, b));
	//b |= State->IsInventoryMode() == true;
	return !b;
}

void ACWeapon::BeginFire()
{
	bFiring = true;

	State->SetActionMode();
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
	State->SetIdleMode();
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
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, OwnerCharacter->GetMesh()->GetSocketLocation(L"pelvis"), OwnerCharacter->GetActorRotation(), 5);
	if (CameraShak != nullptr)
	{

		APlayerController* controller = Cast<APlayerController>(OwnerCharacter->GetController());
		if (controller != nullptr)
		{
			if (bInAim == true && AimCameraShak != nullptr)
				controller->PlayerCameraManager->StartCameraShake(AimCameraShak,1,ECameraShakePlaySpace::UserDefined);
			else
				controller->PlayerCameraManager->StartCameraShake(CameraShak, 1, ECameraShakePlaySpace::UserDefined);
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
	{
		CurrentMagazineCount--;
		if (CanReload() == true)
			Reload();
	}
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
	b |= CurrentMagazineCount >= MaxMagazineCount;
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
	
	CurrentMagazineCount = MaxMagazineCount;

	if (ReloadSound != nullptr)
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
	b |= Weapon->IsKnifeMode() == true;
	b |= Weapon->IsGrenadeMode() == true;
	//b |= State->IsInventoryMode() == true;
	return !b;
}

void ACWeapon::BeginAim()
{
	ACPlayer* Player = Cast<ACPlayer>(OwnerCharacter);
	if (!Player)
		return;
	bInAim = true;
	if (BreathSoundComponent != nullptr && BreathSoundComponent->IsActive())
		BreathSoundComponent->Stop();
	if (BreathSound != nullptr)
		BreathSoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BreathSound, OwnerCharacter->GetActorLocation());
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
	if(BreathSoundComponent != nullptr && BreathSoundComponent->IsActive())
		BreathSoundComponent->Stop();
	if (BreathSound2 != nullptr)
		BreathSoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BreathSound2, OwnerCharacter->GetActorLocation());
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
	HitDatas[0].SnedDamage(OwnerCharacter, InCauser, InOtherCharacter);
	//UGameplayStatics::ApplyDamage(InOtherCharacter, Damage, OwnerCharacter->GetController(), this,UDamageType::StaticClass());
}
