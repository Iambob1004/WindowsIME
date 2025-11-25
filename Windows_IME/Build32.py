import re
import os
import subprocess


def replace_guids_dic_and_desc(filepath_globals, new_clsid, new_guid, new_dic, filepath_register, new_desc):
    """在指定文件中替换 CLSID, GUID, 字典文件名, 和描述"""
    with open(filepath_globals, "r", encoding='utf-8') as file:
        content = file.read()
    content = re.sub(
        r"(SampleIMECLSID = {\n\s*)[^\n]+", lambda m: f"{m.group(1)}{new_clsid}", content)
    content = re.sub(
        r"(SampleIMEGuidProfile = {\n\s*)[^\n]+", lambda m: f"{m.group(1)}{new_guid}", content)
    content = re.sub(
        r'(std::wstring g_DictionaryFileName = L")[^"]+(";)', lambda m: f'{m.group(1)}{new_dic}{m.group(2)}', content)
    with open(filepath_globals, "w", encoding='utf-8') as file:
        file.write(content)

    with open(filepath_register, "r", encoding='utf-8') as file:
        register_content = file.read()
    register_content = re.sub(
        r'(static const WCHAR TEXTSERVICE_DESC\[\] = L")[^"]+(";)', lambda m: f'{m.group(1)}{new_desc}{m.group(2)}', register_content)
    with open(filepath_register, "w", encoding='utf-8') as file:
        file.write(register_content)


def replace_dictionary_logic(filepath, new_dic):
    """在指定文件中精确替换 Global::g_DictionaryFileName 为 43_Sakizaya.txt 的部分，不影响其他内容"""
    with open(filepath, "r", encoding='utf-8') as file:
        content = file.read()

    # 只替换 if 判断中的部分
    content = re.sub(
        r'(Global::g_DictionaryFileName == L")[^"]+(")',
        lambda m: f'{m.group(1)}{new_dic}{m.group(2)}',
        content
    )

    # 只替换赋值中的部分，不影响 "Normal.txt"
    content = re.sub(
        r'(Global::g_DictionaryFileName = L")[^"]+(";)',
        lambda m: f'{m.group(1)}{new_dic}{m.group(2)}' if 'Normal.txt' not in m.group(
            0) else m.group(0),
        content
    )

    with open(filepath, "w", encoding='utf-8') as file:
        file.write(content)


def build_project():
    """建置 Visual Studio 專案"""
    msbuild_path = r"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
    solution_path = r"C:\WindowsIME\Windows_IME\SampleIME.sln"
    subprocess.run([msbuild_path, solution_path,
                   "/p:Configuration=Release", "/p:Platform=Win32"])


def rename_dll(original_name, new_name):
    """重命名 DLL 文件"""
    if os.path.exists(original_name):
        os.rename(original_name, new_name)


def main():
    filepath_globals = "./SampleIME/Globals.cpp"
    clsid_file = "./SampleIME/Dictionary/CLSID.txt"
    guid_file = "./SampleIME/Dictionary/ProfileGUID.txt"
    dic_file = "./SampleIME/Dictionary/DictionaryName.txt"
    filepath_define = "./SampleIME/define.h"
    filepath_register = "./SampleIME/Register.cpp"
    filepath_composition = "./SampleIME/CompositionProcessorEngine.cpp"
    filepath_active_language = "./SampleIME/ActiveLanguageProfileNotifySink.cpp"
    desc_file = "./SampleIME/Dictionary/ChineseName.txt"
    output_dll_name = "./Release/SampleIME.dll"

    with open(clsid_file, "r", encoding='utf-8') as cfile, \
            open(guid_file, "r", encoding='utf-8') as gfile, \
            open(dic_file, "r", encoding='utf-8') as dfile, \
            open(desc_file, "r", encoding='utf-8') as descs:

        dic_lines = dfile.readlines()

        for index, (new_clsid, new_guid, new_desc) in enumerate(zip(cfile, gfile, descs)):
            new_dic = dic_lines[index].strip()
            if not new_dic.endswith(".txt"):
                new_dic += ".txt"

            replace_guids_dic_and_desc(filepath_globals, new_clsid.strip(
            ), new_guid.strip(), new_dic, filepath_register, new_desc.strip())

            # 只在指定的文件中替换 Global::g_DictionaryFileName 为 43_Sakizaya.txt 的部分
            replace_dictionary_logic(filepath_composition, new_dic)
            replace_dictionary_logic(filepath_active_language, new_dic)

            base_name = new_dic.strip()
            if (base_name.endswith(".txt")):
                base_name = base_name[:-4]
            new_output_name = f"./Release/{base_name}.dll"
            build_project()
            rename_dll(output_dll_name, new_output_name)
            print(f"建置 {index + 1}: DLL 已重命名為 {new_output_name}, 數據已更新.")


if __name__ == "__main__":
    main()
