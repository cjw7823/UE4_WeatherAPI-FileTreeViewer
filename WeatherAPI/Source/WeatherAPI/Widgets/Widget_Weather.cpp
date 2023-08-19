// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Weather.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "CustomDataTable.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetStringLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

//#define TODAYMESSAGE_PATH "WeatherAPI/MessageScene/TodaysMessage.csv"

UWidget_Weather::UWidget_Weather(const FObjectInitializer& objectinitializer) : Super(objectinitializer)
{
	static ConstructorHelpers::FObjectFinder<UFont> bold(TEXT("Font'/WeatherAPI/Font/Custom_bold.Custom_bold'"));
	static ConstructorHelpers::FObjectFinder<UFont> regular(TEXT("Font'/WeatherAPI/Font/Custom_regular.Custom_regular'"));
	static ConstructorHelpers::FObjectFinder<UFont> semibold(TEXT("Font'/WeatherAPI/Font/Custom_semibold.Custom_semibold'"));
	static ConstructorHelpers::FObjectFinder<UFont> noto_bold(TEXT("Font'/WeatherAPI/Font/NotoSansCJKkr/NotoSansCJKkr-Bold_Font.NotoSansCJKkr-Bold_Font'"));
	static ConstructorHelpers::FObjectFinder<UFont> noto_regular(TEXT("Font'/WeatherAPI/Font/NotoSansCJKkr/NotoSansCJKkr-Regular_Font.NotoSansCJKkr-Regular_Font'"));

	Font_bold = bold.Object;
	Font_regular = regular.Object;
	Font_semibold = semibold.Object;
	Font_Noto_bold = noto_bold.Object;
	Font_Noto_regular = noto_regular.Object;
}

void UWidget_Weather::NativeConstruct()
{
	Super::NativeConstruct();
	
	//포인터 등록
	UHttp_weather::SetWeatherPointer(this);

	bFirstActivate = true;

	update_time = FString();
	maxindex = 0;
	weather_bg = hyundai_weather::none;
	weather_now = hyundai_weather::none;
	weather_nearfuture_01 = hyundai_weather::none;
	weather_nearfuture_02 = hyundai_weather::none;
	weather_nearfuture_03 = hyundai_weather::none;
	weather_lowest = hyundai_weather::none;
	weather_highest = hyundai_weather::none;

	bImage_bg = true;
	bImage_now = true;
	bImage_near = true;
	bImage_low_high = true;

	SetClock();
	clock_timer = UKismetSystemLibrary::K2_SetTimer(this, FString(TEXT("SetClock")), 1.0f, true);

	//오늘의 메세지 동적 크기 조정을 위한 타이머 설정
	UKismetSystemLibrary::K2_SetTimer(this, FString(TEXT("SetCustomFont")), 0.1f, true);

	SetNowWeather();//내부에서 now_timer 생성
	SetTomorrowWeather();//내부에서 tomorrow_timer 생성
	SetNearFutureWeather();//내부에서 nearfuture_timer 생성
	FirstSetTodayMessage();//내부에서 animplayHandle 생성

	//today_message_path의 변화를 체크해 새로 적용.
	mutex_timer = UKismetSystemLibrary::K2_SetTimer(this, FString(TEXT("CheckTodayMessagePath")), 1.f, true);
}

void UWidget_Weather::NativeDestruct()
{
	UKismetSystemLibrary::K2_ClearTimerHandle(this, clock_timer);
	UKismetSystemLibrary::K2_ClearTimerHandle(this, tomorrow_timer);
	UKismetSystemLibrary::K2_ClearTimerHandle(this, now_timer);
	UKismetSystemLibrary::K2_ClearTimerHandle(this, nearfuture_timer);
	UKismetSystemLibrary::K2_ClearTimerHandle(this, animplayHandle);
	UKismetSystemLibrary::K2_ClearTimerHandle(this, mutex_timer);
}

void UWidget_Weather::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	/*FString a = "";
	if(MediaPlayer != nullptr)
		a= MediaPlayer->GetDesc();
	GEngine->AddOnScreenDebugMessage(2, 1, FColor::Orange,
		a);
	auto b = MediaPlayer
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::Orange,
		FString::Printf(TEXT("MediaPlayer : %f"), InDeltaTime));*/



}

void UWidget_Weather::SetClock()
{
	FDateTime datetime = UKismetMathLibrary::Now().GetDate();

	int32 hour = UKismetMathLibrary::Now().GetHour();
	int32 min = UKismetMathLibrary::Now().GetMinute();

	FString s_hour = FString::FromInt(hour);
	FString s_min = FString::FromInt(min);

	if (hour < 10)
		s_hour = FString("0") + s_hour;
	if (min < 10)
		s_min = FString("0") + s_min;


	FText time = FText::FromString(s_hour + ":" + s_min);
	TextBlock_Time->SetText(time);


	int32 mouth = datetime.GetMonth();
	int32 day = datetime.GetDay();
	EDayOfWeek dayofweek = datetime.GetDayOfWeek();

	FString s_mouth = FString::FromInt(mouth);
	FString s_day = FString::FromInt(day);
	FString s_dayofweek = FString("(월)");

	if (mouth < 10)
		s_mouth = FString("0") + s_mouth;
	if (day < 10)
		s_day = FString("0") + s_day;

	switch (dayofweek)
	{
	case EDayOfWeek::Monday:
		s_dayofweek = FString(TEXT("(월)"));
		break;
	case EDayOfWeek::Tuesday:
		s_dayofweek = FString(TEXT("(화)"));
		break;
	case EDayOfWeek::Wednesday:
		s_dayofweek = FString(TEXT("(수)"));
break;
	case EDayOfWeek::Thursday:
		s_dayofweek = FString(TEXT("(목)"));
		break;
	case EDayOfWeek::Friday:
		s_dayofweek = FString(TEXT("(금)"));
		break;
	case EDayOfWeek::Saturday:
		s_dayofweek = FString(TEXT("(토)"));
		break;
	case EDayOfWeek::Sunday:
		s_dayofweek = FString(TEXT("(일)"));
		break;
	}


	FText date = FText::FromString(s_mouth + "." + s_day + s_dayofweek);
	TextBlock_Date->SetText(date);
}

void UWidget_Weather::SetTomorrowWeather()
{
	FString date, time;
	SetDateAndTime(date, time, 1);

	GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Cyan, FString("Tomorrow : " + date + " / " + time));

	UHttp_weather::VilageFutureCast(date, time);

	float timer_time = 10 - (UKismetMathLibrary::Now().GetMinute() % 10);
	tomorrow_timer = UKismetSystemLibrary::K2_SetTimer(this, FString(TEXT("SetTomorrowWeather")), timer_time * 60, true);

	int32 hour = UKismetMathLibrary::Now().GetHour();
	int32 min = UKismetMathLibrary::Now().GetMinute();

	FString s_hour = FString::FromInt(hour);
	FString s_min = FString::FromInt(min);

	if (hour < 10)
		s_hour = "0" + s_hour;
	if (min < 10)
		s_min = "0" + s_min;

	update_time = s_hour + ":" + s_min;
}

void UWidget_Weather::SetNowWeather()
{
	FString date, time;
	SetDateAndTime(date, time, 2);

	GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Cyan, FString("NowWeater : " + date + " / " + time));

	UHttp_weather::UltraSrtNowCast(date, time);

	float timer_time = 10 - (UKismetMathLibrary::Now().GetMinute() % 10);
	now_timer = UKismetSystemLibrary::K2_SetTimer(this, FString(TEXT("SetNowWeather")), timer_time * 60, false);

	//특수한 내부함수 호출
	FString date_oneminus, time_oneminus;
	SetDateAndTimeOneminus(date_oneminus, time_oneminus);
	UHttp_weather::UltraSrtFutureOneminus(date_oneminus, time_oneminus);
}

void UWidget_Weather::SetNearFutureWeather()
{
	SetNearFutureText();

	FString date, time;
	SetDateAndTime(date, time, 3);

	GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Cyan, FString("NearFuture : " + date + " / " + time));

	UHttp_weather::UltraSrtFutureCast(date, time);

	float timer_time = 10 - (UKismetMathLibrary::Now().GetMinute() % 10);
	nearfuture_timer = UKismetSystemLibrary::K2_SetTimer(this, FString(TEXT("SetNearFutureWeather")), timer_time * 60, false);
}

void UWidget_Weather::FirstSetTodayMessage()
{
	todaymessage_index = 0;

	FString file;

	pre_today_message_path = today_message_path;
	FString path = today_message_path;
	GEngine->AddOnScreenDebugMessage(103, 100, FColor::Purple, "Message Path : " + today_message_path);

	FFileHelper::LoadFileToString(file, *path);
	DT = NewObject<UDataTable>(GetTransientPackage(), FName(TEXT("CSV_DT")));
	DT->RowStruct = FCustomDataTable::StaticStruct();

	auto error = DT->CreateTableFromCSVString(file);// '임포트' 중 에러 항목 리턴

	//생성된 DT의 Row 개수를 저장.
	maxindex = DT->GetRowMap().Num();

	animplayDel.Unbind();
	animplayDel.BindUObject(this, &UWidget_Weather::PlayMainAnim, MainAnim);
	animplayHandle.Invalidate();

	SetTodayMessage();//내부에서 타이머 생성
}

void UWidget_Weather::SetTodayMessage()
{
	//csv파일에 index가 없다면. 함수 재귀 종료.
	if (maxindex == 0)
	{
		TextBlock_maintext->SetText(FText::FromString(""));
		TextBlock_subtext->SetText(FText::FromString(""));
		return;
	}

	mutex_today_message_timer.Lock();

	todaymessage_index++;

	if (todaymessage_index > maxindex)
		todaymessage_index = 1;

	FName index = FName(FString::FromInt(todaymessage_index));
	FString debuging;

	FCustomDataTable* today_message = DT->FindRow<FCustomDataTable>(index, debuging);

	//디버깅 용도, 의도치 않은 상황임.
	if (today_message == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Red, "today_message is nullptr!");
		mutex_today_message_timer.Unlock();
		return;
	}

	//텍스트 설정
	FString temp1 = today_message->MainText.Replace(L"\\n", L"\n");
	FString temp2 = today_message->SubText;
	TextBlock_maintext->SetText(FText::FromString(temp1));
	TextBlock_subtext->SetText(FText::FromString(temp2));

	//위치 설정
	UCanvasPanelSlot* imageSlot_m = Cast<UCanvasPanelSlot>(TextBlock_maintext->Slot);
	UCanvasPanelSlot* imageSlot_s = Cast<UCanvasPanelSlot>(TextBlock_subtext->Slot);

	if (temp2 == FString())
	{
		imageSlot_m->SetPosition(FVector2D(0, 0));
	}
	else
	{
		Super::ForceLayoutPrepass();

		int32 between_pixel = 20;

		int32 mainTextSize_Y_half = TextBlock_maintext->GetDesiredSize().Y / 2;
		int32 subTextSize_Y_half = TextBlock_subtext->GetDesiredSize().Y / 2;
		int32 mainTextPos = 0 - subTextSize_Y_half - (between_pixel / 2);
		int32 subTextPos = 0 + mainTextSize_Y_half + (between_pixel / 2);

		imageSlot_m->SetPosition(FVector2D(0, mainTextPos));
		imageSlot_s->SetPosition(FVector2D(0, subTextPos));
	}

	//타이머 설정
	float timertime = today_message->Time_min * 60; //min -> sec
	animplayHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(animplayHandle, animplayDel, timertime, false);

	mutex_today_message_timer.Unlock();
}

void UWidget_Weather::CheckTodayMessagePath()
{
	bool flag = pre_today_message_path != today_message_path;
	if (flag)
	{
		mutex_today_message_timer.Lock();

		if (animplayHandle.IsValid())
			GetWorld()->GetTimerManager().ClearTimer(animplayHandle);

		mutex_today_message_timer.Unlock();

		FirstSetTodayMessage();
	}
}

void UWidget_Weather::OnEventAnim()
{
	SetTodayMessage();
}

void UWidget_Weather::OnEventAnim_BG()
{
	SetBackgroundVideo();
	bImage_bg = !bImage_bg;
}

void UWidget_Weather::OnEventAnim_BG_close()
{
	if (!bImage_bg && MediaPlayer_sec != nullptr && !MediaPlayer_sec->IsClosed())
		MediaPlayer_sec->Close();
	else if(bImage_bg && MediaPlayer != nullptr && !MediaPlayer->IsClosed())
		MediaPlayer->Close();
}

void UWidget_Weather::OnEventAnim_now()
{
	SetImgSequence_Now();
	bImage_now = !bImage_now;
}

void UWidget_Weather::OnEventAnim_now_close()
{
	if (!bImage_now && MediaPlayer_now_sec != nullptr && !MediaPlayer_now_sec->IsClosed())
		MediaPlayer_now_sec->Close();
	else if (bImage_now && MediaPlayer_now != nullptr && !MediaPlayer_now->IsClosed())
		MediaPlayer_now->Close();
}

void UWidget_Weather::OnEventAnim_near()
{
	SetImgSequence_nearfuture_01();
	SetImgSequence_nearfuture_02();
	SetImgSequence_nearfuture_03();
	bImage_near = !bImage_near;
}

void UWidget_Weather::OnEventAnim_near_close()
{
	if (!bImage_near)
	{
		if (MediaPlayer_nearfuture_01_sec != nullptr && !MediaPlayer_nearfuture_01_sec->IsClosed())
			MediaPlayer_nearfuture_01_sec->Close();
		if (MediaPlayer_nearfuture_02_sec != nullptr && !MediaPlayer_nearfuture_02_sec->IsClosed())
			MediaPlayer_nearfuture_02_sec->Close();
		if (MediaPlayer_nearfuture_03_sec != nullptr && !MediaPlayer_nearfuture_03_sec->IsClosed())
			MediaPlayer_nearfuture_03_sec->Close();
	}
	else
	{
		if (MediaPlayer_nearfuture_01 != nullptr && !MediaPlayer_nearfuture_01->IsClosed())
			MediaPlayer_nearfuture_01->Close();
		if (MediaPlayer_nearfuture_02 != nullptr && !MediaPlayer_nearfuture_02->IsClosed())
			MediaPlayer_nearfuture_02->Close();
		if (MediaPlayer_nearfuture_03 != nullptr && !MediaPlayer_nearfuture_03->IsClosed())
			MediaPlayer_nearfuture_03->Close();
	}
}

void UWidget_Weather::OnEventAnim_low_high()
{
	SetImgSequence_lowest();
	SetImgSequence_highest();
	bImage_low_high = !bImage_low_high;
}

void UWidget_Weather::OnEventAnim_low_high_close()
{
	if (!bImage_low_high)
	{
		if (MediaPlayer_lowest_sec != nullptr && !MediaPlayer_lowest_sec->IsClosed())
			MediaPlayer_lowest_sec->Close();
		if (MediaPlayer_highest_sec != nullptr && !MediaPlayer_highest_sec->IsClosed())
			MediaPlayer_highest_sec->Close();
	}
	else
	{
		if (MediaPlayer_lowest != nullptr && !MediaPlayer_lowest->IsClosed())
			MediaPlayer_lowest->Close();
		if (MediaPlayer_highest != nullptr && !MediaPlayer_highest->IsClosed())
			MediaPlayer_highest->Close();
	}
}

void UWidget_Weather::PlayMainAnim(UWidgetAnimation* mainanim)
{
	//csv파일에 index == 1, 위젯 애님을 재생하지 않음.
	if (maxindex <= 1)
		return;
	PlayAnimation(mainanim);
}

void UWidget_Weather::SetWeather()
{
	if (!bImage_bg)
		PlayAnimation(Dissolve_bg);// SetBackgroundVideo();
	else
		PlayAnimation(Dissolve_bg_sec);
	
	if (!bImage_now)
		PlayAnimation(Dissolve_now); //SetImgSequence_Now();
	else
		PlayAnimation(Dissolve_now_sec);
}

void UWidget_Weather::SetBackgroundVideo()
{
	weather_bg = UHttp_weather::Getdata_now_weather();

	if (TEST_Weather != hyundai_weather::none)
		weather_bg = TEST_Weather;

	FString path = background_video_path;
	GEngine->AddOnScreenDebugMessage(101, 30, FColor::Purple, "BG_video_path : " + background_video_path);

	switch (weather_bg)
	{
	case hyundai_weather::snow_rainy:
		path += FString(TEXT("/눈.mp4"));
		break;
	case hyundai_weather::rainy:
		path += FString(TEXT("/비.mp4"));
		break;
	case hyundai_weather::snow:
		path += FString(TEXT("/눈.mp4"));
		break;
	case hyundai_weather::windy:
		path += FString(TEXT("/강풍.mp4"));
		break;
	case hyundai_weather::cloudy:
		path += FString(TEXT("/흐림.mp4"));
		break;
	case hyundai_weather::mostly_cloudy01:
	case hyundai_weather::mostly_cloudy02:
		path += FString(TEXT("/구름.mp4"));
		break;
	case hyundai_weather::sunny01:
	case hyundai_weather::sunny02:
		path += FString(TEXT("/맑음.mp4"));
		break;
	default:
		break;
	}

	if (bImage_bg)
	{
		if (MediaTexture == nullptr)
			MediaTexture = NewObject<UMediaTexture>(this);
		if (FileMediaSource == nullptr)
			FileMediaSource = NewObject<UFileMediaSource>(this);
		if (MediaPlayer == nullptr)
			MediaPlayer = NewObject<UMediaPlayer>(this);

		FileMediaSource->SetFilePath(path);
		MediaPlayer->SetLooping(true);
		MediaPlayer->OpenSource(FileMediaSource);
		MediaTexture->SetMediaPlayer(MediaPlayer);
		MediaTexture->UpdateResource();
		Image_Background->SetBrushResourceObject(MediaTexture);
	}
	else
	{
		if (MediaTexture_sec == nullptr)
			MediaTexture_sec = NewObject<UMediaTexture>(this);
		if (FileMediaSource_sec == nullptr)
			FileMediaSource_sec = NewObject<UFileMediaSource>(this);
		if (MediaPlayer_sec == nullptr)
			MediaPlayer_sec = NewObject<UMediaPlayer>(this);

		FileMediaSource_sec->SetFilePath(path);
		MediaPlayer_sec->SetLooping(true);
		MediaPlayer_sec->OpenSource(FileMediaSource_sec);
		MediaTexture_sec->SetMediaPlayer(MediaPlayer_sec);
		MediaTexture_sec->UpdateResource();
		Image_Background_sec->SetBrushResourceObject(MediaTexture_sec);
	}
}

void UWidget_Weather::SetImgSequence_Now()
{
	weather_now = UHttp_weather::Getdata_now_weather();

	if (TEST_Weather != hyundai_weather::none)
		weather_now = TEST_Weather;

	FString path = img_sequence_path;
	GEngine->AddOnScreenDebugMessage(102, 30, FColor::Purple, "Img Path : " + img_sequence_path);

	path += ChooseImgPath(weather_now);

	if (bImage_now)
	{
		if (MediaTexture_now == nullptr)
			MediaTexture_now = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_now == nullptr)
			ImgMediaSource_now = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_now == nullptr)
			MediaPlayer_now = NewObject<UMediaPlayer>(this);

		ImgMediaSource_now->SetSequencePath(path);
		MediaPlayer_now->SetLooping(true);
		MediaPlayer_now->OpenSource(ImgMediaSource_now);
		MediaTexture_now->SetMediaPlayer(MediaPlayer_now);
		MediaTexture_now->UpdateResource();
		Image_now->SetBrushResourceObject(MediaTexture_now);
	}
	else
	{
		if (MediaTexture_now_sec == nullptr)
			MediaTexture_now_sec = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_now_sec == nullptr)
			ImgMediaSource_now_sec = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_now_sec == nullptr)
			MediaPlayer_now_sec = NewObject<UMediaPlayer>(this);

		ImgMediaSource_now_sec->SetSequencePath(path);
		MediaPlayer_now_sec->SetLooping(true);
		MediaPlayer_now_sec->OpenSource(ImgMediaSource_now_sec);
		MediaTexture_now_sec->SetMediaPlayer(MediaPlayer_now_sec);
		MediaTexture_now_sec->UpdateResource();
		Image_now_sec->SetBrushResourceObject(MediaTexture_now_sec);
	}
}

void UWidget_Weather::SetImgSequence_nearfuture_01()
{
	weather_nearfuture_01 = UHttp_weather::Getdata_nearfuture_weather_01();

	FString path = img_sequence_path;
	path += ChooseImgPath(weather_nearfuture_01);

	if (bImage_near)
	{
		if (MediaTexture_nearfuture_01 == nullptr)
			MediaTexture_nearfuture_01 = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_nearfuture_01 == nullptr)
			ImgMediaSource_nearfuture_01 = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_nearfuture_01 == nullptr)
			MediaPlayer_nearfuture_01 = NewObject<UMediaPlayer>(this);

		ImgMediaSource_nearfuture_01->SetSequencePath(path);
		MediaPlayer_nearfuture_01->SetLooping(true);
		MediaPlayer_nearfuture_01->OpenSource(ImgMediaSource_nearfuture_01);
		MediaTexture_nearfuture_01->SetMediaPlayer(MediaPlayer_nearfuture_01);
		MediaTexture_nearfuture_01->UpdateResource();
		Image_nearfuture_01->SetBrushResourceObject(MediaTexture_nearfuture_01);
	}
	else
	{
		if (MediaTexture_nearfuture_01_sec == nullptr)
			MediaTexture_nearfuture_01_sec = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_nearfuture_01_sec == nullptr)
			ImgMediaSource_nearfuture_01_sec = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_nearfuture_01_sec == nullptr)
			MediaPlayer_nearfuture_01_sec = NewObject<UMediaPlayer>(this);

		ImgMediaSource_nearfuture_01_sec->SetSequencePath(path);
		MediaPlayer_nearfuture_01_sec->SetLooping(true);
		MediaPlayer_nearfuture_01_sec->OpenSource(ImgMediaSource_nearfuture_01_sec);
		MediaTexture_nearfuture_01_sec->SetMediaPlayer(MediaPlayer_nearfuture_01_sec);
		MediaTexture_nearfuture_01_sec->UpdateResource();
		Image_nearfuture_01_sec->SetBrushResourceObject(MediaTexture_nearfuture_01_sec);
	}
}

void UWidget_Weather::SetImgSequence_nearfuture_02()
{
	weather_nearfuture_02 = UHttp_weather::Getdata_nearfuture_weather_02();

	FString path = img_sequence_path;
	path += ChooseImgPath(weather_nearfuture_02);

	if (bImage_near)
	{
		if (MediaTexture_nearfuture_02 == nullptr)
			MediaTexture_nearfuture_02 = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_nearfuture_02 == nullptr)
			ImgMediaSource_nearfuture_02 = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_nearfuture_02 == nullptr)
			MediaPlayer_nearfuture_02 = NewObject<UMediaPlayer>(this);

		ImgMediaSource_nearfuture_02->SetSequencePath(path);
		MediaPlayer_nearfuture_02->SetLooping(true);
		MediaPlayer_nearfuture_02->OpenSource(ImgMediaSource_nearfuture_02);
		MediaTexture_nearfuture_02->SetMediaPlayer(MediaPlayer_nearfuture_02);
		MediaTexture_nearfuture_02->UpdateResource();
		Image_nearfuture_02->SetBrushResourceObject(MediaTexture_nearfuture_02);
	}
	else
	{
		if (MediaTexture_nearfuture_02_sec == nullptr)
			MediaTexture_nearfuture_02_sec = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_nearfuture_02_sec == nullptr)
			ImgMediaSource_nearfuture_02_sec = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_nearfuture_02_sec == nullptr)
			MediaPlayer_nearfuture_02_sec = NewObject<UMediaPlayer>(this);

		ImgMediaSource_nearfuture_02_sec->SetSequencePath(path);
		MediaPlayer_nearfuture_02_sec->SetLooping(true);
		MediaPlayer_nearfuture_02_sec->OpenSource(ImgMediaSource_nearfuture_02_sec);
		MediaTexture_nearfuture_02_sec->SetMediaPlayer(MediaPlayer_nearfuture_02_sec);
		MediaTexture_nearfuture_02_sec->UpdateResource();
		Image_nearfuture_02_sec->SetBrushResourceObject(MediaTexture_nearfuture_02_sec);
	}
}

void UWidget_Weather::SetImgSequence_nearfuture_03()
{
	weather_nearfuture_03 = UHttp_weather::Getdata_nearfuture_weather_03();

	FString path = img_sequence_path;
	path += ChooseImgPath(weather_nearfuture_03);

	if (bImage_near)
	{
		if (MediaTexture_nearfuture_03 == nullptr)
			MediaTexture_nearfuture_03 = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_nearfuture_03 == nullptr)
			ImgMediaSource_nearfuture_03 = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_nearfuture_03 == nullptr)
			MediaPlayer_nearfuture_03 = NewObject<UMediaPlayer>(this);

		ImgMediaSource_nearfuture_03->SetSequencePath(path);
		MediaPlayer_nearfuture_03->SetLooping(true);
		MediaPlayer_nearfuture_03->OpenSource(ImgMediaSource_nearfuture_03);
		MediaTexture_nearfuture_03->SetMediaPlayer(MediaPlayer_nearfuture_03);
		MediaTexture_nearfuture_03->UpdateResource();
		Image_nearfuture_03->SetBrushResourceObject(MediaTexture_nearfuture_03);
	}
	else
	{
		if (MediaTexture_nearfuture_03_sec == nullptr)
			MediaTexture_nearfuture_03_sec = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_nearfuture_03_sec == nullptr)
			ImgMediaSource_nearfuture_03_sec = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_nearfuture_03_sec == nullptr)
			MediaPlayer_nearfuture_03_sec = NewObject<UMediaPlayer>(this);

		ImgMediaSource_nearfuture_03_sec->SetSequencePath(path);
		MediaPlayer_nearfuture_03_sec->SetLooping(true);
		MediaPlayer_nearfuture_03_sec->OpenSource(ImgMediaSource_nearfuture_03_sec);
		MediaTexture_nearfuture_03_sec->SetMediaPlayer(MediaPlayer_nearfuture_03_sec);
		MediaTexture_nearfuture_03_sec->UpdateResource();
		Image_nearfuture_03_sec->SetBrushResourceObject(MediaTexture_nearfuture_03_sec);
	}
}

void UWidget_Weather::SetImgSequence_lowest()
{
	weather_lowest = UHttp_weather::Getdata_lowest_weather();

	FString path = img_sequence_path;
	path += ChooseImgPath(weather_lowest);

	if (bImage_low_high)
	{
		if (MediaTexture_lowest == nullptr)
			MediaTexture_lowest = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_lowest == nullptr)
			ImgMediaSource_lowest = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_lowest == nullptr)
			MediaPlayer_lowest = NewObject<UMediaPlayer>(this);

		ImgMediaSource_lowest->SetSequencePath(path);
		MediaPlayer_lowest->SetLooping(true);
		MediaPlayer_lowest->OpenSource(ImgMediaSource_lowest);
		MediaTexture_lowest->SetMediaPlayer(MediaPlayer_lowest);
		MediaTexture_lowest->UpdateResource();
		Image_lowest->SetBrushResourceObject(MediaTexture_lowest);
	}
	else
	{
		if (MediaTexture_lowest_sec == nullptr)
			MediaTexture_lowest_sec = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_lowest_sec == nullptr)
			ImgMediaSource_lowest_sec = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_lowest_sec == nullptr)
			MediaPlayer_lowest_sec = NewObject<UMediaPlayer>(this);

		ImgMediaSource_lowest_sec->SetSequencePath(path);
		MediaPlayer_lowest_sec->SetLooping(true);
		MediaPlayer_lowest_sec->OpenSource(ImgMediaSource_lowest_sec);
		MediaTexture_lowest_sec->SetMediaPlayer(MediaPlayer_lowest_sec);
		MediaTexture_lowest_sec->UpdateResource();
		Image_lowest_sec->SetBrushResourceObject(MediaTexture_lowest_sec);
	}
}

void UWidget_Weather::SetImgSequence_highest()
{
	weather_highest = UHttp_weather::Getdata_highest_weather();

	FString path = img_sequence_path;
	path += ChooseImgPath(weather_highest);

	if (bImage_low_high)
	{
		if (MediaTexture_highest == nullptr)
			MediaTexture_highest = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_highest == nullptr)
			ImgMediaSource_highest = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_highest == nullptr)
			MediaPlayer_highest = NewObject<UMediaPlayer>(this);

		ImgMediaSource_highest->SetSequencePath(path);
		MediaPlayer_highest->SetLooping(true);
		MediaPlayer_highest->OpenSource(ImgMediaSource_highest);
		MediaTexture_highest->SetMediaPlayer(MediaPlayer_highest);
		MediaTexture_highest->UpdateResource();
		Image_highest->SetBrushResourceObject(MediaTexture_highest);
	}
	else
	{
		if (MediaTexture_highest_sec == nullptr)
			MediaTexture_highest_sec = NewObject<UMediaTexture>(this);
		if (ImgMediaSource_highest_sec == nullptr)
			ImgMediaSource_highest_sec = NewObject<UImgMediaSource>(this);
		if (MediaPlayer_highest_sec == nullptr)
			MediaPlayer_highest_sec = NewObject<UMediaPlayer>(this);

		ImgMediaSource_highest_sec->SetSequencePath(path);
		MediaPlayer_highest_sec->SetLooping(true);
		MediaPlayer_highest_sec->OpenSource(ImgMediaSource_highest_sec);
		MediaTexture_highest_sec->SetMediaPlayer(MediaPlayer_highest_sec);
		MediaTexture_highest_sec->UpdateResource();
		Image_highest_sec->SetBrushResourceObject(MediaTexture_highest_sec);
	}
}

FString UWidget_Weather::ChooseImgPath(hyundai_weather weather)
{
	switch (weather)
	{
	case hyundai_weather::snow_rainy:
		return FString(TEXT("/Snow_rainy/FOGGY-SNOW-RAIN000.png"));
	case hyundai_weather::rainy:
		return FString(TEXT("/Rainy/RAINY000.png"));
	case hyundai_weather::snow:
		return FString(TEXT("/Snow/SNOW000.png"));
	case hyundai_weather::windy:
		return FString(TEXT("/Windy/WINDY000.png"));
	case hyundai_weather::cloudy:
		return FString(TEXT("/Cloudy/CLOUDY000.png"));
	case hyundai_weather::mostly_cloudy01:
		return FString(TEXT("/Mostly_cloudy01/FOGGY-SUN000.png"));
	case hyundai_weather::mostly_cloudy02:
		return FString(TEXT("/Mostly_cloudy02/FOGGY-MOON000.png"));
	case hyundai_weather::sunny01:
		return FString(TEXT("/Sunny01/SUNNY000.png"));
	case hyundai_weather::sunny02:
		return FString(TEXT("/Sunny02/MOON000.png"));
	}

	return FString();
}

void UWidget_Weather::SetCustomFont()
{
	auto viewport = UWidgetLayoutLibrary::GetViewportSize(GetWorld());

	//(폰트사이즈)/(뷰포트 가로or세로 기준크기)
	float origin_ratio1 = 52.f / 6144.f;
	float origin_ratio2 = 40.f / 6144.f;
	float origin_ratio3 = 52.f / 1312.f;
	float origin_ratio4 = 40.f / 1312.f;

	float origin_ratio5 = 30.f / 6144.f;
	float origin_ratio6 = 30.f / 1312.f;

	float origin_ratio7 = 130.f / 6144.f;
	float origin_ratio8 = 130.f / 1312.f;
	float origin_ratio9 = 48.f / 6144.f;
	float origin_ratio10 = 48.f / 1312.f;

	float custom_ratio1 = viewport.X * origin_ratio1;
	float custom_ratio2 = viewport.X * origin_ratio2;
	float custom_ratio3 = viewport.Y * origin_ratio3;
	float custom_ratio4 = viewport.Y * origin_ratio4;

	float custom_ratio5 = viewport.X * origin_ratio5;
	float custom_ratio6 = viewport.Y * origin_ratio6;
	
	float custom_ratio7 = viewport.X * origin_ratio7;
	float custom_ratio8 = viewport.Y * origin_ratio8;
	float custom_ratio9 = viewport.X * origin_ratio9;
	float custom_ratio10 = viewport.Y * origin_ratio10;

	//가로 세로 중 작은 폰트 사이즈로 설정
	TextBlock_maintext->SetFont(FSlateFontInfo(Font_Noto_bold, custom_ratio1 < custom_ratio3 ? custom_ratio1 : custom_ratio3));
	TextBlock_subtext->SetFont(FSlateFontInfo(Font_Noto_regular, custom_ratio2 < custom_ratio4 ? custom_ratio2 : custom_ratio4));
	TextBlock_nearfuture_tmp_01->SetFont(FSlateFontInfo(Font_semibold, custom_ratio5 < custom_ratio6 ? custom_ratio5 : custom_ratio6));
	TextBlock_nearfuture_tmp_02->SetFont(FSlateFontInfo(Font_semibold, custom_ratio5 < custom_ratio6 ? custom_ratio5 : custom_ratio6));
	TextBlock_nearfuture_tmp_03->SetFont(FSlateFontInfo(Font_semibold, custom_ratio5 < custom_ratio6 ? custom_ratio5 : custom_ratio6));
	TextBlock_lowest->SetFont(FSlateFontInfo(Font_semibold, custom_ratio5 < custom_ratio6 ? custom_ratio5 : custom_ratio6));
	TextBlock_highest->SetFont(FSlateFontInfo(Font_semibold, custom_ratio5 < custom_ratio6 ? custom_ratio5 : custom_ratio6));
	TextBlock_Now->SetFont(FSlateFontInfo(Font_bold, custom_ratio7 < custom_ratio8 ? custom_ratio7 : custom_ratio8));
	TextBlock_Now_->SetFont(FSlateFontInfo(Font_bold, custom_ratio9 < custom_ratio10 ? custom_ratio9 : custom_ratio10));
}

void UWidget_Weather::SetNearFutureText()
{
	FString s_hour01, s_hour02, s_hour03;
	FString text01, text02, text03;
	FString str;


	str = TEXT("오늘 ");
	int32 hour01 = UKismetMathLibrary::Now().GetHour() + 1;
	if (hour01 > 23)
	{
		hour01 = 0;
		str = TEXT("내일 ");
	}

	s_hour01 = FString::FromInt(hour01);
	if (hour01 < 10)
		s_hour01 = FString("0") + FString::FromInt(hour01);

	text01 = str + s_hour01 + TEXT("시");

	str = TEXT("오늘 ");
	int32 hour02 = hour01 + 1;
	if (hour02 > 23)
	{
		hour02 = 0;
		str = TEXT("내일 ");
	}

	s_hour02 = FString::FromInt(hour02);
	if (hour02 < 10)
		s_hour02 = FString("0") + FString::FromInt(hour02);

	text02 = str + s_hour02 + TEXT("시");

	str = TEXT("오늘 ");
	int32 hour03 = hour02 + 1;
	if (hour03 > 23)
	{
		hour03 = 0;
		str = TEXT("내일 ");
	}

	s_hour03 = FString::FromInt(hour03);
	if (hour03 < 10)
		s_hour03 = FString("0") + FString::FromInt(hour03);

	text03 = str + s_hour03 + TEXT("시");

	TextBlock_nearfuture_01->SetText(FText::FromString(text01));
	TextBlock_nearfuture_02->SetText(FText::FromString(text02));
	TextBlock_nearfuture_03->SetText(FText::FromString(text03));
}

void UWidget_Weather::SetDateAndTime(FString& base_date, FString& base_time, uint8 calledAPI)
{
	FDateTime datetime = UKismetMathLibrary::Now().GetDate();

	int32 year = datetime.GetYear();
	int32 month = datetime.GetMonth();
	int32 day = datetime.GetDay();

	FString s_year = FString::FromInt(year);
	FString s_month = FString::FromInt(month);
	FString s_day = FString::FromInt(day);

	if (month < 10)
		s_month = FString("0") + s_month;
	if (day < 10)
		s_day = FString("0") + s_day;

	base_date = s_year + s_month + s_day;

	//단기예보
	if (calledAPI == 1)
	{
		int32 hour = UKismetMathLibrary::Now().GetHour();
		int32 min = UKismetMathLibrary::Now().GetMinute();

		FString s_hour = "";
		FString s_min = FString::FromInt(min);
		if(min<10)
			s_min = FString("0") + FString::FromInt(min);

		if (hour >= 2 && hour < 5)
			s_hour = "02";
		else if (hour >= 5 && hour < 8)
			s_hour = "05";
		else if (hour >= 8 && hour < 11)
			s_hour = "08";
		else if (hour >= 11 && hour < 14)
			s_hour = "11";
		else if (hour >= 14 && hour < 17)
			s_hour = "14";
		else if (hour >= 17 && hour < 20)
			s_hour = "17";
		else if (hour >= 20 && hour < 23)
			s_hour = "20";
		else if (hour >= 23)
			s_hour = "23";
		else//hour == 0 or hour == 01 자정이거나 새벽 1시일 경우
		{
			s_hour = "23";
			if (day != 1)
				day -= 1;
			else
			{
				if (month != 1)
				{
					month -= 1;
					day = UKismetMathLibrary::Now().DaysInMonth(year, month);
				}
				else
				{
					year -= 1;
					month = 12;
					day = UKismetMathLibrary::Now().DaysInMonth(year, month);
				}
			}

			//base_date 전날로 갱신
			base_date = FString::FromInt(year) + FString::FromInt(month) + FString::FromInt(day);
		}

		base_time = s_hour + s_min;
	}

	//초단기실황
	if (calledAPI == 2)
	{
		int32 hour = UKismetMathLibrary::Now().GetHour();
		int32 min = UKismetMathLibrary::Now().GetMinute();

		FString s_hour = FString::FromInt(hour);
		FString s_min = FString::FromInt(min);
		if (hour < 10)
			s_hour = FString("0") + FString::FromInt(hour);
		if (min < 10)
			s_min = FString("0") + FString::FromInt(min);

		if (min < 20)//API 제공시간 이전이라면 시간을 한 시간 전으로 설정.
		{
			int32 temp = hour - 1;
			if (temp < 0)
			{
				temp = 23;
				if (day != 1)
					day -= 1;
				else
				{
					if (month != 1)
					{
						month -= 1;
						day = UKismetMathLibrary::Now().DaysInMonth(year, month);
					}
					else
					{
						year -= 1;
						month = 12;
						day = UKismetMathLibrary::Now().DaysInMonth(year, month);
					}
				}

				//base_date 전날로 갱신
				base_date = FString::FromInt(year) + FString::FromInt(month) + FString::FromInt(day);
			}
			s_hour = FString::FromInt(temp);
			if (temp < 10)
				s_hour = FString("0") + s_hour;
		}

		base_time = s_hour + s_min;
	}

	//초단기예보
	if (calledAPI == 3)
	{
		int32 hour = UKismetMathLibrary::Now().GetHour();
		int32 min = UKismetMathLibrary::Now().GetMinute();

		FString s_hour = FString::FromInt(hour);
		FString s_min = FString::FromInt(min);
		if (hour < 10)
			s_hour = FString("0") + FString::FromInt(hour);
		if (min < 10)
			s_min = FString("0") + FString::FromInt(min);

		if (min < 30)//API 제공시간 이전이라면 시간을 한 시간 전으로 설정.
		{
			int32 temp = hour - 1;
			if (temp < 0)
			{
				temp = 23;
				if (day != 1)
					day -= 1;
				else
				{
					if (month != 1)
					{
						month -= 1;
						day = UKismetMathLibrary::Now().DaysInMonth(year, month);
					}
					else
					{
						year -= 1;
						month = 12;
						day = UKismetMathLibrary::Now().DaysInMonth(year, month);
					}
				}

				//base_date 전날로 갱신
				base_date = FString::FromInt(year) + FString::FromInt(month) + FString::FromInt(day);
			}
			s_hour = FString::FromInt(temp);
			if (temp < 10)
				s_hour = FString("0") + s_hour;
		}

		base_time = s_hour + s_min;
	}
}

void UWidget_Weather::SetDateAndTimeOneminus(FString& base_date, FString& base_time)
{
	FDateTime datetime = UKismetMathLibrary::Now().GetDate();

	int32 year = datetime.GetYear();
	int32 month = datetime.GetMonth();
	int32 day = datetime.GetDay();

	FString s_year = FString::FromInt(year);
	FString s_month = FString::FromInt(month);
	FString s_day = FString::FromInt(day);

	if (month < 10)
		s_month = FString("0") + s_month;
	if (day < 10)
		s_day = FString("0") + s_day;

	base_date = s_year + s_month + s_day;

	int32 hour = UKismetMathLibrary::Now().GetHour() - 1; //확정으로 한 시간 전을 설정.
	if (hour < 0)
	{
		hour = 23;
		if (day != 1)
			day -= 1;
		else
		{
			if (month != 1)
			{
				month -= 1;
				day = UKismetMathLibrary::Now().DaysInMonth(year, month);
			}
			else
			{
				year -= 1;
				month = 12;
				day = UKismetMathLibrary::Now().DaysInMonth(year, month);
			}
		}
		//base_date 갱신
		base_date = FString::FromInt(year) + FString::FromInt(month) + FString::FromInt(day);
	}
	int32 min = UKismetMathLibrary::Now().GetMinute();

	FString s_hour = FString::FromInt(hour);
	FString s_min = FString::FromInt(min);
	if (hour < 10)
		s_hour = FString("0") + s_hour;
	if (min < 10)
		s_min = FString("0") + s_min;

	base_time = s_hour + s_min;
}
