// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldInfoToJson.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "APortalActor.h"

WorldInfoToJson::WorldInfoToJson()
{
}

WorldInfoToJson::~WorldInfoToJson()
{
}

void WorldInfoToJson::SaveWorldInfoToJson(UWorld* World, int MapID)
{
    if (World == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Save Fail : World is nullptr"));
        return;
    }
    FString MapName = World->GetMapName();
    FString OutputString = FString::Printf(TEXT("{\n\t\"MapID\": %d,\n\t\"MapName\": \"%s\",\n\t\"Obstacles\": [\n"), MapID, *MapName);

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(Actor);

        if (MeshActor == nullptr)
		{
			continue;
		}

        UStaticMeshComponent* MeshComponent = MeshActor->GetStaticMeshComponent();

        FTransform Transform = Actor->GetActorTransform();
        FVector Location = Transform.GetLocation();
        FVector Scale = Transform.GetScale3D();

        FBox BoundingBox = MeshComponent->GetStaticMesh()->GetBoundingBox();
        FVector MeshSize = BoundingBox.GetSize();

        FString MeshName = MeshComponent->GetStaticMesh()->GetName();

        OutputString += FString::Printf(TEXT("\t\t{\"Location\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"Scale\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"MeshSize\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"MeshName\": \"%s\"},\n"),
            Location.X, Location.Y, Location.Z, Scale.X, Scale.Y, Scale.Z, MeshSize.X, MeshSize.Y, MeshSize.Z, *MeshName);
    }
    OutputString.RemoveFromEnd(",\n");
    OutputString += "\n\t]\n}";

    FFileHelper::SaveStringToFile(OutputString, *(FPaths::ProjectDir() + "/MapFiles/" + MapName + ".json"));
}

void WorldInfoToJson::SaveWorldPortalInfoToJson(UWorld* World, int MapID)
{
    if (World == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Save Fail : World is nullptr"));
        return;
    }
    FString MapName = World->GetMapName();
    FString OutputString = FString::Printf(TEXT("{\n\t\"MapID\": %d,\n\t\"MapName\": \"%s\",\n\t\"Portals\": [\n"), MapID, *MapName);

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AAPortalActor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AAPortalActor* PortalActor = Cast<AAPortalActor>(Actor);

        if (PortalActor == nullptr)
        {
			continue;
		}

        FTransform Transform = Actor->GetActorTransform();
        FVector Location = Transform.GetLocation();
        FVector Scale = Transform.GetScale3D();
        FVector BoxComponentSize;
        PortalActor->GetPortalBoxExtent(BoxComponentSize);
        int32 TargetMapID = PortalActor->GetTargetMapID();

        OutputString += FString::Printf(TEXT("\t\t{\"Location\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"Scale\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"BoxSize\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"LinkMapID\": %d},\n"),
            Location.X, Location.Y, Location.Z, Scale.X, Scale.Y, Scale.Z, BoxComponentSize.X, BoxComponentSize.Y, BoxComponentSize.Z, TargetMapID);
    }
    OutputString.RemoveFromEnd(",\n");
    OutputString += "\n\t]\n}";
    FFileHelper::SaveStringToFile(OutputString, *(FPaths::ProjectDir() + "/MapLinkFiles/" + MapName + "Link.json"));
}
