#pragma once
#define NoAutoclickE(Arg,Else) if (WND1.CurrentMouseState == false) { if (WND1.Mk.LeftIsPressed() == true) {WND1.CurrentMouseState = true; Arg}Else} //do Arg dajemy funkcje dzia³aj¹c¹ w przypadku klikniêcia, do else funkcje co sie dzieje gdy nie ma klikniêcia
#define NoAutoclick(Arg) if (WND1.CurrentMouseState == false) { if (WND1.Mk.LeftIsPressed() == true) {WND1.CurrentMouseState = true; Arg}} //Autoclick bez elsa
#define ISPressed(Arg) if(WND1.Klt.KeyIsPressed(Arg))