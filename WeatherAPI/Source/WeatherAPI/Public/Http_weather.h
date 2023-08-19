// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Http.h"
#include "Http_weather.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(weather_api, Log, All);

UENUM(BlueprintType)
enum class hyundai_weather : uint8//01 : 오전, 02 : 오후. 수가 낮을 수록 우선순위 높음.
{
	snow_rainy = 0,
	rainy,
	snow,
	windy,
	cloudy,
	mostly_cloudy01,
	mostly_cloudy02,
	sunny01,
	sunny02,
	none,
};

struct weather_bitfield // 우선순위와는 상관없음.
{
	weather_bitfield() : sunny(0), mostly_cloudy(0), cloudy(0), windy(0), snow_rainy(0), rainy(0), snow(0) {};
	unsigned char sunny : 1;
	unsigned char mostly_cloudy : 1;
	unsigned char cloudy : 1;
	unsigned char windy : 1;
	unsigned char snow_rainy : 1;
	unsigned char rainy : 1;
	unsigned char snow : 1;
};

UCLASS()
class WEATHERAPI_API UHttp_weather : public UObject
{
	GENERATED_BODY()

public:
	FHttpModule* Http;
	// Sets default values for this actor's properties
	UHttp_weather();

	UFUNCTION(BlueprintCallable, Category = "HTTP")
		static void MidFutureCast(FString base_date, FString base_time);
	UFUNCTION(BlueprintCallable, Category = "HTTP")
		static void VilageFutureCast(FString base_date, FString base_time);
	UFUNCTION(BlueprintCallable, Category = "HTTP")
		static void UltraSrtFutureCast(FString base_date, FString base_time);
	UFUNCTION(BlueprintCallable, Category = "HTTP")
		static void UltraSrtNowCast(FString base_date, FString base_time);
	//특수 함수
	static void UltraSrtFutureOneminus(FString base_date, FString base_time);


	//사용 안함.
	void OnResponseReceived_MidFutureCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//최고, 최저 기온의 날씨 아이콘 선별.
	void OnResponseReceived_VilageFutureCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//현재, +1,+2,+3 시의 총 4개의 날씨 아이콘 선별.
	void OnResponseReceived_UltraSrtFutureCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//날씨 아이콘 선별 x
	void OnResponseReceived_UltraSrtNowCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//특수 함수의 콜백함수.
	void OnResponseReceived_UltraSrtFutureOneminus(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//기온
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static FString Getdata_lowest_tmp() { return Instance ? Instance->lowest_tmp : FString(); }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static FString Getdata_highest_tmp() { return Instance ? Instance->highest_tmp : FString(); };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static FString Getdata_now_tmp() { return Instance ? Instance->now_tmp : FString(); };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static FString Getdata_nearfuture_tmp_01() { return Instance ? Instance->nearfuture_tmp_01 : FString(); };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static FString Getdata_nearfuture_tmp_02() { return Instance ? Instance->nearfuture_tmp_02 : FString(); };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static FString Getdata_nearfuture_tmp_03() { return Instance ? Instance->nearfuture_tmp_03 : FString(); };

	//날씨 아이콘
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static hyundai_weather Getdata_now_weather() { return Instance ? Instance->now_weather : hyundai_weather(); };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static hyundai_weather Getdata_nearfuture_weather_01() { return Instance ? Instance->nearfuture_weather_01 : hyundai_weather(); };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static hyundai_weather Getdata_nearfuture_weather_02() { return Instance ? Instance->nearfuture_weather_02 : hyundai_weather(); };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static hyundai_weather Getdata_nearfuture_weather_03() { return Instance ? Instance->nearfuture_weather_03 : hyundai_weather(); };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static hyundai_weather Getdata_lowest_weather() { return Instance ? Instance->lowest_weather : hyundai_weather(); };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HTTP")
		static hyundai_weather Getdata_highest_weather() { return Instance ? Instance->highest_weather : hyundai_weather(); };


	//위젯 배경 영상을 재생하기 위해 콜백 함수 등록하기.
	static void SetWeatherPointer(UUserWidget* widget);

private:
	hyundai_weather BitfieldToStruct(weather_bitfield bitfield, int32 additional_time = 0);

	static UHttp_weather* Instance;

	FString lowest_tmp;
	FString highest_tmp;
	FString now_tmp;
	FString nearfuture_tmp_01;
	FString nearfuture_tmp_02;
	FString nearfuture_tmp_03;

	hyundai_weather now_weather;
	hyundai_weather nearfuture_weather_01;
	hyundai_weather nearfuture_weather_02;
	hyundai_weather nearfuture_weather_03;
	hyundai_weather lowest_weather;
	hyundai_weather highest_weather;

	UUserWidget* Widget_Weather;
};
