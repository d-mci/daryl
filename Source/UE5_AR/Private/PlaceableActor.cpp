// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableActor.h"
#include "ARPin.h"
#include "ARBlueprintLibrary.h"
#include "CustomActor.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"



// Sets default values
APlaceableActor::APlaceableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);

	spd = 20.0f;
}

// Called when the game starts or when spawned
void APlaceableActor::BeginPlay()
{
	Super::BeginPlay();
	//StartPlayEvent();

}

void APlaceableActor::StartPlayEvent()
{
	// Start a timer which will call the SpawnCube Function every 4 seconds
	GetWorldTimerManager().SetTimer(Ticker, this, &APlaceableActor::initGame, 4.0f, true, 0.0f);
	UKismetSystemLibrary::PrintString(this, "Timer init", true, true, FLinearColor(0, 0.66, 1, 1), 2);
}

// Called every frame
void APlaceableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Making sure the actor remains on the ARPin that has been found.
	if (PinComponent)
	{
		auto TrackingState = PinComponent->GetTrackingState();

		switch (TrackingState)
		{
		case EARTrackingState::Tracking:
			SceneComponent->SetVisibility(true);
			sourceLocation = PinComponent->GetLocalToWorldTransform();
			SetActorTransform(sourceLocation);
			// Scale down default cube mesh - Change this for your applications.
			SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));
			break;

		case EARTrackingState::NotTracking:
			PinComponent = nullptr;

			break;
		}
	}
}

void::APlaceableActor::initGame()
{
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	const FVector MyLoc(0, 0, 0);
	GetWorld()->SpawnActor<ACustomActor>(FVector(0,0,0), MyRot, SpawnInfo);
}
