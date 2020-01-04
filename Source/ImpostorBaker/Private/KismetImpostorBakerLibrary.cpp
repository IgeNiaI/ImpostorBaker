// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved. 

#include "KismetImpostorBakerLibrary.h"
#include "Engine/StaticMesh.h"
#include "ProceduralMeshConversion.h"
#include "AssetRegistryModule.h"
#include "MeshDescriptionOperations.h"

UStaticMesh* UKismetImpostorBakerLibrary::ConvertProceduralToStatic(UProceduralMeshComponent* ProceduralMeshComponent, FName MeshName, FString MeshPath)
{
	FMeshDescription MeshDescription = BuildMeshDescription(ProceduralMeshComponent);
	MeshPath = MeshPath + MeshName.ToString();

	// If we got some valid data.
	if (MeshDescription.Polygons().Num() > 0)
	{
		// Then find/create it.
		UPackage* Package = CreatePackage(NULL, *MeshPath);
		check(Package);

		// Create StaticMesh object
		UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, MeshName, RF_Public | RF_Standalone);
		StaticMesh->InitResources();

		StaticMesh->LightingGuid = FGuid::NewGuid();

		// Add source to new StaticMesh
		FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
		SrcModel.BuildSettings.bRecomputeNormals = false;
		SrcModel.BuildSettings.bRecomputeTangents = false;
		SrcModel.BuildSettings.bRemoveDegenerates = false;
		SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
		SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
		SrcModel.BuildSettings.bGenerateLightmapUVs = true;
		SrcModel.BuildSettings.SrcLightmapIndex = 0;
		SrcModel.BuildSettings.DstLightmapIndex = 1;
		StaticMesh->CreateMeshDescription(0, MoveTemp(MeshDescription));
		StaticMesh->CommitMeshDescription(0);

		TSet<UMaterialInterface*> UniqueMaterials;
		const int32 NumSections = ProceduralMeshComponent->GetNumSections();
		for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
		{
			FProcMeshSection *ProcSection =
				ProceduralMeshComponent->GetProcMeshSection(SectionIdx);
			UMaterialInterface *Material = ProceduralMeshComponent->GetMaterial(SectionIdx);
			UniqueMaterials.Add(Material);
		}
		// Copy materials to new mesh
		for (auto* Material : UniqueMaterials)
		{
			StaticMesh->StaticMaterials.Add(FStaticMaterial(Material, Material->GetFName(), Material->GetFName()));
		}

		//Set the Imported version before calling the build
		StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;

		// Build mesh from source
		StaticMesh->Build(false);
		StaticMesh->PostEditChange();

		// Notify asset registry of new asset
		FAssetRegistryModule::AssetCreated(StaticMesh);

		return StaticMesh;
	}

	return nullptr;
}
