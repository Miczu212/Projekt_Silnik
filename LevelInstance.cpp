#include "LevelInstance.h"
void LevelInstance::SaveLevel(const std::vector<TextureInstance>& ToSaveT,std::vector<std::wstring>& ToSaveA, const std::wstring& Filename,const Player PlayerInstance) //pamietaj by jak cokolwiek dodasz co wymaga zapisania to to tutaj zapisac
{
    std::ofstream file;
    file.open(Filename + L".dat", std::ios::binary);
    if (file.is_open())
    {   //odpowiendio zapisywanie struktury tablicy textur i player
        file.write(reinterpret_cast<const char*>(&PlayerInstance.PlayerRect), sizeof(D2D1_RECT_F)); //playerrect
        std::wstring::size_type sizepath = PlayerInstance.CurrentPlayerTexture.Path.size();
        file.write(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
        file.write(reinterpret_cast<const char*>(PlayerInstance.CurrentPlayerTexture.Path.c_str()), sizepath * sizeof(wchar_t)); //path to texture instance

        size_t TextureCount = ToSaveT.size();
        file.write(reinterpret_cast<char*>(&TextureCount), sizeof(TextureCount)); //ilosc textur

        for (const auto& texture : ToSaveT)
        {   
            size_t destinationRectTabSize = texture.destinationRectTab.size();
            file.write(reinterpret_cast<char*>(&destinationRectTabSize), sizeof(destinationRectTabSize)); 

            // Zapisz ca³¹ tablicê destinationRectTab jako jeden blok danych
            file.write(reinterpret_cast<const char*>(texture.destinationRectTab.data()), 
                destinationRectTabSize * sizeof(D2D1_RECT_F));

             sizepath = texture.Path.size();
            file.write(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
            file.write(reinterpret_cast<const char*>(texture.Path.c_str()), sizepath * sizeof(wchar_t)); //path to texture
            file.write(reinterpret_cast<const char*>(&texture.IsCollisionOn), sizeof(texture.IsCollisionOn)); //iscolisionon
            file.write(reinterpret_cast<const char*>(&texture.Theight), sizeof(texture.Theight)); //Theight
            file.write(reinterpret_cast<const char*>(&texture.Twidth), sizeof(texture.Twidth)); //Twidth
        }

        std::wstring::size_type sizepathA;
        size_t Tabsize = ToSaveA.size();
        file.write(reinterpret_cast<char*>(&Tabsize), sizeof(Tabsize));
        for (const auto& Path : ToSaveA)
        {
            sizepathA = Path.size();
            file.write(reinterpret_cast<char*>(&sizepathA), sizeof(sizepathA));
            file.write(reinterpret_cast<const char*>(Path.c_str()), sizepathA * sizeof(wchar_t)); //path to Audio instance
        }
        file.close();
    }
}
std::wstring LevelInstance::GetNameOfFile(const std::wstring& path)
{
    int counter=0;
    for (int i = path.size() - 1; i > 0; i--)
    {
        if (path[i] == '\\')
        {
            counter = i;
            break;
        }
    }
    std::wstring NameOfFile;
    for (counter; counter < path.size(); counter++)
    {
        NameOfFile.push_back(path[counter]);
    }
    return NameOfFile;
}
void LevelInstance::ReTargetLevel(const std::wstring& Filename)
{
    Player PlayerInstance;
    std::vector<TextureInstance> ToLoadT;
    std::vector<std::wstring> ToLoadA;
    std::ifstream fileL;
    fileL.open(Filename, std::ios::binary);
    if (fileL.is_open())
    {
        fileL.read(reinterpret_cast<char*>(&PlayerInstance.PlayerRect), sizeof(D2D1_RECT_F));
        std::wstring::size_type sizepath;
        fileL.read(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
        PlayerInstance.CurrentPlayerTexture.Path.resize(sizepath);
        fileL.read(reinterpret_cast<char*>(&PlayerInstance.CurrentPlayerTexture.Path[0]), sizepath * sizeof(wchar_t));

        size_t TextureCount;
        fileL.read(reinterpret_cast<char*>(&TextureCount), sizeof(TextureCount));

        ToLoadT.resize(TextureCount);

        for (size_t i = 0; i < TextureCount; ++i)
        {
            TextureInstance& TempTexture = ToLoadT[i];

            size_t destinationRectTabSize;
            fileL.read(reinterpret_cast<char*>(&destinationRectTabSize), sizeof(destinationRectTabSize));
            TempTexture.destinationRectTab.resize(destinationRectTabSize);

            // Odczytaj ca³¹ tablicê destinationRectTab jako jeden blok danych
            fileL.read(reinterpret_cast<char*>(TempTexture.destinationRectTab.data()), destinationRectTabSize * sizeof(D2D1_RECT_F));

            std::wstring::size_type sizepath1;
            fileL.read(reinterpret_cast<char*>(&sizepath1), sizeof(sizepath1));
            TempTexture.Path.resize(sizepath1);
            fileL.read(reinterpret_cast<char*>(&TempTexture.Path[0]), sizepath1 * sizeof(wchar_t));
            fileL.read(reinterpret_cast<char*>(&TempTexture.IsCollisionOn), sizeof(TempTexture.IsCollisionOn));
            fileL.read(reinterpret_cast<char*>(&TempTexture.Theight), sizeof(TempTexture.Theight));
            fileL.read(reinterpret_cast<char*>(&TempTexture.Twidth), sizeof(TempTexture.Twidth));
        }

        std::wstring::size_type sizepathA;
        size_t Tabsize;
        fileL.read(reinterpret_cast<char*>(&Tabsize), sizeof(Tabsize));
        ToLoadA.resize(Tabsize);
        for (size_t i = 0; i < Tabsize; ++i)
        {

            fileL.read(reinterpret_cast<char*>(&sizepathA), sizeof(sizepathA));
            ToLoadA[i].resize(sizepathA);
            fileL.read(reinterpret_cast<char*>(&ToLoadA[i][0]), sizepathA * sizeof(wchar_t));//path to Audio instance
        }


        fileL.close();
    }


        //Po wczytaniu zmieniamy nazwy sciezek
        std::filesystem::path projectFolder = std::filesystem::current_path();
        PlayerInstance.CurrentPlayerTexture.Path = projectFolder.c_str() + GetNameOfFile(PlayerInstance.CurrentPlayerTexture.Path);

        for ( auto& texture : ToLoadT)
        {
        texture.Path = projectFolder.c_str()+ GetNameOfFile(texture.Path);
  
        }

        for ( auto& Path : ToLoadA)
        {
            Path = projectFolder.c_str() + GetNameOfFile(Path);
        }
        //Zapisywanie nowych sciezek
        std::ofstream fileS;
        fileS.open(Filename, std::ios::binary);
        if (fileS.is_open())
        {   //odpowiendio zapisywanie struktury tablicy textur i player
            fileS.write(reinterpret_cast<const char*>(&PlayerInstance.PlayerRect), sizeof(D2D1_RECT_F)); //playerrect
            std::wstring::size_type sizepath = PlayerInstance.CurrentPlayerTexture.Path.size();
            fileS.write(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
            fileS.write(reinterpret_cast<const char*>(PlayerInstance.CurrentPlayerTexture.Path.c_str()), sizepath * sizeof(wchar_t)); //path to texture instance

            size_t TextureCount = ToLoadT.size();
            fileS.write(reinterpret_cast<char*>(&TextureCount), sizeof(TextureCount)); //ilosc textur

            for (const auto& texture : ToLoadT)
            {
                size_t destinationRectTabSize = texture.destinationRectTab.size();
                fileS.write(reinterpret_cast<char*>(&destinationRectTabSize), sizeof(destinationRectTabSize));

                // Zapisz ca³¹ tablicê destinationRectTab jako jeden blok danych
                fileS.write(reinterpret_cast<const char*>(texture.destinationRectTab.data()),
                    destinationRectTabSize * sizeof(D2D1_RECT_F));

                sizepath = texture.Path.size();
                fileS.write(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
                fileS.write(reinterpret_cast<const char*>(texture.Path.c_str()), sizepath * sizeof(wchar_t)); //path to texture
                fileS.write(reinterpret_cast<const char*>(&texture.IsCollisionOn), sizeof(texture.IsCollisionOn)); //iscolisionon
                fileS.write(reinterpret_cast<const char*>(&texture.Theight), sizeof(texture.Theight)); //Theight
                fileS.write(reinterpret_cast<const char*>(&texture.Twidth), sizeof(texture.Twidth)); //Twidth
            }

            std::wstring::size_type sizepathA;
            size_t Tabsize = ToLoadA.size();
            fileS.write(reinterpret_cast<char*>(&Tabsize), sizeof(Tabsize));
            for (const auto& Path : ToLoadA)
            {
                sizepathA = Path.size();
                fileS.write(reinterpret_cast<char*>(&sizepathA), sizeof(sizepathA));
                fileS.write(reinterpret_cast<const char*>(Path.c_str()), sizepathA * sizeof(wchar_t)); //path to Audio instance
            }
            fileS.close();
        }

    }
void LevelInstance::LoadLevel(std::vector<TextureInstance>& ToLoadT, std::vector<std::wstring>& ToLoadA, const std::wstring& Filename, Player& PlayerInstance) //pamiêtaj ¿e jak chcesz cokolwiek wczytac to musisz parametr przekazac by reference &
{
    std::ifstream file;
    file.open(Filename, std::ios::binary);
    if (file.is_open())
    {
        file.read(reinterpret_cast<char*>(&PlayerInstance.PlayerRect), sizeof(D2D1_RECT_F));
        std::wstring::size_type sizepath;
        file.read(reinterpret_cast<char*>(&sizepath), sizeof(sizepath));
        PlayerInstance.CurrentPlayerTexture.Path.resize(sizepath);
        file.read(reinterpret_cast<char*>(&PlayerInstance.CurrentPlayerTexture.Path[0]), sizepath * sizeof(wchar_t));

        size_t TextureCount;
        file.read(reinterpret_cast<char*>(&TextureCount), sizeof(TextureCount));

        ToLoadT.resize(TextureCount);

        for (size_t i = 0; i < TextureCount; ++i)
        {
            TextureInstance& TempTexture = ToLoadT[i];

            size_t destinationRectTabSize;
            file.read(reinterpret_cast<char*>(&destinationRectTabSize), sizeof(destinationRectTabSize));
            TempTexture.destinationRectTab.resize(destinationRectTabSize);

            // Odczytaj ca³¹ tablicê destinationRectTab jako jeden blok danych
            file.read(reinterpret_cast<char*>(TempTexture.destinationRectTab.data()),destinationRectTabSize * sizeof(D2D1_RECT_F));

            std::wstring::size_type sizepath1;
            file.read(reinterpret_cast<char*>(&sizepath1), sizeof(sizepath1));
            TempTexture.Path.resize(sizepath1);
            file.read(reinterpret_cast<char*>(&TempTexture.Path[0]), sizepath1 * sizeof(wchar_t));
            file.read(reinterpret_cast<char*>(&TempTexture.IsCollisionOn), sizeof(TempTexture.IsCollisionOn));
            file.read(reinterpret_cast<char*>(&TempTexture.Theight), sizeof(TempTexture.Theight));
            file.read(reinterpret_cast<char*>(&TempTexture.Twidth), sizeof(TempTexture.Twidth));
        }

        std::wstring::size_type sizepathA;
        size_t Tabsize;
        file.read(reinterpret_cast<char*>(&Tabsize), sizeof(Tabsize));
        ToLoadA.resize(Tabsize);
        for (size_t i = 0; i < Tabsize; ++i)
        {
            
            file.read(reinterpret_cast<char*>(&sizepathA), sizeof(sizepathA));
            ToLoadA[i].resize(sizepathA);
            file.read(reinterpret_cast<char*>(&ToLoadA[i][0]), sizepathA * sizeof(wchar_t));//path to Audio instance
        }


        file.close();
    }
}



