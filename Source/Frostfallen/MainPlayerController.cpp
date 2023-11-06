// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "DefaultUnreal5Character.h"
#include "Kismet/GameplayStatics.h"


void AMainPlayerController::BeginPlay() {

	Super::BeginPlay();

	playerCharacter = Cast<ADefaultUnreal5Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (WSubs)
	{
		subs = CreateWidget<UUserWidget>(this, WSubs);

		if (subs)
		{
			subs->AddToViewport();
			subs->SetVisibility(ESlateVisibility::Collapsed);
		}
		//	deathScreen->SetAlignmentInViewport(FVector2D(0.f));
	}

	if (WPickUp)
	{
		pickup = CreateWidget<UUserWidget>(this, WPickUp);
		//	WPickUp.
		if (pickup)
		{

			pickup->AddToViewport();
			pickup->SetVisibility(ESlateVisibility::Collapsed);
			//	pickup.image
			//if(playerCharacter->bMainMenu)
			//	DisplayPickUp();
		}
		//pickup->SetAlignmentInViewport(FVector2D(0.f));
	}

	if (WPickedUpItem)
	{
		pickedUpItem = CreateWidget<UUserWidget>(this, WPickedUpItem);
		//	WPickUp.
		if (pickedUpItem)
		{

			pickedUpItem->AddToViewport();
			pickedUpItem->SetVisibility(ESlateVisibility::Collapsed);
			//	pickup.image
			//if(playerCharacter->bMainMenu)
			//	DisplayPickUp();
		}
		//pickup->SetAlignmentInViewport(FVector2D(0.f));
	}

	if (WDialogueInteract)
	{
		dialogueInteract = CreateWidget<UUserWidget>(this, WDialogueInteract);

		if (dialogueInteract)
		{
			dialogueInteract->AddToViewport();
			dialogueInteract->SetVisibility(ESlateVisibility::Collapsed);

			if (playerCharacter)
			{
				if (playerCharacter->bMainMenu)
					DisplayDialogueInteract();
			}
			
		}
		//	deathScreen->SetAlignmentInViewport(FVector2D(0.f));
	}

	if (WCrossHair)
	{
		crossHair = CreateWidget<UUserWidget>(this, WCrossHair);

		if (crossHair)
		{
			crossHair->AddToViewport();
			crossHair->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (WToggle)
	{
		toggle = CreateWidget<UUserWidget>(this, WToggle);

		if (toggle)
		{
			toggle->AddToViewport();
			toggle->SetVisibility(ESlateVisibility::Collapsed);
		}
		toggle->SetAlignmentInViewport(FVector2D(0.f));
	}

	if (WPauseMenu)
	{
		pauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);

		if (pauseMenu)
		{
			pauseMenu->AddToViewport();
			pauseMenu->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (WMainMenu)
	{
		mainMenu = CreateWidget<UUserWidget>(this, WMainMenu);

		if (mainMenu)
		{
			mainMenu->AddToViewport();
			mainMenu->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (WSplashArt)
	{
		splashArt = CreateWidget<UUserWidget>(this, WSplashArt);

		if (splashArt)
		{
			splashArt->AddToViewport();
			splashArt->SetVisibility(ESlateVisibility::Collapsed);

			if (playerCharacter)
			{
				if (playerCharacter->bMainMenu)
				{
					DisplaySplashArt();
				}

			}
			
		}
	}

	
	if (WDeathScreen)
	{
		deathScreen = CreateWidget<UUserWidget>(this, WDeathScreen);

		if (deathScreen)
		{
			deathScreen->AddToViewport();
			deathScreen->SetVisibility(ESlateVisibility::Collapsed);
		}
	//	deathScreen->SetAlignmentInViewport(FVector2D(0.f));
	}

	if (WEndScreen)
	{
		endScreen = CreateWidget<UUserWidget>(this, WEndScreen);

		if (endScreen)
		{
			endScreen->AddToViewport();
			endScreen->SetVisibility(ESlateVisibility::Collapsed);
		}
		//	deathScreen->SetAlignmentInViewport(FVector2D(0.f));
	}

}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (toggle)
	{
		FVector2D viewportPosition;

		ProjectWorldLocationToScreen(enemyLocation, viewportPosition);

		FVector2D sizeInViewport(50);

		toggle->SetPositionInViewport(viewportPosition);
		toggle->SetDesiredSizeInViewport(sizeInViewport);
	}


	if (dialogueInteract)
	{
		FVector2D viewportPosition;

		ProjectWorldLocationToScreen(npcLocation, viewportPosition);

		FVector2D sizeInViewport(50);

		dialogueInteract->SetPositionInViewport(viewportPosition);
		dialogueInteract->SetDesiredSizeInViewport(sizeInViewport);
	}

}


void AMainPlayerController::DisplayPauseMenu()
{
	if (pauseMenu)
	{
		bPauseMenuVisible = true;	
		//UIModeOnly();
	//	FInputModeGameAndUI
		FInputModeGameAndUI sdafsdaf;
		SetInputMode(sdafsdaf);
		pauseMenu->SetVisibility(ESlateVisibility::Visible);
		
		//bShowMouseCursor = true;
	}
}
void AMainPlayerController::RemovePauseMenu()
{
	if (pauseMenu)
	{
		bPauseMenuVisible = false;
		pauseMenu->SetVisibility(ESlateVisibility::Hidden);
		GameModeOnly();	

	//	bShowMouseCursor = false;
	}
}


void AMainPlayerController::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		playerCharacter->bPause = false;
		RemovePauseMenu();
	}
	else
	{
		playerCharacter->bPause = true;
		DisplayPauseMenu();
	}
}

void AMainPlayerController::GameModeOnly()
{
	FInputModeGameOnly inputModeGameOnly;
	SetInputMode(inputModeGameOnly);
///	SetInputMode();
}

void AMainPlayerController::UIModeOnly()
{
	FInputModeUIOnly inputModeUIOnly;
	EnableInput(this);
	SetInputMode(inputModeUIOnly);
	///	SetInputMode();
}

void AMainPlayerController::DisplayCrossHair()
{
	if (crossHair)
	{
		crossHair->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveCrossHair()
{
	if (crossHair)
	{
		crossHair->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayPickedUpItem()
{

	if (pickedUpItem)
	{
		pickedUpItem->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayerController::RemovePickedUpItem, 2.f, false);
		
	}
}

void AMainPlayerController::RemovePickedUpItem()
{
	if (pickedUpItem)
	{
		//pickedUpItem->SetVisibility(ESlateVisibility::Hidden);

		playerCharacter->bReversePickedUp = true;

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayerController::RealRemovePickedUpItem, .5f, false);

		//playerCharacter->bReversePickedUp = false;
		playerCharacter->bPickedUpMoon = false;
		playerCharacter->bPickedUpBomb = false;
		playerCharacter->bPickedUpAxe = false;
		playerCharacter->bPickedUpBat = false;
//		AmountPickedUpItem--;
	}
}

void AMainPlayerController::RealRemovePickedUpItem()
{
	if (pickedUpItem)
	{
		playerCharacter->bReversePickedUp = false;
		pickedUpItem->SetVisibility(ESlateVisibility::Collapsed);
		
		//GameModeOnly();
	}
}

void AMainPlayerController::DisplayDialogueInteract()
{
	if (dialogueInteract)
	{
		dialogueInteract->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::DisplaySplashArt()
{
	if (splashArt)
	{
		splashArt->SetVisibility(ESlateVisibility::Visible);
		

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayerController::RemoveSplashArt, 4.f, false);

		FTimerHandle UnusedHandle2;
		GetWorldTimerManager().SetTimer(UnusedHandle2, this, &AMainPlayerController::DisplayMainMenu, 3.5f, false);
	}	
}

void AMainPlayerController::RemoveSplashArt()
{
	if (splashArt)
	{
		splashArt->SetVisibility(ESlateVisibility::Collapsed);

	}
}

void AMainPlayerController::DisplayMainMenu()
{
	if (mainMenu)
	{
		UIModeOnly();
		mainMenu->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveMainMenu()
{
	if (mainMenu)
	{
		mainMenu->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayDeathScreen()
{
	if (deathScreen)
	{
		deathScreen->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle UnusedHandle;


		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayerController::RemoveDeathScreen, 1.5f, false);

	}
}

void AMainPlayerController::RemoveDeathScreen()
{
	if (deathScreen)
	{
		playerCharacter->reverseDeathScreen = true;

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayerController::RealRemoveDeathScreen, 2.f, false);

	}
}
void AMainPlayerController::RealRemoveDeathScreen()
{
	if (deathScreen)
	{
		deathScreen->SetVisibility(ESlateVisibility::Collapsed);
		playerCharacter->reverseDeathScreen = false;
		GameModeOnly();
	}
}

void AMainPlayerController::DisplayEndScreen()
{
	if (endScreen)
	{
		endScreen->SetVisibility(ESlateVisibility::Visible);

	}
}

void AMainPlayerController::RemoveEndScreen()
{
	if (endScreen)
	{
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayerController::RealRemoveDeathScreen, .5f, false);
		//	GameModeOnly();
	}
}


void AMainPlayerController::DisplayPickUp()
{
	if (pickup)
	{

		pickup->SetVisibility(ESlateVisibility::Visible);

	}
}

void AMainPlayerController::RemovePickUp()
{
	if (pickup)
	{
		pickup->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::RealRemovePickUp()
{
	if (pickup)
	{
		pickup->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayToggle()
{
	if (toggle)
	{
		toggle->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveToggle()
{
	if (toggle)
	{
		toggle->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AMainPlayerController::DisplaySubs()
{
	if (subs)	
	{
		subs->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveSubs()
{
	if (subs)
	{
		subs->SetVisibility(ESlateVisibility::Hidden);

		playerCharacter->reverseSubs = true;
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayerController::RealRemoveSubs, .5f, false);
	}
}

void AMainPlayerController::RealRemoveSubs()
{
	if (subs)
	{
		playerCharacter->reverseSubs = false;
		subs->SetVisibility(ESlateVisibility::Hidden);
	}
}
