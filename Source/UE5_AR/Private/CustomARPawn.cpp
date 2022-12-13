#include "CustomARPawn.h"
#include "CustomActor.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ARBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "CustomGameMode.h"
#include "PlaceableActor.h"
#include "..\Public\CustomARPawn.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACustomARPawn::ACustomARPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);


	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	CameraComponent->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();
	UKismetSystemLibrary::PrintString(this, "I pressed", true, true, FLinearColor(0, 0.66, 1, 1), 2);

}

// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACustomARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind various player inputs to functions
	// There are a few types - BindTouch, BindAxis, and BindEvent.  
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACustomARPawn::OnScreenTouch);
}


bool ACustomARPawn::WorldHitTest(FVector2D screenPos, FHitResult& hitResult)
{
	// Get player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	// Perform deprojection taking 2d clicked area and generating reference in 3d world-space.
	FVector worldPosition;
	FVector worldDirection; // Unit Vector
	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, screenPos, /*out*/
		worldPosition, /*out*/ worldDirection);

	// construct trace vector (from point clicked to 1000.0 units beyond in same direction)
	FVector traceEndVector = worldDirection * 1000.0;
	traceEndVector = worldPosition + traceEndVector;
	// perform line trace (Raycast)
	bool traceSuccess = GetWorld()->LineTraceSingleByChannel(hitResult, worldPosition, traceEndVector,
		ECollisionChannel::ECC_WorldDynamic);
	// return if the operation was successful
	return traceSuccess;
}


void ACustomARPawn::OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{
	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(Temp);

	FHitResult	hitTestResult;
	GM->LineTraceSpawnActor(ScreenPos);
	// Perform a hitTest, get the return values as hitTesTResult
	if (!WorldHitTest(FVector2D(ScreenPos), hitTestResult))
	{
		// HitTest returned false, get out.
		UKismetSystemLibrary::PrintString(this, "Nothing pressed", true, true, FLinearColor(0, 0.66, 1, 1), 2);
		return;


	}

	// Get object of actor hit.
	UClass* hitActorClass = UGameplayStatics::GetObjectClass(hitTestResult.GetActor());
	// if we've hit a target.
	if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, ACustomActor::StaticClass()))
	{
		UKismetSystemLibrary::PrintString(this, "Cube clicked!", true, true, FLinearColor(0, 0.66, 1, 1), 2);
	}
}
