float4 main(float2 Position  : POSITION) : SV_Position 
{
    return float4(Position.x, Position.y, 0.0f, 1.0f); // stworzenie vertex shadera, z jest na 0 dlatego ze dzialamy w 2 wymiarach
}