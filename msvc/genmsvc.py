# genmsvc.py - generate MSVC files for a QMM plugin
# Copyright 2004-2025
# https://github.com/thecybermind/stub_qmm/
# 3-clause BSD license: https://opensource.org/license/bsd-3-clause
# 
# Created By:
#     Kevin Masterson < cybermind@gmail.com >

import os
import re
import sys

games = [
    "Q3A",
]

builds = [
    "Debug",
    "Release",
]

arches = [
    ("x86", "Win32"),
    ("x64", "x64"),
]


def gen_sln(name):
    with open(f"{name}.sln", "w", encoding="utf-8-sig") as f:
        f.write(
            f"""
Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio Version 17
VisualStudioVersion = 17.12.35728.132
MinimumVisualStudioVersion = 10.0.40219.1
Project("{{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}}") = "{name}", "{name}.vcxproj", "{{1D5BE629-82D1-4511-8559-5116BFE4338C}}"
EndProject
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
"""
        )
        for build in builds:
            for game in games:
                for arch, _ in arches:
                    f.write(f"		{build}-{game}|{arch} = {build}-{game}|{arch}\n")
        f.write(
            """	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
"""
        )
        for build in builds:
            for game in games:
                for arch, arch2 in arches:
                    if arch in ["x86"] and game in ["Q2R"]:
                        arch2 = "x64"
                    f.write(
                        f"		{{1D5BE629-82D1-4511-8559-5116BFE4338C}}.{build}-{game}|{arch}.ActiveCfg = {build}-{game}|{arch2}\n"
                    )
                    f.write(
                        f"		{{1D5BE629-82D1-4511-8559-5116BFE4338C}}.{build}-{game}|{arch}.Build.0 = {build}-{game}|{arch2}\n"
                    )
        f.write(
            """	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
EndGlobal
"""
        )


def gen_vcxproj(name, sourcefiles, headerfiles):
    with open(f"{name}.vcxproj", "w", encoding="utf-8") as f:
        f.write(
            """<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
"""
        )
        for build in builds:
            for arch, arch2 in arches:
                for game in games:
                    if arch in ["x86"] and game in ["Q2R"]:
                        continue
                    f.write(
                        f"""    <ProjectConfiguration Include="{build}-{game}|{arch2}">
      <Configuration>{build}-{game}</Configuration>
      <Platform>{arch2}</Platform>
    </ProjectConfiguration>
"""
                    )
        f.write("  </ItemGroup>\n  <ItemGroup>\n")
        for headerfile in headerfiles:
            f.write(f'    <ClInclude Include="{headerfile}" />\n')
        f.write("  </ItemGroup>\n  <ItemGroup>\n")
        for sourcefile in sourcefiles:
            f.write(f'    <ClCompile Include="{sourcefile}" />\n')
        f.write(
            f"""  </ItemGroup>
  <ItemGroup>
    <ResourceCompile Include="resource.rc">
      <PreprocessorDefinitions Condition="'$(Platform)'=='x64'">_UNICODE;UNICODE;%(PreprocessorDefinitions);_WIN64</PreprocessorDefinitions>
    </ResourceCompile>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <VCProjectVersion>17.0</VCProjectVersion>
    <Keyword>Win32Proj</Keyword>
    <ProjectGuid>{{1d5be629-82d1-4511-8559-5116bfe4338c}}</ProjectGuid>
    <RootNamespace>{name}</RootNamespace>
    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props" />
  <PropertyGroup Label="Configuration">
    <ConfigurationType>DynamicLibrary</ConfigurationType>
    <PlatformToolset>v143</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="$(Configuration.Contains('Debug-'))==true" Label="Configuration">
    <UseDebugLibraries>true</UseDebugLibraries>
    <WholeProgramOptimization>false</WholeProgramOptimization>
  </PropertyGroup>
  <PropertyGroup Condition="$(Configuration.Contains('Release-'))==true" Label="Configuration">
    <UseDebugLibraries>false</UseDebugLibraries>
    <WholeProgramOptimization>true</WholeProgramOptimization>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <ImportGroup Label="Shared">
  </ImportGroup>
  <ImportGroup Condition="'$(Platform)'=='Win32'" Label="PropertySheets">
    <Import Project="$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup>
    <OutDir>..\\bin\\$(Configuration)\\$(PlatformShortName)\\</OutDir>
    <IntDir>..\\obj\\$(Configuration)\\$(PlatformShortName)\\</IntDir>
    <IncludePath>$(VC_IncludePath);$(WindowsSDK_IncludePath);</IncludePath>
    <ExternalIncludePath>$(VC_IncludePath);$(WindowsSDK_IncludePath);</ExternalIncludePath>
  </PropertyGroup>
"""
        )
        for arch, arch2 in arches:
            if arch2 in "Win32":
                filearch = ""
            else:
                filearch = "x86_64_"
            for game in games:
                if arch in ["x86"] and game in ["Q2R"]:
                    continue
                f.write(
                    f"""  <PropertyGroup Condition="$(Platform)=='{arch2}' And $(Configuration.EndsWith('-{game}'))==true">
    <TargetName>$(ProjectName)_{filearch}{game}</TargetName>
  </PropertyGroup>
"""
                )
        f.write(
            """  <ItemDefinitionGroup>
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <SDLCheck>true</SDLCheck>
      <ConformanceMode>true</ConformanceMode>
      <PrecompiledHeader>NotUsing</PrecompiledHeader>
      <PrecompiledHeaderFile>pch.h</PrecompiledHeaderFile>
      <LanguageStandard>stdcpp17</LanguageStandard>
      <TreatAngleIncludeAsExternal>true</TreatAngleIncludeAsExternal>
      <DisableAnalyzeExternal>true</DisableAnalyzeExternal>
      <ExternalWarningLevel>TurnOffAllWarnings</ExternalWarningLevel>
      <AdditionalIncludeDirectories>../include;../../qmm_sdks;../../qmm2/include</AdditionalIncludeDirectories>
      <PreprocessorDefinitions>WIN32;_WINDOWS;_USRDLL;%(PreprocessorDefinitions)</PreprocessorDefinitions>
    </ClCompile>
    <Link>
      <SubSystem>Windows</SubSystem>
      <EnableUAC>false</EnableUAC>
	</Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="$(Configuration.StartsWith('Debug-'))==true">
    <ClCompile>
      <PreprocessorDefinitions>%(PreprocessorDefinitions);_DEBUG</PreprocessorDefinitions>
    </ClCompile>
    <Link>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="$(Configuration.StartsWith('Release-'))==true">
    <ClCompile>
      <PreprocessorDefinitions>%(PreprocessorDefinitions);NDEBUG</PreprocessorDefinitions>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
    </ClCompile>
    <Link>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
      <GenerateDebugInformation>false</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
"""
        )
        for game in games:
            f.write(
                f"""  <ItemDefinitionGroup Condition="$(Configuration.EndsWith('-{game}'))==true">
    <ClCompile>
      <PreprocessorDefinitions>%(PreprocessorDefinitions);GAME_{game}</PreprocessorDefinitions>
    </ClCompile>
  </ItemDefinitionGroup>
"""
            )
        f.write(
            """  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>"""
        )


def gen_vcxproj_filters(name, sourcefiles, headerfiles):
    with open(f"{name}.vcxproj.filters", "w", encoding="utf-8-sig") as f:
        f.write(
            """<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup>
    <Filter Include="Source Files">
      <UniqueIdentifier>{4FC737F1-C7A5-4376-A066-2A32D752A2FF}</UniqueIdentifier>
      <Extensions>cpp;c;cc;cxx;c++;cppm;ixx;def;odl;idl;hpj;bat;asm;asmx</Extensions>
    </Filter>
    <Filter Include="Header Files">
      <UniqueIdentifier>{93995380-89BD-4b04-88EB-625FBE52EBFB}</UniqueIdentifier>
      <Extensions>h;hh;hpp;hxx;h++;hm;inl;inc;ipp;xsd</Extensions>
    </Filter>
    <Filter Include="Resource Files">
      <UniqueIdentifier>{67DA6AB6-F800-4c08-8B7A-83BB121AAD01}</UniqueIdentifier>
      <Extensions>rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav;mfcribbon-ms</Extensions>
    </Filter>
  </ItemGroup>
  <ItemGroup>
"""
        )
        for headerfile in headerfiles:
            f.write(
                f"""    <ClInclude Include="{headerfile}">
      <Filter>Header Files</Filter>
    </ClInclude>
"""
            )
        f.write("  </ItemGroup>\n  <ItemGroup>\n")
        for sourcefile in sourcefiles:
            f.write(
                f"""    <ClCompile Include="{sourcefile}">
      <Filter>Source Files</Filter>
    </ClCompile>
"""
            )
        f.write(
            f"""  </ItemGroup>
  <ItemGroup>
    <ResourceCompile Include="{name}.rc">
      <Filter>Resource Files</Filter>
    </ResourceCompile>
  </ItemGroup>
</Project>"""
        )


def find_files(name):
    sourcefiles = []
    headerfiles = []

    filematch = re.compile(r'\s+<Cl\w+ .+="(.*)"\s+/>\s+')
    try:
        with open(f"{name}.vcxproj", "r", encoding="utf-8") as f:
            for line in f:
                if "ClCompile " in line:
                    m = filematch.match(line)
                    sourcefiles.append(m.group(1))
                elif "ClInclude " in line:
                    m = filematch.match(line)
                    headerfiles.append(m.group(1))
    except FileNotFoundError:
        pass
    return sourcefiles, headerfiles


def main():
    if len(sys.argv) <= 1:
        arg0 = os.path.basename(sys.argv[0])
        print(f"Usage:\n{arg0} <projectname>")
        return
    name = sys.argv[1]

    # find existing files in vcxproj
    sourcefiles, headerfiles = find_files(name)
    if not sourcefiles:
      sourcefiles = ["..\\src\\main.cpp"]
    if not headerfiles:
      headerfiles = ["..\\include\\game.h", "..\\include\\version.h"]

    gen_sln(name)

    gen_vcxproj(name, sourcefiles, headerfiles)
    gen_vcxproj_filters(name, sourcefiles, headerfiles)


if __name__ == "__main__":
    sys.exit(main())
