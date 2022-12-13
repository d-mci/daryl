// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CustomActor.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "PlaceableActor.generated.h"

class UARPin;

UCLASS()
class UE5_AR_API APlaceableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlaceableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void StartPlayEvent();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void moveObjects(float DeltaTime);
	void initGame();
	FTransform getSourceLocation();


	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* CameraComponent;

	UARPin* PinComponent;

private:
	FTransform sourceLocation;
	float spd;
	FTimerHandle Ticker;
	ACustomActor Actor;



};
