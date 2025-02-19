// Fill out your copyright notice in the Description page of Project Settings.


#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	PointValue = 50;
}

void ABigCoinItem::KeyPressedActivate(AActor* Activator)
{
	Super::KeyPressedActivate(Activator);
}

void ABigCoinItem::Use(AActor* Target)
{

}


