// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ListItem_Data.generated.h"

/**
 * 
 */
class UMessage_Manager;

UCLASS()
class WEATHERAPI_API UListItem_Data : public UObject
{
	GENERATED_BODY()

public:
	UListItem_Data();
	~UListItem_Data();

	UPROPERTY()
		FText Main;
	UPROPERTY()
		FText Sub;
	UPROPERTY()
		UMessage_Manager* parent;
	UPROPERTY()
		UListItem_Data* data;

};
