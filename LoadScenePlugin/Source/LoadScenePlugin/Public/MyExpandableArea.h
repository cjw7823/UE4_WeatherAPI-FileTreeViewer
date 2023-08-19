// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ExpandableArea.h"
#include "MyExpandableArea.generated.h"

/**
 * 
 */
UCLASS()
class LOADSCENEPLUGIN_API UMyExpandableArea : public UExpandableArea
{
	GENERATED_BODY()

public:
	UMyExpandableArea();

	UMyExpandableArea* AddDirectory(FText header_name);
	void AddFile(UUserWidget* widget);
	void ResetDir();
};