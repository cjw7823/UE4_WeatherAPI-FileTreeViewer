// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomListView.h"
#include "SCustomListView.h"

UCustomListView::UCustomListView(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetBarStyle = FCoreStyle::Get().GetWidgetStyle<FScrollBarStyle>("Scrollbar");
}

TSharedRef<STableViewBase> UCustomListView::RebuildListWidget()
{
	auto outRef = ConstructListView<SCustomListView>();
	auto customViewRef = StaticCastSharedRef<SCustomListView<UObject*>>(outRef);

	customViewRef->UpdateBarThickness(12);
	customViewRef->UpdateBarStyle(WidgetBarStyle);

	return outRef;
}