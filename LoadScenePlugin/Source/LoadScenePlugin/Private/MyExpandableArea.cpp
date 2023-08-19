// Fill out your copyright notice in the Description page of Project Settings.


#include "MyExpandableArea.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Engine/Font.h"

UMyExpandableArea::UMyExpandableArea()
{
	//배경 지우기
	BorderBrush.TintColor = FSlateColor(FLinearColor(1, 1, 1, 0));

	//Padding 설정
	AreaPadding.Left = 40;

	//폴더 이미지 설정.
	static ConstructorHelpers::FObjectFinder<UTexture2D> closed(TEXT("Texture2D'/LoadScenePlugin/texture/Closed_Folder.Closed_Folder'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> opened(TEXT("Texture2D'/LoadScenePlugin/texture/Opened_Folder.Opened_Folder'"));

	FSlateBrush collapsed_brush;
	collapsed_brush.SetResourceObject(closed.Object);
	collapsed_brush.SetImageSize(FVector2D(30.f, 30.f));
	FSlateBrush opened_brush;
	opened_brush.SetResourceObject(opened.Object);
	opened_brush.SetImageSize(FVector2D(30.f, 30.f));

	Style.SetCollapsedImage(collapsed_brush);
	Style.SetExpandedImage(opened_brush);
	
	//Body 설정
	auto bodycontent = CreateDefaultSubobject<UVerticalBox>(TEXT("Body_Contents"));
	BodyContent = bodycontent;

	//Header 설정
	static ConstructorHelpers::FObjectFinder<UFont> noto_regular(TEXT("Font'/WeatherAPI/Font/NotoSansCJKkr/NotoSansCJKkr-Regular_Font.NotoSansCJKkr-Regular_Font'"));

	auto headercontent = CreateDefaultSubobject<UTextBlock>(TEXT("Directory_Name"));
	headercontent->SetText(FText::FromString(TEXT(" Scene Directory")));
	headercontent->SetColorAndOpacity(FSlateColor(FLinearColor(0, 0, 0)));
	headercontent->SetFont(FSlateFontInfo(noto_regular.Object, 18));
	HeaderContent = headercontent;
}

UMyExpandableArea* UMyExpandableArea::AddDirectory(FText header_name)
{
	UMyExpandableArea* child = NewObject<UMyExpandableArea>();

	FString s_header_name = header_name.ToString();
	s_header_name = " " + s_header_name;

	Cast<UTextBlock>(child->HeaderContent)->SetText(FText::FromString(s_header_name));

	Cast<UVerticalBox>(BodyContent)->AddChildToVerticalBox(child);

	return child;
}

void UMyExpandableArea::AddFile(UUserWidget* widget)
{
	Cast<UVerticalBox>(BodyContent)->AddChildToVerticalBox(widget);
}

void UMyExpandableArea::ResetDir()
{
	Cast<UVerticalBox>(BodyContent)->ClearChildren();
}
