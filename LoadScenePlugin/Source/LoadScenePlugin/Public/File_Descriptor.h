// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "File_Descriptor.generated.h"

class ULoadScene;

UCLASS()
class LOADSCENEPLUGIN_API UFile_Descriptor : public UUserWidget
{
	GENERATED_BODY()

public:
	UFile_Descriptor(const FObjectInitializer& obj);

	virtual void NativeConstruct() override;

	UFUNCTION()
		FEventReply OnMouseButtonDownEvent_BG_Click(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UPROPERTY(meta = (bindwidget))
		UTextBlock* TextBlock_Header;

	UPROPERTY(meta = (bindwidget))
		UImage* BG_Click;

	ULoadScene* pLoadScene;

	FString path;
};
