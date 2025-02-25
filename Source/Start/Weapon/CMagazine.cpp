#include "CMagazine.h"

ACMagazine::ACMagazine()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh_Full = CreateDefaultSubobject<UStaticMeshComponent>("Mesh_Full");
	Mesh_Full->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshFull(L"/Script/Engine.StaticMesh'/Game/Assets/Mesh/Weapons/Meshes/Ka47/SM_KA47_Mag.SM_KA47_Mag'");
	if (meshFull.Succeeded() == true)
		Mesh_Full->SetStaticMesh(meshFull.Object);
	Mesh_Empaty = CreateDefaultSubobject<UStaticMeshComponent>("Mesh_Empaty");
	Mesh_Empaty->SetupAttachment(Root);
	Mesh_Full->SetCollisionProfileName("Magazine");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> meshEmpaty(L"/Script/Engine.StaticMesh'/Game/Assets/Mesh/Weapons/Meshes/Ka47/SM_KA47_Mag_Empty.SM_KA47_Mag_Empty'");
	if (meshEmpaty.Succeeded() == true)
		Mesh_Empaty->SetStaticMesh(meshEmpaty.Object);
	Mesh_Empaty->SetupAttachment(Root);
	Mesh_Empaty->SetCollisionProfileName("Magazine");
	
	
}

// Called when the game starts or when spawned
void ACMagazine::BeginPlay()
{
	Super::BeginPlay();
	if (bEject == false)
		Mesh_Full->SetVisibility(true);
}

void ACMagazine::SetEject()
{
	bEject = true;

	Mesh_Full->SetVisibility(false);
	Mesh_Empaty->SetSimulatePhysics(true);
}

