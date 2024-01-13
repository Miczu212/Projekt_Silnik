#include "LevelInstance.h"

void LevelInstance::SaveLevel(TextureInstance ToSave,std::string Filename) //TODO NAPISAC OD NOWA
{
    std::ofstream file;
    file.open(Filename + ".dat", std::ios::binary);
    if (file.is_open())
    {
        size_t destinationRectTabSize = ToSave.destinationRectTab.size();
        file.write(reinterpret_cast<char*>(&destinationRectTabSize), sizeof(destinationRectTabSize));
        for (int i = 0; i < ToSave.destinationRectTab.size(); i++)
        {
            file.write(reinterpret_cast<char*>(&ToSave.destinationRectTab[i]), sizeof(ToSave.destinationRectTab[i]));
        }
        std::wstring::size_type sizepath = ToSave.PATHTest.size();
        file.write(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
        file.write(reinterpret_cast<const char*>(ToSave.PATHTest.c_str()), sizepath * sizeof(wchar_t));
        file.close();
    }
}

void LevelInstance::LoadLevel(TextureInstance& ToLoad, std::string Filename)  //TODO NAPISAC OD NOWA
{
    std::ifstream file;
    file.open(Filename + ".dat", std::ios::binary);
    if (file.is_open())
    {
        size_t destinationRectTabSize;
        ToLoad.destinationRectTab.clear();
        file.read(reinterpret_cast<char*>(&destinationRectTabSize), sizeof(destinationRectTabSize));
        for (int i = 0; i < destinationRectTabSize; i++)
        {
            D2D1_RECT_F TempRect;
            file.read(reinterpret_cast<char*>(&TempRect), sizeof(TempRect));
            ToLoad.destinationRectTab.push_back(TempRect);
        }
        std::wstring::size_type sizepath;
        file.read(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
        ToLoad.PATHTest.resize(sizepath);
        file.read(reinterpret_cast<char*>(&ToLoad.PATHTest[0]), sizepath * sizeof(wchar_t));

        file.close();
    }
}
