#include "Weapon/CBullet.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"

ACBullet::ACBullet()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	Capsule->SetRelativeRotation(FRotator(90, 0, 0));
	Capsule->SetCapsuleHalfHeight(50);
	Capsule->SetCapsuleRadius(2);
	Capsule->SetCollisionProfileName("BlockAllDynamic");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (mesh.Succeeded() == true)
		Mesh->SetStaticMesh(mesh.Object);
	Mesh->SetRelativeLocation(FVector(1.29,0.00,0.00));
	Mesh->SetRelativeScale3D(FVector(1, 0.025, 0.0275));
	Mesh->SetRelativeRotation(FRotator(90, 0, 0));

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> material(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/Material/M_Bullet_Inst.M_Bullet_Inst'"));
	if (material.Succeeded() == true)
		Material = material.Object;
	Mesh->SetMaterial(0, Material);

	Projectile->InitialSpeed = 2000;
	Projectile->MaxSpeed = 2000;
	Projectile->ProjectileGravityScale = 0;
	
}

void ACBullet::BeginPlay()
{
	Super::BeginPlay();
	Projectile->SetActive(false);
	Capsule->OnComponentHit.AddDynamic(this,&ACBullet::OnComponentHit);
}

void ACBullet::Shoot(const FVector& InDirection)
{
	SetLifeSpan(LifeTime);
	Projectile->Velocity = InDirection*Projectile->InitialSpeed;
	Projectile->SetActive(true);
}


void ACBullet::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}