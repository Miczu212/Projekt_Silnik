#include "LevelInstance.h"
void LevelInstance::SaveLevel(const std::vector<TextureInstance>& ToSave, const std::wstring& Filename)
{
    std::ofstream file;
    file.open(Filename + L".dat", std::ios::binary);
    if (file.is_open())
    {
        size_t TextureCount = ToSave.size();
        file.write(reinterpret_cast<char*>(&TextureCount), sizeof(TextureCount));

        for (const auto& texture : ToSave)
        {
            size_t destinationRectTabSize = texture.destinationRectTab.size();
            file.write(reinterpret_cast<char*>(&destinationRectTabSize), sizeof(destinationRectTabSize));

            // Zapisz ca³¹ tablicê destinationRectTab jako jeden blok danych
            file.write(reinterpret_cast<const char*>(texture.destinationRectTab.data()),
                destinationRectTabSize * sizeof(D2D1_RECT_F));

            std::wstring::size_type sizepath = texture.PATHTest.size();
            file.write(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
            file.write(reinterpret_cast<const char*>(texture.PATHTest.c_str()), sizepath * sizeof(wchar_t));
        }

        file.close();
    }
}

void LevelInstance::LoadLevel(std::vector<TextureInstance>& ToLoad, const std::wstring& Filename)
{
    std::ifstream file;
    file.open(Filename, std::ios::binary);
    if (file.is_open())
    {
        size_t TextureCount;
        file.read(reinterpret_cast<char*>(&TextureCount), sizeof(TextureCount));

        ToLoad.resize(TextureCount);

        for (size_t i = 0; i < TextureCount; ++i)
        {
            TextureInstance& TempTexture = ToLoad[i];

            size_t destinationRectTabSize;
            file.read(reinterpret_cast<char*>(&destinationRectTabSize), sizeof(destinationRectTabSize));
            TempTexture.destinationRectTab.resize(destinationRectTabSize);

            // Odczytaj ca³¹ tablicê destinationRectTab jako jeden blok danych
            file.read(reinterpret_cast<char*>(TempTexture.destinationRectTab.data()),
                destinationRectTabSize * sizeof(D2D1_RECT_F));

            std::wstring::size_type sizepath;
            file.read(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
            TempTexture.PATHTest.resize(sizepath);
            file.read(reinterpret_cast<char*>(&TempTexture.PATHTest[0]), sizepath * sizeof(wchar_t));
        }

        file.close();
    }
}


