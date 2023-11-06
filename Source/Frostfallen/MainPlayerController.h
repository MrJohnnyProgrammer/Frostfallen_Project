// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FROSTFALLEN_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		class ADefaultUnreal5Character* playerCharacter;

	//Reference to UMG asset in editor
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	//Variable Holder
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WPauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* pauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> WDialogueInteract;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* dialogueInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> WMainMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* mainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> WSplashArt;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* splashArt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WCrossHair;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* crossHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WToggle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* toggle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WPickUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* pickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> WPickedUpItem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* pickedUpItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* pickedUpItem2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		int AmountPickedUpItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WDeathScreen;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* deathScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> WEndScreen;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* endScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WSubs;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* subs;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector enemyLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector npcLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector pickUpLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	bool bPauseMenuVisible;



protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void DisplayPauseMenu();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void TogglePauseMenu();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void RemovePauseMenu();
		
	UFUNCTION(Category = "HUD")
	void DisplayPickUp();

	UFUNCTION(Category = "HUD")
	void RemovePickUp();

	UFUNCTION(Category = "HUD")
		void RealRemovePickUp();

	UFUNCTION(Category = "HUD")
	void DisplayDialogueInteract();

	UFUNCTION(Category = "HUD")
	void DisplayCrossHair();


	UFUNCTION(Category = "HUD")
	void RemoveCrossHair();

	UFUNCTION(Category = "HUD")
	void DisplayPickedUpItem();

	UFUNCTION(Category = "HUD")
	void RemovePickedUpItem();

	UFUNCTION(Category = "HUD")
	void RealRemovePickedUpItem();

	UFUNCTION(Category = "HUD")
	void DisplayToggle();

	UFUNCTION(Category = "HUD")
	void RemoveToggle();

	UFUNCTION(Category = "HUD")
	void DisplayMainMenu();

	UFUNCTION(Category = "HUD")
	void RemoveMainMenu();

	UFUNCTION(Category = "HUD")
	void DisplayEndScreen();

	UFUNCTION(Category = "HUD")
	void RemoveEndScreen();

	UFUNCTION(Category = "HUD")
		void DisplayDeathScreen();

	UFUNCTION(Category = "HUD")
		void RemoveDeathScreen();

	UFUNCTION(Category = "HUD")
		void RealRemoveDeathScreen();

	UFUNCTION(Category = "HUD")
	void DisplaySplashArt();

	UFUNCTION(Category = "HUD")
	void RemoveSplashArt();

	UFUNCTION(Category = "HUD")
		void DisplaySubs();

	UFUNCTION(Category = "HUD")
		void RemoveSubs();

	UFUNCTION(Category = "HUD")
		void RealRemoveSubs();

	UFUNCTION(Category = "HUD")
	void GameModeOnly();

	void UIModeOnly();
	
};
