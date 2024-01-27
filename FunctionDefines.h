#pragma once
#define NoAutoclick(Arg) if (WND1.CurrentMouseState == false) {Arg} //¿eby makro dzialalo w srodku trzeba umiescic  if (WND1.Mk.LeftIsPressed() == true) { WND1.CurrentMouseState=true;}
#define ISPressed WND1.Klt.KeyIsPressed