// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
//#include "InputCoreTypes.h"
#include "CustomListView.generated.h"

/**
 * 
 */
UCLASS()
class WEATHERAPI_API UCustomListView : public UListView
{
	GENERATED_BODY()
public:
	UCustomListView(const FObjectInitializer& ObjectInitializer);

protected:
	virtual TSharedRef<STableViewBase> RebuildListWidget() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "Bar Style"))
		FScrollBarStyle WidgetBarStyle;
};