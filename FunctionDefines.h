#pragma once
#define NoAutoclickE(Arg,Else) if (WND1.CurrentMouseState == false) { if (WND1.Mk.LeftIsPressed() == true) {WND1.CurrentMouseState = true; Arg}Else} //do Arg dajemy funkcje dzia�aj�c� w przypadku klikni�cia, do else funkcje co sie dzieje gdy nie ma klikni�cia
#define NoAutoclick(Arg) if (WND1.CurrentMouseState == false) { if (WND1.Mk.LeftIsPressed() == true) {WND1.CurrentMouseState = true; Arg}} //Autoclick bez elsa
#define ISPressed(Arg) if(WND1.Klt.KeyIsPressed(Arg))