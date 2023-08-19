// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Http.h"
#include "Http_weather.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(weather_api, Log, All);

UENUM(BlueprintType)
enum class hyundai_weather : uint8//01 : ����, 02 : ����. ���� ���� ���� �켱���� ����.
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

struct weather_bitfield // �켱�����ʹ� �������.
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
	//Ư�� �Լ�
	static void UltraSrtFutureOneminus(FString base_date, FString base_time);


	//��� ����.
	void OnResponseReceived_MidFutureCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//�ְ�, ���� ����� ���� ������ ����.
	void OnResponseReceived_VilageFutureCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//����, +1,+2,+3 ���� �� 4���� ���� ������ ����.
	void OnResponseReceived_UltraSrtFutureCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//���� ������ ���� x
	void OnResponseReceived_UltraSrtNowCast(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//Ư�� �Լ��� �ݹ��Լ�.
	void OnResponseReceived_UltraSrtFutureOneminus(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//���
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

	//���� ������
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


	//���� ��� ������ ����ϱ� ���� �ݹ� �Լ� ����ϱ�.
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
