// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetToolFunctionLibrary.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "HAL/IConsoleManager.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"

#include "EditorAssetLibrary.h"
#include "FileHelpers.h" 



// �p�b�P�[�W�����w�肵�ăA�Z�b�g���擾����
bool UAssetToolFunctionLibrary::GetAssetsByPackageName(const FName &PackageName, TArray<FAssetData> &OutAssetData, bool bIncludeOnlyOnDiskAssets)
{
    if (PackageName.IsNone())
    {
        return false;
    }

    FARFilter Filter;
    Filter.PackageNames.Add(PackageName);
    Filter.bIncludeOnlyOnDiskAssets = bIncludeOnlyOnDiskAssets;

    return IAssetRegistry::Get()->GetAssets(Filter, OutAssetData);
}

// �w�肵���p�X�ȉ��̃p�b�P�[�W���擾����
bool UAssetToolFunctionLibrary::GetAssetsByPaths(const TArray<FName>& PackagePaths, TArray<FAssetData>& OutAssetData, bool bRecursivePaths, bool bIncludeOnlyOnDiskAssets)
{
    FARFilter Filter;
    Filter.bRecursivePaths = bRecursivePaths;
    Filter.bIncludeOnlyOnDiskAssets = bIncludeOnlyOnDiskAssets;

    if (!PackagePaths.IsEmpty())
    {
        Filter.PackagePaths.Append(PackagePaths);
    }

    return IAssetRegistry::Get()->GetAssets(Filter, OutAssetData);
}

// �w�肵���N���X�����S�ẴA�Z�b�g���擾����
bool UAssetToolFunctionLibrary::GetAssetsByClass(FTopLevelAssetPath ClassPathName, TArray<FAssetData> &OutAssetData, bool bSearchSubClasses)
{
    if (!ClassPathName.IsValid())
    {
        return false;
    }

    FARFilter Filter;
    Filter.ClassPaths.Add(ClassPathName);
    Filter.bRecursiveClasses = bSearchSubClasses;

    return IAssetRegistry::Get()->GetAssets(Filter, OutAssetData);
}

// �w�肵���p�b�P�[�W���ˑ�����A�Z�b�g���擾
bool UAssetToolFunctionLibrary::GetDependencies(const FName& PackageName, TArray<FAssetData>& OutDependencyAssetData, bool bIncludeOnlyOnDiskAssets)
{
    if (PackageName.IsNone())
    {
        return false;
    }

    TArray<FAssetIdentifier> TempAssetIdentifier;
    bool bResult = IAssetRegistry::Get()->GetDependencies(FAssetIdentifier(PackageName), TempAssetIdentifier);

    if (bResult)
    {
        for (const auto& AssetId : TempAssetIdentifier)
        {
            if (AssetId.PackageName != NAME_None)
            {
                TArray<FAssetData> TempAssetData;
                GetAssetsByPackageName(AssetId.PackageName, TempAssetData, bIncludeOnlyOnDiskAssets);

                if (!TempAssetData.IsEmpty())
                {
                    OutDependencyAssetData.Append(TempAssetData);
                }
            }
        }
    }

    return bResult;
}

// �w�肵���p�b�P�[�W���Q�Ƃ���A�Z�b�g���擾
bool UAssetToolFunctionLibrary::GetReferencers(const FName& PackageName, TArray<FAssetData>& OutDependencyAssetData, bool bIncludeOnlyOnDiskAssets)
{
    if (PackageName.IsNone())
    {
        return false;
    }

    TArray<FAssetIdentifier> TempAssetIdentifier;
    bool bResult = IAssetRegistry::Get()->GetReferencers(FAssetIdentifier(PackageName), TempAssetIdentifier);

    if (bResult) {
        for (const auto &AssetId : TempAssetIdentifier)
        {
            if (AssetId.PackageName != NAME_None) {
                TArray<FAssetData> TempAssetData;
                GetAssetsByPackageName(AssetId.PackageName, TempAssetData, bIncludeOnlyOnDiskAssets);

                if (!TempAssetData.IsEmpty()) {
                    OutDependencyAssetData.Append(TempAssetData);
                }
            }
        }
    }

    return bResult;
}

// ���[�h�ς݃A�Z�b�g�𕡐�����
UObject*UAssetToolFunctionLibrary::DuplicateLoadedAsset(UObject* SrcAsset, const FName& DstPackageName)
{
    if (!IsValid(SrcAsset) || DstPackageName.IsNone()) {
        return nullptr;
    }

    return UEditorAssetLibrary::DuplicateLoadedAsset(SrcAsset, DstPackageName.ToString());
}

// �A�Z�b�g�𕡐�����
UObject* UAssetToolFunctionLibrary::DuplicateAsset(const FName& SrcPackageName, const FName& DstPackageName)
{
    if (SrcPackageName.IsNone() || DstPackageName.IsNone())
    {
        return nullptr;
    }

    const FString SrcPackageNameStr = SrcPackageName.ToString();
    const FString DstPackageNameStr = DstPackageName.ToString();

    return UEditorAssetLibrary::DuplicateAsset(SrcPackageNameStr, DstPackageNameStr);
}

#if WITH_EDITOR

// �w��p�X�ȉ��̃A�Z�b�g���擾
static FAutoConsoleCommand TestGetAssetByPaths = FAutoConsoleCommand(
    TEXT("Test.AssetTools.GetAssetsByPaths"),
    TEXT("Test.AssetTools.GetAssetsByPaths [PackagePath...]"),
    FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
    {
        TArray<FName> PackagePaths;

        if (0 < Args.Num())
        {
            for (auto &Arg : Args)
            {
                PackagePaths.Add(FName(Arg));
            }
        }
        
        const bool bRecursivePaths = true;
        const bool bIncludeOnlyOnDiskAssets = false;

        TArray<FAssetData> OutAssetData;
        if (UAssetToolFunctionLibrary::GetAssetsByPaths(PackagePaths, OutAssetData, bRecursivePaths, bIncludeOnlyOnDiskAssets))
        {
            int32 Index = 0;
            for (auto& AssetData : OutAssetData)
            {
                UE_LOG(LogTemp, Log, TEXT("Found asset[%d]: '%s' %s"), Index++, *(AssetData.AssetClassPath.GetAssetName().ToString()), *(AssetData.PackageName.ToString()));
            }
        }
    }));

// �S�Ẵ}�e���A���A�Z�b�g���擾
static FAutoConsoleCommand TestGetAllMaterialAssets = FAutoConsoleCommand(
    TEXT("Test.AssetTools.GetAllMaterialAssets"),
    TEXT("Get all material assets"),
    FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
    {
        const bool bSearchSubClasses = true;

        TArray<FAssetData> OutAssetData;
        if (UAssetToolFunctionLibrary::GetAssetsByClass(UMaterial::StaticClass()->GetClassPathName(), OutAssetData, bSearchSubClasses))
        {
            int32 Index = 0;
            for (auto &AssetData : OutAssetData) {
                UE_LOG(LogTemp, Log, TEXT("Found asset[%d]: '%s' %s"), Index++, *(AssetData.AssetClassPath.GetAssetName().ToString()), *(AssetData.PackageName.ToString()));
            }
        }
    }));

// �S�Ă̐ÓI���b�V���A�Z�b�g���擾
static FAutoConsoleCommand TestGetAllStaticMeshAssets = FAutoConsoleCommand(
    TEXT("Test.AssetTools.GetAllStaticMeshAssets"),
    TEXT("Get all static mesh assets"),
    FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
    {
        const bool bSearchSubClasses = true;

        TArray<FAssetData> OutAssetData;
        if (UAssetToolFunctionLibrary::GetAssetsByClass(UStaticMesh::StaticClass()->GetClassPathName(), OutAssetData, bSearchSubClasses))
        {
            int32 Index = 0;
            for (auto &AssetData : OutAssetData) {
                UE_LOG(LogTemp, Log, TEXT("Found asset[%d]: '%s' %s"), Index++, *(AssetData.AssetClassPath.GetAssetName().ToString()), *(AssetData.PackageName.ToString()));
            }
        }
    }));

// �ˑ�����A�Z�b�g��S�Ď擾
static FAutoConsoleCommand TestGetDependencies = FAutoConsoleCommand(
    TEXT("Test.AssetTools.GetDependencies"),
    TEXT("Test.AssetTools.GetDependencies PackageName"),
    FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
    {
        if (0 == Args.Num()) {
            return;
        }

        const FName PackageName(Args[0]);
        const bool bIncludeOnlyOnDiskAssets = false;

        TArray<FAssetData> OutAssetData;
        if (UAssetToolFunctionLibrary::GetDependencies(PackageName, OutAssetData, bIncludeOnlyOnDiskAssets)) {
            int32 Index = 0;
            for (auto &AssetData : OutAssetData) {
                UE_LOG(LogTemp, Log, TEXT("Found dependency asset[%d]: '%s' %s"), Index++, *(AssetData.AssetClassPath.GetAssetName().ToString()), *(AssetData.PackageName.ToString()));
            }
        }
    }));

// �Q�Ƃ��Ă���A�Z�b�g��S�Ď擾
static FAutoConsoleCommand TestGetReferencers = FAutoConsoleCommand(
    TEXT("Test.AssetTools.GetReferencers"),
    TEXT("Test.AssetTools.GetReferencers PackageName"),
    FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args) {
    if (0 == Args.Num()) {
        return;
    }

    const FName PackageName(Args[0]);
    const bool bIncludeOnlyOnDiskAssets = false;

    TArray<FAssetData> OutAssetData;
    if (UAssetToolFunctionLibrary::GetReferencers(PackageName, OutAssetData, bIncludeOnlyOnDiskAssets)) {
        int32 Index = 0;
        for (auto &AssetData : OutAssetData) {
            UE_LOG(LogTemp, Log, TEXT("Found reference asset[%d]: '%s' %s"), Index++, *(AssetData.AssetClassPath.GetAssetName().ToString()), *(AssetData.PackageName.ToString()));
        }
    }
}));

#endif // WITH_EDITOR
