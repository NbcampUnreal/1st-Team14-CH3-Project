#include "CGrenadesItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACGrenadesItem::ACGrenadesItem()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	//SkeletalMesh->SetupAttachment(InteractableCollision);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Mesh/Weapons/Meshes/G67_Grenade/SK_G67.SK_G67'"));
	if (asset.Succeeded() == true)
		SkeletalMesh->SetSkeletalMesh(asset.Object);
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->ProjectileGravityScale = 1;
	Projectile->InitialSpeed = 1000;
	Projectile->MaxSpeed = 1000;

	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LifeTime = 3;
	ExplosiveDelay = 3;
	ExplosiveRadius = 300;
	ExplosiveDamage = 30;
	//ItemType = EItemType::EIT_Grenades;
	
	MineCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MineCollision"));
	MineCollision->InitSphereRadius(ExplosiveRadius);
	MineCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MineCollision->SetupAttachment(SkeletalMesh);

	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(L"/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'");
	//if (ParticleAsset.Succeeded())
	//{
	//	Particle = ParticleAsset.Object;
	//}
	//static ConstructorHelpers::FObjectFinder<USoundWave> SoundAsset(L"/ Script / Engine.SoundWave'/Game/StarterContent/Audio/Explosion01.Explosion01'");
	//if (SoundAsset.Succeeded())
	//{
	//	Sound = SoundAsset.Object;
	//}
}


void ACGrenadesItem::BeginPlay()
{
	Super::BeginPlay();
	Projectile->SetActive(false);
}

void ACGrenadesItem::Shoot(const ACharacter* OwnerCharacter, const FVector& InDirection)
{
	// 충돌 설정
	SkeletalMesh->SetCollisionProfileName("PhysicsActor");

	// 투사체 속도 및 방향 설정
	FVector LaunchVelocity = InDirection * Projectile->InitialSpeed;

	// 충돌체에 물리적 힘 적용
	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->AddImpulse(LaunchVelocity, NAME_None, true);

	// 회전 추가 (더 자연스러운 던지기 효과)
	FVector RandomTorque = FVector(
		FMath::RandRange(-500.0f, 500.0f),
		FMath::RandRange(-500.0f, 500.0f),
		FMath::RandRange(-500.0f, 500.0f)
	);
	SkeletalMesh->AddTorqueInRadians(RandomTorque, NAME_None, true);

	// 소유자 충돌 무시
	SkeletalMesh->IgnoreActorWhenMoving(Owner, true);

	// 투사체 활성화
	Projectile->SetActive(true);

	// 폭발 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(
		ExplosiveTimerHandle,
		this,
		&ACGrenadesItem::Explode,
		LifeTime,
		false
	);
}


void ACGrenadesItem::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn Location: %s"), *GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Particle Scale: %s"), *ParticleScale.ToString());

	if (Particle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			Particle,
			GetActorLocation(),
			GetActorRotation(),
			ParticleScale  // 기본 스케일로 시도
		);
	}
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
	}
	TArray<AActor*> overlappingActors;
	MineCollision->GetOverlappingActors(overlappingActors);

	for (AActor* actor : overlappingActors)
	{
		APawn* pawn = Cast<APawn>(actor);
		if (pawn != nullptr)
			Hits.AddUnique(pawn);
	}
	
	for (APawn* hit : Hits)
		HitData.SnedDamage((APawn*)GetOwner(), this, hit);

	Destroy();
}
