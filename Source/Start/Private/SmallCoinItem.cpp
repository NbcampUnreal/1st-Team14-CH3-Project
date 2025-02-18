// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallCoinItem.h"

ASmallCoinItem::ASmallCoinItem()
{
	PointValue = 10;
	ItemType = "Small Coin";
}

void ASmallCoinItem::KeyPressedActivate(AActor* Activator)
{
	Super::KeyPressedActivate(Activator);
}

void ASmallCoinItem::Use()
{
}



