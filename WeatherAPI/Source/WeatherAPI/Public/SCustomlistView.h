// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/Views/SListView.h"

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

template <typename ItemType>
class WEATHERAPI_API SCustomListView : public SListView<ItemType>
{
public:
	void UpdateBarStyle(const FScrollBarStyle& inBarStyle)
	{
		WidgetBarStyle = inBarStyle;
		if (STableViewBase::ScrollBar.IsValid())
		{
			STableViewBase::ScrollBar->SetStyle(&WidgetBarStyle);

			FChildren* barChilds = STableViewBase::ScrollBar->GetChildren();
			if (barChilds == nullptr && barChilds->Num() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("SCustomListView : Can't update background image, ScrollbarWidget Childs are either empty or null"));
				return;
			}
			TSharedRef<SWidget> verticalBox = barChilds->GetChildAt(0);

			FChildren* vBoxChilds = verticalBox->GetChildren();
			if (vBoxChilds == nullptr && vBoxChilds->Num() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("SCustomListView : Can't update background image, ScrollbarWidget VerticalBox Childs are either empty or null"));
				return;
			}
			TSharedRef<SWidget> abstractBackgroundBorder = vBoxChilds->GetChildAt(0);
			auto backgroundBorder = StaticCastSharedRef<SBorder>(abstractBackgroundBorder);
			backgroundBorder->SetBorderImage(STableViewBase::ScrollBar->GetOrientation() == EOrientation::Orient_Vertical ?
				&inBarStyle.VerticalBackgroundImage : &inBarStyle.HorizontalBackgroundImage);
		}
	}

	void UpdateBarThickness(const float& inThickness)
	{
		if (STableViewBase::ScrollBar.IsValid())
		{	
			TSharedPtr<SWidget> parentPtr = STableViewBase::ScrollBar->GetParentWidget();
			auto scrollBarBox = StaticCastSharedPtr<SBox>(parentPtr);
			
			scrollBarBox->SetWidthOverride(FOptionalSize(inThickness));
			STableViewBase::ScrollBar->SetThickness(FVector2D(inThickness, 16.0f));
		}
	}

private:
	FScrollBarStyle WidgetBarStyle;
};
