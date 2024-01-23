#include "LevelInstance.h"
void LevelInstance::SaveLevel(const std::vector<TextureInstance>& ToSave, const std::wstring& Filename,const Player PlayerInstance)
{
    std::ofstream file;
    file.open(Filename + L".dat", std::ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<const char*>(&PlayerInstance.PlayerRect), sizeof(D2D1_RECT_F));
        std::wstring::size_type sizepath = PlayerInstance.CurrentPlayerTexture.PATHTest.size();
        file.write(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
        file.write(reinterpret_cast<const char*>(PlayerInstance.CurrentPlayerTexture.PATHTest.c_str()), sizepath * sizeof(wchar_t));

        size_t TextureCount = ToSave.size();
        file.write(reinterpret_cast<char*>(&TextureCount), sizeof(TextureCount));

        for (const auto& texture : ToSave)
        {
            size_t destinationRectTabSize = texture.destinationRectTab.size();
            file.write(reinterpret_cast<char*>(&destinationRectTabSize), sizeof(destinationRectTabSize));

            // Zapisz ca³¹ tablicê destinationRectTab jako jeden blok danych
            file.write(reinterpret_cast<const char*>(texture.destinationRectTab.data()),
                destinationRectTabSize * sizeof(D2D1_RECT_F));

             sizepath = texture.PATHTest.size();
            file.write(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
            file.write(reinterpret_cast<const char*>(texture.PATHTest.c_str()), sizepath * sizeof(wchar_t));
        }

        file.close();
    }
}

void LevelInstance::LoadLevel(std::vector<TextureInstance>& ToLoad, const std::wstring& Filename, Player& PlayerInstance) //pamiêtaj ¿e jak chcesz cokolwiek wczytac to musisz parametr przekazac by reference &
{
    std::ifstream file;
    file.open(Filename, std::ios::binary);
    if (file.is_open())
    {
        file.read(reinterpret_cast<char*>(&PlayerInstance.PlayerRect), sizeof(D2D1_RECT_F));
        std::wstring::size_type sizepath;
        file.read(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
        PlayerInstance.CurrentPlayerTexture.PATHTest.resize(sizepath);
        file.read(reinterpret_cast<char*>(&PlayerInstance.CurrentPlayerTexture.PATHTest[0]), sizepath * sizeof(wchar_t));

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
            file.read(reinterpret_cast<char*>(TempTexture.destinationRectTab.data()),destinationRectTabSize * sizeof(D2D1_RECT_F));

            std::wstring::size_type sizepath1;
            file.read(reinterpret_cast<char*>(&sizepath1), sizeof(sizepath1));
            TempTexture.PATHTest.resize(sizepath1);
            file.read(reinterpret_cast<char*>(&TempTexture.PATHTest[0]), sizepath1 * sizeof(wchar_t));
        }


        file.close();
    }
}


