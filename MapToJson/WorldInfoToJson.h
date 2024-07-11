// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTKJCLIENT_API WorldInfoToJson
{
public:
	WorldInfoToJson();
	~WorldInfoToJson();
	static void SaveWorldInfoToJson(UWorld* World, int MapID);
	static void SaveWorldPortalInfoToJson(UWorld* World, int MapID);
};
