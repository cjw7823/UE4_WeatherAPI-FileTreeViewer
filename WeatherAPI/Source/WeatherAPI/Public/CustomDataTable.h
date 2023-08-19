// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CustomDataTable.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FCustomDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FCustomDataTable() : MainText(""), SubText(""), Time_min(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString MainText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString SubText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Time_min;
};