// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralMeshComponent.h"
#include "KismetImpostorBakerLibrary.generated.h"

UCLASS(meta = (ScriptName = "ImpostorBakerLibrary"))
class IMPOSTORBAKER_API UKismetImpostorBakerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Convert ProceduralMesh to StaticMesh and save the new asset in specified location. */
	UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh")
	static UStaticMesh* ConvertProceduralToStatic(UProceduralMeshComponent* ProceduralMeshComponent, FName MeshName, FString MeshPath);

};
